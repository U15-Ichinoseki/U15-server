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

public:
    BaseClient* client;

    explicit ClientSettingForm(QWidget *parent = 0);
    ~ClientSettingForm();

public slots:

    void SetStandby ();
    void Connected  ();
    void DisConnected();
    void ConnectionToggled(bool state);
    void ComboBoxChenged(QString text);
    void SetPortSpin(int num);

signals:
    void Standby(ClientSettingForm* client,bool complate);//準備完了
private:
    Ui::ClientSettingForm *ui;
    QProcess *botProcess;
};

#endif // CLIENTSETTINGFORM_H
