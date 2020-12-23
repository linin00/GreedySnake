#include <QPainter>
#include <time.h>
#include <QTimer>
#include <QString>
#include <QMessageBox>
#include "snake.h"


Snake::Snake(QWidget *parent)
    : QWidget(parent)
{
    //初始化场景界面尺寸
    resize(MARGIN*4+(AREA_COL+3)*BLOCK_SIZE,MARGIN*2+AREA_ROW*BLOCK_SIZE);
    //初始化游戏
    InitGame();
}

Snake::~Snake()
{

}

void Snake::InitGame()
{
    TIME_INTERVAL = 100;
    //初始化贪吃蛇,初始长度5,注意头部在前，尾部在后
    QList<QPoint> newsnake;
    snake = newsnake;
    QList<QPoint> newblock;
    block = newblock;
    for(int j=4;j>=0;j--)
        snake.push_back(QPoint(j,0));
    dir=RIGHT;//初始时往右走
    //初始化砖块
    srand(time(0));
    for (int j = 0; j < 5; j ++) {
        block.push_back(QPoint(rand()%AREA_COL, rand()%AREA_ROW));
        if (snake.contains(block[j])) {
            block.pop_back();
            block.push_back(QPoint(rand()%AREA_COL, rand()%AREA_ROW));
        }
    }
    //初始化果实
    srand(time(0));
    GenerateFood();
    //初始化游戏分数
    score=0;
    //初始化暂停变量
    isPause=false;
    //初始化计时器
    gameTimer=new QTimer(this);
    connect(gameTimer,SIGNAL(timeout()),this,SLOT(SnakeUpdate()));
    //gameTimer->start(TIME_INTERVAL);
}

void Snake::GenerateFood()
{
    //随机产生果实1位置
    foodPoint.setX(rand()%AREA_COL);
    foodPoint.setY(rand()%AREA_ROW);
    foodType_d = rand()%10;
    //如果与贪吃蛇位置冲突，重新生成
    if(snake.contains(foodPoint))
        GenerateFood();
    //如果与砖块位置冲突，重新生成
    if(block.contains(foodPoint))
        GenerateFood();
}

void Snake::paintEvent(QPaintEvent *event)
{
    if (mode == 1) {
        QPainter painter(this);
        //绘制菜单场景
        painter.setBrush(Qt::red);
        painter.setPen(Qt::black);
        painter.drawRect(MARGIN,MARGIN,(AREA_COL+3)*BLOCK_SIZE + 2*MARGIN,AREA_ROW*BLOCK_SIZE);

        /**************
         *绘制游戏开始菜单*
        ***************/
        //绘制菜单头
        painter.setPen(Qt::black);
        painter.setFont(QFont("Arial", 20));
        painter.drawText((MARGIN*4+(AREA_COL+3)*BLOCK_SIZE)/2 - 90,MARGIN+2*BLOCK_SIZE,"Greedy ");
        painter.setPen(Qt::white);
        painter.setFont(QFont("Arial", 20));
        painter.drawText((MARGIN*4+(AREA_COL+3)*BLOCK_SIZE)/2 + 10,MARGIN+2*BLOCK_SIZE,"Snake!");
        //绘制选项
        painter.setPen(Qt::black);
        painter.setFont(QFont("Arial", 14));
        painter.drawText((MARGIN*4+(AREA_COL+3)*BLOCK_SIZE)/2 - 70,MARGIN+4*BLOCK_SIZE,"press P to start");
        painter.drawText((MARGIN*4+(AREA_COL+3)*BLOCK_SIZE)/2 - 70,MARGIN+6*BLOCK_SIZE,"press Q to quit");
        painter.drawText((MARGIN*4+(AREA_COL+3)*BLOCK_SIZE)/2 - 70,MARGIN+8*BLOCK_SIZE,"press M to learn more");
    }
    else if (mode == 2){
        QPainter painter(this);
        //绘制游戏场景
        painter.setBrush(Qt::black);
        painter.setPen(Qt::black);
        painter.drawRect(MARGIN,MARGIN,AREA_COL*BLOCK_SIZE,AREA_ROW*BLOCK_SIZE);
        //绘制贪吃蛇
        painter.setBrush(Qt::white);
        painter.setPen(Qt::black);
        for(int i=0;i<snake.size();i++)
            painter.drawRect(MARGIN+snake[i].x()*BLOCK_SIZE,MARGIN+snake[i].y()*BLOCK_SIZE,BLOCK_SIZE,BLOCK_SIZE);
        //绘制果实
        if (foodType_d == 0) {
            foodType = LIFE;
            painter.setBrush(Qt::green);
            painter.drawEllipse(MARGIN+foodPoint.x()*BLOCK_SIZE,MARGIN+foodPoint.y()*BLOCK_SIZE,BLOCK_SIZE,BLOCK_SIZE);
        }
            else if (foodType_d>= 1 && foodType_d <= 2) {
                foodType = SLOW_DOWN;
                painter.setBrush(Qt::blue);
                painter.drawEllipse(MARGIN+foodPoint.x()*BLOCK_SIZE,MARGIN+foodPoint.y()*BLOCK_SIZE,BLOCK_SIZE,BLOCK_SIZE);
            }
                else if (foodType_d>= 3 && foodType_d <= 5) {
                    foodType = SPEED_UP;
                    painter.setBrush(Qt::yellow);
                    painter.drawEllipse(MARGIN+foodPoint.x()*BLOCK_SIZE,MARGIN+foodPoint.y()*BLOCK_SIZE,BLOCK_SIZE,BLOCK_SIZE);
                }
                    else if (foodType_d>= 6 && foodType_d <= 9) {
                        foodType = COMMON;
                        painter.setBrush(Qt::white);
                        painter.drawEllipse(MARGIN+foodPoint.x()*BLOCK_SIZE,MARGIN+foodPoint.y()*BLOCK_SIZE,BLOCK_SIZE,BLOCK_SIZE);
                    }
        //绘制砖块
        painter.setBrush(Qt::red);
        for(int i=0;i<block.size();i++){
            painter.drawRect(MARGIN+block[i].x()*BLOCK_SIZE,MARGIN+block[i].y()*BLOCK_SIZE,BLOCK_SIZE,BLOCK_SIZE);
        }
        //绘制游戏分数
        painter.setPen(Qt::black);
        painter.setFont(QFont("Arial",10));
        painter.drawText(MARGIN*3+AREA_COL*BLOCK_SIZE,MARGIN+2*BLOCK_SIZE,"score: "+QString::number(score));
        //绘制速度档
        painter.setPen(Qt::black);
        painter.setFont(QFont("Arial",10));
        painter.drawText(MARGIN*3+AREA_COL*BLOCK_SIZE,MARGIN+4*BLOCK_SIZE,"speed: "+QString::number(speed));
        //绘制生命数
        painter.setPen(Qt::black);
        painter.setFont(QFont("Arial",10));
        painter.drawText(MARGIN*3+AREA_COL*BLOCK_SIZE,MARGIN+6*BLOCK_SIZE,"life: "+QString::number(oneMoreTime));
    }

    else if (mode == 3) {
        QPainter painter(this);
        //绘制场景
        painter.setBrush(Qt::black);
        painter.setPen(Qt::black);
        painter.drawRect(MARGIN,MARGIN,(AREA_COL+3)*BLOCK_SIZE + 2*MARGIN,AREA_ROW*BLOCK_SIZE);

        /**************
         *绘制游戏选择菜单*
        ***************/
        //绘制菜单头
        painter.setPen(Qt::red);
        painter.setFont(QFont("Arial", 20));
        painter.drawText((MARGIN*4+(AREA_COL+3)*BLOCK_SIZE)/2 - 90,MARGIN+2*BLOCK_SIZE,"YOU ");
        painter.setPen(Qt::red);
        painter.setFont(QFont("Arial", 20));
        painter.drawText((MARGIN*4+(AREA_COL+3)*BLOCK_SIZE)/2 + 10,MARGIN+2*BLOCK_SIZE,"DIE!");
        //绘制选项
        painter.setPen(Qt::red);
        painter.setFont(QFont("Arial", 14));
        painter.drawText((MARGIN*4+(AREA_COL+3)*BLOCK_SIZE)/2 - 114,MARGIN+4*BLOCK_SIZE,"you still have " + QString::number(oneMoreTime) + " chances");
        painter.drawText((MARGIN*4+(AREA_COL+3)*BLOCK_SIZE)/2 - 80,MARGIN+6*BLOCK_SIZE,"one more time?");
        painter.drawText((MARGIN*4+(AREA_COL+3)*BLOCK_SIZE)/2 - 80,MARGIN+10*BLOCK_SIZE,"press P to relife");
        painter.drawText((MARGIN*4+(AREA_COL+3)*BLOCK_SIZE)/2 - 80,MARGIN+12*BLOCK_SIZE,"press Q to quit");

    }
}

void Snake::keyPressEvent(QKeyEvent *event)
{
    if (mode == 2) {
        switch(event->key())
        {
        case Qt::Key_Up:
            if(dir!=DOWN)
                dir=UP;
            break;
        case Qt::Key_Down:
            if(dir!=UP)
                dir=DOWN;
            break;
        case Qt::Key_Left:
            if(dir!=RIGHT)
                dir=LEFT;
            break;
        case Qt::Key_Right:
            if(dir!=LEFT)
                dir=RIGHT;
            break;
        case Qt::Key_Space:
            PauseResumeGame();
            break;
        case Qt::Key_Q:
            Exit();
            break;
        default:
            break;
        }
    }
    else if(mode == 1){
        switch(event->key())
        {
        case Qt::Key_P:
            mode = 2;
            update();
            isPause = true;
            PauseResumeGame();
            break;
        case Qt::Key_Q:
            close();
            break;
        case Qt::Key_M:
            close();
            break;
        default:
            break;
        }
    }
    else if(mode == 3){
        switch(event->key())
        {
        case Qt::Key_P :
            oneMoreTime--;
            TIME_INTERVAL = 200;
            speed = 600 - TIME_INTERVAL;
            if (snake.size() > 4) {
                score -= 4;
                snake.pop_front();
                snake.pop_front();
                snake.pop_front();
                snake.pop_front();
            }
            else {
                for (int i = 0; i < snake.size() - 1; i++) {
                    snake.pop_front();
                    score--;
                }
            }
            mode = 2;
            update();
            break;
        case Qt::Key_Q:
            close();
            break;
        default:
            break;
        }
    }
}

bool Snake::IsGameOver()
{
    int x=snake.front().x();
    int y=snake.front().y();
    //出边界
    if(x<0||x>AREA_COL-1||y<0||y>AREA_ROW-1)
        return true;
    //撞了自己
    for(int i=3;i<snake.size();i++)
        if(snake[i]==snake.front())
            return true;
    //撞了砖块
    for(int i=0;i<block.size();i++)
        if(block[i]==snake.front())
            return true;
    return false;
}

void Snake::SnakeUpdate()
{
    //贪吃蛇移动的策略是每次删除尾部，然后添加新的头部，维护一个动态链表
    switch(dir)
    {
    case UP:
        snake.push_front(QPoint(snake.front().x(),snake.front().y()-1));
        break;
    case DOWN:
        snake.push_front(QPoint(snake.front().x(),snake.front().y()+1));
        break;
    case LEFT:
        snake.push_front(QPoint(snake.front().x()-1,snake.front().y()));
        break;
    case RIGHT:
        snake.push_front(QPoint(snake.front().x()+1,snake.front().y()));
        break;
    default:
        break;
    }
    //如果吃到了普通果实，则尾部不删除，否则删除尾部更新头部
    if(snake.contains(foodPoint))
    {
        score+=1; //得分
        if (foodType_d == 0) {
            oneMoreTime++;
            snake.pop_back();
        }
        else if (foodType_d >= 1 && foodType_d <= 2) {
            TIME_INTERVAL += 100;
            if (TIME_INTERVAL > 600) TIME_INTERVAL -= 100;
            gameTimer->start(TIME_INTERVAL);
            speed = 600 - TIME_INTERVAL;
            snake.pop_back();
        }
            else if (foodType_d >= 3 && foodType_d <= 5) {
                if (TIME_INTERVAL > 150)
                    TIME_INTERVAL -= 100;
                else TIME_INTERVAL -= 10;
                if (TIME_INTERVAL <= 50) TIME_INTERVAL = 50;
                gameTimer->start(TIME_INTERVAL);
                speed = 600 - TIME_INTERVAL;
                snake.pop_back();
            }
                else if (foodType_d >= 6 && foodType_d <= 9) { }
        GenerateFood(); //重新生成果实
    }
    else
        snake.pop_back();
    //游戏是否结束
    if(IsGameOver())
    {
        if (oneMoreTime > 0 && snake.size() > 1) {
            mode = 3;
            update();
            //isPause = true;
            PauseResumeGame();
        }
        else {
            mode = 1;
            GameOver();
            InitGame();
            update();
        }
    }
    update(); //重绘，比repaint函数效果好
}

void Snake::GameOver()
{
    gameTimer->stop();
    QMessageBox::information(this,"failed","game over!");
}

void Snake::Exit()
{
    gameTimer->stop();
    QMessageBox::information(this,"Quit","game over!");
    InitGame();
    mode = 1;
    update();
}
void Snake::PauseResumeGame()
{
    //暂停和恢复定时器
    if(!isPause)
    {
        isPause=!isPause;
        gameTimer->stop();
    }
    else
    {
        isPause=!isPause;
        gameTimer->start(TIME_INTERVAL);
    }
}
