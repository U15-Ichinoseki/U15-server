#include "SettingDialog.h"
#include "ui_SettingDialog.h"

SettingDialog::SettingDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SettingDialog)
{
    QSettings *Settings;
    Settings = new QSettings("setting.ini", QSettings::IniFormat); // iniファイルで設定を保存
    // Settings->setIniCodec( "UTF-8" ); // iniファイルの文字コード
    ui->setupUi(this);

    ui->tabWidget->setCurrentIndex(0);

    setProgramFileList();
    setMapFileList();
    setMusicFileList();
    setImageThemeList();

    QVariant v;
  
    v = Settings->value("Timeout");
    if (v.typeId() != QMetaType::UnknownType)
        ui->Timeout->setValue(v.toInt());

    v = Settings->value("Gamespeed");
    if (v.typeId() != QMetaType::UnknownType)
        ui->Gamespeed->setValue(v.toInt());

    v = Settings->value("DoubleMode");
    if (v.typeId() != QMetaType::UnknownType)
        ui->DoubleModeCheck->setChecked(v.toBool());

    v = Settings->value("Repeat");
    if (v.typeId() != QMetaType::UnknownType)
        ui->RepeatCheck->setChecked(v.toBool());


    Settings->beginGroup("Design");
    v = Settings->value("Map");
    if (v.typeId() != QMetaType::UnknownType)
        ui->Mapspeed->setValue(v.toInt());

    v = Settings->value("Team");
    if (v.typeId() != QMetaType::UnknownType)
        ui->Teamspeed->setValue(v.toInt());    

    v = Settings->value("Silent");
    if (v.typeId() != QMetaType::UnknownType)
        ui->SilentCheck->setChecked(v.toBool());

    v = Settings->value("Maximum");
    if (v.typeId() != QMetaType::UnknownType)
        ui->MaximumCheck->setChecked(v.toBool());

    v = Settings->value( "Dark" );
    if (v.typeId() != QMetaType::UnknownType)
    {
        ui->DarkBox->setChecked(v.toBool());
    }

    v = Settings->value("DemoMode");
    if (v.typeId() != QMetaType::UnknownType)
        ui->DemoCheck->setChecked(v.toBool());

    v = Settings->value( "Bot" );
    if (v.typeId() != QMetaType::UnknownType){
        ui->BotBox->setChecked(v.toBool());
    }
    Settings->endGroup();
    
    Settings->beginGroup("RandomMap");
    v = Settings->value("ItemNum");
    if (v.typeId() != QMetaType::UnknownType)
        ui->Default_ItemNum->setValue(v.toInt());

    v = Settings->value("BlockNum");
    if (v.typeId() != QMetaType::UnknownType)
        ui->Default_BlockNum->setValue(v.toInt());    

    v = Settings->value("TurnNum");
    if (v.typeId() != QMetaType::UnknownType)
        ui->Default_TurnNum->setValue(v.toInt());    

    v = Settings->value("Mirror");
    if (v.typeId() != QMetaType::UnknownType)
        ui->MirrorMap->setChecked(v.toBool());    
    Settings->endGroup();

    Settings->beginGroup("Path");
    v = Settings->value("LogFilepath");
    if (v.typeId() != QMetaType::UnknownType)
        ui->Log->setText(v.toString());

    v = Settings->value("BotCommand");
    if (v.typeId() != QMetaType::UnknownType)
        ui->Bot->setText(v.toString());

    v = Settings->value("PythonCommand");
    if (v.typeId() != QMetaType::UnknownType)
        ui->Python->setText(v.toString());

    v = Settings->value("ProgramFilePath");
    if (v.typeId() != QMetaType::UnknownType)
        ui->Program->setText(v.toString());

    v = Settings->value("MapFilePath");
    if (v.typeId() != QMetaType::UnknownType)
        ui->Map->setText(v.toString());
    Settings->endGroup();

    Settings->beginGroup("Default");
    v = Settings->value("CoolProgram");
    if (v.typeId() != QMetaType::UnknownType)
        ui->CoolProgram->setCurrentText(v.toString());

    v = Settings->value("HotProgram");
    if (v.typeId() != QMetaType::UnknownType)
        ui->HotProgram->setCurrentText(v.toString());

    v = Settings->value("DefaultMap");
    if (v.typeId() != QMetaType::UnknownType)
        ui->DefaultMap->setCurrentText(v.toString());

    v = Settings->value("DefaultTexture");
    if (v.typeId() != QMetaType::UnknownType)
        ui->DefaultTexture->setCurrentText(v.toString());
    else
        ui->DefaultTexture->setCurrentText("あっさり");

    v = Settings->value("DefaultBGM_1");
    if (v.typeId() != QMetaType::UnknownType)
        ui->DefaultBGM_1->setCurrentText(v.toString());
    else
        ui->DefaultBGM_1->setCurrentText("TwinBeeNew.wav");

    v = Settings->value("DefaultBGM_2");
    if (v.typeId() != QMetaType::UnknownType)
        ui->DefaultBGM_2->setCurrentText(v.toString());
    else
        ui->DefaultBGM_2->setCurrentText("Necromancer.wav");
    Settings->endGroup();
}

void SettingDialog::Export()
{
    QSettings *Settings;
    Settings = new QSettings("setting.ini", QSettings::IniFormat); // iniファイルで設定を保存
    // Settings->setIniCodec( "UTF-8" ); // iniファイルの文字コード

    // Settings->beginGroup("General");
    Settings->setValue("Timeout", ui->Timeout->value());
    Settings->setValue("Gamespeed", ui->Gamespeed->value());
    Settings->setValue("DoubleMode", ui->DoubleModeCheck->isChecked());
    Settings->setValue("Repeat", ui->RepeatCheck->isChecked());
    // Settings->endGroup();

    Settings->beginGroup("Design");
    Settings->setValue("Map", ui->Mapspeed->value());
    Settings->setValue("Team", ui->Teamspeed->value());
    Settings->setValue("Silent", ui->SilentCheck->isChecked());
    Settings->setValue("Maximum", ui->MaximumCheck->isChecked());
    Settings->setValue( "Dark"    , ui->DarkBox->isChecked());
    Settings->setValue("DemoMode", ui->DemoCheck->isChecked());
    Settings->setValue( "Bot"     , ui->BotBox->isChecked());
    Settings->endGroup();

    Settings->beginGroup("RandomMap");
    Settings->setValue("ItemNum", ui->Default_ItemNum->value());
    Settings->setValue("BlockNum", ui->Default_BlockNum->value());
    Settings->setValue("TurnNum", ui->Default_TurnNum->value());
    Settings->setValue("Mirror", ui->MirrorMap->isChecked());
    Settings->endGroup();
    
    Settings->beginGroup("Path");
    Settings->setValue("LogFilepath", ui->Log->text());
    Settings->setValue("BotCommand", ui->Bot->text());
    Settings->setValue("PythonCommand", ui->Python->text());
    Settings->setValue("ProgramFilePath", ui->Program->text());
    Settings->setValue("MapFilePath", ui->Map->text());
    Settings->endGroup();

    Settings->beginGroup("Default");
    Settings->setValue("CoolProgram", ui->CoolProgram->currentText());
    Settings->setValue("HotProgram", ui->HotProgram->currentText());
    Settings->setValue("DefaultMap", ui->DefaultMap->currentText());
    Settings->setValue("DefaultTexture", ui->DefaultTexture->currentText());
    Settings->setValue("DefaultBGM_1", ui->DefaultBGM_1->currentText());
    Settings->setValue("DefaultBGM_2", ui->DefaultBGM_2->currentText());
    Settings->endGroup();
}

void SettingDialog::setProgramFileList()
{
    QDir dir(ui->Program->text());

    ui->CoolProgram->clear();
    ui->HotProgram->clear();
    ui->CoolProgram->addItem("None");
    ui->HotProgram->addItem("None");
    if (dir.exists()) {
        QStringList filelist = dir.entryList({"*.py"}, QDir::Files | QDir::NoSymLinks);
        if (! filelist.isEmpty()) {
            ui->CoolProgram->addItems(filelist);
            ui->HotProgram->addItems(filelist);
        }
    } 
}

void SettingDialog::setMapFileList()
{
    QDir dir(ui->Map->text());

    ui->DefaultMap->clear();
    ui->DefaultMap->addItem("RandomMap");
    if (dir.exists()) {
        QStringList filelist = dir.entryList({"*.map"}, QDir::Files | QDir::NoSymLinks);
        if (! filelist.isEmpty()) {
            ui->DefaultMap->addItems(filelist);
        }
    }
}

void SettingDialog::setMusicFileList()
{
    QDir dir("./Music");

    ui->DefaultBGM_1->clear();
    ui->DefaultBGM_2->clear();
    if (dir.exists()) { //ディレクトリが存在していたらmp3とwavのファイルをリストに追加する
        QStringList filelist = dir.entryList({"*.mp3", "*.wav"}, QDir::Files | QDir::NoSymLinks);
        if (filelist.isEmpty()) { //ディレクトリが存在していても、mp3とwavのファイルがなければ、Noneにして無効化
            ui->DefaultBGM_1->addItem("None");
            ui->DefaultBGM_2->addItem("None");
            ui->DefaultBGM_1->setEnabled(false);
            ui->DefaultBGM_2->setEnabled(false);
        } else {
            ui->DefaultBGM_1->addItems(filelist);
            ui->DefaultBGM_2->addItems(filelist);
        }
    } else { //なかったらNoneにして無効化
        ui->DefaultBGM_1->addItem("None");
        ui->DefaultBGM_2->addItem("None");
        ui->DefaultBGM_1->setEnabled(false);
        ui->DefaultBGM_2->setEnabled(false);
    }
}

void SettingDialog::setImageThemeList()
{
    QDir dir("./Image");

    ui->DefaultTexture->clear();
    ui->DefaultTexture->addItems({"ほうせき", "あっさり", "こってり", "RPG"}); //デフォルトの4テーマの追加

    if (dir.exists()) { //ディレクトリが存在していたら
        QStringList filelist = dir.entryList(QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot);
        ui->DefaultTexture->addItems(filelist);
    }
}

void SettingDialog::openDirectory()
{
    QString directory = QFileDialog::getExistingDirectory(this, tr("ログファイル保存先を選択"));
    if (!directory.isEmpty()) 
       ui->Log->setText(directory);
}

void SettingDialog::openProgramDirectory()
{
    QString directory = QFileDialog::getExistingDirectory(this, tr("プログラム保存先を選択"));
    if (!directory.isEmpty()) 
       ui->Program->setText(directory);    
}

void SettingDialog::openMapDirectory()
{
    QString directory = QFileDialog::getExistingDirectory(this, tr("マップファイル保存先を選択"));
    if (!directory.isEmpty()) 
       ui->Map->setText(directory);
}

void SettingDialog::openPythonCommand()
{
    QString folder = QDir::currentPath();
    QString cap = tr("Pythonコマンドを開く");
    QString filter = tr("Pythonコマンド (python.exe)");

    QString pythoncommand = QFileDialog::getOpenFileName(this, cap, folder, filter);
    if(QFile::exists(pythoncommand))
        ui->Python->setText(pythoncommand);
}

void SettingDialog::openBotCommand()
{
    QString folder = QDir::currentPath();
    QString cap = tr("ボットコマンドを開く");
    QString filter = tr("ボットコマンド (bot*.exe)");

    QString botcommand = QFileDialog::getOpenFileName(this, cap, folder, filter);
    if(QFile::exists(botcommand))
        ui->Bot->setText(botcommand);
}

SettingDialog::~SettingDialog()
{
    delete ui;
}
