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

    bool teamReady[TEAM_COUNT];
    bool isMapReady;

    QString programPath;
    QString mapPath;

    void initializeClientGroupBox(ClientSettingForm* groupBox, int port);
    void setGroupBoxProgram(ClientSettingForm* groupBox, QCommandLineParser& parser, QCommandLineOption& option, QString defaultProgramKey);

public:
    QString musicText;
    GameSystem::Map map;

    ClientSettingForm* teamClient[TEAM_COUNT];
    MainWindow* mainWindow;

public:
    explicit StartupDialog(MainWindow *parent = 0);
    ~StartupDialog();

    bool loadMap(const QString& filePath);
    void setMusicFileList();
    void setImageThemeList();

    void setGameStartButtonEnabled(bool set);
    void showGameStartButton(bool enabled);
    void enableMusicSelection(bool set);
    void gameStartButtonClick();
    void showStandbyButton(bool set);
    void enableSetupMode(bool set);
    void enableConnectionChange(bool set);

    void resetMap(int block_num, int item_num, int turn, bool mirror);
    void swapClientConnections();
    void randomizeClientConnections();
    void setGameStartButtonToEnd(bool repeat);

    void setGameMusicCombo(const QString &text);
    void setBotCommand(const QString &command);
    void setPythonCommand(const QString &command);
    void setProgramPath(const QString &path);
    void setMapPath(const QString &path);

    void setCommandLineOptions();
    QVariant getDefaultSetting(const QString &key);



public slots:

    void MusicComboChanged(const QString &text);
    void checkAllClientsReady();
    void showMapEditDialog();
    void MapSelectButtonClicked();

    void ClientStandby(ClientSettingForm* client,bool complate);
    void setMapReady (bool state);

    void TextureChanged(const QString &text);

    //設定
    void openSettingsDialog();

};

#endif // STARTUPDIALOG_H
