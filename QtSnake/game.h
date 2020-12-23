#ifndef GAME_H
#define GAME_H
#include "snake.h"
#include <QApplication>
#include <QLabel>

class Qsnake{
public:
    int run(int argc, char *argv[]);
    int menu(int argc, char *argv[]);
    int start(int argc, char *argv[]);
};

#endif // GAME_H
