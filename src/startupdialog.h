#ifndef STARTUPDIALOG_H
#define STARTUPDIALOG_H

#include <QTimer>
#include <QDialog>
#include <QMessageBox>

#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QHostInfo>
#include <QNetworkInterface>

#include <QCommandLineParser>

#include "ClientSettingForm.h"
#include "MapEditerDialog.h"

#include "SettingDialog.h"
#include "DesignDialog.h"

#include "mainwindow.h"

namespace Ui {
class StartupDialog;
}

class StartupDialog : public QDialog
{
    Q_OBJECT

private:
    Ui::StartupDialog *ui;
    bool team_standby[TEAM_COUNT];
    bool map_standby;

public:
    QString music_text;
    GameSystem::Map map;

    ClientSettingForm* team_client[TEAM_COUNT];

public:
    bool MapRead(const QString& dir);
    void setMusicFileList();
    void setImageThemeList();

    explicit StartupDialog(QWidget *parent = 0);
    ~StartupDialog();

public slots:

    void ChangeMusicCombo(QString text);
    void CheckStandby();
    void ShowMapEditDialog();
    void PushedMapSelect();
    void ShowDesignDialog();

    void ClientStandby(ClientSettingForm* client,bool complate);
    void SetMapStandby (bool state);

    void ChangedTexture(QString text);

    //設定
    void Setting();

};

#endif // STARTUPDIALOG_H
