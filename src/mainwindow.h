#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QTimer>
#include <QKeyEvent>
#include <QDateTime>
#include <QFile>
#include <QFileInfo>
#include <QSoundEffect>
#include <QDataStream>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QThread>

#include <QSettings>
#include <QRandomGenerator>
#include "Definition.h"

#include <QMainWindow>

#include "GameSystem.h"
#include "StableLog.h"
#include "startupdialog.h"

namespace Ui {
class MainWindow;
}

class StartupDialog;

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    Ui::MainWindow *ui;
    StartupDialog *startup; //スタートアップダイアログ

    int player;      //次ターン行動プレイヤー
    int round;
    int point[TEAM_COUNT][ROUND_COUNT][3];
    
    GameSystem::GAME_STATUS game_status;

    bool getready_flag;

    bool silent;
    bool dark;         //暗転処理

    bool isbotbattle; //ボット戦モード
    bool isrepeat;    //繰り返し対戦
    bool isdemo;      //デモ対戦
    bool isfullmode;  //

    int FRAME_RATE = 150;   //ゲームフレームレート

    int anime_map_time   = 100; //マップ構築アニメーション時間
    int anime_team_time  = 500; //チーム配置アニメーション時間

    QTimer* clock;          //ゲームクロック
    QTimer* startup_anime;  //開始アニメーション
    QTimer* teamshow_anime; //チーム表示アニメーション

    QMediaPlayer *bgm;          //音楽
    QAudioOutput *audio_output; //音声出力
    QMediaPlayer *SE_C;           //SE
    QAudioOutput *SE_C_AO;        //SE出力
    QMediaPlayer *SE_H;           //SE
    QAudioOutput *SE_H_AO;        //SE出力

    QFile* file;   //ログファイル
    StableLog log; //ログストリーム

    const QString Cool_Label_style = "border-radius: 15px;border:2px solid blue;color:blue;background-color:white;";
    const QString Cool_Label_Win_style = "border-radius: 15px;border:2px solid blue;background-color:blue;color:white;";

    const QString Hot_Label_style = "border-radius: 15px;border:2px solid red;color:red;background-color:white;";
    const QString Hot_Label_Win_style = "border-radius: 15px;border:2px solid red;background-color:red;color:white;";

    const QString Total_Label_style = "border-radius: 15px;border:3px solid green;color:green;background-color:white;";
    const QString Total_Label_Lead_style = "border-radius: 15px;border:3px solid green;background-color:#C1DB81;";
    const QString Total_Label_Win_style = "border-radius: 15px;color:white;background-color:green;";
    const QString Total_Label_decide_style = "border-radius: 15px;border:3px solid green;background-color:white;";

    const QString drawColor = "border-radius: 15px;background-color:#C1DB81;";
    const QString Cool_Point_Label_style = "border-radius: 15px;border:2px solid blue;background-color:#54C3F1;";
    const QString Hot_Point_Label_style  = "border-radius: 15px;border:2px solid red;background-color:#EE87B4;";
    const QString Cool_Score_Label_style = "border-radius: 15px;border:3px solid blue;background-color:white;";
    const QString Hot_Score_Label_style  = "border-radius: 15px;border:3px solid red;background-color:white;";

protected:
    void keyPressEvent(QKeyEvent* event);

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void setSetting();
    void setDesign();
    void setPath();

    void repaintMap();

    void RefreshItem(GameSystem::Method method);
    void RefreshScore();
    GameSystem::GAME_STATUS Judge();
    void Finish(GameSystem::GAME_STATUS win);

    bool isBlunder();

    static QString getTime();
    static QString convertString(GameSystem::Method method);

    // ScoreLabelStyle
    void ResetScoreLebel();
    void StartSetUpScoreLabel();
    void StartGameScoreLabel();
    void FinishedScoreLabelStyle();
    void RefreshScoreLabel();
    void BottomRoundLabelShow(bool set);

private slots:
    //ゲーム進行
    void ReSetUp();
    void StartSetUp();
    void StartGame();
    void StartAnimation();
    void ShowTeamAnimation();
    void StepGame();
    void RepeatGame();
    void EndGame();

    void SaveFile();
};

#endif // MAINWINDOW_H
