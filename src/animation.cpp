#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "CircleLabel.h"

void MainWindow::startAnimation()
{
    static int timer = 1;

    static int ANIMATION_SIZE = 4;
    static int ANIMATION_TYPE = QRandomGenerator::global()->generate() % ANIMATION_SIZE;

    ui->Field->RefreshOverlay(dark);

    if (timer == 1)
    {
        ANIMATION_TYPE = QRandomGenerator::global()->generate() % ANIMATION_SIZE;
        showCountDown("Ready", 2000, Qt::red, Qt::white);
    }
    
    ANIMATION_TYPE = 0;
    if (ANIMATION_TYPE == 1)
    {
        // 上からガーって
        int count = 0;
        for (int y = 0; y < this->startupDialog->map.size.y(); y++)
        {
            for (int x = 0; x < this->startupDialog->map.size.x(); x++)
            {
                if (count >= timer)
                {
                    this->ui->Field->overlay[y][x] = GameSystem::MAP_OVERLAY::NOTHING;
                }
                count++;
            }
        }
    }
    else if (ANIMATION_TYPE == 2)
    {
        // なんかはさみ込む感じで
        int count = 0;
        for (int x = 0; x < this->startupDialog->map.size.x(); x++)
        {
            for (int y = 0; y < this->startupDialog->map.size.y(); y++)
            {
                if (count >= timer * 2)
                {
                    this->ui->Field->overlay[y][x] = GameSystem::MAP_OVERLAY::NOTHING;
                }
                count++;
            }
        }
        count = 0;
        for (int x = this->startupDialog->map.size.x() - 1; x >= 0; x--)
        {
            for (int y = this->startupDialog->map.size.y() - 1; y >= 0; y--)
            {
                if (count >= timer * 2)
                {
                    this->ui->Field->overlay[y][x] = GameSystem::MAP_OVERLAY::NOTHING;
                }
                count++;
            }
        }
    }
    else if (ANIMATION_TYPE == 3)
    {
        // 下からガーって
        int count = 0;
        for (int y = this->startupDialog->map.size.y() - 1; y >= 0; y--)
        {
            for (int x = this->startupDialog->map.size.x() - 1; x >= 0; x--)
            {
                if (count >= timer)
                {
                    this->ui->Field->overlay[y][x] = GameSystem::MAP_OVERLAY::NOTHING;
                }
                count++;
            }
        }
    }
    else
    {
        // 中心から
        int cx = startupDialog->map.size.x() / 2;
        int cy = startupDialog->map.size.y() / 2;
        for (int y = this->startupDialog->map.size.y() - 1; y >= 0; y--)
        {
            for (int x = this->startupDialog->map.size.x() - 1; x >= 0; x--)
            {
                if ((abs(cx - x) + abs(cy - y)) * this->startupDialog->map.size.x() >= timer)
                {
                    this->ui->Field->overlay[y][x] = GameSystem::MAP_OVERLAY::NOTHING;
                }
            }
        }
    }
    if (timer >= startupDialog->map.size.x() * startupDialog->map.size.y())
    {
        timer = 1;

        startupMapAnimation->stop();
        if (this->teamShowAnimation)
        {
            this->teamShowAnimation->start(this->teamAnimationTime / TEAM_COUNT);
        }
    }
    else
    {
        timer += 4;
    }
    repaint();
}

void MainWindow::showTeamAnimation()
{
    const int blinking = 3;
    static int team_count = 0;
    
    if (team_count == 0)
    {
        showCountDown("Go!", 1200, Qt::darkGreen, Qt::white);
    }

    if (team_count < blinking * 2)
    {
        if (team_count % 2 == 0)
        {
            ui->Field->team_pos[0] = this->startupDialog->map.team_first_point[0];
        }
        else
        {
            ui->Field->team_pos[0] = QPoint(-3, -3);
        }
    }
    else
    {
        ui->Field->team_pos[0] = this->startupDialog->map.team_first_point[0];
        if (team_count % 2 == 0)
        {
            ui->Field->team_pos[1] = this->startupDialog->map.team_first_point[1];
        }
        else
        {
            ui->Field->team_pos[1] = QPoint(-3, -3);
        }
    }

    repaint();

    if (team_count >= blinking * 2 * TEAM_COUNT)
    {
        ui->Field->team_pos[0] = this->startupDialog->map.team_first_point[0];
        ui->Field->team_pos[1] = this->startupDialog->map.team_first_point[1];

        this->teamShowAnimation->stop();

        startGameScoreLabels();
        this->gameStatus.winner = GameSystem::GAME_STATUS::WINNER::CONTINUE;
        clock->start(FRAME_RATE);

        if (!silent)
        {
            bgm->play();
        }

        team_count = -1;
    }

    team_count++;
}

void MainWindow::showCountDown(QString stepText, int stepDuration, QColor textColor, QColor outlineColor, bool feadOut)
{

    QWidget *field = ui->Field;
    if (field)
    {
        CircleLabel *circle = field->findChild<CircleLabel *>("countdownCircle");
        if (!circle)
        {
            circle = new CircleLabel(field);
            circle->setObjectName("countdownCircle");
            circle->setBgColor(QColor(255, 255, 255, 4));
        }
        // スタイル（Go は緑系）
        circle->setTextColor(textColor);
        circle->setTextOutlineColor(outlineColor);
        circle->setTextOutlineWidth(8);
        circle->enableTextShadow(true);
        circle->setTextShadowColor(QColor(0, 0, 0, 160));
        circle->setTextShadowOffset(QPoint(6, 6));

        circle->setVisible(false);
        circle->raise();

        QRect frect = circle->parentWidget()->rect();
        int large = qMax((int)(frect.width() * 0.6), 200);
        int small = qMax((int)(frect.width() * 0.12), 80);

        circle->setText(stepText);
        circle->setDiameter(small);
        circle->setVisible(true);

        QGraphicsOpacityEffect *eff = qobject_cast<QGraphicsOpacityEffect *>(circle->graphicsEffect());
        if (!eff)
        {
            eff = new QGraphicsOpacityEffect(circle);
            circle->setGraphicsEffect(eff);
        }
        eff->setOpacity(0.0);

        QPropertyAnimation *anim = new QPropertyAnimation(circle, "diameter", circle);
        anim->setDuration(stepDuration);
        anim->setStartValue(small);
        anim->setEndValue(large);
        anim->setEasingCurve(QEasingCurve::OutCubic);
        anim->start(QAbstractAnimation::DeleteWhenStopped);

        QPropertyAnimation *op = new QPropertyAnimation(eff, "opacity", eff);
        op->setDuration(stepDuration);
        if(feadOut){
            op->setKeyValueAt(0.0, 0.0);
            op->setKeyValueAt(0.25, 1.0);
            op->setKeyValueAt(0.85, 1.0);
            op->setKeyValueAt(1.0, 0.0);
        } else {
            op->setKeyValueAt(0.0, 0.4);
            op->setKeyValueAt(1.0, 1.0);
        }
        op->setEasingCurve(QEasingCurve::OutCubic);
        op->start(QAbstractAnimation::DeleteWhenStopped);

        QTimer::singleShot(stepDuration + 150, this, [circle, feadOut]()
        {
            if(feadOut){
                circle->setVisible(false);
            }
            circle->setGraphicsEffect(nullptr);
        });
    }
}
