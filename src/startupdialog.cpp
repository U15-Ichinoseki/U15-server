#include "startupdialog.h"
#include "ui_startupdialog.h"

StartupDialog::StartupDialog(MainWindow *parent)
    : QDialog(parent)
    , ui(new Ui::StartupDialog)
    , isMapReady(false)
{
    this->parent = parent;

    //UI初期化
    ui->setupUi(this);
    
    setMusicFileList();
    setImageThemeList();

    initializeClientGroupBox(ui->CoolGroupBox, 2009);
    initializeClientGroupBox(ui->HotGroupBox, 2010);

    //クライアント初期化
    this->teamClient[static_cast<int>(GameSystem::TEAM::COOL)] = ui->CoolGroupBox;
    this->teamClient[static_cast<int>(GameSystem::TEAM::HOT)] = ui->HotGroupBox;
    for (int i = 0; i < TEAM_COUNT; i++) {
        teamReady[i] = false;
    }

    connect(ui->HotGroupBox,  &ClientSettingForm::Standby, this, &StartupDialog::ClientStandby);
    connect(ui->CoolGroupBox, &ClientSettingForm::Standby, this, &StartupDialog::ClientStandby);

    //ローカルIPの探索
    foreach (const QHostAddress &address, QNetworkInterface::allAddresses()) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress(QHostAddress::LocalHost)){
            this->ui->localIPLabel->setText(address.toString());
            break;
        }
    }
    this->ui->hostName->setText(QHostInfo::localHostName());

    map.createRandomMap();
    isMapReady = true;

    connect(this->ui->standbyButton,SIGNAL(clicked()),  parent,SLOT(startSetup()));
    connect(this->ui->gameStartButton,SIGNAL(clicked()),parent,SLOT(startGame()));
}

void StartupDialog::initializeClientGroupBox(ClientSettingForm* groupBox, int port)
{
    groupBox->setPortSpin(port);
    groupBox->setConnectionEnabled(true);
}

QVariant StartupDialog::getDefaultSetting(const QString &key)
{
    QSettings *settings;
    settings = new QSettings("setting.ini", QSettings::IniFormat); // iniファイルで設定を保存
    settings->beginGroup("Default");
    return settings->value(key);
}

void StartupDialog::setCommandLineOptions()
{
    QVariant v;

    QCommandLineParser parser;
    // アプリの説明
    parser.setApplicationDescription("CHaser server");
    parser.addHelpOption();

    QCommandLineOption randomMapOption({"r", "random"}, "ランダムマップ");
    parser.addOption(randomMapOption);

    QCommandLineOption coolOption("cool", "Coolプログラムファイル", "program file");
    parser.addOption(coolOption);

    QCommandLineOption hotOption("hot", "Hotプログラムファイル", "program file");
    parser.addOption(hotOption);

    v = getDefaultSetting("DefaultMap");
    QCommandLineOption mapOption({"m", "map"}, "マップファイル", "map",         
                (v.typeId() != QMetaType::UnknownType) ? v.toString() : "Default.map" 
        );
    parser.addOption(mapOption);

    v = getDefaultSetting("DefaultTexture");
    QCommandLineOption textureOption({"t", "texture"}, "テクスチャ", "texture",     
                (v.typeId() != QMetaType::UnknownType) ? v.toString() : "あっさり" 
        );
    parser.addOption(textureOption);

    v = getDefaultSetting("DefaultBGM_1");
    QCommandLineOption bgm1Option({"b", "b1", "bgm", "bgm1"}, "BGM1", "bgm1",            
                (v.typeId() != QMetaType::UnknownType) ? v.toString() : "TwinBeeNew.wav" 
        );
    parser.addOption(bgm1Option);

    v = getDefaultSetting("DefaultBGM_2");
    QCommandLineOption bgm2Option({"b2", "bgm2"}, "BGM2", "bgm2",            
                (v.typeId() != QMetaType::UnknownType) ? v.toString() : "TwinBeeNew.wav" 
        );
    parser.addOption(bgm2Option);

    parser.process(QCoreApplication::arguments());
    
    setGroupBoxProgram(ui->CoolGroupBox, parser, coolOption, "CoolProgram");
    setGroupBoxProgram(ui->HotGroupBox, parser, hotOption, "HotProgram");

    if(! parser.isSet(randomMapOption)) {
        QString filePath;
        filePath = mapPath + "/" + parser.value(mapOption);
        if (QFile::exists(filePath)) {
            this->ui->mapDirEdit->setText(filePath);
            isMapReady = loadMap(filePath);
        }
    }

    parent->setMusicList(parser.value(bgm1Option), 0);
    setGameMusicCombo(parser.value(bgm1Option));
    musicText = ui->gameMusicCombo->currentText();

    parent->setMusicList(parser.value(bgm2Option), 1);

    this->ui->textureThemeCombo->setCurrentText(parser.value(textureOption));
}

void StartupDialog::setGroupBoxProgram(ClientSettingForm* groupBox, QCommandLineParser& parser, QCommandLineOption& option, QString defaultProgramKey)
{
    QVariant v;
    if(parser.value(option) != "")
        groupBox->reset("Python", parser.value(option));
    else {
        v = getDefaultSetting(defaultProgramKey);
        if (v.typeId() != QMetaType::UnknownType && v.toString() != "None"){
            groupBox->setProgramFile(programPath, v.toString());
        } else {
            groupBox->setProgramFile(programPath, "player.py");
        }
    }
}

StartupDialog::~StartupDialog()
{
    delete ui;
}

void StartupDialog::resetMap(int blockNum, int itemNum, int turn, bool mirror)
{
    if(this->ui->mapDirEdit->text() == "")
    {
        map.createRandomMap(blockNum, itemNum, turn, mirror);
        setMapReady(true);
    }
}

void StartupDialog::MusicComboChanged(const QString &text)
{
    musicText = text;
    parent->setMusicList(musicText);
}

void StartupDialog::checkAllClientsReady()
{
    bool allOf = true;
    for (int i = 0; i < TEAM_COUNT; i++) {
        if (!teamReady[i])
            allOf = false;
    }
    this->ui->standbyButton->setEnabled(allOf && isMapReady);
}

void StartupDialog::setGameStartButtonEnabled(bool set)
{
    this->ui->gameStartButton->setEnabled(set);
}

void StartupDialog::gameStartButtonClick()
{
    this->ui->gameStartButton->click();
}

void StartupDialog::showGameStartButton(bool set)
{
    if (set) {
        disconnect(this->ui->gameStartButton,SIGNAL(clicked()),this->parent,SLOT(startGame()));
        disconnect(this->ui->gameStartButton,SIGNAL(clicked()),this->parent,SLOT(repeatGame()));
        this->ui->standbyButton->show();
        this->ui->gameStartButton->show();
        this->ui->gameStartButton->setText("スタート");
        this->ui->gameStartButton->setEnabled(false);
        connect(this->ui->gameStartButton,SIGNAL(clicked()),this->parent,SLOT(startGame()));
    } else {
        this->ui->standbyButton->hide();
        this->ui->gameStartButton->hide();
    }
}

void StartupDialog::setGameStartButtonToEnd(bool repeat)
{
    this->ui->standbyButton->hide();
    this->ui->gameStartButton->show();
    this->ui->gameStartButton->setEnabled(true);
    disconnect(this->ui->gameStartButton,SIGNAL(clicked()),this->parent,SLOT(startGame()));
    if (repeat) {
        this->ui->gameStartButton->setText("再戦");
        connect(this->ui->gameStartButton,SIGNAL(clicked()),this->parent,SLOT(repeatGame()));
    } else {
        this->ui->gameStartButton->setText("終了");
        connect(this->ui->gameStartButton,SIGNAL(clicked()),this->parent,SLOT(endGame()));
    }
}

void StartupDialog::showStandbyButton(bool set)
{
    if (set) {
        this->ui->standbyButton->show();
    } else {
        this->ui->standbyButton->hide();
    }
}

void StartupDialog::enableSetupMode(bool set)
{
    if (set) {
        ui->mapDirEdit->setReadOnly(false);
        ui->mapEditButton->setEnabled(true);
        ui->mapSelectButton->setEnabled(true);
        ui->textureThemeCombo->setEnabled(true);
    } else {
        ui->mapDirEdit->setReadOnly(true);
        ui->mapEditButton->setEnabled(false);
        ui->mapSelectButton->setEnabled(false);
        ui->textureThemeCombo->setEnabled(false);
    }
}

void StartupDialog::enableMusicSelection(bool set)
{
    if (set) {
        ui->gameMusicCombo->setEnabled(true);
    } else {
        ui->gameMusicCombo->setEnabled(false);
    }
}

void StartupDialog::swapClientConnections()
{
    QString coolPlayer = this->ui->CoolGroupBox->getPlayer();
    QString coolProgramFile = this->ui->CoolGroupBox->getProgramFile();

    QString hotPlayer = this->ui->HotGroupBox->getPlayer();
    QString hotProgramFile = this->ui->HotGroupBox->getProgramFile();

    this->ui->CoolGroupBox->reset(hotPlayer, hotProgramFile);
    this->ui->HotGroupBox->reset(coolPlayer, coolProgramFile);
}

void StartupDialog::randomizeClientConnections()
{
    QString coolPlayer = this->ui->CoolGroupBox->getPlayer();
    QString coolRandomProgramFile = this->ui->CoolGroupBox->getRandomProgramFile();

    QString hotPlayer = this->ui->HotGroupBox->getPlayer();
    QString hotRandomProgramFile = this->ui->HotGroupBox->getRandomProgramFile();

    this->ui->CoolGroupBox->reset(hotPlayer, coolRandomProgramFile);
    this->ui->HotGroupBox->reset(coolPlayer, hotRandomProgramFile);
}

void StartupDialog::showMapEditDialog()
{
    MapEditerDialog diag(map, mapPath);
    if (diag.exec()) {
        if (diag.filepath == "") {
            this->ui->mapDirEdit->setText("[CUSTOM MAP]");
            map = diag.getMap();
        } else {
            //再読み込み
            if (loadMap(diag.filepath))
                this->ui->mapDirEdit->setText(diag.filepath);
        }
    }
    setMapReady(true);
}

bool StartupDialog::loadMap(const QString &dir)
{
    //ファイルからマップを読み込む
    this->map.Import(dir);
    map.size.setY(map.field.size());
    return true;
}

void StartupDialog::setMusicFileList()
{
    QDir dir("./Music");

    ui->gameMusicCombo->clear();
    if (dir.exists()) { //ディレクトリが存在していたらmp3とwavのファイルをリストに追加する
        QStringList filelist = dir.entryList({"*.mp3", "*.wav"}, QDir::Files | QDir::NoSymLinks);
        if (filelist.isEmpty()) { //ディレクトリが存在していても、mp3とwavのファイルがなければ、Noneにして無効化
            ui->gameMusicCombo->addItem("None");
            ui->gameMusicCombo->setEnabled(false);
        } else
            ui->gameMusicCombo->addItems(filelist);
    } else { //なかったらNoneにして無効化
        ui->gameMusicCombo->addItem("None");
        ui->gameMusicCombo->setEnabled(false);
    }
}

void StartupDialog::setImageThemeList()
{
    ui->textureThemeCombo->clear();
    ui->textureThemeCombo->addItems({"ほうせき", "あっさり", "こってり", "RPG"}); //デフォルトの3テーマの追加

    QDir dir("./Image");

    if (dir.exists()) { //ディレクトリが存在していたら
        QStringList filelist = dir.entryList(QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot);
        ui->textureThemeCombo->addItems(filelist);
    }
}

void StartupDialog::MapSelectButtonClicked()
{
    QString cap = tr("マップを開く");
    QString filter = tr("マップファイル (*.map)");

    QString mapfile = QFileDialog::getOpenFileName(this, cap, mapPath, filter);
    if (QFile::exists(mapfile)) {
        this->ui->mapDirEdit->setText(mapfile);
        setMapReady(loadMap(mapfile));
    }
}

void StartupDialog::ClientStandby(ClientSettingForm *client, bool complete)
{
    for (int i = 0; i < TEAM_COUNT; i++) {
        if (teamClient[i] == client) {
            teamReady[i] = complete;
            checkAllClientsReady();
            return;
        }
    }
}

void StartupDialog::setMapReady(bool state)
{
    isMapReady = state;
    if (state) parent->repaintMap();
    checkAllClientsReady();
}

void StartupDialog::TextureChanged(const QString &text)
{
    if (text == "あっさり")      this->map.textureDirPath = ":/Image/Light";
    else if (text == "こってり") this->map.textureDirPath = ":/Image/Heavy";
    else if (text == "ほうせき") this->map.textureDirPath = ":/Image/Jewel";
    else if (text == "RPG")   this->map.textureDirPath = ":/Image/RPG";
    else                      this->map.textureDirPath = "Image/" + text;
}

void StartupDialog::openSettingsDialog()
{
    SettingDialog *diag;
    diag = new SettingDialog;
    if (diag->exec() == QDialog::Accepted) {
        //設定を保存
        diag->Export();
        parent->initializeSettings();
        parent->applyDesign();
        parent->setRandomMapParameters();
        parent->setPaths();
        parent->setMusicList(getDefaultSetting("DefaultBGM_1").toString(), 
                             getDefaultSetting("DefaultBGM_2").toString());
    }
    delete diag;
}

void StartupDialog::setGameMusicCombo(const QString &text)
{
    int index = ui->gameMusicCombo->findText(text);
    if (index != -1)
        this->ui->gameMusicCombo->setCurrentText(text);
}

void StartupDialog::setBotCommand(const QString &command)
{
    ui->CoolGroupBox->setBotCommand(command);
    ui->HotGroupBox->setBotCommand(command);
}

void StartupDialog::setPythonCommand(const QString &command)
{
    ui->CoolGroupBox->setPythonCommand(command);
    ui->HotGroupBox->setPythonCommand(command);
}

void StartupDialog::setProgramPath(const QString &path)
{
    programPath = path;
}

void StartupDialog::setMapPath(const QString &path)
{
    mapPath = path;
}

void StartupDialog::enableConnectionChange(bool set)
{
    this->ui->CoolGroupBox->setChangeEnable(set);
    this->ui->HotGroupBox->setChangeEnable(set);
}
