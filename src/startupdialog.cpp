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
    music_text = ui->GameMusicCombo->currentText();
    ui->CoolGroupBox->SetPortSpin(2009);
    ui->HotGroupBox->SetPortSpin(2010);

    QCommandLineParser parser;
    // アプリの説明
    parser.setApplicationDescription("CHaser server");
    parser.addHelpOption();

    QCommandLineOption RandomMapOption({"r", "random"}, "ランダムマップ");
    parser.addOption(RandomMapOption);

    QCommandLineOption MapOption({"m", "map"}, "マップファイル",
                               "map",         // 値の名称
                               "Default.map"  // 値のデフォルト値
                               );
    parser.addOption(MapOption);

    QCommandLineOption TextureOption({"t", "texture"}, "テクスチャ",
                               "texture",  // 値の名称
                               "RPG"       // 値のデフォルト値
                               );
    parser.addOption(TextureOption);

    QCommandLineOption BGMOption({"b", "bgm"}, "BGM",
                               "bgm",  // 値の名称
                               "YuHi"  // 値のデフォルト値
                               );
    parser.addOption(BGMOption);

    parser.process(QCoreApplication::arguments());


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

    this->ui->CoolGroupBox->ConnectionToggled(true);
    this->ui->HotGroupBox->ConnectionToggled(true);
    
    map.CreateRandomMap();
    map_standby = true;

    connect(this->ui->StandbyButton,SIGNAL(clicked()),  parent,SLOT(StartSetUp()));
    connect(this->ui->GameStartButton,SIGNAL(clicked()),parent,SLOT(StartGame()));

    if(! parser.isSet(RandomMapOption)) {
        QString filePath = QDir::currentPath()+ "/Map/" + parser.value(MapOption);
        if (QFile::exists(filePath)) {
            this->ui->MapDirEdit->setText(filePath);
            SetMapStandby(MapRead(filePath));
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

void StartupDialog::ShowMapEditDialog()
{
    MapEditerDialog diag(map);
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
        //qDebug()<<filelist;
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
        //qDebug()<<filelist;

        ui->TextureThemeCombo->addItems(filelist);
    }
}

void StartupDialog::PushedMapSelect()
{
    QString folder = QDir::currentPath() + "/Map/";
    QString cap = tr("マップを開く");
    QString filter = tr("マップファイル (*.map)");

    QString filePath = QFileDialog::getOpenFileName(this, cap, folder, filter);
    this->ui->MapDirEdit->setText(filePath);
    SetMapStandby(MapRead(filePath));
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
    }
    delete diag;
}

void StartupDialog::ShowDesignDialog()
{
    DesignDialog *diag;
    diag = new DesignDialog;
    if (diag->exec() == QDialog::Accepted) {
        //設定を保存
        diag->Export();
    }
    delete diag;
}
