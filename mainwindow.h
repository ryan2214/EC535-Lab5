/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QRandomGenerator>
#include <QDialog>
#include <QPainter>
#include <QKeyEvent>
#include <QFont>
#include <QRect>
#include <QTimer>
#include "bbbitem.h"
#include "player.h"
#include "mob.h"

QT_BEGIN_NAMESPACE
class QAction;
class QActionGroup;
class QLabel;
class QMenu;
class QString;
class QRect;
class QTimer;
QT_END_NAMESPACE

//! [0]
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    void paintEvent(QPaintEvent *event) override;
    void InitGame();
    QRect CreateRect(int x,int y);
    void spawnMob(int);
    int isBlock(int,int,int,bool);
    void restart();
    void generateStatusStr();
    void attack_voiceover_append(QString m_name, int p_dmg, QString w_name);
    void got_hit_voiceover_append(QString m_name, int m_dmg);
    int calculateDistoPlayer(int,int);
    void spawnByTick();
    void process_battle();
    void player_move();
protected:
//! [0]

//! [1]
private slots:
    void save();
    void about();
    void game_update();
//! [1]

//! [2]
private:
    void createActions();
    void createMenus();
    void keyPressEvent(QKeyEvent *key) override;
//! [2]
    QRect recPlayer;
    QRect recTemp;
    QTimer *timer;
    int moveDirection = 0;
    int lastDirection = 0;
    int voiceover_interval;
    int voiceover_height;
    QVector<QString> voiceover;
    QString statusStr;
    QString consoleMessage;
    int step = 0;
    int step_t = 0;
    int speed = 100;
    int tick = 0;
    int spawnTick = 100;
    int maxMobAmount = 5;
    bool IsStart = false;
    bool IsOver = false;
    player currentPlayer;
    QVector<mob> mob_list;
    QVector<bbbitem> ground_items;


//! [3]
//!
    QMenu *fileMenu;
    QMenu *helpMenu;
    QAction *saveAct;
    QAction *exitAct;
    QAction *aboutAct;
};
//! [3]



#endif
