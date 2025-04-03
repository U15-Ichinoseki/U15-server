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
const QString ScoreLabelStyle::coolPointLabelStyle = "border-radius: 15px;border:2px solid blue;background-color:#54C3F1;";
const QString ScoreLabelStyle::hotPointLabelStyle = "border-radius: 15px;border:2px solid red;background-color:#EE87B4;";
const QString ScoreLabelStyle::coolScoreLabelStyle = "border-radius: 15px;border:3px solid blue;background-color:white;";
const QString ScoreLabelStyle::hotScoreLabelStyle = "border-radius: 15px;border:3px solid red;background-color:white;";

void MainWindow::resetScoreLabels()
{
    ui->ScoreLabel_A->setStyleSheet(ScoreLabelStyle::coolPointLabelStyle);
    ui->NameLabel_A->setStyleSheet(ScoreLabelStyle::coolPointLabelStyle);
    ui->NameLabel_A->setText("U15 プロコン");

    ui->ScoreLabel_B->setStyleSheet(ScoreLabelStyle::hotPointLabelStyle);
    ui->NameLabel_B->setStyleSheet(ScoreLabelStyle::hotPointLabelStyle);
    ui->NameLabel_B->setText("一関大会");

    ui->reason_1_A->setText("");
    ui->reason_1_B->setText("");

    ui->item_1_A->setStyleSheet("");
    ui->item_1_B->setStyleSheet("");

    ui->bonus_1_A->setStyleSheet("");
    ui->bonus_1_B->setStyleSheet("");

    ui->score_1_A->setStyleSheet("");
    ui->score_1_B->setStyleSheet("");

    ui->result_1_A->setStyleSheet("");
    ui->result_1_B->setStyleSheet("");

    ui->COOL_label_A->setStyleSheet(ScoreLabelStyle::coolLabelStyle);
    ui->HOT_label_B->setStyleSheet(ScoreLabelStyle::hotLabelStyle);

    ui->reason_2_A->setText("");
    ui->reason_2_B->setText("");

    ui->item_2_A->setStyleSheet("");
    ui->item_2_B->setStyleSheet("");
    
    ui->bonus_2_A->setStyleSheet("");
    ui->bonus_2_B->setStyleSheet("");
    
    ui->score_2_A->setStyleSheet("");
    ui->score_2_B->setStyleSheet("");

    ui->result_2_A->setStyleSheet("");
    ui->result_2_B->setStyleSheet("");
    
    ui->score_total_A->setStyleSheet("");
    ui->score_total_B->setStyleSheet("");

    ui->result_A->setStyleSheet("");
    ui->result_B->setStyleSheet("");

    ui->COOL_label_B->setStyleSheet(ScoreLabelStyle::coolLabelStyle);
    ui->HOT_label_A->setStyleSheet(ScoreLabelStyle::hotLabelStyle);
    
    ui->total_label_A->setStyleSheet(ScoreLabelStyle::totalLabelStyle);
    ui->total_label_B->setStyleSheet(ScoreLabelStyle::totalLabelStyle);

    showBottomRoundLabel(isDoubleGameMode);
}

void MainWindow::startSetupScoreLabels()
{

    if(currentround == 0) {
        ui->ScoreLabel_A->setStyleSheet(ScoreLabelStyle::coolPointLabelStyle);
        ui->NameLabel_A->setStyleSheet(ScoreLabelStyle::coolPointLabelStyle);

        ui->ScoreLabel_B->setStyleSheet(ScoreLabelStyle::hotPointLabelStyle);
        ui->NameLabel_B->setStyleSheet(ScoreLabelStyle::hotPointLabelStyle);

        // スタート前にスコアを色付け
        ui->result_1_A->setStyleSheet(ScoreLabelStyle::coolPointLabelStyle);
        ui->score_1_A->setStyleSheet(ScoreLabelStyle::coolPointLabelStyle);
        ui->item_1_A->setStyleSheet(ScoreLabelStyle::coolPointLabelStyle);
        ui->bonus_1_A->setStyleSheet(ScoreLabelStyle::coolPointLabelStyle);

        ui->result_1_B->setStyleSheet(ScoreLabelStyle::hotPointLabelStyle);
        ui->score_1_B->setStyleSheet(ScoreLabelStyle::hotPointLabelStyle);
        ui->item_1_B->setStyleSheet(ScoreLabelStyle::hotPointLabelStyle);
        ui->bonus_1_B->setStyleSheet(ScoreLabelStyle::hotPointLabelStyle);
    }
    if(currentround == 1) {
        ui->ScoreLabel_A->setStyleSheet(ScoreLabelStyle::hotPointLabelStyle);
        ui->NameLabel_A->setStyleSheet(ScoreLabelStyle::hotPointLabelStyle);

        ui->ScoreLabel_B->setStyleSheet(ScoreLabelStyle::coolPointLabelStyle);
        ui->NameLabel_B->setStyleSheet(ScoreLabelStyle::coolPointLabelStyle);

        ui->result_2_A->setStyleSheet(ScoreLabelStyle::hotPointLabelStyle);
        ui->score_2_A->setStyleSheet(ScoreLabelStyle::hotPointLabelStyle);
        ui->item_2_A->setStyleSheet(ScoreLabelStyle::hotPointLabelStyle);
        ui->bonus_2_A->setStyleSheet(ScoreLabelStyle::hotPointLabelStyle);

        ui->result_2_B->setStyleSheet(ScoreLabelStyle::coolPointLabelStyle);
        ui->score_2_B->setStyleSheet(ScoreLabelStyle::coolPointLabelStyle);
        ui->item_2_B->setStyleSheet(ScoreLabelStyle::coolPointLabelStyle);
        ui->bonus_2_B->setStyleSheet(ScoreLabelStyle::coolPointLabelStyle);
    }
}

void MainWindow::startGameScoreLabels()
{
    if(currentround==0) {
        ui->result_1_A->setStyleSheet("");
        ui->score_1_A->setStyleSheet("");
        ui->item_1_A->setStyleSheet(ScoreLabelStyle::coolPointLabelStyle);
        ui->bonus_1_A->setStyleSheet("");
        ui->reason_1_A->setStyleSheet("");

        ui->result_1_B->setStyleSheet("");
        ui->score_1_B->setStyleSheet("");
        ui->item_1_B->setStyleSheet(ScoreLabelStyle::hotPointLabelStyle);
        ui->bonus_1_B->setStyleSheet("");
        ui->reason_1_B->setStyleSheet("");
    }
    if(currentround==1) {
        ui->result_2_A->setStyleSheet("");
        ui->score_2_A->setStyleSheet("");
        ui->item_2_A->setStyleSheet(ScoreLabelStyle::hotPointLabelStyle);
        ui->bonus_2_A->setStyleSheet("");
        ui->reason_2_A->setStyleSheet("");

        ui->result_2_B->setStyleSheet("");
        ui->score_2_B->setStyleSheet("");
        ui->item_2_B->setStyleSheet(ScoreLabelStyle::coolPointLabelStyle);
        ui->bonus_2_B->setStyleSheet("");
        ui->reason_2_B->setStyleSheet("");
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

    QLabel *reason[TEAM_COUNT];
    if (currentround==0) {
        reason[0] = ui->reason_1_A;
        reason[1] = ui->reason_1_B;    
    }
    else {
        reason[0] = ui->reason_2_A;    
        reason[1] = ui->reason_2_B;
    }

    if (gameStatus.reason == GameSystem::GAME_STATUS::REASON::SCORE)
    {
        reason[winner]->setText("");
        reason[loser ]->setText("");
    }else if(!isBlunder()) {
        reason[loser ]->setText("");
        if (gameStatus.reason == GameSystem::GAME_STATUS::REASON::TRAPPED)
            reason[winner]->setText("包囲");
        else if (gameStatus.reason == GameSystem::GAME_STATUS::REASON::ATTACK)
            reason[winner]->setText("アタック");
    }else{
        reason[winner]->setText("");
        if (gameStatus.reason == GameSystem::GAME_STATUS::REASON::CONFINED)
            reason[loser ]->setText("自縛");
        else if (gameStatus.reason == GameSystem::GAME_STATUS::REASON::COLLISION)
            reason[loser ]->setText("衝突");
        else if (gameStatus.reason == GameSystem::GAME_STATUS::REASON::FOULED)
            reason[loser ]->setText("エラー");
    }

    this->point[coolplayer][currentround][0] = this->ui->Field->team_score[static_cast<int>(GameSystem::TEAM::COOL)];
    this->point[hotplayer ][currentround][0] = this->ui->Field->team_score[static_cast<int>(GameSystem::TEAM::HOT)];
    if (gameStatus.reason == GameSystem::GAME_STATUS::REASON::SCORE)
    {
        this->point[winner][currentround][1] = 0;
        this->point[loser ][currentround][1] = 0;
    }else if(!isBlunder()) {
        this->point[winner][currentround][1] = 7 * this->ui->Field->leave_items;
        this->point[loser ][currentround][1] = 0;
    }else{
        this->point[winner][currentround][1] = 0;
        this->point[loser ][currentround][1] = -3 * this->point[loser][currentround][0];
    }

    if(gameStatus.winner == GameSystem::GAME_STATUS::WINNER::DRAW){
        point[winner][currentround][2] = 1;
        point[loser ][currentround][2] = 1;
    } else {
        point[winner][currentround][2] = 3;
        point[loser ][currentround][2] = 0;
    }

    if (gameStatus.winner == GameSystem::GAME_STATUS::WINNER::DRAW){
        this->ui->WinnerLabel->setStyleSheet(ScoreLabelStyle::drawColor);
        this->ui->ResultLabel->setStyleSheet(ScoreLabelStyle::drawColor);
    } else if (gameStatus.winner == GameSystem::GAME_STATUS::WINNER::COOL){
        this->ui->WinnerLabel->setStyleSheet(ScoreLabelStyle::coolPointLabelStyle);
        this->ui->ResultLabel->setStyleSheet(ScoreLabelStyle::coolPointLabelStyle);
    } else if (gameStatus.winner == GameSystem::GAME_STATUS::WINNER::HOT){
        this->ui->WinnerLabel->setStyleSheet(ScoreLabelStyle::hotPointLabelStyle);
        this->ui->ResultLabel->setStyleSheet(ScoreLabelStyle::hotPointLabelStyle);
    }

    if(currentround==0) {
        ui->result_1_A->setStyleSheet(ScoreLabelStyle::coolScoreLabelStyle);
        ui->score_1_A->setStyleSheet(ScoreLabelStyle::coolScoreLabelStyle);
        ui->item_1_A->setStyleSheet(ScoreLabelStyle::coolPointLabelStyle);

        ui->result_1_B->setStyleSheet(ScoreLabelStyle::hotScoreLabelStyle);
        ui->score_1_B->setStyleSheet(ScoreLabelStyle::hotScoreLabelStyle);
        ui->item_1_B->setStyleSheet(ScoreLabelStyle::hotPointLabelStyle);

        if(gameStatus.winner == GameSystem::GAME_STATUS::WINNER::COOL){
            if(! (gameStatus.reason == GameSystem::GAME_STATUS::REASON::SCORE)){
                if(!isBlunder())
                    ui->bonus_1_A->setStyleSheet(ScoreLabelStyle::coolPointLabelStyle);
                else
                    ui->bonus_1_B->setStyleSheet(ScoreLabelStyle::hotPointLabelStyle);
            }
            ui->COOL_label_A->setStyleSheet(ScoreLabelStyle::coolLabelWinStyle);

        } else if(gameStatus.winner == GameSystem::GAME_STATUS::WINNER::HOT){
            if(! (gameStatus.reason == GameSystem::GAME_STATUS::REASON::SCORE)){
                if(!isBlunder())
                    ui->bonus_1_B->setStyleSheet(ScoreLabelStyle::hotPointLabelStyle);
                else
                    ui->bonus_1_A->setStyleSheet(ScoreLabelStyle::coolPointLabelStyle);
            }
            ui->HOT_label_B->setStyleSheet(ScoreLabelStyle::hotLabelWinStyle);
        }
    } else if(currentround==1) {
        ui->result_2_B->setStyleSheet(ScoreLabelStyle::coolScoreLabelStyle);
        ui->score_2_B->setStyleSheet(ScoreLabelStyle::coolScoreLabelStyle);
        ui->item_2_B->setStyleSheet(ScoreLabelStyle::coolPointLabelStyle);

        ui->result_2_A->setStyleSheet(ScoreLabelStyle::hotScoreLabelStyle);
        ui->score_2_A->setStyleSheet(ScoreLabelStyle::hotScoreLabelStyle);
        ui->item_2_A->setStyleSheet(ScoreLabelStyle::hotPointLabelStyle);

        if(gameStatus.winner == GameSystem::GAME_STATUS::WINNER::COOL){
            if(! (gameStatus.reason == GameSystem::GAME_STATUS::REASON::SCORE)){
                if(!isBlunder())
                    ui->bonus_2_B->setStyleSheet(ScoreLabelStyle::coolPointLabelStyle);
                else
                    ui->bonus_2_A->setStyleSheet(ScoreLabelStyle::hotPointLabelStyle);
            }
            ui->COOL_label_B->setStyleSheet(ScoreLabelStyle::coolLabelWinStyle);
        } else if(gameStatus.winner == GameSystem::GAME_STATUS::WINNER::HOT){
            if(! (gameStatus.reason == GameSystem::GAME_STATUS::REASON::SCORE)){
                if(!isBlunder())
                    ui->bonus_2_A->setStyleSheet(ScoreLabelStyle::hotPointLabelStyle);
                else
                    ui->bonus_2_B->setStyleSheet(ScoreLabelStyle::coolPointLabelStyle);
            }
            ui->HOT_label_A->setStyleSheet(ScoreLabelStyle::hotLabelWinStyle);
        }
    }

    refreshScoreLabels();
}

void MainWindow::refreshScoreLabels()
{
    int score_1_A = 10 * this->point[0][0][0] + this->point[0][0][1];
    int score_2_A = 10 * this->point[0][1][0] + this->point[0][1][1];
    int score_1_B = 10 * this->point[1][0][0] + this->point[1][0][1];
    int score_2_B = 10 * this->point[1][1][0] + this->point[1][1][1];

    int score_total_A = score_1_A + score_2_A;
    int score_total_B = score_1_B + score_2_B;

    int result_A = this->point[0][0][2] + this->point[0][1][2];
    int result_B = this->point[1][0][2] + this->point[1][1][2];

    ui->item_1_A->setText(QString::number(10 * this->point[0][0][0]));
    ui->item_2_A->setText(QString::number(10 * this->point[0][1][0]));
    ui->item_1_B->setText(QString::number(10 * this->point[1][0][0]));
    ui->item_2_B->setText(QString::number(10 * this->point[1][1][0]));

    ui->bonus_1_A->setText(QString::number(this->point[0][0][1]));
    ui->bonus_2_A->setText(QString::number(this->point[0][1][1]));
    ui->bonus_1_B->setText(QString::number(this->point[1][0][1]));
    ui->bonus_2_B->setText(QString::number(this->point[1][1][1]));

    ui->score_1_A->setText(QString::number(score_1_A));
    ui->score_2_A->setText(QString::number(score_2_A));
    ui->score_1_B->setText(QString::number(score_1_B));
    ui->score_2_B->setText(QString::number(score_2_B));

    ui->score_total_A->setText(QString::number(score_total_A));
    ui->score_total_B->setText(QString::number(score_total_B));
    
    ui->result_1_A->setText(QString::number(this->point[0][0][2]));
    ui->result_2_A->setText(QString::number(this->point[0][1][2]));
    ui->result_1_B->setText(QString::number(this->point[1][0][2]));
    ui->result_2_B->setText(QString::number(this->point[1][1][2]));

    ui->result_A->setText(QString::number(result_A));
    ui->result_B->setText(QString::number(result_B));

    ui->score_total_A->setStyleSheet("");
    ui->score_total_B->setStyleSheet("");
    ui->result_A->setStyleSheet("");
    ui->result_B->setStyleSheet("");

    int intermediate_result_A = 0;
    int intermediate_result_B = 0;
    if(this->gameStatus.winner == GameSystem::GAME_STATUS::WINNER::CONTINUE){
        if (currentround == 0) {
            if(score_1_A > score_1_B){
                intermediate_result_A = 3;
                intermediate_result_B = 0;
            }
            if(score_1_A < score_1_B){
                intermediate_result_A = 0;
                intermediate_result_B = 3;
            }
        }
        if (currentround == 1) {
            if(score_2_A > score_2_B){
                intermediate_result_A = 3;
                intermediate_result_B = 0;
            }
            if(score_2_A < score_2_B){
                intermediate_result_A = 0;
                intermediate_result_B = 3;
            }
        }
        ui->total_label_A->setStyleSheet(ScoreLabelStyle::totalLabelStyle);
        ui->total_label_B->setStyleSheet(ScoreLabelStyle::totalLabelStyle);
        if(((result_A + intermediate_result_A) * 1000 + score_total_A) >
        ((result_B + intermediate_result_B) * 1000 + score_total_B)){
            ui->total_label_A->setStyleSheet(ScoreLabelStyle::totalLabelLeadStyle);
        }
        if(((result_A + intermediate_result_A) * 1000 + score_total_A) <
        ((result_B + intermediate_result_B) * 1000 + score_total_B)){
            ui->total_label_B->setStyleSheet(ScoreLabelStyle::totalLabelLeadStyle);
        }
    } else {
        // FINISHED
        if (currentround == 0 || this->gameStatus.winner == GameSystem::GAME_STATUS::WINNER::NONE) {
            if((result_A * 1000 + score_total_A) > (result_B * 1000 + score_total_B)){
                ui->total_label_A->setStyleSheet(ScoreLabelStyle::totalLabelLeadStyle);
                ui->total_label_B->setStyleSheet(ScoreLabelStyle::totalLabelStyle);
            }
            if((result_A * 1000 + score_total_A) < (result_B * 1000 + score_total_B)){
                ui->total_label_B->setStyleSheet(ScoreLabelStyle::totalLabelLeadStyle);
                ui->total_label_A->setStyleSheet(ScoreLabelStyle::totalLabelStyle);
            }
        }
        else if (currentround == 1) {
            ui->score_total_A->setStyleSheet(ScoreLabelStyle::totalLabelDecideStyle);
            ui->result_A->setStyleSheet(ScoreLabelStyle::totalLabelDecideStyle);
            ui->score_total_B->setStyleSheet(ScoreLabelStyle::totalLabelDecideStyle);
            ui->result_B->setStyleSheet(ScoreLabelStyle::totalLabelDecideStyle);
            
            if((result_A * 1000 + score_total_A) == (result_B  * 1000 + score_total_B)){
                ui->total_label_A->setStyleSheet(ScoreLabelStyle::totalLabelLeadStyle);
                ui->total_label_B->setStyleSheet(ScoreLabelStyle::totalLabelLeadStyle);
            }
            if((result_A * 1000 + score_total_A) > (result_B  * 1000 + score_total_B)){
                ui->total_label_A->setStyleSheet(ScoreLabelStyle::totalLabelWinStyle);
                ui->total_label_B->setStyleSheet(ScoreLabelStyle::totalLabelStyle);
            }
            if((result_A * 1000 + score_total_A) < (result_B * 1000 + score_total_B)){
                ui->total_label_B->setStyleSheet(ScoreLabelStyle::totalLabelWinStyle);
                ui->total_label_A->setStyleSheet(ScoreLabelStyle::totalLabelStyle);
            }
        }
    }
}

void MainWindow::showBottomRoundLabel(bool set)
{
    if(set) {
        ui->reason_2_A->show();
        ui->reason_2_B->show();

        ui->item_2_A->show();
        ui->item_2_B->show();
        
        ui->bonus_2_A->show();
        ui->bonus_2_B->show();
        
        ui->score_2_A->show();
        ui->score_2_B->show();

        ui->result_2_A->show();
        ui->result_2_B->show();
        
        ui->score_total_A->show();
        ui->score_total_B->show();
        ui->result_A->show();
        ui->result_B->show();

        ui->COOL_label_B->show();
        ui->HOT_label_A->show();
        
        ui->total_label_A->show();
        ui->total_label_B->show();

        ui->item_label_A2->show();
        ui->bonus_label_A2->show();
        ui->score_label_A2->show();
        ui->result_label_A2->show();

        ui->score_label_A->show();
        ui->result_label_A->show();

        ui->item_label_B2->show();
        ui->bonus_label_B2->show();
        ui->score_label_B2->show();
        ui->result_label_B2->show();

        ui->score_label_B->show();
        ui->result_label_B->show();
    }else{
        ui->reason_2_A->hide();
        ui->reason_2_B->hide();

        ui->item_2_A->hide();
        ui->item_2_B->hide();
        
        ui->bonus_2_A->hide();
        ui->bonus_2_B->hide();
        
        ui->score_2_A->hide();
        ui->score_2_B->hide();

        ui->result_2_A->hide();
        ui->result_2_B->hide();
        
        ui->score_total_A->hide();
        ui->score_total_B->hide();
        ui->result_A->hide();
        ui->result_B->hide();

        ui->COOL_label_B->hide();
        ui->HOT_label_A->hide();
        
        ui->total_label_A->hide();
        ui->total_label_B->hide();

        ui->item_label_A2->hide();
        ui->bonus_label_A2->hide();
        ui->score_label_A2->hide();
        ui->result_label_A2->hide();

        ui->score_label_A->hide();
        ui->result_label_A->hide();

        ui->item_label_B2->hide();
        ui->bonus_label_B2->hide();
        ui->score_label_B2->hide();
        ui->result_label_B2->hide();

        ui->score_label_B->hide();
        ui->result_label_B->hide();
    }
    refreshScoreLabels();
}
