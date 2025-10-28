#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ScoreLabelStyle.h"

const QString ScoreLabelStyle::coolLabelStyle = "border-radius: 15px;border:2px solid blue;color:blue;background-color:white;";
const QString ScoreLabelStyle::coolLabelWinStyle = "border-radius: 15px;border:2px solid blue;background-color:blue;color:white;";

const QString ScoreLabelStyle::hotLabelStyle = "border-radius: 15px;border:2px solid red;color:red;background-color:white;";
const QString ScoreLabelStyle::hotLabelWinStyle = "border-radius: 15px;border:2px solid red;background-color:red;color:white;";

const QString ScoreLabelStyle::totalLabelStyle = "border-radius: 15px;border:3px solid green;color:green;background-color:white;";
const QString ScoreLabelStyle::totalLabelLeadStyle = "border-radius: 15px;border:3px solid green;background-color:#C1DB81;";
const QString ScoreLabelStyle::totalLabelWinStyle = "border-radius: 15px;color:white;background-color:green;";
const QString ScoreLabelStyle::totalLabelDecideStyle = "border-radius: 15px;border:3px solid green;background-color:white;";

const QString ScoreLabelStyle::drawColor = "border-radius: 15px;background-color:#C1DB81;";
const QString ScoreLabelStyle::coolScoreLabelStyle = "border-radius: 15px;border:2px solid blue;background-color:#54C3F1;";
const QString ScoreLabelStyle::hotScoreLabelStyle = "border-radius: 15px;border:2px solid red;background-color:#EE87B4;";
const QString ScoreLabelStyle::coolPointLabelStyle = "border-radius: 10px;border:1px solid blue;background-color:white;";
const QString ScoreLabelStyle::hotPointLabelStyle = "border-radius: 10px;border:1px solid red;background-color:white;";

const QString ScoreLabelStyle::defaultBoxStyle = "border-radius: 6px;border:none;background-color:#F6F6E0;";
const QString ScoreLabelStyle::coolBoxStyle = "border-radius: 6px;border:none;background-color:#54C3F1;";
const QString ScoreLabelStyle::hotBoxStyle = "border-radius: 6px;border:none;background-color:#EE87B4;";

const QString defaultTimeBarStyle = "QProgressBar{border-radius: 16px;background-color:#C1DB81;} QProgressBar::chunk{border-radius: 16px;background-color:green;}";
const QString lastTimeBarStyle = "QProgressBar{border-radius: 3px;background-color:#F6C27A;} QProgressBar::chunk{border-radius: 3px;background-color:#FF7518;}";

void MainWindow::resetScoreLabels()
{
    ui->ScoreLabel_A->setStyleSheet(ScoreLabelStyle::coolScoreLabelStyle);
    ui->NameLabel_A->setStyleSheet(ScoreLabelStyle::coolScoreLabelStyle);
    ui->NameLabel_A->setText("U15 プロコン");

    ui->ScoreLabel_B->setStyleSheet(ScoreLabelStyle::hotScoreLabelStyle);
    ui->NameLabel_B->setStyleSheet(ScoreLabelStyle::hotScoreLabelStyle);
    ui->NameLabel_B->setText("一関大会");

    ui->item_1_A->setStyleSheet("");
    ui->item_1_B->setStyleSheet("");

    ui->bonus_1_A->setStyleSheet("");
    ui->bonus_1_B->setStyleSheet("");

    ui->score_1_A->setStyleSheet("");
    ui->score_1_B->setStyleSheet("");

    ui->sweep_1_A->setStyleSheet("");
    ui->sweep_1_B->setStyleSheet("");

    ui->COOL_label_A->setStyleSheet(ScoreLabelStyle::coolLabelStyle);
    ui->HOT_label_B->setStyleSheet(ScoreLabelStyle::hotLabelStyle);

    ui->item_2_A->setStyleSheet("");
    ui->item_2_B->setStyleSheet("");
    
    ui->bonus_2_A->setStyleSheet("");
    ui->bonus_2_B->setStyleSheet("");
    
    ui->score_2_A->setStyleSheet("");
    ui->score_2_B->setStyleSheet("");

    ui->sweep_2_A->setStyleSheet("");
    ui->sweep_2_B->setStyleSheet("");
    
    ui->score_total_A->setStyleSheet("");
    ui->score_total_B->setStyleSheet("");

    ui->wins_A->setStyleSheet("");
    ui->wins_B->setStyleSheet("");

    ui->COOL_label_B->setStyleSheet(ScoreLabelStyle::coolLabelStyle);
    ui->HOT_label_A->setStyleSheet(ScoreLabelStyle::hotLabelStyle);
    
    ui->total_label_A->setStyleSheet(ScoreLabelStyle::totalLabelStyle);
    ui->total_label_B->setStyleSheet(ScoreLabelStyle::totalLabelStyle);

    ui->Cool_box_A->setStyleSheet(ScoreLabelStyle::defaultBoxStyle);
    ui->Hot_box_A->setStyleSheet(ScoreLabelStyle::defaultBoxStyle);
    ui->Total_box_A->setStyleSheet(ScoreLabelStyle::defaultBoxStyle);

    ui->Hot_box_B->setStyleSheet(ScoreLabelStyle::defaultBoxStyle);
    ui->Cool_box_B->setStyleSheet(ScoreLabelStyle::defaultBoxStyle);
    ui->Total_box_B->setStyleSheet(ScoreLabelStyle::defaultBoxStyle);

    showBottomRoundLabel(isDoubleGameMode);
}

void MainWindow::startSetupScoreLabels()
{

    if(currentround == 0) {
        ui->ScoreLabel_A->setStyleSheet(ScoreLabelStyle::coolScoreLabelStyle);
        ui->NameLabel_A->setStyleSheet(ScoreLabelStyle::coolScoreLabelStyle);

        ui->ScoreLabel_B->setStyleSheet(ScoreLabelStyle::hotScoreLabelStyle);
        ui->NameLabel_B->setStyleSheet(ScoreLabelStyle::hotScoreLabelStyle);

        // スタート前にポイントを色付け
        ui->sweep_1_A->setStyleSheet(ScoreLabelStyle::coolScoreLabelStyle);
        ui->score_1_A->setStyleSheet(ScoreLabelStyle::coolScoreLabelStyle);
        ui->item_1_A->setStyleSheet(ScoreLabelStyle::coolScoreLabelStyle);
        ui->bonus_1_A->setStyleSheet(ScoreLabelStyle::coolScoreLabelStyle);

        ui->sweep_1_B->setStyleSheet(ScoreLabelStyle::hotScoreLabelStyle);
        ui->score_1_B->setStyleSheet(ScoreLabelStyle::hotScoreLabelStyle);
        ui->item_1_B->setStyleSheet(ScoreLabelStyle::hotScoreLabelStyle);
        ui->bonus_1_B->setStyleSheet(ScoreLabelStyle::hotScoreLabelStyle);

        ui->Cool_box_A->setStyleSheet(ScoreLabelStyle::coolBoxStyle);
        ui->Hot_box_A->setStyleSheet(ScoreLabelStyle::defaultBoxStyle);
        ui->Total_box_A->setStyleSheet(ScoreLabelStyle::defaultBoxStyle);

        ui->Hot_box_B->setStyleSheet(ScoreLabelStyle::hotBoxStyle);
        ui->Cool_box_B->setStyleSheet(ScoreLabelStyle::defaultBoxStyle);
        ui->Total_box_B->setStyleSheet(ScoreLabelStyle::defaultBoxStyle);
    }
    if(currentround == 1) {
        ui->ScoreLabel_A->setStyleSheet(ScoreLabelStyle::hotScoreLabelStyle);
        ui->NameLabel_A->setStyleSheet(ScoreLabelStyle::hotScoreLabelStyle);

        ui->ScoreLabel_B->setStyleSheet(ScoreLabelStyle::coolScoreLabelStyle);
        ui->NameLabel_B->setStyleSheet(ScoreLabelStyle::coolScoreLabelStyle);

        ui->sweep_2_A->setStyleSheet(ScoreLabelStyle::hotScoreLabelStyle);
        ui->score_2_A->setStyleSheet(ScoreLabelStyle::hotScoreLabelStyle);
        ui->item_2_A->setStyleSheet(ScoreLabelStyle::hotScoreLabelStyle);
        ui->bonus_2_A->setStyleSheet(ScoreLabelStyle::hotScoreLabelStyle);

        ui->sweep_2_B->setStyleSheet(ScoreLabelStyle::coolScoreLabelStyle);
        ui->score_2_B->setStyleSheet(ScoreLabelStyle::coolScoreLabelStyle);
        ui->item_2_B->setStyleSheet(ScoreLabelStyle::coolScoreLabelStyle);
        ui->bonus_2_B->setStyleSheet(ScoreLabelStyle::coolScoreLabelStyle);

        ui->Cool_box_A->setStyleSheet(ScoreLabelStyle::defaultBoxStyle);
        ui->Hot_box_A->setStyleSheet(ScoreLabelStyle::hotBoxStyle);
        ui->Total_box_A->setStyleSheet(ScoreLabelStyle::defaultBoxStyle);

        ui->Hot_box_B->setStyleSheet(ScoreLabelStyle::defaultBoxStyle);
        ui->Cool_box_B->setStyleSheet(ScoreLabelStyle::coolBoxStyle);
        ui->Total_box_B->setStyleSheet(ScoreLabelStyle::defaultBoxStyle);
    }
}

void MainWindow::startGameScoreLabels()
{
    if(currentround==0) {
        ui->sweep_1_A->setStyleSheet("");
        ui->score_1_A->setStyleSheet(ScoreLabelStyle::coolScoreLabelStyle);
        ui->item_1_A->setStyleSheet("");
        ui->bonus_1_A->setStyleSheet("");

        ui->sweep_1_B->setStyleSheet("");
        ui->score_1_B->setStyleSheet(ScoreLabelStyle::hotScoreLabelStyle);
        ui->item_1_B->setStyleSheet("");
        ui->bonus_1_B->setStyleSheet("");
    }
    if(currentround==1) {
        ui->sweep_2_A->setStyleSheet("");
        ui->score_2_A->setStyleSheet(ScoreLabelStyle::hotScoreLabelStyle);
        ui->item_2_A->setStyleSheet("");
        ui->bonus_2_A->setStyleSheet("");

        ui->sweep_2_B->setStyleSheet("");
        ui->score_2_B->setStyleSheet(ScoreLabelStyle::coolScoreLabelStyle);
        ui->item_2_B->setStyleSheet("");
        ui->bonus_2_B->setStyleSheet("");
    }
}


void MainWindow::finishScoreLabelStyle()
{
    int coolplayer, hotplayer;
    if (currentround==0) {
        coolplayer = 0;
        hotplayer  = 1;
    }else{
        coolplayer = 1;
        hotplayer  = 0;
    }

    int winner, loser;
    if(gameStatus.winner == GameSystem::GAME_STATUS::WINNER::COOL){
        winner = coolplayer;
        loser  = hotplayer;
    } else {
        winner = hotplayer;
        loser  = coolplayer;
    }

    this->point[coolplayer][currentround][0] = this->ui->Field->team_score[static_cast<int>(GameSystem::TEAM::COOL)];
    this->point[hotplayer ][currentround][0] = this->ui->Field->team_score[static_cast<int>(GameSystem::TEAM::HOT)];
    if (gameStatus.reason == GameSystem::GAME_STATUS::REASON::SCORE)
    {
        this->point[winner][currentround][1] = 0;
        this->point[winner][currentround][2] = 0;
        this->point[loser ][currentround][1] = 0;
        this->point[loser ][currentround][2] = 0;
    } else {
        if(isBlunder()) {
            this->point[loser ][currentround][1] = -3 * this->point[loser][currentround][0];
        } else {
            this->point[winner][currentround][1] = 50;
        }
        this->point[winner][currentround][2] = 6 * this->ui->Field->leave_items;
    }

    if(!(gameStatus.winner == GameSystem::GAME_STATUS::WINNER::DRAW)){
        point[winner][currentround][3] = 1;
    }

    if (gameStatus.winner == GameSystem::GAME_STATUS::WINNER::DRAW){
        this->ui->WinnerLabel->setStyleSheet(ScoreLabelStyle::drawColor);
        this->ui->ResultLabel->setStyleSheet(ScoreLabelStyle::drawColor);
    } else if (gameStatus.winner == GameSystem::GAME_STATUS::WINNER::COOL){
        this->ui->WinnerLabel->setStyleSheet(ScoreLabelStyle::coolScoreLabelStyle);
        this->ui->ResultLabel->setStyleSheet(ScoreLabelStyle::coolScoreLabelStyle);
    } else if (gameStatus.winner == GameSystem::GAME_STATUS::WINNER::HOT){
        this->ui->WinnerLabel->setStyleSheet(ScoreLabelStyle::hotScoreLabelStyle);
        this->ui->ResultLabel->setStyleSheet(ScoreLabelStyle::hotScoreLabelStyle);
    }

    if(currentround==0) {
        ui->score_1_A->setStyleSheet(ScoreLabelStyle::coolScoreLabelStyle);
        ui->item_1_A->setStyleSheet(ScoreLabelStyle::coolPointLabelStyle);

        ui->score_1_B->setStyleSheet(ScoreLabelStyle::hotScoreLabelStyle);
        ui->item_1_B->setStyleSheet(ScoreLabelStyle::hotPointLabelStyle);
 
        if(gameStatus.winner == GameSystem::GAME_STATUS::WINNER::COOL){
            ui->COOL_label_A->setStyleSheet(ScoreLabelStyle::coolLabelWinStyle);
            if(! (gameStatus.reason == GameSystem::GAME_STATUS::REASON::SCORE)){
                ui->sweep_1_A->setStyleSheet(ScoreLabelStyle::coolPointLabelStyle);
                if(isBlunder()) {
                    ui->bonus_1_B->setStyleSheet(ScoreLabelStyle::hotPointLabelStyle);
                } else {
                    ui->bonus_1_A->setStyleSheet(ScoreLabelStyle::coolPointLabelStyle);
                }
            }
        } else if(gameStatus.winner == GameSystem::GAME_STATUS::WINNER::HOT){
            ui->HOT_label_B->setStyleSheet(ScoreLabelStyle::hotLabelWinStyle);
            if(! (gameStatus.reason == GameSystem::GAME_STATUS::REASON::SCORE)){
                ui->sweep_1_B->setStyleSheet(ScoreLabelStyle::hotPointLabelStyle);
               if(isBlunder()) {
                    ui->bonus_1_A->setStyleSheet(ScoreLabelStyle::coolPointLabelStyle);
                } else {
                    ui->bonus_1_B->setStyleSheet(ScoreLabelStyle::hotPointLabelStyle);
                }
            }
        }
    } else if(currentround==1) {
        ui->score_2_B->setStyleSheet(ScoreLabelStyle::coolScoreLabelStyle);
        ui->item_2_B->setStyleSheet(ScoreLabelStyle::coolPointLabelStyle);


        ui->score_2_A->setStyleSheet(ScoreLabelStyle::hotScoreLabelStyle);
        ui->item_2_A->setStyleSheet(ScoreLabelStyle::hotPointLabelStyle);

        
        if(gameStatus.winner == GameSystem::GAME_STATUS::WINNER::COOL){
            ui->COOL_label_B->setStyleSheet(ScoreLabelStyle::coolLabelWinStyle);
            if(! (gameStatus.reason == GameSystem::GAME_STATUS::REASON::SCORE)){
                ui->sweep_2_B->setStyleSheet(ScoreLabelStyle::coolPointLabelStyle);
                if(isBlunder()) {
                    ui->bonus_2_A->setStyleSheet(ScoreLabelStyle::hotPointLabelStyle);
                } else {
                    ui->bonus_2_B->setStyleSheet(ScoreLabelStyle::coolPointLabelStyle);
                }
            }
        } else if(gameStatus.winner == GameSystem::GAME_STATUS::WINNER::HOT){
            ui->HOT_label_A->setStyleSheet(ScoreLabelStyle::hotLabelWinStyle);
            if(! (gameStatus.reason == GameSystem::GAME_STATUS::REASON::SCORE)){
                ui->sweep_2_A->setStyleSheet(ScoreLabelStyle::hotPointLabelStyle);
                if(isBlunder()) {
                    ui->bonus_2_B->setStyleSheet(ScoreLabelStyle::coolPointLabelStyle);
                } else {
                    ui->bonus_2_A->setStyleSheet(ScoreLabelStyle::hotPointLabelStyle);
                }
            }
        }
    }

    refreshScoreLabels();
}

void MainWindow::refreshScoreLabels()
{
    int score_1_A = 10 * this->point[0][0][0] + this->point[0][0][1] + this->point[0][0][2];
    int score_2_A = 10 * this->point[0][1][0] + this->point[0][1][1] + this->point[0][1][2];
    int score_1_B = 10 * this->point[1][0][0] + this->point[1][0][1] + this->point[1][0][2];
    int score_2_B = 10 * this->point[1][1][0] + this->point[1][1][1] + this->point[1][1][2];

    int score_total_A = score_1_A + score_2_A;
    int score_total_B = score_1_B + score_2_B;

    int wins_A = this->point[0][0][3] + this->point[0][1][3];
    int wins_B = this->point[1][0][3] + this->point[1][1][3];

    ui->item_1_A->setText(QString::number(10 * this->point[0][0][0]));
    ui->item_2_A->setText(QString::number(10 * this->point[0][1][0]));
    ui->item_1_B->setText(QString::number(10 * this->point[1][0][0]));
    ui->item_2_B->setText(QString::number(10 * this->point[1][1][0]));

    ui->bonus_1_A->setText(QString::number(this->point[0][0][1]));
    ui->bonus_2_A->setText(QString::number(this->point[0][1][1]));
    ui->bonus_1_B->setText(QString::number(this->point[1][0][1]));
    ui->bonus_2_B->setText(QString::number(this->point[1][1][1]));

    ui->sweep_1_A->setText(QString::number(this->point[0][0][2]));
    ui->sweep_2_A->setText(QString::number(this->point[0][1][2]));
    ui->sweep_1_B->setText(QString::number(this->point[1][0][2]));
    ui->sweep_2_B->setText(QString::number(this->point[1][1][2]));

    ui->score_1_A->setText(QString::number(score_1_A));
    ui->score_2_A->setText(QString::number(score_2_A));
    ui->score_1_B->setText(QString::number(score_1_B));
    ui->score_2_B->setText(QString::number(score_2_B));

    ui->score_total_A->setText(QString::number(score_total_A));
    ui->score_total_B->setText(QString::number(score_total_B));
  

    ui->wins_A->setText(QString::number(wins_A));
    ui->wins_B->setText(QString::number(wins_B));

    ui->score_total_A->setStyleSheet("");
    ui->score_total_B->setStyleSheet("");
    ui->wins_A->setStyleSheet("");
    ui->wins_B->setStyleSheet("");

    int intermediate_wins_A = 0;
    int intermediate_wins_B = 0;
    if(this->gameStatus.winner == GameSystem::GAME_STATUS::WINNER::CONTINUE){
        if (currentround == 0) {
            if(score_1_A > score_1_B){
                intermediate_wins_A = 1;
                intermediate_wins_B = 0;
            }
            if(score_1_A < score_1_B){
                intermediate_wins_A = 0;
                intermediate_wins_B = 1;
            }
        }
        if (currentround == 1) {
            if(score_2_A > score_2_B){
                intermediate_wins_A = 1;
                intermediate_wins_B = 0;
            }
            if(score_2_A < score_2_B){
                intermediate_wins_A = 0;
                intermediate_wins_B = 1;
            }
        }
        ui->total_label_A->setStyleSheet(ScoreLabelStyle::totalLabelStyle);
        ui->total_label_B->setStyleSheet(ScoreLabelStyle::totalLabelStyle);
        if(((wins_A + intermediate_wins_A) * 1000 + score_total_A) >
        ((wins_B + intermediate_wins_B) * 1000 + score_total_B)){
            ui->total_label_A->setStyleSheet(ScoreLabelStyle::totalLabelLeadStyle);
        }
        if(((wins_A + intermediate_wins_A) * 1000 + score_total_A) <
        ((wins_B + intermediate_wins_B) * 1000 + score_total_B)){
            ui->total_label_B->setStyleSheet(ScoreLabelStyle::totalLabelLeadStyle);
        }
    } else {
        // FINISHED
        if (currentround == 0 || this->gameStatus.winner == GameSystem::GAME_STATUS::WINNER::NONE) {
            if((wins_A * 1000 + score_total_A) > (wins_B * 1000 + score_total_B)){
                ui->total_label_A->setStyleSheet(ScoreLabelStyle::totalLabelLeadStyle);
                ui->total_label_B->setStyleSheet(ScoreLabelStyle::totalLabelStyle);
            }
            if((wins_A * 1000 + score_total_A) < (wins_B * 1000 + score_total_B)){
                ui->total_label_B->setStyleSheet(ScoreLabelStyle::totalLabelLeadStyle);
                ui->total_label_A->setStyleSheet(ScoreLabelStyle::totalLabelStyle);
            }
        }
        else if (currentround == 1) {
            ui->score_total_A->setStyleSheet(ScoreLabelStyle::totalLabelDecideStyle);
            ui->wins_A->setStyleSheet(ScoreLabelStyle::totalLabelDecideStyle);
            ui->score_total_B->setStyleSheet(ScoreLabelStyle::totalLabelDecideStyle);
            ui->wins_B->setStyleSheet(ScoreLabelStyle::totalLabelDecideStyle);
            
            if((wins_A * 1000 + score_total_A) == (wins_B  * 1000 + score_total_B)){
                ui->total_label_A->setStyleSheet(ScoreLabelStyle::totalLabelLeadStyle);
                ui->total_label_B->setStyleSheet(ScoreLabelStyle::totalLabelLeadStyle);
            }
            if((wins_A * 1000 + score_total_A) > (wins_B  * 1000 + score_total_B)){
                ui->total_label_A->setStyleSheet(ScoreLabelStyle::totalLabelWinStyle);
                ui->total_label_B->setStyleSheet(ScoreLabelStyle::totalLabelStyle);
            }
            if((wins_A * 1000 + score_total_A) < (wins_B * 1000 + score_total_B)){
                ui->total_label_B->setStyleSheet(ScoreLabelStyle::totalLabelWinStyle);
                ui->total_label_A->setStyleSheet(ScoreLabelStyle::totalLabelStyle);
            }
        }
    }
}

void MainWindow::showBottomRoundLabel(bool set)
{
    if(set) {
        ui->item_2_A->show();
        ui->item_2_B->show();
        
        ui->bonus_2_A->show();
        ui->bonus_2_B->show();
        
        ui->score_2_A->show();
        ui->score_2_B->show();

        ui->sweep_2_A->show();
        ui->sweep_2_B->show();
        
        ui->score_total_A->show();
        ui->score_total_B->show();
        ui->wins_A->show();
        ui->wins_B->show();

        ui->COOL_label_B->show();
        ui->HOT_label_A->show();
        
        ui->total_label_A->show();
        ui->total_label_B->show();

        ui->item_label_A2->show();
        ui->bonus_label_A2->show();
        ui->score_label_A2->show();

        ui->score_label_A->show();
        ui->wins_label_A->show();

        ui->item_label_B2->show();
        ui->bonus_label_B2->show();
        ui->score_label_B2->show();

        ui->score_label_B->show();
        ui->wins_label_B->show();
    }else{
        ui->item_2_A->hide();
        ui->item_2_B->hide();
        
        ui->bonus_2_A->hide();
        ui->bonus_2_B->hide();
        
        ui->score_2_A->hide();
        ui->score_2_B->hide();

        ui->sweep_2_A->hide();
        ui->sweep_2_B->hide();
        
        ui->score_total_A->hide();
        ui->score_total_B->hide();
        ui->wins_A->hide();
        ui->wins_B->hide();

        ui->COOL_label_B->hide();
        ui->HOT_label_A->hide();
        
        ui->total_label_A->hide();
        ui->total_label_B->hide();

        ui->item_label_A2->hide();
        ui->bonus_label_A2->hide();
        ui->score_label_A2->hide();

        ui->score_label_A->hide();
        ui->wins_label_A->hide();

        ui->item_label_B2->hide();
        ui->bonus_label_B2->hide();
        ui->score_label_B2->hide();

        ui->score_label_B->hide();
        ui->wins_label_B->hide();
    }
    refreshScoreLabels();
}

void MainWindow::changeTimeBarsColor(bool change)
{
    if (change) {
        ui->TimeBar_A->setStyleSheet(lastTimeBarStyle);
        ui->TimeBar_B->setStyleSheet(lastTimeBarStyle);
    } else {
        ui->TimeBar_A->setStyleSheet(defaultTimeBarStyle);
        ui->TimeBar_B->setStyleSheet(defaultTimeBarStyle);
    }
}
