#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::resetSetup()
{
    currentround = -1;

    this->startupDialog->resetMap(
        randomMapParameters.defaultBlock,
        randomMapParameters.defaultItem,
        randomMapParameters.defaultTurn,
        randomMapParameters.defaultMirror);
    ui->Field->setFlip(false);
    this->ui->Field->setMap(this->startupDialog->map);

    for(int i = 0; i < TEAM_COUNT; i++){
        for(int j = 0; j < ROUND_COUNT; j++){
            for(int k = 0; k < 3; k++)
                    this->point[i][j][k]=0;
        }
    }

    resetScoreLabels();
}

void MainWindow::startSetup()
{
    currentround++;
    qDebug()<< "round:" << currentround;

    this->gameStatus.winner = GameSystem::GAME_STATUS::WINNER::NONE;

    this->ui->WinnerLabel->hide();
    this->ui->ResultLabel->hide();

    //マップ初期化
    for(int i=0;i<TEAM_COUNT;i++){
        this->ui->Field->team_pos[i] = this->startupDialog->map.team_first_point[i];
    }
    //ui初期化
    this->ui->Field->setMap(this->startupDialog->map);
    this->ui->Field->turn_count = this->startupDialog->map.turn;

    this->ui->TimeBar_A->setMaximum(this->startupDialog->map.turn);
    this->ui->TimeBar_A->setValue  (this->startupDialog->map.turn);
    this->ui->TimeBar_A->show();
    this->ui->TimeBar_B->setMaximum(this->startupDialog->map.turn);
    this->ui->TimeBar_B->setValue  (this->startupDialog->map.turn);
    this->ui->TimeBar_B->show();

    this->ui->TurnLabel->setText(QString::number(this->startupDialog->map.turn));

    this->startupDialog->setGameStartButtonEnabled(true);

    if(currentround == 0) {
        this->ui->NameLabel_A->setText(
            this->startupDialog->teamClient[static_cast<int>(GameSystem::TEAM::COOL)]->client->Name == "" ?
            "Cool" :
            this->startupDialog->teamClient[static_cast<int>(GameSystem::TEAM::COOL)]->client->Name);

        this->ui->NameLabel_B->setText(
            this->startupDialog->teamClient[static_cast<int>(GameSystem::TEAM::HOT )]->client->Name == "" ?
            "Hot" :
            this->startupDialog->teamClient[static_cast<int>(GameSystem::TEAM::HOT )]->client->Name);
        ui->Field->setFlip(false);
    }
    if(currentround == 1) {
        this->ui->NameLabel_B->setText(
            this->startupDialog->teamClient[static_cast<int>(GameSystem::TEAM::COOL)]->client->Name == "" ?
            "Cool" :
            this->startupDialog->teamClient[static_cast<int>(GameSystem::TEAM::COOL)]->client->Name);
        this->ui->NameLabel_A->setText(
            this->startupDialog->teamClient[static_cast<int>(GameSystem::TEAM::HOT )]->client->Name == "" ?
            "Hot" :
            this->startupDialog->teamClient[static_cast<int>(GameSystem::TEAM::HOT )]->client->Name);
        ui->Field->setFlip(true);
    }
    startSetupScoreLabels();

    //ボット戦モードならば表記の変更
    if(this->isBotBattleMode){
        this->ui->ScoreLabel_B ->setText(QString::number(this->startupDialog->map.turn) + "(ITEM:0)");
        this->ui->ScoreLabel_A->setText(QString::number(this->startupDialog->map.turn) + "(ITEM:0)");
    }else{
        this->ui->ScoreLabel_B ->setText("0");
        this->ui->ScoreLabel_A->setText("0");
    }

    if (currentround<-1){
        //画面のサイズに合わせてリサイズ
        // resize(QSize(window_width, window_height));
    }
 
    player = 0;

    for(int i=0; i<TEAM_COUNT; i++)
        this->ui->Field->team_score[i] = 0;

    //アイテム数ラベルセット
    this->ui->Field->leave_items = 0;
    for(int i=0;i<startupDialog->map.size.y();i++){
       for(int j=0;j<startupDialog->map.size.x();j++){
            if(startupDialog->map.field[i][j] == GameSystem::MAP_OBJECT::ITEM)this->ui->Field->leave_items++;
       }
    }
    ui->ItemLeaveLabel->setText(QString::number(this->ui->Field->leave_items));

    ui->Field->repaint();

    this->startupDialog->showStandbyButton(false);
    this->startupDialog->enableSetupMode(false);
    if (isDemoMode) {
        QTimer::singleShot(100, this, SLOT(startGame()));
    }

    isReady=true;

    this->startupDialog->enableConnectionChange(false);
    
    logStream << getCurrentTime() + "セットアップ完了　ゲームを開始します。\r\n";
}

void MainWindow::startGame()
{
    qDebug() << "Game Start";
    this->startupDialog->showGameStartButton(false);
    
    this->startupDialog->enableMusicSelection(false);
    //消音モードじゃない かつ Musicフォルダに音楽が存在する ならBGMセット
    if(!silent && this->startupDialog->musicText != "none"){
        bgm = new QMediaPlayer;
        audioOutput = new QAudioOutput;
        bgm->setAudioOutput(audioOutput);
        bgm->setSource(QUrl::fromLocalFile("./Music/" + this->startupDialog->musicText));
        audioOutput->setVolume(1.0);
        logStream << "[ Music : " + this->startupDialog->musicText + " ]" + "\r\n";
    }

    for(int i=0;i<TEAM_COUNT;i++){
        ui->Field->team_pos[i].setX(-1);
        ui->Field->team_pos[i].setY(-1);
    }
    startupMapAnimation->start(qMax(this->mapAnimationTime / (startupDialog->map.size.x()*startupDialog->map.size.y()), 1));
}

void MainWindow::stepGame()
{
    //ゲーム進行
    static GameSystem::Method team_mehod[TEAM_COUNT];

    this->ui->Field->RefreshOverlay(dark);

    if(player == 0 && isReady){
        //ターンログ出力
        logStream << QString("-----残") + QString::number(this->ui->Field->turn_count) + "ターン-----" + "\r\n";
        qDebug() << QString("-----残") + QString::number(this->ui->Field->turn_count) + "ターン-----";
     }

    //GetReadyの取得
    if(isReady){
        // GetReady
        if(!startupDialog->teamClient[player]->client->WaitGetReady()){
            logStream << getCurrentTime() + "[停止]" + GameSystem::TEAM_PROPERTY::getTeamName(static_cast<GameSystem::TEAM>(player)) + "が正常にGetReadyを返しませんでした!0" << "\r\n";
            startupDialog->teamClient[player]->client->is_disconnected = true;
            gameStatus.cause = GameSystem::GAME_STATUS::CAUSE::NOGETREADY;
        }else{
            //logStream << getCurrentTime() + "GetReady" + "\r\n";
            GameSystem::AroundData buffer = ui->Field->FieldAccessAround(GameSystem::Method{static_cast<GameSystem::TEAM>(player),
                                                                         GameSystem::Method::ACTION::GETREADY,
                                                                         GameSystem::Method::ROTE::UNKNOWN},
                                                                         ui->Field->team_pos[player]);
            team_mehod[player] = startupDialog->teamClient[player]->client->WaitReturnMethod(buffer);

            if(team_mehod[player].action == GameSystem::Method::ACTION::GETREADY){
                logStream << getCurrentTime() + "[停止]" + GameSystem::TEAM_PROPERTY::getTeamName(static_cast<GameSystem::TEAM>(player)) + "が二度GetReadyを行いました!" << "\r\n";
                startupDialog->teamClient[player]->client->is_disconnected = true;
                gameStatus.cause = GameSystem::GAME_STATUS::CAUSE::RESEND;
            }
            team_mehod[player].team = static_cast<GameSystem::TEAM>(player);
        }
        //End
        refreshItems(team_mehod[player]);
        this->gameStatus = judgeGame();

        if(gameStatus.winner != GameSystem::GAME_STATUS::WINNER::CONTINUE){

            player++;
            player %= TEAM_COUNT;

            startupDialog->teamClient[player]->client->WaitGetReady();
            GameSystem::AroundData buffer = ui->Field->FieldAccessAround(GameSystem::Method{static_cast<GameSystem::TEAM>(player),
                                                                         GameSystem::Method::ACTION::GETREADY,
                                                                         GameSystem::Method::ROTE::UNKNOWN},
                                                                         ui->Field->team_pos[player]);
            buffer.connect = GameSystem::CONNECTING_STATUS::FINISHED;
            team_mehod[player] = startupDialog->teamClient[player]->client->WaitReturnMethod(buffer);
            finishGame(gameStatus);
        }

    }else{

        // Method
        GameSystem::AroundData around = ui->Field->FieldAccessMethod(team_mehod[player]);
        //アイテムの回収
        refreshItems(team_mehod[player]);
        this->gameStatus = judgeGame();
        if(this->gameStatus.winner != GameSystem::GAME_STATUS::WINNER::CONTINUE){
            around.connect = GameSystem::CONNECTING_STATUS::FINISHED;
        }

        if(startupDialog->teamClient[player]->client->WaitEndSharp(around)){
            //不正行動をはじく
            if(team_mehod[player].action == GameSystem::Method::ACTION::UNKNOWN){
                logStream << getCurrentTime() + "[停止]" + GameSystem::TEAM_PROPERTY::getTeamName(static_cast<GameSystem::TEAM>(player)) + "が不正なメソッドを呼んでいます！" << "\r\n";
                startupDialog->teamClient[player]->client->is_disconnected = true;
                gameStatus.cause = GameSystem::GAME_STATUS::CAUSE::UNKNOWNACTION;
            }
            if(team_mehod[player].rote   == GameSystem::Method::ROTE::UNKNOWN){
                logStream << getCurrentTime() + "[停止]" + GameSystem::TEAM_PROPERTY::getTeamName(static_cast<GameSystem::TEAM>(player)) + "の行動メソッドが不正な方向を示しています！" << "\r\n";
                startupDialog->teamClient[player]->client->is_disconnected = true;
                gameStatus.cause = GameSystem::GAME_STATUS::CAUSE::UNKNOWNROTE;
            }

            //行動ログの出力
            logStream << getCurrentTime() + "[行動]" + GameSystem::TEAM_PROPERTY::getTeamName(static_cast<GameSystem::TEAM>(player)) + "が" + convertMethodToString(team_mehod[player]) + "を行いました。" << "\r\n";

            GameBoard*& board = this->ui->Field;
            GameSystem::AroundData team_around = board->FieldAccessAround(static_cast<GameSystem::TEAM>(player));
            logStream << getCurrentTime() + GameSystem::TEAM_PROPERTY::getTeamName(static_cast<GameSystem::TEAM>(player)) + ":" + team_around.toString() << "\r\n";

            //refresh
            if(player == TEAM_COUNT-1){
                ui->Field->turn_count --;
                ui->TurnLabel->setText(QString::number(this->ui->Field->turn_count));
                ui->TimeBar_A->setValue(this->ui->Field->turn_count);
                ui->TimeBar_A->repaint();

                //ボット戦モードならば表記のリアルタイム更新
                if(this->isBotBattleMode){
                    int ScoreBuf = this->ui->Field->team_score[static_cast<int>(GameSystem::TEAM::COOL)];
                    ui->ScoreLabel_A->setText(QString::number(ui->Field->turn_count + ScoreBuf*3) + "(ITEM:" + QString::number(ScoreBuf) + ")");
                    ScoreBuf = this->ui->Field->team_score[static_cast<int>(GameSystem::TEAM::HOT)];
                    ui->ScoreLabel_B ->setText(QString::number(ui->Field->turn_count + ScoreBuf*3) + "(ITEM:" + QString::number(ScoreBuf) + ")");

                }
            }else{
                ui->TimeBar_B->setValue(ui->Field->turn_count);
                ui->TimeBar_B->repaint();                
            }
        }else{
            logStream << getCurrentTime() + "[停止]" + GameSystem::TEAM_PROPERTY::getTeamName(static_cast<GameSystem::TEAM>(player)) + "が正常にGetReadyを返しませんでした!1" << "\r\n";
            startupDialog->teamClient[player]->client->is_disconnected = true;
        }
        //End

        player++;
        player %= TEAM_COUNT;

        if(gameStatus.winner != GameSystem::GAME_STATUS::WINNER::CONTINUE){
            startupDialog->teamClient[player]->client->WaitGetReady();
            GameSystem::AroundData buffer = ui->Field->FieldAccessAround(GameSystem::Method{static_cast<GameSystem::TEAM>(player),
                                                                         GameSystem::Method::ACTION::GETREADY,
                                                                         GameSystem::Method::ROTE::UNKNOWN},
                                                                         ui->Field->team_pos[player]);
            buffer.connect = GameSystem::CONNECTING_STATUS::FINISHED;
            team_mehod[player] = startupDialog->teamClient[player]->client->WaitReturnMethod(buffer);
            finishGame(gameStatus);
        }

    }

    ui->Field->repaint();
    isReady = !isReady;
}

void MainWindow::repeatGame()
{
    qDebug() << "Game Reset";
    this->startupDialog->showGameStartButton(true);
    this->startupDialog->enableConnectionChange(true);
    this->startupDialog->enableSetupMode(true);

    if (!isDemoMode) {
        this->startupDialog->swapClientConnections();
        resetSetup();
    } else {
        this->startupDialog->randomizeClientConnections();
        resetSetup();
        QTimer::singleShot(1000, this, SLOT(startSetup()));
    }
}

void MainWindow::endGame()
{
    exit(0);
}

void MainWindow::refreshItems(GameSystem::Method method)
{
    static int leave_item = 0;
    if(leave_item == 0)leave_item = this->ui->Field->leave_items;
    if(this->ui->Field->leave_items != leave_item){
        if(!silent){
            if (player==0){
                soundEffectCool->setPosition(0);
                soundEffectCool->play();
            }
            else {
                soundEffectHot->setPosition(0);
                soundEffectHot->play();
            }
        }
        ui->ItemLeaveLabel->setText(QString::number(this->ui->Field->leave_items));
        logStream << getCurrentTime() + "[取得]" + GameSystem::TEAM_PROPERTY::getTeamName(method.team) + "がアイテムを取得しました。" << "\r\n";
        if(this->isBotBattleMode){
            int ScoreBuf = this->ui->Field->team_score[static_cast<int>(GameSystem::TEAM::COOL)];
            ui->ScoreLabel_A->setText(QString::number(ui->Field->turn_count + ScoreBuf*3) + "(ITEM:" + QString::number(ScoreBuf) + ")");
            ScoreBuf = this->ui->Field->team_score[static_cast<int>(GameSystem::TEAM::HOT)];
            ui->ScoreLabel_B ->setText(QString::number(ui->Field->turn_count + ScoreBuf*3) + "(ITEM:" + QString::number(ScoreBuf) + ")");
        }else{
            if(currentround==0) {
                ui->ScoreLabel_A->setText(QString::number(this->ui->Field->team_score[static_cast<int>(GameSystem::TEAM::COOL)]));
                ui->ScoreLabel_B ->setText(QString::number(this->ui->Field->team_score[static_cast<int>(GameSystem::TEAM::HOT)]));
            } else {
                ui->ScoreLabel_B->setText(QString::number(this->ui->Field->team_score[static_cast<int>(GameSystem::TEAM::COOL)]));
                ui->ScoreLabel_A ->setText(QString::number(this->ui->Field->team_score[static_cast<int>(GameSystem::TEAM::HOT)]));
            }
        }
        leave_item = this->ui->Field->leave_items;
        refreshScores();
    }
}

void MainWindow::refreshScores()
{
    int coolplayer, hotplayer;
    if (currentround==0) {
        coolplayer = 0;
        hotplayer  = 1;           
    }
    else {
        coolplayer = 1;
        hotplayer  = 0;           
    }

    this->point[coolplayer][currentround][0] = this->ui->Field->team_score[static_cast<int>(GameSystem::TEAM::COOL)];
    this->point[hotplayer ][currentround][0] = this->ui->Field->team_score[static_cast<int>(GameSystem::TEAM::HOT)];
    refreshScoreLabels();
}

GameSystem::GAME_STATUS MainWindow::judgeGame()
{
    bool team_lose[TEAM_COUNT];
    int _player = player;
    for(int i=0;i<TEAM_COUNT;i++)team_lose[i] = false;
    GameBoard*& board = this->ui->Field;

    GameSystem::GAME_STATUS gameStatus;

    gameStatus.winner = GameSystem::GAME_STATUS::WINNER::NONE;
    gameStatus.reason = GameSystem::GAME_STATUS::REASON::NONE;

    for(int i=0;i<TEAM_COUNT;i++){
        // 現在のplayer(COOL or HOT)
        _player = (_player + 1) % TEAM_COUNT;

        GameSystem::AroundData team_around = board->FieldAccessAround(static_cast<GameSystem::TEAM>(_player));
        // logStream << getCurrentTime() + GameSystem::TEAM_PROPERTY::getTeamName(static_cast<GameSystem::TEAM>(_player)) + ":" + team_around.toString() << "\r\n";

        //ブロック置かれ負け
        if(team_around.data[4] == GameSystem::MAP_OBJECT::BLOCK){
            logStream << getCurrentTime() + "[敗因]" + GameSystem::TEAM_PROPERTY::getTeamName(static_cast<GameSystem::TEAM>(_player)) + "ブロック下敷き" << "\r\n";
            if (player != _player){
                gameStatus.reason = GameSystem::GAME_STATUS::REASON::ATTACK;

            }else{
                gameStatus.reason = GameSystem::GAME_STATUS::REASON::COLLISION;
            }
            team_around.finish();
            team_lose[_player]=true;
            break;
        }

        //ブロック囲まれ負け
        if(team_around.data[1] == GameSystem::MAP_OBJECT::BLOCK &&
           team_around.data[3] == GameSystem::MAP_OBJECT::BLOCK &&
           team_around.data[5] == GameSystem::MAP_OBJECT::BLOCK &&
           team_around.data[7] == GameSystem::MAP_OBJECT::BLOCK){
            logStream << getCurrentTime() + "[敗因]" + GameSystem::TEAM_PROPERTY::getTeamName(static_cast<GameSystem::TEAM>(_player)) + "ブロック囲まれ" << "\r\n";
            if (player != _player){
                gameStatus.reason = GameSystem::GAME_STATUS::REASON::TRAPPED;

            }else{
                gameStatus.reason = GameSystem::GAME_STATUS::REASON::CONFINED;
            }
            team_around.finish();
            team_lose[_player]=true;
            break;
        }

        //切断負け
        if(startupDialog->teamClient[_player]->client->is_disconnected){
            logStream << getCurrentTime() + "[敗因]" + GameSystem::TEAM_PROPERTY::getTeamName(static_cast<GameSystem::TEAM>(_player)) + "通信切断" << "\r\n";
            gameStatus.reason = GameSystem::GAME_STATUS::REASON::FOULED;
            team_around.finish();
            team_lose[_player]=true;
            break;
        }

    }

    //相打ち、または時間切れ時はアイテム判定とする
    if(!std::find(team_lose,team_lose+TEAM_COUNT,false) || ui->Field->turn_count==0){
        logStream << getCurrentTime() + "[情報]相打ちまたは、タイムアップのためアイテム判定を行います" + "\r\n";
        logStream << getCurrentTime() + "[得点]";
        for(int i=0;i<TEAM_COUNT;i++){
            logStream << GameSystem::TEAM_PROPERTY::getTeamName(static_cast<GameSystem::TEAM>(i)) + ":" + QString::number(this->ui->Field->team_score[i]) + "  ";
            team_lose[i] = false;
        }
        logStream << "\r\n";

        //引き分けかな？
        gameStatus.reason = GameSystem::GAME_STATUS::REASON::SCORE;
        QSet<int> team_score_set; //スコア
        for(int i=0;i<TEAM_COUNT;i++){
            team_score_set.insert(this->ui->Field->team_score[i]);
        }
        if(team_score_set.size()==1){
            gameStatus.winner = GameSystem::GAME_STATUS::WINNER::DRAW;
            return gameStatus;
        }

        int index=0;
        for(int i=0;i<TEAM_COUNT;i++){
            if(this->ui->Field->team_score[index] < this->ui->Field->team_score[i])index = i;
        }
        //勝者判定
        gameStatus.winner = static_cast<GameSystem::GAME_STATUS::WINNER>(index);
        return gameStatus;
    }

    if(team_lose[0]){
        gameStatus.winner = GameSystem::GAME_STATUS::WINNER::HOT;
        return gameStatus;
    }
    else if(team_lose[1]){
        gameStatus.winner = GameSystem::GAME_STATUS::WINNER::COOL;
        return gameStatus;
    }
    else {
        gameStatus.winner = GameSystem::GAME_STATUS::WINNER::CONTINUE;
        return gameStatus;
    }
}

//終了処理
void MainWindow::finishGame(GameSystem::GAME_STATUS gameStatus)
{
    qDebug() << "Game Finish";
    this->clock->stop();

    QString append_str = "";
    //disconnect
    for(int i=0;i<TEAM_COUNT;i++){
        if(startupDialog->teamClient[i]->client->is_disconnected){
            logStream << getCurrentTime() + "[終了]" + GameSystem::TEAM_PROPERTY::getTeamName(static_cast<GameSystem::TEAM>(i)) + "との通信が切断されています。" << "\r\n";
        }
    }
    logStream << this->ui->WinnerLabel->text() << "\r\n";

    if(!silent){
        bgm->stop();
        if(gameStatus.reason == GameSystem::GAME_STATUS::REASON::SCORE)
            bgm->setSource(QUrl("qrc:/Sound/finish.mp3"));
        else if (!isBlunder())
            bgm->setSource(QUrl("qrc:/Sound/win.mp3"));
        else
            bgm->setSource(QUrl("qrc:/Sound/lose.mp3"));
        audioOutput->setVolume(0.7);
        bgm->play();
    }

    if(gameStatus.winner == GameSystem::GAME_STATUS::WINNER::DRAW){
        this->ui->WinnerLabel->setText("DRAW");
        logStream << getCurrentTime() + "[決着]引き分けです。" << "\r\n";
    }
    else if(gameStatus.winner == GameSystem::GAME_STATUS::WINNER::COOL){
        if (!isBlunder()) {
            this->ui->WinnerLabel->setText("COOL WIN!");
        }else{
           this->ui->WinnerLabel->setText("HOT LOSE!");
        }
        logStream << getCurrentTime() + "[決着]COOLが勝利しました。" << "\r\n";
        //負けチームのスコア更新（ターン数分減らす）
        if(this->isBotBattleMode){
            int ScoreBuf = this->ui->Field->team_score[static_cast<int>(GameSystem::TEAM::HOT)];
            ui->ScoreLabel_B ->setText(QString::number(ScoreBuf*3) + "(ITEM:" + QString::number(ScoreBuf) + ")");
        }
    }
    else if(gameStatus.winner == GameSystem::GAME_STATUS::WINNER::HOT){
        if (!isBlunder()){
            this->ui->WinnerLabel->setText("HOT WIN!");
        }else{

            this->ui->WinnerLabel->setText("COOL LOSE!");
        }
        logStream << getCurrentTime() + "[決着]HOTが勝利しました。" << "\r\n";
        //負けチームのスコア更新（ターン数分減らす）
        if(this->isBotBattleMode){
            int ScoreBuf = this->ui->Field->team_score[static_cast<int>(GameSystem::TEAM::COOL)];
            ui->ScoreLabel_A->setText(QString::number(ScoreBuf*3) + "(ITEM:" + QString::number(ScoreBuf) + ")");
        }
    }

    if(gameStatus.reason == GameSystem::GAME_STATUS::REASON::FOULED){
        if(gameStatus.cause == GameSystem::GAME_STATUS::CAUSE::NOGETREADY)
            append_str.append(" [get_readyがありません]");
        else if(gameStatus.cause == GameSystem::GAME_STATUS::CAUSE::RESEND)
            append_str.append(" [get_readyが２回ありました]");
        else if(gameStatus.cause == GameSystem::GAME_STATUS::CAUSE::UNKNOWNACTION)
            append_str.append(" [不明なアクション命令がありました]");
        else if(gameStatus.cause == GameSystem::GAME_STATUS::CAUSE::UNKNOWNROTE)
            append_str.append(" [不明な方向指定ありました]");
        else append_str.append(" [タイムアウト]");
        this->ui->ResultLabel->setText("エラー" + append_str);
        logStream << getCurrentTime() + "[勝因]通信エラー" << "\r\n";
    }
    else if(gameStatus.reason == GameSystem::GAME_STATUS::REASON::SCORE){
        this->ui->ResultLabel->setText("アイテム数" + append_str);
        logStream << getCurrentTime() + "[勝因]アイテム数" << "\r\n";
    }
    else if(gameStatus.reason == GameSystem::GAME_STATUS::REASON::TRAPPED){
        this->ui->ResultLabel->setText("包囲" + append_str);
        logStream << getCurrentTime() + "[勝因]包囲" << "\r\n";
    }
    else if(gameStatus.reason == GameSystem::GAME_STATUS::REASON::CONFINED){
        this->ui->ResultLabel->setText("自縛" + append_str);
        logStream << getCurrentTime() + "[勝因]自縛" << "\r\n";
    }
    else if(gameStatus.reason == GameSystem::GAME_STATUS::REASON::ATTACK){
        this->ui->ResultLabel->setText("アタック"  + append_str);
        logStream << getCurrentTime() + "[勝因]アタック" << "\r\n";
    }
    else if(gameStatus.reason == GameSystem::GAME_STATUS::REASON::COLLISION){
        this->ui->ResultLabel->setText("衝突" + append_str);
        logStream << getCurrentTime() + "[勝因]衝突" << "\r\n";
    }

    finishScoreLabelStyle();

    this->ui->WinnerLabel->show();
    this->ui->ResultLabel->show();
    this->ui->TimeBar_A->hide();
    this->ui->TimeBar_B->hide();
  
    this->startupDialog->enableMusicSelection(true);
    this->startupDialog->setGameMusicCombo(musicList[(currentround+1)%2]);

    this->startupDialog->swapClientConnections();
    
    if(isDoubleGameMode && currentround<1){
        this->startupDialog->enableConnectionChange(true);    
        this->startupDialog->showGameStartButton(true);
        if (isDemoMode) {
            QTimer::singleShot(5000, this, SLOT(startSetup()));
        }
    }else{
        if (! isRepeatMode)
            this->startupDialog->setGameStartButtonToEnd(false);
        else {
            this->startupDialog->setGameStartButtonToEnd(true);
            if (isDemoMode) {
                QTimer::singleShot(10000, this, SLOT(repeatGame()));
            }
        }
    }
}

bool MainWindow::isBlunder()
{
 return (gameStatus.reason == GameSystem::GAME_STATUS::REASON::CONFINED
      || gameStatus.reason == GameSystem::GAME_STATUS::REASON::COLLISION
      || gameStatus.reason == GameSystem::GAME_STATUS::REASON::FOULED);   
}
