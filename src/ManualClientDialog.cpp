#include "ManualClientDialog.h"
#include "ui_ManualClientDialog.h"

void ManualClientDialog::closeEvent(QCloseEvent* ce){
    emit CloseWindow();
    ce->accept();
}
void ManualClientDialog::keyPressEvent(QKeyEvent * event){
    if(event->key()==Qt::Key_W||event->key()==Qt::Key_Up)   UPButtonClicked();
    if(event->key()==Qt::Key_S||event->key()==Qt::Key_Down) DOWNButtonClicked();
    if(event->key()==Qt::Key_D||event->key()==Qt::Key_Right)RIGHTButtonClicked();
    if(event->key()==Qt::Key_A||event->key()==Qt::Key_Left) LEFTButtonClicked();
}

GameSystem::Method::ACTION ManualClientDialog::GetAction(){
    if     (this->ui->WalkRadio->isChecked())  return GameSystem::Method::ACTION::WALK;
    else if(this->ui->PutRadio->isChecked())   return GameSystem::Method::ACTION::PUT;
    else if(this->ui->SearchRadio->isChecked())return GameSystem::Method::ACTION::SEARCH;
    else if(this->ui->LookRadio->isChecked())  return GameSystem::Method::ACTION::LOOK;
    else                                       return GameSystem::Method::ACTION::UNKNOWN;
}

void ManualClientDialog::UPButtonClicked()
{
    next_method.rote = GameSystem::Method::ROTE::UP;
    next_method.action = GetAction();
    emit ReadyAction();
}

void ManualClientDialog::DOWNButtonClicked()
{
    next_method.rote = GameSystem::Method::ROTE::DOWN;
    next_method.action = GetAction();
    emit ReadyAction();
}

void ManualClientDialog::RIGHTButtonClicked()
{
    next_method.rote = GameSystem::Method::ROTE::RIGHT;
    next_method.action = GetAction();
    emit ReadyAction();
}

void ManualClientDialog::LEFTButtonClicked()
{
    next_method.rote = GameSystem::Method::ROTE::LEFT;
    next_method.action = GetAction();
    emit ReadyAction();
}

void ManualClientDialog::AppendLog(const QString &str)
{
    this->ui->LogEdit->appendPlainText(str);
}

ManualClientDialog::ManualClientDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ManualClientDialog)
{
    ui->setupUi(this);
    connect(this->ui->UPButton,    &QPushButton::clicked, this, &ManualClientDialog::UPButtonClicked);
	connect(this->ui->DOWNButton,  &QPushButton::clicked, this, &ManualClientDialog::DOWNButtonClicked);
	connect(this->ui->RIGHTButton, &QPushButton::clicked, this, &ManualClientDialog::RIGHTButtonClicked);
	connect(this->ui->LEFTButton,  &QPushButton::clicked, this, &ManualClientDialog::LEFTButtonClicked);
}

ManualClientDialog::~ManualClientDialog()
{
    hide();
    delete ui;
}
