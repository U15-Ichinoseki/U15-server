#ifndef CLIENTSETTINGFORM_H
#define CLIENTSETTINGFORM_H

#include <QFile>
#include <QDir>
#include <QFileDialog>
#include <QProcess>
#include <QRandomGenerator>

#include <QGroupBox>

#include "TcpClient.h"
#include "ComClient.h"

#include "ManualClient.h"

namespace Ui {
class ClientSettingForm;
}

class ClientSettingForm : public QGroupBox
{
    Q_OBJECT

private:
    Ui::ClientSettingForm *ui;

    QString botcommand = "./Bot/bot.exe";
    QString pythoncommand = "../WinPython/python/python.exe";
    QString programpath = "../CHaser";

public:
    BaseClient* client;

    explicit ClientSettingForm(QWidget *parent = 0);
    ~ClientSettingForm();

    void setProgramFile(QString path, QString file);
public slots:

    void reset(QString combotext, QString pythontext);
    void SetStandby ();
    void Connected  ();
    void DisConnected();
    void ConnectionToggled(bool state);
    void ComboBoxChenged(QString text);
    void SetPortSpin(int num);

signals:
    void Standby(ClientSettingForm* client,bool complate);//準備完了
private slots:
    void PushedProgramSelect();

private:
    QProcess *botProcess;

    void resetCombBox();
};

#endif // CLIENTSETTINGFORM_H
