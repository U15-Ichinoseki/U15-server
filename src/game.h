#ifndef GAME_H
#define GAME_H

#include "mainwindow.h"

class MainWindow;

class Game {
public:
    void resetSetup(MainWindow* mainWindow);
    void startSetup(MainWindow* mainWindow);
    void startGame(MainWindow* mainWindow);
    void stepGame(MainWindow* mainWindow);
    void repeatGame(MainWindow* mainWindow);
    void endGame(MainWindow* mainWindow);
    void refreshItems(MainWindow* mainWindow, GameSystem::Method method);
    void refreshScores(MainWindow* mainWindow);
    GameSystem::GAME_STATUS judgeGame(MainWindow* mainWindow);
    void finishGame(MainWindow* mainWindow, GameSystem::GAME_STATUS gameStatus);
    bool isBlunder(MainWindow* mainWindow);
};

#endif // GAME_H
