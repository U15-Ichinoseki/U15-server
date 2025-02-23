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

public:
    explicit SettingDialog(QWidget *parent = nullptr);
    ~SettingDialog();

    void Export();

public slots:
    void openDirectory();

};

#endif // SETTINGDIALOG_H
