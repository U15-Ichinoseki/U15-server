#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::ResetScoreLebel()
{
    ui->ScoreLabel_A->setStyleSheet(Cool_Point_Label_style);
    ui->NameLabel_A->setStyleSheet(Cool_Point_Label_style);
    ui->NameLabel_A->setText("U15 プロコン");

    ui->ScoreLabel_B->setStyleSheet(Hot_Point_Label_style);
    ui->NameLabel_B->setStyleSheet(Hot_Point_Label_style);
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

    ui->COOL_label_A->setStyleSheet(Cool_Label_style);
    ui->HOT_label_B->setStyleSheet(Hot_Label_style);

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

    ui->COOL_label_B->setStyleSheet(Cool_Label_style);
    ui->HOT_label_A->setStyleSheet(Hot_Label_style);
    
    ui->total_label_A->setStyleSheet(Total_Label_style);
    ui->total_label_B->setStyleSheet(Total_Label_style);

    BottomRoundLabelShow(isfullmode);
}

void MainWindow::StartSetUpScoreLabel()
{

    if(round == 0) {
        ui->ScoreLabel_A->setStyleSheet(Cool_Point_Label_style);
        ui->NameLabel_A->setStyleSheet(Cool_Point_Label_style);

        ui->ScoreLabel_B->setStyleSheet(Hot_Point_Label_style);
        ui->NameLabel_B->setStyleSheet(Hot_Point_Label_style);

        // スタート前にスコアを色付け
        ui->result_1_A->setStyleSheet(Cool_Point_Label_style);
        ui->score_1_A->setStyleSheet(Cool_Point_Label_style);
        ui->item_1_A->setStyleSheet(Cool_Point_Label_style);
        ui->bonus_1_A->setStyleSheet(Cool_Point_Label_style);

        ui->result_1_B->setStyleSheet(Hot_Point_Label_style);
        ui->score_1_B->setStyleSheet(Hot_Point_Label_style);
        ui->item_1_B->setStyleSheet(Hot_Point_Label_style);
        ui->bonus_1_B->setStyleSheet(Hot_Point_Label_style);
    }
    if(round == 1) {
        ui->ScoreLabel_A->setStyleSheet(Hot_Point_Label_style);
        ui->NameLabel_A->setStyleSheet(Hot_Point_Label_style);

        ui->ScoreLabel_B->setStyleSheet(Cool_Point_Label_style);
        ui->NameLabel_B->setStyleSheet(Cool_Point_Label_style);

        ui->result_2_A->setStyleSheet(Hot_Point_Label_style);
        ui->score_2_A->setStyleSheet(Hot_Point_Label_style);
        ui->item_2_A->setStyleSheet(Hot_Point_Label_style);
        ui->bonus_2_A->setStyleSheet(Hot_Point_Label_style);

        ui->result_2_B->setStyleSheet(Cool_Point_Label_style);
        ui->score_2_B->setStyleSheet(Cool_Point_Label_style);
        ui->item_2_B->setStyleSheet(Cool_Point_Label_style);
        ui->bonus_2_B->setStyleSheet(Cool_Point_Label_style);
    }
}

void MainWindow::StartGameScoreLabel()
{
    if(round==0) {
        ui->result_1_A->setStyleSheet("");
        ui->score_1_A->setStyleSheet("");
        ui->item_1_A->setStyleSheet(Cool_Point_Label_style);
        ui->bonus_1_A->setStyleSheet("");
        ui->reason_1_A->setStyleSheet("");

        ui->result_1_B->setStyleSheet("");
        ui->score_1_B->setStyleSheet("");
        ui->item_1_B->setStyleSheet(Hot_Point_Label_style);
        ui->bonus_1_B->setStyleSheet("");
        ui->reason_1_B->setStyleSheet("");
    }
    if(round==1) {
        ui->result_2_A->setStyleSheet("");
        ui->score_2_A->setStyleSheet("");
        ui->item_2_A->setStyleSheet(Hot_Point_Label_style);
        ui->bonus_2_A->setStyleSheet("");
        ui->reason_2_A->setStyleSheet("");

        ui->result_2_B->setStyleSheet("");
        ui->score_2_B->setStyleSheet("");
        ui->item_2_B->setStyleSheet(Cool_Point_Label_style);
        ui->bonus_2_B->setStyleSheet("");
        ui->reason_2_B->setStyleSheet("");
    }
}


void MainWindow::FinishedScoreLabelStyle()
{
    int coolplayer, hotplayer;
    if (round==0) {
        coolplayer = 0;
        hotplayer  = 1;
    }else{
        coolplayer = 1;
        hotplayer  = 0;
    }

    int winner, loser;
    if(game_status.winner == GameSystem::GAME_STATUS::WINNER::COOL){
        winner = coolplayer;
        loser  = hotplayer;
    } else {
        winner = hotplayer;
        loser  = coolplayer;
    }

    QLabel *reason[TEAM_COUNT];
    if (round==0) {
        reason[0] = ui->reason_1_A;
        reason[1] = ui->reason_1_B;    
    }
    else {
        reason[0] = ui->reason_2_A;    
        reason[1] = ui->reason_2_B;
    }

    if (game_status.reason == GameSystem::GAME_STATUS::REASON::SCORE)
    {
        reason[winner]->setText("");
        reason[loser ]->setText("");
    }else if(!isBlunder()) {
        reason[loser ]->setText("");
        if (game_status.reason == GameSystem::GAME_STATUS::REASON::TRAPPED)
            reason[winner]->setText("包囲");
        else if (game_status.reason == GameSystem::GAME_STATUS::REASON::ATTACK)
            reason[winner]->setText("アタック");
    }else{
        reason[winner]->setText("");
        if (game_status.reason == GameSystem::GAME_STATUS::REASON::CONFINED)
            reason[loser ]->setText("自縛");
        else if (game_status.reason == GameSystem::GAME_STATUS::REASON::COLLISION)
            reason[loser ]->setText("衝突");
        else if (game_status.reason == GameSystem::GAME_STATUS::REASON::FOULED)
            reason[loser ]->setText("エラー");
    }

    this->point[coolplayer][round][0] = this->ui->Field->team_score[static_cast<int>(GameSystem::TEAM::COOL)];
    this->point[hotplayer ][round][0] = this->ui->Field->team_score[static_cast<int>(GameSystem::TEAM::HOT)];
    if (game_status.reason == GameSystem::GAME_STATUS::REASON::SCORE)
    {
        this->point[winner][round][1] = 0;
        this->point[loser ][round][1] = 0;
    }else if(!isBlunder()) {
        this->point[winner][round][1] = 7 * this->ui->Field->leave_items;
        this->point[loser ][round][1] = 0;
    }else{
        this->point[winner][round][1] = 0;
        this->point[loser ][round][1] = -3 * this->point[loser][round][0];
    }

    if(game_status.winner == GameSystem::GAME_STATUS::WINNER::DRAW){
        point[winner][round][2] = 1;
        point[loser ][round][2] = 1;
    } else {
        point[winner][round][2] = 3;
        point[loser ][round][2] = 0;
    }

    if (game_status.winner == GameSystem::GAME_STATUS::WINNER::DRAW){
        this->ui->WinnerLabel->setStyleSheet(drawColor);
        this->ui->ResultLabel->setStyleSheet(drawColor);
    } else if (game_status.winner == GameSystem::GAME_STATUS::WINNER::COOL){
        this->ui->WinnerLabel->setStyleSheet(Cool_Point_Label_style);
        this->ui->ResultLabel->setStyleSheet(Cool_Point_Label_style);
    } else if (game_status.winner == GameSystem::GAME_STATUS::WINNER::HOT){
        this->ui->WinnerLabel->setStyleSheet(Hot_Point_Label_style);
        this->ui->ResultLabel->setStyleSheet(Hot_Point_Label_style);
    }

    if(round==0) {
        ui->result_1_A->setStyleSheet(Cool_Score_Label_style);
        ui->score_1_A->setStyleSheet(Cool_Score_Label_style);
        ui->item_1_A->setStyleSheet(Cool_Point_Label_style);

        ui->result_1_B->setStyleSheet(Hot_Score_Label_style);
        ui->score_1_B->setStyleSheet(Hot_Score_Label_style);
        ui->item_1_B->setStyleSheet(Hot_Point_Label_style);

        if(game_status.winner == GameSystem::GAME_STATUS::WINNER::COOL){
            if(! (game_status.reason == GameSystem::GAME_STATUS::REASON::SCORE)){
                if(!isBlunder())
                    ui->bonus_1_A->setStyleSheet(Cool_Point_Label_style);
                else
                    ui->bonus_1_B->setStyleSheet(Hot_Point_Label_style);
            }
            ui->COOL_label_A->setStyleSheet(Cool_Label_Win_style);

        } else if(game_status.winner == GameSystem::GAME_STATUS::WINNER::HOT){
            if(! (game_status.reason == GameSystem::GAME_STATUS::REASON::SCORE)){
                if(!isBlunder())
                    ui->bonus_1_B->setStyleSheet(Hot_Point_Label_style);
                else
                    ui->bonus_1_A->setStyleSheet(Cool_Point_Label_style);
            }
            ui->HOT_label_B->setStyleSheet(Hot_Label_Win_style);
        }
    } else if(round==1) {
        ui->result_2_B->setStyleSheet(Cool_Score_Label_style);
        ui->score_2_B->setStyleSheet(Cool_Score_Label_style);
        ui->item_2_B->setStyleSheet(Cool_Point_Label_style);

        ui->result_2_A->setStyleSheet(Hot_Score_Label_style);
        ui->score_2_A->setStyleSheet(Hot_Score_Label_style);
        ui->item_2_A->setStyleSheet(Hot_Point_Label_style);

        if(game_status.winner == GameSystem::GAME_STATUS::WINNER::COOL){
            if(! (game_status.reason == GameSystem::GAME_STATUS::REASON::SCORE)){
                if(!isBlunder())
                    ui->bonus_2_B->setStyleSheet(Cool_Point_Label_style);
                else
                    ui->bonus_2_A->setStyleSheet(Hot_Point_Label_style);
            }
            ui->COOL_label_B->setStyleSheet(Cool_Label_Win_style);
        } else if(game_status.winner == GameSystem::GAME_STATUS::WINNER::HOT){
            if(! (game_status.reason == GameSystem::GAME_STATUS::REASON::SCORE)){
                if(!isBlunder())
                    ui->bonus_2_A->setStyleSheet(Hot_Point_Label_style);
                else
                    ui->bonus_2_B->setStyleSheet(Cool_Point_Label_style);
            }
            ui->HOT_label_A->setStyleSheet(Hot_Label_Win_style);
        }
    }

    RefreshScoreLabel();
}

void MainWindow::RefreshScoreLabel()
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
    if(this->game_status.winner == GameSystem::GAME_STATUS::WINNER::CONTINUE){
        if (round == 0) {
            if(score_1_A > score_1_B){
                intermediate_result_A = 3;
                intermediate_result_B = 0;
            }
            if(score_1_A < score_1_B){
                intermediate_result_A = 0;
                intermediate_result_B = 3;
            }
        }
        if (round == 1) {
            if(score_2_A > score_2_B){
                intermediate_result_A = 3;
                intermediate_result_B = 0;
            }
            if(score_2_A < score_2_B){
                intermediate_result_A = 0;
                intermediate_result_B = 3;
            }
        }
        ui->total_label_A->setStyleSheet(Total_Label_style);
        ui->total_label_B->setStyleSheet(Total_Label_style);
        if(((result_A + intermediate_result_A) * 1000 + score_total_A) >
        ((result_B + intermediate_result_B) * 1000 + score_total_B)){
            ui->total_label_A->setStyleSheet(Total_Label_Lead_style);
        }
        if(((result_A + intermediate_result_A) * 1000 + score_total_A) <
        ((result_B + intermediate_result_B) * 1000 + score_total_B)){
            ui->total_label_B->setStyleSheet(Total_Label_Lead_style);
        }
    } else {
        // FINISHED
        if (round == 0 || this->game_status.winner == GameSystem::GAME_STATUS::WINNER::NONE) {
            if((result_A * 1000 + score_total_A) > (result_B * 1000 + score_total_B)){
                ui->total_label_A->setStyleSheet(Total_Label_Lead_style);
                ui->total_label_B->setStyleSheet(Total_Label_style);
            }
            if((result_A * 1000 + score_total_A) < (result_B * 1000 + score_total_B)){
                ui->total_label_B->setStyleSheet(Total_Label_Lead_style);
                ui->total_label_A->setStyleSheet(Total_Label_style);
            }
        }
        else if (round == 1) {
            ui->score_total_A->setStyleSheet(Total_Label_decide_style);
            ui->result_A->setStyleSheet(Total_Label_decide_style);
            ui->score_total_B->setStyleSheet(Total_Label_decide_style);
            ui->result_B->setStyleSheet(Total_Label_decide_style);
            
            if((result_A * 1000 + score_total_A) == (result_B  * 1000 + score_total_B)){
                ui->total_label_A->setStyleSheet(Total_Label_Lead_style);
                ui->total_label_B->setStyleSheet(Total_Label_Lead_style);
            }
            if((result_A * 1000 + score_total_A) > (result_B  * 1000 + score_total_B)){
                ui->total_label_A->setStyleSheet(Total_Label_Win_style);
                ui->total_label_B->setStyleSheet(Total_Label_style);
            }
            if((result_A * 1000 + score_total_A) < (result_B * 1000 + score_total_B)){
                ui->total_label_B->setStyleSheet(Total_Label_Win_style);
                ui->total_label_A->setStyleSheet(Total_Label_style);
            }
        }
    }
}

void MainWindow::BottomRoundLabelShow(bool set)
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
    RefreshScoreLabel();
}
