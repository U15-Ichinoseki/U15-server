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

#include "mainwindow.h"

namespace Ui {
class StartupDialog;
}

class MainWindow;

class StartupDialog : public QDialog
{
    Q_OBJECT

private:
    Ui::StartupDialog *ui;
    MainWindow *parent;

    bool team_standby[TEAM_COUNT];
    bool map_standby;

    QString programpath;
    QString mappath;

public:
    QString music_text;
    GameSystem::Map map;

    ClientSettingForm* team_client[TEAM_COUNT];

public:
    explicit StartupDialog(MainWindow *parent = 0);
    ~StartupDialog();

    bool MapRead(const QString& dir);
    void setMusicFileList();
    void setImageThemeList();

    void setGameStartButtonEnabled(bool set);
    void setBotCommand(QString command);
    void setPythonCommand(QString command);
    void setProgramPath(QString path);
    void setMapPath(QString path);
    void setCommandLineOptions();
    QVariant getDefault(QString key);

public slots:

    void ChangeMusicCombo(QString text);
    void CheckStandby();
    void ShowMapEditDialog();
    void PushedMapSelect();

    void ClientStandby(ClientSettingForm* client,bool complate);
    void SetMapStandby (bool state);

    void ChangedTexture(QString text);

    //設定
    void Setting();

};

#endif // STARTUPDIALOG_H
