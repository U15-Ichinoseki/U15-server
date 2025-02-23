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

    QString botcommand;
    QString pythoncommand;
    QString programpath;

public:
    BaseClient* client;

    explicit ClientSettingForm(QWidget *parent);
    ~ClientSettingForm();

    void setBotCommand(QString command);
    void setPythonCommand(QString command);
    void setProgramFile(QString path, QString file);

    void setChangeEnable(bool set);

public slots:

    void reset(QString combotext, QString pythontext);
    void SetStandby ();
    void Connected  ();
    void DisConnected();
    void ConnectionToggled(bool state);
    void ComboBoxChenged(QString text);
    void SetPortSpin(int num);
    QString getPlayer();
    QString getProgramFile();
    QString getRandomProgramFile();

signals:
    void Standby(ClientSettingForm* client,bool complate);//準備完了
private slots:
    void PushedProgramSelect();

private:
    QProcess *botProcess;

    void resetCombBox();
};

#endif // CLIENTSETTINGFORM_H
