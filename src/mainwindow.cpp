#include "mainwindow.h"
#include "ui_mainwindow.h"

QString MainWindow::getTime()
{
    return QString("[") + QDateTime::currentDateTime().toString("yyyyMMddhhmmss") + QString("]");
}

QString MainWindow::convertString(GameSystem::Method method)
{
    QString str;
    if(method.action == GameSystem::Method::ACTION::GETREADY)str += "GetReady";
    if(method.action == GameSystem::Method::ACTION::LOOK)    str += "Look";
    if(method.action == GameSystem::Method::ACTION::PUT)     str += "Put";
    if(method.action == GameSystem::Method::ACTION::SEARCH)  str += "Search";
    if(method.action == GameSystem::Method::ACTION::WALK)    str += "Walk";

    if(method.rote == GameSystem::Method::ROTE::UP)   str += "Up";
    if(method.rote == GameSystem::Method::ROTE::RIGHT)str += "Right";
    if(method.rote == GameSystem::Method::ROTE::LEFT) str += "Left";
    if(method.rote == GameSystem::Method::ROTE::DOWN) str += "Down";

    return str;
}

void MainWindow::keyPressEvent([[maybe_unused]] QKeyEvent *event)
{
    if(event->key()==Qt::Key_F){
        if (this->isFullScreen()) {
            this->showNormal();
        } else {
            this->showFullScreen();
        }
    }
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->startup = new StartupDialog(this);

    //Functorベースでは動作しないため、Stringベースでconnect
    connect(this,SIGNAL(destroyed()),this,SLOT(SaveFile()));
    //connect(this, &MainWindow::destroyed, this, &MainWindow::SaveFile);

    setSetting();
    setDesign();
    setPath();

    SE_C = new QMediaPlayer;
    SE_C_AO = new QAudioOutput;
    SE_C->setAudioOutput(SE_C_AO);
    SE_C->setSource(QUrl("qrc:/Sound/get_C.mp3"));
    SE_C_AO->setVolume(0.2);
    
    SE_H = new QMediaPlayer;
    SE_H_AO = new QAudioOutput;
    SE_H->setAudioOutput(SE_H_AO);
    SE_H->setSource(QUrl("qrc:/Sound/get_H.mp3"));
    SE_H_AO->setVolume(0.2);

    clock = new QTimer();
    connect(clock, &QTimer::timeout, this, &MainWindow::StepGame);

    startup_anime = new QTimer();
    connect(startup_anime, &QTimer::timeout, this, &MainWindow::StartAnimation);

    teamshow_anime = new QTimer();
    connect(teamshow_anime, &QTimer::timeout, this, &MainWindow::ShowTeamAnimation);

    this->startup->setCommandLineOptions();

    this->ui->Field->setMap(this->startup->map);

    ReSetUp();

    this->startup->show();
}

void MainWindow::setSetting()
{    
    //ServerSetting読み込み
    QSettings* Settings;
    QVariant v;
    Settings = new QSettings( "setting.ini", QSettings::IniFormat ); // iniファイルで設定を保存
    // Settings->setIniCodec( "UTF-8" ); // iniファイルの文字コード

    v = Settings->value( "Gamespeed" );
    if (v.typeId() != QMetaType::UnknownType)FRAME_RATE = v.toInt();
    
    v = Settings->value( "FullMode" );
    if (v.typeId() != QMetaType::UnknownType)isfullmode = v.toBool();
    else isfullmode = false;

    v = Settings->value( "Repeat" );
    if (v.typeId() != QMetaType::UnknownType)isrepeat = v.toBool();
    else isrepeat = false;
}

void MainWindow::setDesign()
{    
    //ServerSetting読み込み
    QSettings* Settings;
    QVariant v;
    
    Settings = new QSettings( "setting.ini", QSettings::IniFormat ); // iniファイルで設定を保存
    
    Settings->beginGroup("Design");
    v = Settings->value( "Map" );
    if (v.typeId() != QMetaType::UnknownType)anime_map_time = v.toInt();
    else anime_map_time=150;

    v = Settings->value( "Team" );
    if (v.typeId() != QMetaType::UnknownType)anime_team_time = v.toInt();
    else anime_team_time=150;

    v = Settings->value( "Silent" );
    if (v.typeId() != QMetaType::UnknownType)silent = v.toBool();
    else silent = false;

    v = Settings->value( "Maximum" );
    if (v.typeId() != QMetaType::UnknownType && v.toBool()){
        setWindowState(Qt::WindowMaximized);
        // setWindowState(Qt::WindowFullScreen);
    }

    //デザイン設定を書き換え
    v = Settings->value( "Dark" );
    if (v.typeId() != QMetaType::UnknownType)dark = v.toBool();
    else dark = false;

    v = Settings->value( "Bot" );
    if (v.typeId() != QMetaType::UnknownType)isbotbattle = v.toBool();
    else isbotbattle = false;
    Settings->endGroup();
}

void MainWindow::setPath()
{    
    //ServerSetting読み込み
    QSettings* Settings;
    QVariant v;
    QString path = "";

    Settings = new QSettings( "setting.ini", QSettings::IniFormat ); // iniファイルで設定を保存
    
    Settings->beginGroup("Path");
    v = Settings->value( "LogFilepath" );
    if (v.typeId() != QMetaType::UnknownType)path = v.toString();

    //ログファイルオープン
    if(path == "")path = "../Logs";
    QDir dir(path);
    // フォルダが存在しない場合に作成
    if (!dir.exists()) dir.mkpath(".");

    log = StableLog(path + "/log" + getTime() + ".txt");
    
    QString botcommand;
    v = Settings->value( "BotCommand");
    if (v.typeId() != QMetaType::UnknownType)botcommand = v.toString();
    else botcommand = "./Bot/bot.exe";
    this->startup->setBotCommand(botcommand);

    QString pythoncommand;
    v = Settings->value( "PythonCommand");
    if (v.typeId() != QMetaType::UnknownType)pythoncommand = v.toString();
    else pythoncommand = "../WinPython/python/python.exe";
   this->startup->setPythonCommand(pythoncommand);

    QString programpath;
    v = Settings->value( "ProgramFilePath");
    if (v.typeId() != QMetaType::UnknownType)programpath = v.toString();
    else programpath = "../CHaser";
    this->startup->setProgramPath(programpath);

    QString mappath;
    v = Settings->value( "MapFilePath");
    if (v.typeId() != QMetaType::UnknownType)mappath = v.toString();
    else mappath = "../Map";
    this->startup->setMapPath(mappath);

    Settings->endGroup();
}

void MainWindow::ReSetUp()
{
    round = -1;

    this->ui->Field->setMap(this->startup->map);

    for(int i = 0; i < TEAM_COUNT; i++){
        for(int j = 0; j < ROUND_COUNT; j++){
            for(int k = 0; k < 3; k++)
                    this->point[i][j][k]=0;
        }
    }

    ResetScoreLebel();
}

void MainWindow::StartSetUp()
{
    round++;
    qDebug()<< "round:" << round;

    this->game_status.winner = GameSystem::GAME_STATUS::WINNER::NONE;

    //消音モードじゃない かつ Musicフォルダに音楽が存在する ならBGMセット
    if(!silent && this->startup->music_text != "none"){
        bgm = new QMediaPlayer;
        audio_output = new QAudioOutput;
        bgm->setAudioOutput(audio_output);
        //connect(bgm, SIGNAL(positionChanged(qint64)), this, SLOT(positionChanged(qint64)));
        bgm->setSource(QUrl::fromLocalFile("./Music/" + this->startup->music_text));
        audio_output->setVolume(1.0);
        // bgm->play();
        log << "[ Music : " + this->startup->music_text + " ]" + "\r\n";
    }

    this->ui->WinnerLabel->hide();
    this->ui->ResultLabel->hide();

    //マップ初期化
    for(int i=0;i<TEAM_COUNT;i++){
        this->ui->Field->team_pos[i] = this->startup->map.team_first_point[i];
    }
    //ui初期化
    this->ui->Field->setMap(this->startup->map);
    this->ui->Field->turn_count = this->startup->map.turn;

    this->ui->TimeBar_A->setMaximum(this->startup->map.turn);
    this->ui->TimeBar_A->setValue  (this->startup->map.turn);
    this->ui->TimeBar_A->show();
    this->ui->TimeBar_B->setMaximum(this->startup->map.turn);
    this->ui->TimeBar_B->setValue  (this->startup->map.turn);
    this->ui->TimeBar_B->show();

    this->ui->TurnLabel->setText(QString::number(this->startup->map.turn));

    this->startup->setGameStartButtonEnabled(true);

    if(round == 0) {
        this->ui->NameLabel_A->setText(
            this->startup->team_client[static_cast<int>(GameSystem::TEAM::COOL)]->client->Name == "" ?
            "Cool" :
            this->startup->team_client[static_cast<int>(GameSystem::TEAM::COOL)]->client->Name);

        this->ui->NameLabel_B->setText(
            this->startup->team_client[static_cast<int>(GameSystem::TEAM::HOT )]->client->Name == "" ?
            "Hot" :
            this->startup->team_client[static_cast<int>(GameSystem::TEAM::HOT )]->client->Name);
        ui->Field->setFlip(false);
    }
    if(round == 1) {
        this->ui->NameLabel_B->setText(
            this->startup->team_client[static_cast<int>(GameSystem::TEAM::COOL)]->client->Name == "" ?
            "Cool" :
            this->startup->team_client[static_cast<int>(GameSystem::TEAM::COOL)]->client->Name);
        this->ui->NameLabel_A->setText(
            this->startup->team_client[static_cast<int>(GameSystem::TEAM::HOT )]->client->Name == "" ?
            "Hot" :
            this->startup->team_client[static_cast<int>(GameSystem::TEAM::HOT )]->client->Name);
        ui->Field->setFlip(true);
    }
    StartSetUpScoreLabel();

    //ボット戦モードならば表記の変更
    if(this->isbotbattle){
        this->ui->ScoreLabel_B ->setText(QString::number(this->startup->map.turn) + "(ITEM:0)");
        this->ui->ScoreLabel_A->setText(QString::number(this->startup->map.turn) + "(ITEM:0)");
    }else{
        this->ui->ScoreLabel_B ->setText("0");
        this->ui->ScoreLabel_A->setText("0");
    }

    if (round<-1){
        //画面のサイズに合わせてリサイズ
        // resize(QSize(window_width, window_height));
    }
 
    player = 0;

    for(int i=0; i<TEAM_COUNT; i++)
        this->ui->Field->team_score[i] = 0;

    //アイテム数ラベルセット
    this->ui->Field->leave_items = 0;
    for(int i=0;i<startup->map.size.y();i++){
       for(int j=0;j<startup->map.size.x();j++){
            if(startup->map.field[i][j] == GameSystem::MAP_OBJECT::ITEM)this->ui->Field->leave_items++;
       }
    }
    ui->ItemLeaveLabel->setText(QString::number(this->ui->Field->leave_items));

    ui->Field->repaint();

    getready_flag=true;
    
    log << getTime() + "セットアップ完了　ゲームを開始します。\r\n";
}

void MainWindow::StartGame()
{
    qDebug() << "Game Start";
    
    for(int i=0;i<TEAM_COUNT;i++){
        ui->Field->team_pos[i].setX(-1);
        ui->Field->team_pos[i].setY(-1);
    }
    startup_anime->start(qMax(this->anime_map_time / (startup->map.size.x()*startup->map.size.y()), 1));
}

void MainWindow::StartAnimation()
{
    static int timer = 1;
    static Field<GameSystem::MAP_OVERLAY> f(this->startup->map.size.y(),
                                            QVector<GameSystem::MAP_OVERLAY>(this->startup->map.size.x(),GameSystem::MAP_OVERLAY::ERASE));
    static int ANIMATION_SIZE = 5;
    static int ANIMATION_TYPE = QRandomGenerator::global()->generate() % ANIMATION_SIZE;
    int count = 0;

    ui->Field->RefreshOverlay();

    QPoint pos[2];
    if(ANIMATION_TYPE == 0){
        //ランダムにワサッて
        //一度に2マス表示させるのですわ
        int i_max = 2;
        if(timer == 1)i_max ++;
        for(int i=0;i<i_max;i++){
            do{
                pos[i].setX(QRandomGenerator::global()->generate() % this->startup->map.size.x());
                pos[i].setY(QRandomGenerator::global()->generate() % this->startup->map.size.y());
            }while(timer < startup->map.size.x() * startup->map.size.y() && f[pos[i].y()][pos[i].x()] != GameSystem::MAP_OVERLAY::ERASE);
            f[pos[i].y()][pos[i].x()] = GameSystem::MAP_OVERLAY::NOTHING;
        }
        for(int i=0;i<this->startup->map.size.y();i++){
            for(int j=0;j<this->startup->map.size.x();j++){
                this->ui->Field->overlay[i][j] = f[i][j];
            }
        }
    }else if(ANIMATION_TYPE == 1){
        //上からガーって
        for(int j=0;j<this->startup->map.size.y();j++){
            for(int k=0;k<this->startup->map.size.x();k++){
                if(count >= timer){
                    this->ui->Field->overlay[j][k] = f[j][k];
                }
                count++;
            }
        }
    }else if(ANIMATION_TYPE == 2){
        //なんかはさみ込む感じで
        for(int j=0;j<this->startup->map.size.y();j++){
            for(int k=0;k<this->startup->map.size.x();k++){
                if(count*2 < timer){
                    f[startup->map.size.y() - j - 1][startup->map.size.x() - k - 1] = GameSystem::MAP_OVERLAY::NOTHING;
                    f[j][k] = GameSystem::MAP_OVERLAY::NOTHING;
                }
                count++;
            }
        }

        for(int i=0;i<this->startup->map.size.y();i++){
            for(int j=0;j<this->startup->map.size.x();j++){
                this->ui->Field->overlay[i][j] = f[i][j];
            }
        }
    }else if(ANIMATION_TYPE == 3){
        //下からガーって
        for(int j=this->startup->map.size.y()-1;j>=0;j--){
            for(int k=this->startup->map.size.x()-1;k>=0;k--){
                if(count >= timer){
                    this->ui->Field->overlay[j][k] = f[j][k];
                }
                count++;
            }
        }
    }else if(ANIMATION_TYPE == 4){
        //中心から
        int cx = startup->map.size.x()/2;
        int cy = startup->map.size.y()/2;
        for(int j=this->startup->map.size.y()-1;j>=0;j--){
            for(int k=this->startup->map.size.x()-1;k>=0;k--){
                if((abs(cx-k)+abs(cy-j)) * this->startup->map.size.x() >= timer){
                    this->ui->Field->overlay[j][k] = f[j][k];
                }
            }
        }
    }
    if(timer >= startup->map.size.x() * startup->map.size.y()){
        timer = 1;

        startup_anime->stop();
        teamshow_anime->start(anime_team_time/TEAM_COUNT);

    } else {
        timer += 2;
    }
    repaint();
}

void MainWindow::ShowTeamAnimation()
{
    const int blinking = 3;
    static int team_count;

    if (team_count < blinking*2){
        if(team_count%2==0){
            ui->Field->team_pos[0] = this->startup->map.team_first_point[0];
        }else{
            ui->Field->team_pos[0] = QPoint(-3,-3);
        }
    } else {
        ui->Field->team_pos[0] = this->startup->map.team_first_point[0];
        if(team_count%2==0){
            ui->Field->team_pos[1] = this->startup->map.team_first_point[1];
        }else{
            ui->Field->team_pos[1] = QPoint(-3,-3);
        }
    }

    repaint();

    if(team_count >= blinking * 2 * TEAM_COUNT){
        ui->Field->team_pos[0] = this->startup->map.team_first_point[0];
        ui->Field->team_pos[1] = this->startup->map.team_first_point[1];

        this->teamshow_anime->stop();

        StartGameScoreLabel();
        this->game_status.winner = GameSystem::GAME_STATUS::WINNER::CONTINUE;
        clock->start(FRAME_RATE);

        if(!silent){
            bgm->play();
        }

        team_count = -1;
    }

    team_count++;
}

void MainWindow::StepGame()
{
    //ゲーム進行
    static GameSystem::Method team_mehod[TEAM_COUNT];

    this->ui->Field->RefreshOverlay();

    if(player == 0 && getready_flag){
        //ターンログ出力
        log << QString("-----残") + QString::number(this->ui->Field->turn_count) + "ターン-----" + "\r\n";
        qDebug() << QString("-----残") + QString::number(this->ui->Field->turn_count) + "ターン-----";
     }

    //GetReadyの取得
    if(getready_flag){
        // GetReady
        if(!startup->team_client[player]->client->WaitGetReady()){
            log << getTime() + "[停止]" + GameSystem::TEAM_PROPERTY::getTeamName(static_cast<GameSystem::TEAM>(player)) + "が正常にGetReadyを返しませんでした!0" << "\r\n";
            startup->team_client[player]->client->is_disconnected = true;
            game_status.cause = GameSystem::GAME_STATUS::CAUSE::NOGETREADY;
        }else{
            //log << getTime() + "GetReady" + "\r\n";
            GameSystem::AroundData buffer = ui->Field->FieldAccessAround(GameSystem::Method{static_cast<GameSystem::TEAM>(player),
                                                                         GameSystem::Method::ACTION::GETREADY,
                                                                         GameSystem::Method::ROTE::UNKNOWN},
                                                                         ui->Field->team_pos[player]);
            team_mehod[player] = startup->team_client[player]->client->WaitReturnMethod(buffer);

            if(team_mehod[player].action == GameSystem::Method::ACTION::GETREADY){
                log << getTime() + "[停止]" + GameSystem::TEAM_PROPERTY::getTeamName(static_cast<GameSystem::TEAM>(player)) + "が二度GetReadyを行いました!" << "\r\n";
                startup->team_client[player]->client->is_disconnected = true;
                game_status.cause = GameSystem::GAME_STATUS::CAUSE::RESEND;
            }
            team_mehod[player].team = static_cast<GameSystem::TEAM>(player);
        }
        //End
        RefreshItem(team_mehod[player]);
        this->game_status = Judge();

        if(game_status.winner != GameSystem::GAME_STATUS::WINNER::CONTINUE){

            player++;
            player %= TEAM_COUNT;

            startup->team_client[player]->client->WaitGetReady();
            GameSystem::AroundData buffer = ui->Field->FieldAccessAround(GameSystem::Method{static_cast<GameSystem::TEAM>(player),
                                                                         GameSystem::Method::ACTION::GETREADY,
                                                                         GameSystem::Method::ROTE::UNKNOWN},
                                                                         ui->Field->team_pos[player]);
            buffer.connect = GameSystem::CONNECTING_STATUS::FINISHED;
            team_mehod[player] = startup->team_client[player]->client->WaitReturnMethod(buffer);
            Finish(game_status);
        }

    }else{

        // Method
        GameSystem::AroundData around = ui->Field->FieldAccessMethod(team_mehod[player]);
        //アイテムの回収
        RefreshItem(team_mehod[player]);
        this->game_status = Judge();
        if(this->game_status.winner != GameSystem::GAME_STATUS::WINNER::CONTINUE){
            around.connect = GameSystem::CONNECTING_STATUS::FINISHED;
        }

        if(startup->team_client[player]->client->WaitEndSharp(around)){
            //不正行動をはじく
            if(team_mehod[player].action == GameSystem::Method::ACTION::UNKNOWN){
                log << getTime() + "[停止]" + GameSystem::TEAM_PROPERTY::getTeamName(static_cast<GameSystem::TEAM>(player)) + "が不正なメソッドを呼んでいます！" << "\r\n";
                startup->team_client[player]->client->is_disconnected = true;
                game_status.cause = GameSystem::GAME_STATUS::CAUSE::UNKNOWNACTION;
            }
            if(team_mehod[player].rote   == GameSystem::Method::ROTE::UNKNOWN){
                log << getTime() + "[停止]" + GameSystem::TEAM_PROPERTY::getTeamName(static_cast<GameSystem::TEAM>(player)) + "の行動メソッドが不正な方向を示しています！" << "\r\n";
                startup->team_client[player]->client->is_disconnected = true;
                game_status.cause = GameSystem::GAME_STATUS::CAUSE::UNKNOWNROTE;
            }

            //行動ログの出力
            log << getTime() + "[行動]" + GameSystem::TEAM_PROPERTY::getTeamName(static_cast<GameSystem::TEAM>(player)) + "が" + convertString(team_mehod[player]) + "を行いました。" << "\r\n";

            GameBoard*& board = this->ui->Field;
            GameSystem::AroundData team_around = board->FieldAccessAround(static_cast<GameSystem::TEAM>(player));
            log << getTime() + GameSystem::TEAM_PROPERTY::getTeamName(static_cast<GameSystem::TEAM>(player)) + ":" + team_around.toString() << "\r\n";

            //refresh
            if(player == TEAM_COUNT-1){
                ui->Field->turn_count --;
                ui->TurnLabel->setText(QString::number(this->ui->Field->turn_count));
                ui->TimeBar_A->setValue(this->ui->Field->turn_count);
                ui->TimeBar_A->repaint();

                //ボット戦モードならば表記のリアルタイム更新
                if(this->isbotbattle){
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
            log << getTime() + "[停止]" + GameSystem::TEAM_PROPERTY::getTeamName(static_cast<GameSystem::TEAM>(player)) + "が正常にGetReadyを返しませんでした!1" << "\r\n";
            startup->team_client[player]->client->is_disconnected = true;
        }
        //End

        player++;
        player %= TEAM_COUNT;

        if(game_status.winner != GameSystem::GAME_STATUS::WINNER::CONTINUE){
            startup->team_client[player]->client->WaitGetReady();
            GameSystem::AroundData buffer = ui->Field->FieldAccessAround(GameSystem::Method{static_cast<GameSystem::TEAM>(player),
                                                                         GameSystem::Method::ACTION::GETREADY,
                                                                         GameSystem::Method::ROTE::UNKNOWN},
                                                                         ui->Field->team_pos[player]);
            buffer.connect = GameSystem::CONNECTING_STATUS::FINISHED;
            team_mehod[player] = startup->team_client[player]->client->WaitReturnMethod(buffer);
            Finish(game_status);
        }

    }

    ui->Field->repaint();
    getready_flag = !getready_flag;
}

void MainWindow::RepeatGame()
{
    qDebug() << "Game Reset";
 
    ReSetUp();
}

void MainWindow::EndGame()
{
    exit(0);
}

void MainWindow::SaveFile()
{
    file->close();
}

void MainWindow::repaintMap()
{
    ui->Field->setMap(this->startup->map);
    ui->Field->resizeImage();
    ui->Field->repaint();
}

void MainWindow::RefreshItem(GameSystem::Method method)
{
    static int leave_item = 0;
    if(leave_item == 0)leave_item = this->ui->Field->leave_items;
    if(this->ui->Field->leave_items != leave_item){
        if(!silent){
            if (player==0){
                SE_C->setPosition(0);
                SE_C->play();
            }
            else {
                SE_H->setPosition(0);
                SE_H->play();
            }
        }
        ui->ItemLeaveLabel->setText(QString::number(this->ui->Field->leave_items));
        log << getTime() + "[取得]" + GameSystem::TEAM_PROPERTY::getTeamName(method.team) + "がアイテムを取得しました。" << "\r\n";
        if(this->isbotbattle){
            int ScoreBuf = this->ui->Field->team_score[static_cast<int>(GameSystem::TEAM::COOL)];
            ui->ScoreLabel_A->setText(QString::number(ui->Field->turn_count + ScoreBuf*3) + "(ITEM:" + QString::number(ScoreBuf) + ")");
            ScoreBuf = this->ui->Field->team_score[static_cast<int>(GameSystem::TEAM::HOT)];
            ui->ScoreLabel_B ->setText(QString::number(ui->Field->turn_count + ScoreBuf*3) + "(ITEM:" + QString::number(ScoreBuf) + ")");
        }else{
            if(round==0) {
                ui->ScoreLabel_A->setText(QString::number(this->ui->Field->team_score[static_cast<int>(GameSystem::TEAM::COOL)]));
                ui->ScoreLabel_B ->setText(QString::number(this->ui->Field->team_score[static_cast<int>(GameSystem::TEAM::HOT)]));
            } else {
                ui->ScoreLabel_B->setText(QString::number(this->ui->Field->team_score[static_cast<int>(GameSystem::TEAM::COOL)]));
                ui->ScoreLabel_A ->setText(QString::number(this->ui->Field->team_score[static_cast<int>(GameSystem::TEAM::HOT)]));
            }
        }
        leave_item = this->ui->Field->leave_items;
        RefreshScore();
    }
}

void MainWindow::RefreshScore()
{
    int coolplayer, hotplayer;
    if (round==0) {
        coolplayer = 0;
        hotplayer  = 1;           
    }
    else {
        coolplayer = 1;
        hotplayer  = 0;           
    }

    this->point[coolplayer][round][0] = this->ui->Field->team_score[static_cast<int>(GameSystem::TEAM::COOL)];
    this->point[hotplayer ][round][0] = this->ui->Field->team_score[static_cast<int>(GameSystem::TEAM::HOT)];
    RefreshScoreLabel();
}

GameSystem::GAME_STATUS MainWindow::Judge()
{
    bool team_lose[TEAM_COUNT];
    int _player = player;
    for(int i=0;i<TEAM_COUNT;i++)team_lose[i] = false;
    GameBoard*& board = this->ui->Field;

    GameSystem::GAME_STATUS game_status;

    game_status.winner = GameSystem::GAME_STATUS::WINNER::NONE;
    game_status.reason = GameSystem::GAME_STATUS::REASON::NONE;

    for(int i=0;i<TEAM_COUNT;i++){
        // 現在のplayer(COOL or HOT)
        _player = (_player + 1) % TEAM_COUNT;

        GameSystem::AroundData team_around = board->FieldAccessAround(static_cast<GameSystem::TEAM>(_player));
        // log << getTime() + GameSystem::TEAM_PROPERTY::getTeamName(static_cast<GameSystem::TEAM>(_player)) + ":" + team_around.toString() << "\r\n";

        //ブロック置かれ負け
        if(team_around.data[4] == GameSystem::MAP_OBJECT::BLOCK){
            log << getTime() + "[敗因]" + GameSystem::TEAM_PROPERTY::getTeamName(static_cast<GameSystem::TEAM>(_player)) + "ブロック下敷き" << "\r\n";
            if (player != _player){
                game_status.reason = GameSystem::GAME_STATUS::REASON::ATTACK;

            }else{
                game_status.reason = GameSystem::GAME_STATUS::REASON::COLLISION;
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
            log << getTime() + "[敗因]" + GameSystem::TEAM_PROPERTY::getTeamName(static_cast<GameSystem::TEAM>(_player)) + "ブロック囲まれ" << "\r\n";
            if (player != _player){
                game_status.reason = GameSystem::GAME_STATUS::REASON::TRAPPED;

            }else{
                game_status.reason = GameSystem::GAME_STATUS::REASON::CONFINED;
            }
            team_around.finish();
            team_lose[_player]=true;
            break;
        }

        //切断負け
        if(startup->team_client[_player]->client->is_disconnected){
            log << getTime() + "[敗因]" + GameSystem::TEAM_PROPERTY::getTeamName(static_cast<GameSystem::TEAM>(_player)) + "通信切断" << "\r\n";
            game_status.reason = GameSystem::GAME_STATUS::REASON::FOULED;
            team_around.finish();
            team_lose[_player]=true;
            break;
        }

    }

    //相打ち、または時間切れ時はアイテム判定とする
    if(!std::find(team_lose,team_lose+TEAM_COUNT,false) || ui->Field->turn_count==0){
        log << getTime() + "[情報]相打ちまたは、タイムアップのためアイテム判定を行います" + "\r\n";
        log << getTime() + "[得点]";
        for(int i=0;i<TEAM_COUNT;i++){
            log << GameSystem::TEAM_PROPERTY::getTeamName(static_cast<GameSystem::TEAM>(i)) + ":" + QString::number(this->ui->Field->team_score[i]) + "  ";
            team_lose[i] = false;
        }
        log << "\r\n";

        //引き分けかな？
        game_status.reason = GameSystem::GAME_STATUS::REASON::SCORE;
        QSet<int> team_score_set; //スコア
        for(int i=0;i<TEAM_COUNT;i++){
            team_score_set.insert(this->ui->Field->team_score[i]);
        }
        if(team_score_set.size()==1){
            game_status.winner = GameSystem::GAME_STATUS::WINNER::DRAW;
            return game_status;
        }

        int index=0;
        for(int i=0;i<TEAM_COUNT;i++){
            if(this->ui->Field->team_score[index] < this->ui->Field->team_score[i])index = i;
        }
        //勝者判定
        game_status.winner = static_cast<GameSystem::GAME_STATUS::WINNER>(index);
        return game_status;
    }

    if(team_lose[0]){
        game_status.winner = GameSystem::GAME_STATUS::WINNER::HOT;
        return game_status;
    }
    else if(team_lose[1]){
        game_status.winner = GameSystem::GAME_STATUS::WINNER::COOL;
        return game_status;
    }
    else {
        game_status.winner = GameSystem::GAME_STATUS::WINNER::CONTINUE;
        return game_status;
    }
}

//終了処理
void MainWindow::Finish(GameSystem::GAME_STATUS game_status)
{
    this->clock->stop();

    QString append_str = "";
    //disconnect
    for(int i=0;i<TEAM_COUNT;i++){
        if(startup->team_client[i]->client->is_disconnected){
            log << getTime() + "[終了]" + GameSystem::TEAM_PROPERTY::getTeamName(static_cast<GameSystem::TEAM>(i)) + "との通信が切断されています。" << "\r\n";
        }
    }
    log << this->ui->WinnerLabel->text() << "\r\n";

    if(!silent){
        bgm->stop();
        if(game_status.reason == GameSystem::GAME_STATUS::REASON::SCORE)
            bgm->setSource(QUrl("qrc:/Sound/finish.mp3"));
        else if (!isBlunder())
            bgm->setSource(QUrl("qrc:/Sound/win.mp3"));
        else
            bgm->setSource(QUrl("qrc:/Sound/lose.mp3"));
        audio_output->setVolume(0.7);
        bgm->play();
    }

    if(game_status.winner == GameSystem::GAME_STATUS::WINNER::DRAW){
        this->ui->WinnerLabel->setText("DRAW");
        log << getTime() + "[決着]引き分けです。" << "\r\n";
    }
    else if(game_status.winner == GameSystem::GAME_STATUS::WINNER::COOL){
        if (!isBlunder()) {
            this->ui->WinnerLabel->setText("COOL WIN!");
        }else{
           this->ui->WinnerLabel->setText("HOT LOSE!");
        }
        log << getTime() + "[決着]COOLが勝利しました。" << "\r\n";
        //負けチームのスコア更新（ターン数分減らす）
        if(this->isbotbattle){
            int ScoreBuf = this->ui->Field->team_score[static_cast<int>(GameSystem::TEAM::HOT)];
            ui->ScoreLabel_B ->setText(QString::number(ScoreBuf*3) + "(ITEM:" + QString::number(ScoreBuf) + ")");
        }
    }
    else if(game_status.winner == GameSystem::GAME_STATUS::WINNER::HOT){
        if (!isBlunder()){
            this->ui->WinnerLabel->setText("HOT WIN!");
        }else{

            this->ui->WinnerLabel->setText("COOL LOSE!");
        }
        log << getTime() + "[決着]HOTが勝利しました。" << "\r\n";
        //負けチームのスコア更新（ターン数分減らす）
        if(this->isbotbattle){
            int ScoreBuf = this->ui->Field->team_score[static_cast<int>(GameSystem::TEAM::COOL)];
            ui->ScoreLabel_A->setText(QString::number(ScoreBuf*3) + "(ITEM:" + QString::number(ScoreBuf) + ")");
        }
    }

    if(game_status.reason == GameSystem::GAME_STATUS::REASON::FOULED){
        if(game_status.cause == GameSystem::GAME_STATUS::CAUSE::NOGETREADY)
            append_str.append(" [get_readyがありません]");
        else if(game_status.cause == GameSystem::GAME_STATUS::CAUSE::RESEND)
            append_str.append(" [get_readyが２回ありました]");
        else if(game_status.cause == GameSystem::GAME_STATUS::CAUSE::UNKNOWNACTION)
            append_str.append(" [不明なアクション命令がありました]");
        else if(game_status.cause == GameSystem::GAME_STATUS::CAUSE::UNKNOWNROTE)
            append_str.append(" [不明な方向指定ありました]");
        else append_str.append(" [タイムアウト]");
        this->ui->ResultLabel->setText("エラー" + append_str);
        log << getTime() + "[勝因]通信エラー" << "\r\n";
    }
    else if(game_status.reason == GameSystem::GAME_STATUS::REASON::SCORE){
        this->ui->ResultLabel->setText("アイテム数" + append_str);
        log << getTime() + "[勝因]アイテム数" << "\r\n";
    }
    else if(game_status.reason == GameSystem::GAME_STATUS::REASON::TRAPPED){
        this->ui->ResultLabel->setText("包囲" + append_str);
        log << getTime() + "[勝因]包囲" << "\r\n";
    }
    else if(game_status.reason == GameSystem::GAME_STATUS::REASON::CONFINED){
        this->ui->ResultLabel->setText("自縛" + append_str);
        log << getTime() + "[勝因]自縛" << "\r\n";
    }
    else if(game_status.reason == GameSystem::GAME_STATUS::REASON::ATTACK){
        this->ui->ResultLabel->setText("アタック"  + append_str);
        log << getTime() + "[勝因]アタック" << "\r\n";
    }
    else if(game_status.reason == GameSystem::GAME_STATUS::REASON::COLLISION){
        this->ui->ResultLabel->setText("衝突" + append_str);
        log << getTime() + "[勝因]衝突" << "\r\n";
    }

    FinishedScoreLabelStyle();

    this->ui->WinnerLabel->show();
    this->ui->ResultLabel->show();
    this->ui->TimeBar_A->hide();
    this->ui->TimeBar_B->hide();
  
    if(round<1){

    }
}

bool MainWindow::isBlunder()
{
 return (game_status.reason == GameSystem::GAME_STATUS::REASON::CONFINED
      || game_status.reason == GameSystem::GAME_STATUS::REASON::COLLISION
      || game_status.reason == GameSystem::GAME_STATUS::REASON::FOULED);   
}

MainWindow::~MainWindow()
{
    delete ui;
}
