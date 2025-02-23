#ifndef GAMESYSTEM_H
#define GAMESYSTEM_H

#include <QObject>
#include <QPoint>
#include <QVector>
#include <QtGlobal>
#include <QTime>

#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QDebug>
#include <QRandomGenerator>

//二次元配列エイリアス
template <class T>
using Field = QVector<QVector<T>>;

//チーム情報マクロ
#define TEAM_COUNT 2
#define TEAMS COOL,HOT

//チーム情報マクロ
#define ROUND_COUNT 2
#define ROUNDS TOP,BOTTOM

class GameSystem
{
public:
    //通常マップサイズ
    const static int DEFAULT_MAP_WIDTH  = 15;
    const static int DEFAULT_MAP_HEIGHT = 17;

    //チーム
    enum class TEAM{
        TEAMS,
        UNKNOWN
    };

    //チーム情報
    struct TEAM_PROPERTY {
        const static int TEAMS_COUNT = TEAM_COUNT; //チーム数

        //チーム名取得
        static QString getTeamName(GameSystem::TEAM team);
    };

    struct GAME_STATUS{
        //勝った人
        enum class WINNER{
            TEAMS,
            DRAW,
            CONTINUE,
            NONE,
        };

        //勝因
        enum class REASON{
            SCORE,    //スコア
            TRAPPED,  //包囲
            CONFINED, //自縛
            ATTACK,   //アタック
            COLLISION,//衝突
            FOULED,   //エラー
            NONE,     //未定
        };
    
        //エラー原因
        enum class CAUSE{
            NOGETREADY,     //get_readyなし
            RESEND,        //get_readyを２回送る
            UNKNOWNACTION, //不明なアクション
            UNKNOWNROTE,   //不明な方向
            NONE,          //未定
        };

        WINNER winner;
        REASON reason;
        CAUSE  cause;
    };
 

    //接続状態
    enum class CONNECTING_STATUS{
        FINISHED,
        CONTINUE,
    };

    //マップ上に存在する物体
    enum class MAP_OBJECT{
        NOTHING = 0,
        TARGET  = 1,
        BLOCK   = 2,
        ITEM    = 3,
    };

    //テクスチャ
    enum class Texture{
        Light = 0, //Light
        Heavy = 1, //Heavy
        Jewel = 2, //Jewel
        Other = 3  //ディレクトリから読み込み
    };

    //マップ上に描画する非物体
    enum class MAP_OVERLAY{
        NOTHING,
        LOOK,
        SEARCH,
        GETREADY,
        BLIND,
        ERASE,
    };

    //ゲーム盤
    struct Map{
        Field<GameSystem::MAP_OBJECT> field;
        int turn;                       //ターン
        QString name;                   //ステージ名
        QPoint size;                    //マップサイズ
        QPoint team_first_point[TEAM_COUNT];//チーム初期位置
        QString texture_dir_path;    //テクスチャ

        Map();

        void SetSize(QPoint size, int block_num = 20, int item_num = 51);
        QPoint MirrorPoint(const QPoint& pos);

        void CreateRandomMap(int block_num = 20, int item_num = 51, int turn=100, bool mirror=true);
        bool Import(QString Filename);
        bool Export(QString Filename);
        bool CheckBlockRole(QPoint pos);
    };

    //クライアントの行動
    struct Method{
        //クライアントの行動
        enum class ACTION{
            WALK,
            LOOK,
            SEARCH,
            PUT,
            GETREADY,
            UNKNOWN,
        };
        //方向
        enum class ROTE{
            UP,
            DOWN,
            RIGHT,
            LEFT,
            UNKNOWN
        };
        TEAM   team;
        ACTION action;
        ROTE   rote;

        QPoint  GetRoteVector(); //方向ベクトル取得
        static Method fromString(const QString& str);
    };
    //周辺情報
    struct AroundData{
        CONNECTING_STATUS connect;
        MAP_OBJECT data[9];

        QString toString();      //文字列変換
        void finish();
    };
};

#endif // GAMESYSTEM_H
