QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtSnake
TEMPLATE = app


SOURCES += main.cpp \
    snake.cpp

HEADERS  += \
    snake.h

RC_ICONS = snake.ico
