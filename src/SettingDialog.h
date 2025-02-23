#ifndef SETTINGDIALOG_H
#define SETTINGDIALOG_H

#include <QFileDialog>
#include <QSettings>

#include <QDialog>
#include <QMessageBox>

namespace Ui {
class SettingDialog;
}

class SettingDialog : public QDialog
{
    Q_OBJECT
private:
    Ui::SettingDialog *ui;

    void setProgramFileList();
    void setMapFileList();
    void setMusicFileList();
    void setImageThemeList();

public:
    explicit SettingDialog(QWidget *parent = nullptr);
    ~SettingDialog();

    void Export();

public slots:
    void openDirectory();
    void openProgramDirectory();
    void openMapDirectory();
    void openPythonCommand();
    void openBotCommand();
};

#endif // SETTINGDIALOG_H
