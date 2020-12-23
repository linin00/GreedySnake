#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPaintEvent>
#include <QKeyEvent>

const int BLOCK_SIZE=25; //方块单元边长
const int MARGIN=5; //边距
const int AREA_ROW=15; //行数
const int AREA_COL=15; //列数

enum Direction
{
    UP,
    DOWN,
    LEFT,
    RIGHT,
    EXIT
};

enum Choose
{
    NO,
    MODE,
    START,
    ABOUT,
    QUIT
};

enum Food
{
    COMMON,
    SPEED_UP,
    SLOW_DOWN,
    LIFE
};
class Snake : public QWidget
{
    Q_OBJECT

public:
    Snake(QWidget *parent = 0);
    ~Snake();
    virtual void paintEvent(QPaintEvent *event); //界面刷新
    virtual void keyPressEvent(QKeyEvent *event); //键盘监听

public:
    void InitGame(); //初始化游戏
    void PauseResumeGame(); //暂停和恢复游戏
    void GameOver(); //游戏失败退出
    void Exit(); //游戏正常退出
    void GenerateFood(); //随机产生果实
    bool IsGameOver(); //判断是否游戏结束
    int mode = 1; // 1是菜单模式，2是游戏模式,3是上帝模式

private slots:
    void SnakeUpdate(); //蛇增长

private:
    QTimer *gameTimer; //游戏计时器
    bool isPause; //是否暂停
    QPoint foodPoint; //普通果实出现坐标
    QList<QPoint> snake; //贪吃蛇结构
    QList<QPoint> block; //砖块结构
    Direction dir; //方向
    int score; //分数
    int TIME_INTERVAL=100; //定时器间隔时间
    int speed = 600 - TIME_INTERVAL; //速度
    Food foodType = COMMON;
    int foodType_d = 6;
    int oneMoreTime = 1;

};

#endif // WIDGET_H
