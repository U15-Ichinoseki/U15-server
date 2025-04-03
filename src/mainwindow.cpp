#include "mainwindow.h"
#include "ui_mainwindow.h"

QString MainWindow::getCurrentTime()
{
    return QString("[") + QDateTime::currentDateTime().toString("yyyyMMddhhmmss") + QString("]");
}

QString MainWindow::convertMethodToString(GameSystem::Method method)
{
    QString str;
    if(method.action == GameSystem::Method::ACTION::GETREADY) str += "GetReady";
    if(method.action == GameSystem::Method::ACTION::LOOK) str += "Look";
    if(method.action == GameSystem::Method::ACTION::PUT) str += "Put";
    if(method.action == GameSystem::Method::ACTION::SEARCH) str += "Search";
    if(method.action == GameSystem::Method::ACTION::WALK) str += "Walk";

    if(method.rote == GameSystem::Method::ROTE::UP) str += "Up";
    if(method.rote == GameSystem::Method::ROTE::RIGHT) str += "Right";
    if(method.rote == GameSystem::Method::ROTE::LEFT) str += "Left";
    if(method.rote == GameSystem::Method::ROTE::DOWN) str += "Down";

    return str;
}

void MainWindow::keyPressEvent([[maybe_unused]] QKeyEvent *event)
{
    if(event->key() == Qt::Key_F){
        if (this->isFullScreen()) {
            this->showNormal();
        } else {
            this->showFullScreen();
        }
    }
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    currentround = 0;
    ui->setupUi(this);
    this->startupDialog = new StartupDialog(this);

    //Functorベースでは動作しないため、Stringベースでconnect
    connect(this, SIGNAL(destroyed()), this, SLOT(saveToFile()));
    //connect(this, &MainWindow::destroyed, this, &MainWindow::saveToFile);

    initializeSettings();
    applyDesign();
    setRandomMapParameters();
    setPaths();

    soundEffectCool = new QMediaPlayer;
    soundEffectAudioOutputCool = new QAudioOutput;
    soundEffectCool->setAudioOutput(soundEffectAudioOutputCool);
    soundEffectCool->setSource(QUrl("qrc:/Sound/get_C.mp3"));
    soundEffectAudioOutputCool->setVolume(0.2);
    
    soundEffectHot = new QMediaPlayer;
    soundEffectAudioOutputHot = new QAudioOutput;
    soundEffectHot->setAudioOutput(soundEffectAudioOutputHot);
    soundEffectHot->setSource(QUrl("qrc:/Sound/get_H.mp3"));
    soundEffectAudioOutputHot->setVolume(0.2);

    clock = new QTimer();
    connect(clock, &QTimer::timeout, this, &MainWindow::stepGame);

    startupMapAnimation = new QTimer();
    connect(startupMapAnimation, &QTimer::timeout, this, &MainWindow::startAnimation);

    teamShowAnimation = new QTimer();
    connect(teamShowAnimation, &QTimer::timeout, this, &MainWindow::showTeamAnimation);

    this->startupDialog->setCommandLineOptions();

    this->ui->Field->setMap(this->startupDialog->map);

    resetSetup();

    this->startupDialog->show();
}

template<typename T>
T loadSettingValue(QSettings* settings, const QString& key, const T& defaultValue) {
    QVariant v = settings->value(key);
    if (v.typeId() != QMetaType::UnknownType) {
        return v.value<T>();
    }
    return defaultValue;
}

void MainWindow::initializeSettings()
{    
    QSettings* Settings = new QSettings("setting.ini", QSettings::IniFormat);

    FRAME_RATE = loadSettingValue(Settings, "Gamespeed", 60);
    isDoubleGameMode = loadSettingValue(Settings, "DoubleMode", false);
    isRepeatMode = loadSettingValue(Settings, "Repeat", false);
}

void MainWindow::applyDesign()
{    
    QSettings* Settings = new QSettings("setting.ini", QSettings::IniFormat);
    
    Settings->beginGroup("Design");
    mapAnimationTime = loadSettingValue(Settings, "Map", 150);
    teamAnimationTime = loadSettingValue(Settings, "Team", 150);
    silent = loadSettingValue(Settings, "Silent", false);
    if (loadSettingValue(Settings, "Maximum", false)) {
        setWindowState(Qt::WindowMaximized);
    }
    dark = loadSettingValue(Settings, "Dark", false);
    isDemoMode = loadSettingValue(Settings, "DemoMode", false);
    isBotBattleMode = loadSettingValue(Settings, "Bot", false);
    Settings->endGroup();
}

void MainWindow::setRandomMapParameters()
{    
    QSettings* Settings = new QSettings("setting.ini", QSettings::IniFormat);
    
    Settings->beginGroup("RandomMap");
    randomMapParameters.defaultItem = loadSettingValue(Settings, "ItemNum", 51);
    randomMapParameters.defaultBlock = loadSettingValue(Settings, "BlockNum", 20);
    randomMapParameters.defaultTurn = loadSettingValue(Settings, "TurnNum", 120);
    randomMapParameters.defaultMirror = loadSettingValue(Settings, "Mirror", true);
    Settings->endGroup();
}

void MainWindow::setPaths()
{    
    QSettings* Settings = new QSettings("setting.ini", QSettings::IniFormat);
    
    Settings->beginGroup("Path");
    QString path = loadSettingValue(Settings, "LogFilepath", QString("../Logs"));
    QDir dir(path);
    if (!dir.exists()) dir.mkpath(".");

    logStream = StableLog(path + "/log" + getCurrentTime() + ".txt");
    
    this->startupDialog->setBotCommand(loadSettingValue(Settings, "BotCommand", QString("./Bot/bot.exe")));
    this->startupDialog->setPythonCommand(loadSettingValue(Settings, "PythonCommand", QString("../WinPython/python/python.exe")));
    this->startupDialog->setProgramPath(loadSettingValue(Settings, "ProgramFilePath", QString("../CHaser")));
    this->startupDialog->setMapPath(loadSettingValue(Settings, "MapFilePath", QString("../Map")));
    Settings->endGroup();
}

void MainWindow::setMusicList( QString filename)
{
    this->musicList[(this->currentround + 1) % 2] = filename;
}

void MainWindow::setMusicList( QString filename, int round)
{
    musicList[round] = filename;
}

void MainWindow::setMusicList( QString filename1,  QString filename2)
{
    musicList[0] = filename1;
    musicList[1] = filename2;
}

void MainWindow::saveToFile()
{
    logFile->close();
}

void MainWindow::repaintMap()
{
    ui->Field->setMap(this->startupDialog->map);
    ui->Field->resizeImage();
    ui->Field->repaint();
}

MainWindow::~MainWindow()
{
    delete ui;
}
