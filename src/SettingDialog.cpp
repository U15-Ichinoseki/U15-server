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

    QVariant v;
  
    v = Settings->value("Timeout");
    if (v.typeId() != QMetaType::UnknownType)
        ui->Timeout->setValue(v.toInt());

    v = Settings->value("Gamespeed");
    if (v.typeId() != QMetaType::UnknownType)
        ui->Gamespeed->setValue(v.toInt());

    v = Settings->value("FullMode");
    if (v.typeId() != QMetaType::UnknownType)
        ui->FullModeCheck->setChecked(v.toBool());

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

    v = Settings->value("DefaultBGM");
    if (v.typeId() != QMetaType::UnknownType)
        ui->DefaultBGM->setCurrentText(v.toString());
    else
        ui->DefaultBGM->setCurrentText("TwinBeeNew.wav");
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
    Settings->setValue("FullMode", ui->FullModeCheck->isChecked());
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
    Settings->setValue("DefaultBGM", ui->DefaultBGM->currentText());
    Settings->setValue("DefaultTexture", ui->DefaultTexture->currentText());
    Settings->endGroup();
}

void SettingDialog::openDirectory()
{
    ui->Log->setText(QFileDialog::getExistingDirectory(this, tr("ログファイル保存先を選択")));
}

SettingDialog::~SettingDialog()
{
    delete ui;
}
