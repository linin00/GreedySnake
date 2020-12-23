#include "game.h"
#include <QPainter>
#include <QTimer>
int Qsnake::run(int argc, char *argv[]) {
    QApplication a(argc, argv);
    Snake snake;
    snake.show();
    return a.exec();
}
