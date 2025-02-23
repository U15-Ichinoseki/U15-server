#include "ClientSettingForm.h"
#include "ui_ClientSettingForm.h"

ClientSettingForm::ClientSettingForm(QWidget *parent) :
    QGroupBox(parent),
    ui(new Ui::ClientSettingForm)
{
    ui->setupUi(this);
    resetCombBox();

    this->client = new TCPClient();
    connect(this->client, &TCPClient::Connected,    this, &ClientSettingForm::Connected);
    connect(this->client, &TCPClient::Ready,        this, &ClientSettingForm::SetStandby);
    connect(this->client, &TCPClient::Disconnected, this, &ClientSettingForm::DisConnected);
}

void ClientSettingForm::resetCombBox()
{
    QString currentText = this->ui->ComboBox->currentText();

    ui->ComboBox->blockSignals(true);
    ui->ComboBox->clear();
    ui->ComboBox->addItem("TCPユーザー");

    //Windowsでボットのexeファイルがあるときのみ、ボットプログラムを動かせるように設定
    #ifdef Q_OS_WINDOWS        
        if(QFile::exists(pythoncommand))
            ui->ComboBox->addItem("Python");
    #endif
    //Windowsでボットのexeファイルがあるときのみ、ボットプログラムを動かせるように設定
    #ifdef Q_OS_WINDOWS
        if(QFile::exists(botcommand))
            ui->ComboBox->addItem("ボットプログラム");
    #endif

    ui->ComboBox->addItem("CPU");
    ui->ComboBox->addItem("マニュアル");

    ui->ComboBox->setCurrentText(currentText);

    ui->ComboBox->blockSignals(false);
}

void ClientSettingForm::PushedProgramSelect()
{
    QString cap = tr("プログラムを開く");
    QString filter = tr("プログラムファイル (*.py)");

    QString programfile = QFileDialog::getOpenFileName(this, cap, programpath, filter);
    if ((QFile::exists(pythoncommand)) && (QFile::exists(programfile)))
    {
        reset("Python", programfile);
    }
}

ClientSettingForm::~ClientSettingForm()
{
    delete ui;
    delete this->client;
    delete botProcess;
}

void ClientSettingForm::SetStandby()
{
    this->ui->NameLabel ->setText(this->client->Name == "" ? "Hot" : this->client->Name);
    this->ui->IPLabel   ->setText(this->client->IP);
    this->ui->StateLabel->setText("準備完了");
    //if(this->ui->ComboBox->currentText() != "TCPユーザー");
    this->ui->ConnectButton->setText("切断");

    emit Standby(this,true);
}

void ClientSettingForm::Connected()
{
    this->ui->IPLabel      ->setText(this->client->IP);
    this->ui->StateLabel   ->setText("接続中");
    this->ui->ConnectButton->setText("切断");
}

void ClientSettingForm::DisConnected() {

    disconnect(this->client, &TCPClient::Disconnected, this, &ClientSettingForm::DisConnected);
    //TCP待機やめ
    if (dynamic_cast<TCPClient*>(this->client) != nullptr) {
        dynamic_cast<TCPClient*>(this->client)->CloseSocket();
        this->client = new TCPClient(this);
    }

    //再connectしクライアントの接続を待つ
    connect(this->client, &TCPClient::Connected,    this, &ClientSettingForm::Connected);
    connect(this->client, &TCPClient::Ready,        this, &ClientSettingForm::SetStandby);
    connect(this->client, &TCPClient::Disconnected, this, &ClientSettingForm::DisConnected);

    this->client->Startup();

    this->ui->ConnectButton->setText("接続開始");
    this->ui->StateLabel->setText("非接続");
    this->ui->NameLabel->setText("不明");
    this->ui->IPLabel->setText("不明");
    this->ui->PortSpinBox->setEnabled(true);

    //状態解除
    if (this->ui->ConnectButton->isChecked())
        this->ui->ConnectButton->toggle();

    emit Standby(this, false);
}

void ClientSettingForm::ConnectionToggled(bool state)
{
    if(state){
        //TCP待機開始
        dynamic_cast<TCPClient*>(this->client)->OpenSocket(ui->PortSpinBox->value());
        this->ui->ConnectButton->setText("待機終了");
        this->ui->StateLabel->setText("TCP接続待ち状態");
        this->ui->PortSpinBox->setEnabled(false);
    }else{
        //TCP待機やめ
        dynamic_cast<TCPClient*>(this->client)->CloseSocket();
        //this->client->Startup();

        this->ui->ConnectButton->setText("接続開始");
        this->ui->StateLabel->setText("非接続");
        this->ui->NameLabel->setText("不明");
        this->ui->IPLabel->setText("不明");
        this->ui->PortSpinBox->setEnabled(true);
        emit Standby(this,false);
    }
    if (this->ui->ConnectButton->isChecked() != state)
        this->ui->ConnectButton->toggle();
}

void ClientSettingForm::reset(QString combotext, QString pythontext)
{
    this->ui->ComboBox->setCurrentText("TCPユーザー");
    this->ComboBoxChenged("TCPユーザー");

    this->ui->ProgramFileEdit->setText(pythontext);
    this->ui->ComboBox->setCurrentText(combotext);
}

void ClientSettingForm::ComboBoxChenged(QString text)
{
    //接続初期化
    delete client;
    this->ui->ConnectButton->setChecked(false);
    if(text=="TCPユーザー"){
        this->client = new TCPClient(this);
        this->ui->StateLabel->setText("非接続");
        this->ui->NameLabel->setText("不明");
        this->ui->ConnectButton->setText("接続開始");
        this->ui->PortSpinBox->setEnabled(true);
        this->ui->ConnectButton->setEnabled(true);
        ConnectionToggled(true);
    } else {
        this->ui->PortSpinBox->setEnabled(false);
        this->ui->ConnectButton->setEnabled(false);
        if(text=="CPU")this->client = new ComClient(this);
        if(text=="マニュアル")this->client = new ManualClient(this);

        //ボットを接続
        if((text=="Python") || (text=="ボットプログラム")){
            QString command;
            QStringList option;

            //Pythonプログラムを接続
            if(text=="Python"){
                // "python.exe program.py --host 127.0.0.1 --port 2009"のような文字列を作る
                command = pythoncommand;
                option << ui->ProgramFileEdit->text();
                option << "--host" << "127.0.0.1";
                option << "--port" << QString::number(ui->PortSpinBox->value());
            }

            //ボットを接続
            if(text=="ボットプログラム"){
                // "Bot.exe a:127.0.0.1 p:2009 n:ボットプログラム"のような文字列を作る
                command = botcommand;
                option << "a:127.0.0.1"; 
                option << "p:" + QString::number(ui->PortSpinBox->value()); 
                option << "n:ボットプログラム";
            }

            this->client = new TCPClient(this);
            //待機開始
            ConnectionToggled(true);

            botProcess = new QProcess();
            botProcess->start(command, option);
        }
    }

    emit Standby(this,false);
    //再connectしクライアントの接続を待つ
    connect(this->client, &TCPClient::Connected,    this, &ClientSettingForm::Connected);
    connect(this->client, &TCPClient::Ready,        this, &ClientSettingForm::SetStandby);
    connect(this->client, &TCPClient::Disconnected, this, &ClientSettingForm::DisConnected);
    this->client->Startup();
}

void ClientSettingForm::SetPortSpin(int num)
{
    ui->PortSpinBox->setValue(num);
}

void ClientSettingForm::setBotCommand(QString command)
{
    botcommand = command;
    resetCombBox();
}

void ClientSettingForm::setPythonCommand(QString command)
{
    pythoncommand = command;
    resetCombBox();
}

void ClientSettingForm::setProgramFile(QString path, QString file)
{
    programpath = path;
    ui->ProgramFileEdit->setText(programpath + "/" + file);
}