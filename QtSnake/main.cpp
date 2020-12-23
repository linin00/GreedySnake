#include "snake.h"
#include <QApplication>
#include <QLabel>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Snake snake;
    snake.show();
    return a.exec();
}
