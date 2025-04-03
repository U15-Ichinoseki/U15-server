#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::startAnimation()
{
    static int timer = 1;
    static Field<GameSystem::MAP_OVERLAY> f(this->startupDialog->map.size.y(),
                                            QVector<GameSystem::MAP_OVERLAY>(this->startupDialog->map.size.x(),GameSystem::MAP_OVERLAY::ERASE));
    static int ANIMATION_SIZE = 5;
    static int ANIMATION_TYPE = QRandomGenerator::global()->generate() % ANIMATION_SIZE;
    int count = 0;

    ui->Field->RefreshOverlay(dark);

    QPoint pos[2];
    if(ANIMATION_TYPE == 0){
        //ランダムにワサッて
        //一度に2マス表示させるのですわ
        int i_max = 2;
        if(timer == 1)i_max ++;
        for(int i=0;i<i_max;i++){
            do{
                pos[i].setX(QRandomGenerator::global()->generate() % this->startupDialog->map.size.x());
                pos[i].setY(QRandomGenerator::global()->generate() % this->startupDialog->map.size.y());
            }while(timer < startupDialog->map.size.x() * startupDialog->map.size.y() && f[pos[i].y()][pos[i].x()] != GameSystem::MAP_OVERLAY::ERASE);
            f[pos[i].y()][pos[i].x()] = GameSystem::MAP_OVERLAY::NOTHING;
        }
        for(int i=0;i<this->startupDialog->map.size.y();i++){
            for(int j=0;j<this->startupDialog->map.size.x();j++){
                this->ui->Field->overlay[i][j] = f[i][j];
            }
        }
    }else if(ANIMATION_TYPE == 1){
        //上からガーって
        for(int j=0;j<this->startupDialog->map.size.y();j++){
            for(int k=0;k<this->startupDialog->map.size.x();k++){
                if(count >= timer){
                    this->ui->Field->overlay[j][k] = f[j][k];
                }
                count++;
            }
        }
    }else if(ANIMATION_TYPE == 2){
        //なんかはさみ込む感じで
        for(int j=0;j<this->startupDialog->map.size.y();j++){
            for(int k=0;k<this->startupDialog->map.size.x();k++){
                if(count*2 < timer){
                    f[startupDialog->map.size.y() - j - 1][startupDialog->map.size.x() - k - 1] = GameSystem::MAP_OVERLAY::NOTHING;
                    f[j][k] = GameSystem::MAP_OVERLAY::NOTHING;
                }
                count++;
            }
        }

        for(int i=0;i<this->startupDialog->map.size.y();i++){
            for(int j=0;j<this->startupDialog->map.size.x();j++){
                this->ui->Field->overlay[i][j] = f[i][j];
            }
        }
    }else if(ANIMATION_TYPE == 3){
        //下からガーって
        for(int j=this->startupDialog->map.size.y()-1;j>=0;j--){
            for(int k=this->startupDialog->map.size.x()-1;k>=0;k--){
                if(count >= timer){
                    this->ui->Field->overlay[j][k] = f[j][k];
                }
                count++;
            }
        }
    }else if(ANIMATION_TYPE == 4){
        //中心から
        int cx = startupDialog->map.size.x()/2;
        int cy = startupDialog->map.size.y()/2;
        for(int j=this->startupDialog->map.size.y()-1;j>=0;j--){
            for(int k=this->startupDialog->map.size.x()-1;k>=0;k--){
                if((abs(cx-k)+abs(cy-j)) * this->startupDialog->map.size.x() >= timer){
                    this->ui->Field->overlay[j][k] = f[j][k];
                }
            }
        }
    }
    if(timer >= startupDialog->map.size.x() * startupDialog->map.size.y()){
        timer = 1;

        startupMapAnimation->stop();
        teamShowAnimation->start(teamAnimationTime/TEAM_COUNT);

    } else {
        timer += 2;
    }
    repaint();
}

void MainWindow::showTeamAnimation()
{
    const int blinking = 3;
    static int team_count;

    if (team_count < blinking*2){
        if(team_count%2==0){
            ui->Field->team_pos[0] = this->startupDialog->map.team_first_point[0];
        }else{
            ui->Field->team_pos[0] = QPoint(-3,-3);
        }
    } else {
        ui->Field->team_pos[0] = this->startupDialog->map.team_first_point[0];
        if(team_count%2==0){
            ui->Field->team_pos[1] = this->startupDialog->map.team_first_point[1];
        }else{
            ui->Field->team_pos[1] = QPoint(-3,-3);
        }
    }

    repaint();

    if(team_count >= blinking * 2 * TEAM_COUNT){
        ui->Field->team_pos[0] = this->startupDialog->map.team_first_point[0];
        ui->Field->team_pos[1] = this->startupDialog->map.team_first_point[1];

        this->teamShowAnimation->stop();

        startGameScoreLabels();
        this->gameStatus.winner = GameSystem::GAME_STATUS::WINNER::CONTINUE;
        clock->start(FRAME_RATE);

        if(!silent){
            bgm->play();
        }

        team_count = -1;
    }

    team_count++;
}
