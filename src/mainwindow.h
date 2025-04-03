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
#include "game.h"
#include "animation.h"
#include "ScoreLabelStyle.h" // 新しく追加

namespace Ui {
class MainWindow;
}

class StartupDialog;

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    Ui::MainWindow *ui;
    StartupDialog *startupDialog; // スタートアップダイアログ

    int player;      //次ターン行動プレイヤー
    int currentround;
    int point[TEAM_COUNT][ROUND_COUNT][3];

    struct RandomMapParameters {
        int defaultItem;
        int defaultBlock;
        int defaultTurn;
        bool defaultMirror;
    };

    RandomMapParameters randomMapParameters;

    GameSystem::GAME_STATUS gameStatus;

    bool isReady;

    bool silent;
    bool dark; // 暗転処理

    bool isBotBattleMode; // ボット戦モード
    bool isRepeatMode;    // 繰り返し対戦
    bool isDemoMode;      // デモ対戦
    bool isDoubleGameMode;  //

    int FRAME_RATE = 150; // ゲームフレームレート

    int mapAnimationTime = 100; // マップ構築アニメーション時間
    int teamAnimationTime = 500; // チーム配置アニメーション時間

    QTimer* clock;          // ゲームクロック
    QTimer* startupMapAnimation;   // 開始アニメーション
    QTimer* teamShowAnimation;  // チーム表示アニメーション

    QString musicList[2];

    QMediaPlayer *bgm; // 音楽
    QAudioOutput *audioOutput;     // 音声出力
    QMediaPlayer *soundEffectCool;    // SE
    QAudioOutput *soundEffectAudioOutputCool; // SE出力
    QMediaPlayer *soundEffectHot;    // SE
    QAudioOutput *soundEffectAudioOutputHot; // SE出力

    QFile* logFile;   // ログファイル
    StableLog logStream; // ログストリーム

    // Game game;
    // Animation animation;

protected:
    void keyPressEvent(QKeyEvent* event);

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void initializeSettings();
    void applyDesign();
    void setRandomMapParameters();
    void setPaths();
    void setMusicList( QString filename);
    void setMusicList( QString filename, int round);
    void setMusicList( QString filename1,  QString filename2);

    void repaintMap();

    void refreshItems(GameSystem::Method method);
    void refreshScores();
    GameSystem::GAME_STATUS judgeGame();
    void finishGame(GameSystem::GAME_STATUS win);

    bool isBlunder();

    static QString getCurrentTime();
    static QString convertMethodToString(GameSystem::Method method);

private slots:
    void resetSetup();
    void startSetup();
    void startGame();
    void startAnimation();
    void showTeamAnimation();
    void stepGame();
    void repeatGame();
    void endGame();

    void saveToFile();

    void resetScoreLabels();
    void startSetupScoreLabels();
    void startGameScoreLabels();
    void finishScoreLabelStyle();
    void refreshScoreLabels();
    void showBottomRoundLabel(bool set);
};

#endif // MAINWINDOW_H
