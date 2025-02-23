#include "startupdialog.h"
#include "ui_startupdialog.h"

StartupDialog::StartupDialog(MainWindow *parent)
    : QDialog(parent)
    , ui(new Ui::StartupDialog)
    , map_standby(false)
{
    this->parent = parent;

    //UI初期化
    ui->setupUi(this);
    
    setMusicFileList();
    setImageThemeList();

    ui->CoolGroupBox->SetPortSpin(2009);
    ui->CoolGroupBox->ConnectionToggled(true);

    ui->HotGroupBox-> SetPortSpin(2010);
    ui->HotGroupBox-> ConnectionToggled(true);

    //クライアント初期化
    this->team_client[static_cast<int>(GameSystem::TEAM::COOL)] = ui->CoolGroupBox;
    this->team_client[static_cast<int>(GameSystem::TEAM::HOT)] = ui->HotGroupBox;
    for (int i = 0; i < TEAM_COUNT; i++) {
        team_standby[i] = false;
    }

    connect(ui->HotGroupBox,  &ClientSettingForm::Standby, this, &StartupDialog::ClientStandby);
    connect(ui->CoolGroupBox, &ClientSettingForm::Standby, this, &StartupDialog::ClientStandby);

    //ローカルIPの探索
    foreach (const QHostAddress &address, QNetworkInterface::allAddresses()) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress(QHostAddress::LocalHost)){
            this->ui->LocalIPLabel->setText(address.toString());
            break;
        }
    }
    this->ui->HostName->setText(QHostInfo::localHostName());

    map.CreateRandomMap();
    map_standby = true;

    connect(this->ui->StandbyButton,SIGNAL(clicked()),  parent,SLOT(StartSetUp()));
    connect(this->ui->GameStartButton,SIGNAL(clicked()),parent,SLOT(StartGame()));
}

QVariant StartupDialog::getDefault(QString key)
{
    QSettings *Settings;
    Settings = new QSettings("setting.ini", QSettings::IniFormat); // iniファイルで設定を保存
    Settings->beginGroup("Default");
    return Settings->value(key);
}

void StartupDialog::setCommandLineOptions()
{
    QVariant v;

    QCommandLineParser parser;
    // アプリの説明
    parser.setApplicationDescription("CHaser server");
    parser.addHelpOption();

    QCommandLineOption RandomMapOption({"r", "random"}, "ランダムマップ");
    parser.addOption(RandomMapOption);

    QCommandLineOption CoolOption("cool", "Coolプログラムファイル", "program file");
    parser.addOption(CoolOption);

    QCommandLineOption HotOption("hot", "Hotプログラムファイル", "program file");
    parser.addOption(HotOption);

    
    v = getDefault("DefaultMap");
    QCommandLineOption MapOption({"m", "map"}, "マップファイル", "map",         // 値の名称
                (v.typeId() != QMetaType::UnknownType) ? v.toString() : "Default.map" // 値のデフォルト値
        );
    parser.addOption(MapOption);

    v = getDefault("DefaultTexture");
    QCommandLineOption TextureOption({"t", "texture"}, "テクスチャ", "texture",     // 値の名称
                (v.typeId() != QMetaType::UnknownType) ? v.toString() : "あっさり" // 値のデフォルト値
        );
    parser.addOption(TextureOption);

    v = getDefault("DefaultBGM");
    QCommandLineOption BGMOption({"b", "bgm"}, "BGM", "bgm",            // 値の名称
                (v.typeId() != QMetaType::UnknownType) ? v.toString() : "TwinBeeNew.wav" // 値のデフォルト値
        );
    parser.addOption(BGMOption);

    parser.process(QCoreApplication::arguments());
    
    if(parser.value(CoolOption) != "")
        this->ui->CoolGroupBox->reset("Python", programpath + "/" + parser.value(CoolOption));
    else {
        v = getDefault("CoolProgram");
        if (v.typeId() != QMetaType::UnknownType && v.toString() != "None"){
            this->ui->CoolGroupBox->setProgramFile(programpath, v.toString());
        } else {
            this->ui->CoolGroupBox->setProgramFile(programpath, "player.py");
        }
    }

    if(parser.value(HotOption) != "")
        this->ui->HotGroupBox->reset("Python", programpath + "/" + parser.value(HotOption));
    else {
        v = getDefault("HotProgram");
        if (v.typeId() != QMetaType::UnknownType && v.toString() != "None"){
            this->ui->HotGroupBox->setProgramFile(programpath, v.toString());
        } else {
            this->ui->HotGroupBox->setProgramFile(programpath, "player.py");
        }
    }


    if(! parser.isSet(RandomMapOption)) {
        QString filePath;
        filePath = mappath + "/" + parser.value(MapOption);
        if (QFile::exists(filePath)) {
            this->ui->MapDirEdit->setText(filePath);
            map_standby = MapRead(filePath);
        }
    }
    
    this->ui->GameMusicCombo->setCurrentText(parser.value(BGMOption));
    music_text = ui->GameMusicCombo->currentText();

    this->ui->TextureThemeCombo->setCurrentText(parser.value(TextureOption));
}

StartupDialog::~StartupDialog()
{
    delete ui;
}

void StartupDialog::resetMap()
{
    if(this->ui->MapDirEdit->text() == "")
    {
        map.CreateRandomMap(20,55,100,false);
        SetMapStandby(true);
    }
}

void StartupDialog::ChangeMusicCombo(QString text)
{
    music_text = text;
}

void StartupDialog::CheckStandby()
{
    bool all_of = true;
    for (int i = 0; i < TEAM_COUNT; i++) {
        if (!team_standby[i])
            all_of = false;
    }
    this->ui->StandbyButton->setEnabled(all_of && map_standby);
}

void StartupDialog::setGameStartButtonEnabled(bool set)
{
    this->ui->GameStartButton->setEnabled(set);
}

void StartupDialog::GameStartButtonClick()
{
    this->ui->GameStartButton->click();
}

void StartupDialog::setGameStartButtonShow(bool set)
{
    if (set) {
        disconnect(this->ui->GameStartButton,SIGNAL(clicked()),this->parent,SLOT(StartGame()));
        disconnect(this->ui->GameStartButton,SIGNAL(clicked()),this->parent,SLOT(RepeatGame()));
        this->ui->StandbyButton->show();
        this->ui->GameStartButton->show();
        this->ui->GameStartButton->setText("スタート");
        this->ui->GameStartButton->setEnabled(false);
        connect(this->ui->GameStartButton,SIGNAL(clicked()),this->parent,SLOT(StartGame()));
    } else {
        this->ui->StandbyButton->hide();
        this->ui->GameStartButton->hide();
    }
}

void StartupDialog::setGameStartButtonToEnd(bool repeat)
{
    this->ui->StandbyButton->hide();
    this->ui->GameStartButton->show();
    this->ui->GameStartButton->setEnabled(true);
    disconnect(this->ui->GameStartButton,SIGNAL(clicked()),this->parent,SLOT(StartGame()));
    if (repeat) {
        this->ui->GameStartButton->setText("再戦");
        connect(this->ui->GameStartButton,SIGNAL(clicked()),this->parent,SLOT(RepeatGame()));
    } else {
        this->ui->GameStartButton->setText("終了");
        connect(this->ui->GameStartButton,SIGNAL(clicked()),this->parent,SLOT(EndGame()));
    }
}

void StartupDialog::setStandbyButtonShow(bool set)
{
    if (set) {
        this->ui->StandbyButton->show();
    } else {
        this->ui->StandbyButton->hide();
    }
}

void StartupDialog::setSetupModeEnable(bool set)
{
    if (set) {
        ui->MapDirEdit->setReadOnly(false);
        ui->MapEditButton->setEnabled(true);
        ui->MapSelectButton->setEnabled(true);
        ui->TextureThemeCombo->setEnabled(true);
        ui->GameMusicCombo->setEnabled(true);
    } else {
        ui->MapDirEdit->setReadOnly(true);
        ui->MapEditButton->setEnabled(false);
        ui->MapSelectButton->setEnabled(false);
        ui->TextureThemeCombo->setEnabled(false);
        ui->GameMusicCombo->setEnabled(false);
    }
}

void StartupDialog::connectionReset()
{
    QString CoolP = this->ui->CoolGroupBox->getPlayer();
    QString CoolP2 = this->ui->CoolGroupBox->getProgramFile();

    QString HotP = this->ui->HotGroupBox->getPlayer();
    QString HotP2 = this->ui->HotGroupBox->getProgramFile();

    this->ui->CoolGroupBox->reset(HotP, HotP2);
    this->ui->HotGroupBox->reset(CoolP,CoolP2);
}

void StartupDialog::randomConnectionReset()
{
    QString CoolP = this->ui->CoolGroupBox->getPlayer();
    QString CoolP2 = this->ui->CoolGroupBox->getRandomProgramFile();

    QString HotP = this->ui->HotGroupBox->getPlayer();
    QString HotP2 = this->ui->HotGroupBox->getRandomProgramFile();

    this->ui->CoolGroupBox->reset(HotP, HotP2);
    this->ui->HotGroupBox->reset(CoolP,CoolP2);
}

void StartupDialog::ShowMapEditDialog()
{
    MapEditerDialog diag(map, mappath);
    if (diag.exec()) {
        if (diag.filepath == "") {
            this->ui->MapDirEdit->setText("[CUSTOM MAP]");
            map = diag.GetMap();
        } else {
            //再読み込み
            if (MapRead(diag.filepath))
                this->ui->MapDirEdit->setText(diag.filepath);
        }
    }
    SetMapStandby(true);
}

bool StartupDialog::MapRead(const QString &dir)
{
    //ファイルからマップを読み込む
    this->map.Import(dir);
    map.size.setY(map.field.size());
    return true;
}

void StartupDialog::setMusicFileList()
{
    QDir dir("./Music");

    ui->GameMusicCombo->clear();
    if (dir.exists()) { //ディレクトリが存在していたらmp3とwavのファイルをリストに追加する
        QStringList filelist = dir.entryList({"*.mp3", "*.wav"}, QDir::Files | QDir::NoSymLinks);
        if (filelist.isEmpty()) { //ディレクトリが存在していても、mp3とwavのファイルがなければ、Noneにして無効化
            ui->GameMusicCombo->addItem("None");
            ui->GameMusicCombo->setEnabled(false);
        } else
            ui->GameMusicCombo->addItems(filelist);
    } else { //なかったらNoneにして無効化
        ui->GameMusicCombo->addItem("None");
        ui->GameMusicCombo->setEnabled(false);
    }
}

void StartupDialog::setImageThemeList()
{
    ui->TextureThemeCombo->clear();
    ui->TextureThemeCombo->addItems({"ほうせき", "あっさり", "こってり", "RPG"}); //デフォルトの3テーマの追加

    QDir dir("./Image");

    if (dir.exists()) { //ディレクトリが存在していたら
        QStringList filelist = dir.entryList(QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot);
        ui->TextureThemeCombo->addItems(filelist);
    }
}

void StartupDialog::PushedMapSelect()
{
    QString cap = tr("マップを開く");
    QString filter = tr("マップファイル (*.map)");

    QString mapfile = QFileDialog::getOpenFileName(this, cap, mappath, filter);
    if (QFile::exists(mapfile)) {
        this->ui->MapDirEdit->setText(mapfile);
        SetMapStandby(MapRead(mapfile));
    }
}

void StartupDialog::ClientStandby(ClientSettingForm *client, bool complate)
{
    for (int i = 0; i < TEAM_COUNT; i++) {
        if (team_client[i] == client) {
            team_standby[i] = complate;
            CheckStandby();
            return;
        }
    }
}

void StartupDialog::SetMapStandby(bool state)
{
    map_standby = state;
    if (state) parent->repaintMap();
    CheckStandby();
}

void StartupDialog::ChangedTexture(QString text)
{
    if (text == "あっさり")      this->map.texture_dir_path = ":/Image/Light";
    else if (text == "こってり") this->map.texture_dir_path = ":/Image/Heavy";
    else if (text == "ほうせき") this->map.texture_dir_path = ":/Image/Jewel";
    else if (text == "RPG")   this->map.texture_dir_path = ":/Image/RPG";
    else                      this->map.texture_dir_path = "Image/" + text;
}

void StartupDialog::Setting()
{
    SettingDialog *diag;
    diag = new SettingDialog;
    if (diag->exec() == QDialog::Accepted) {
        //設定を保存
        diag->Export();
        parent->setSetting();
        parent->setDesign();
        parent->setPath();
    }
    delete diag;
}

void StartupDialog::setBotCommand(QString command)
{
    ui->CoolGroupBox->setBotCommand(command);
    ui->HotGroupBox->setBotCommand(command);
}

void StartupDialog::setPythonCommand(QString command)
{
    ui->CoolGroupBox->setPythonCommand(command);
    ui->HotGroupBox->setPythonCommand(command);
}

void StartupDialog::setProgramPath(QString path)
{
    programpath = path;
}


void StartupDialog::setMapPath(QString path)
{
    mappath = path;
}

void StartupDialog::setConnectionChangeEnable(bool set)
{
    this->ui->CoolGroupBox->setChangeEnable(set);
    this->ui->HotGroupBox->setChangeEnable(set);
}
