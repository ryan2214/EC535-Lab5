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

#include <QtWidgets>

#include "mainwindow.h"

//! [0]
MainWindow::MainWindow()
{
    QWidget *widget = new QWidget;
    setCentralWidget(widget);
//! [0]

//! [1]
    QWidget *topFiller = new QWidget;
    topFiller->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QWidget *bottomFiller = new QWidget;
    bottomFiller->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setContentsMargins(5, 5, 5, 5);
    layout->addWidget(topFiller);
    //layout->addWidget(infoLabel);
    layout->addWidget(bottomFiller);
    widget->setLayout(layout);
//! [1]
    currentPlayer = player(); // man with a long_sword
//! [2]
    createActions();
    createMenus();

    setWindowTitle(tr("Nethack Lite"));
    setMinimumSize(160, 160);
    resize(480,272);
}
//! [2]

void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    if(!IsStart)
    {
        InitGame();
    }
    //background
    painter.setPen(Qt::black);
    painter.setBrush(Qt::black);
    painter.drawRect(5,30,470,237);
    //step count
    QFont font2("Courier",10);
    painter.setFont(font2);
    painter.setPen(Qt::white);
    painter.setBrush(Qt::blue);
    QString statusStr = "X: ";
    statusStr.append(QString::number(currentPlayer.get_x()));
    statusStr.append(" Y: ");
    statusStr.append(QString::number(currentPlayer.get_y()));
    statusStr.append(" Steps: ");
    statusStr.append(QString::number(step));
    painter.drawText(10,260,statusStr);
    //painter.drawText(100,200,QString::number(step));
    //draw
    painter.setBrush(Qt::green);
    painter.setPen(Qt::black);
    painter.drawRect(recPlayer);
    painter.setBrush(Qt::red);
    for(QRect r:rec_Mob)
        painter.drawRect(r);
    if(IsOver)
    {
        timer->stop();
    }
}

void MainWindow::InitGame()
{
    moveDirection = 0;
    lastDirection = 0;
    sStepLabel = "Steps:";
    consoleMessage = tr("Welcome to the bbb world!");
    step = 0;
    IsStart = true;
    IsOver = false;
    spawnMob(1);//spawn 1 slime
    QRect rect(80,50,10,10);
    recPlayer = rect;
    speed = 100;
    timer = new QTimer(this);
    timer->start(speed); // timer for update
    connect(timer,SIGNAL(timeout()),SLOT(game_update()));
}
void MainWindow::game_update()
{
    sDisplay = "";

    // if nothing block the player
    if(isBlock()==0){
        switch(moveDirection)
        {
            case 0: break;
            case 1:{
                recPlayer.setHeight(recPlayer.height()-10);
                recPlayer.setTop(recPlayer.top()-10);
                currentPlayer.move(1,10);
                lastDirection = 1;
                step++;
                moveDirection = 0;
            }break;
            case 2:{
                recPlayer.setHeight(recPlayer.height()+10);
                recPlayer.setTop(recPlayer.top()+10);
                currentPlayer.move(2,10);
                lastDirection = 2;
                step++;
                moveDirection = 0;
            }break;
            case 3:{
                recPlayer.setLeft(recPlayer.left()-10);
                recPlayer.setRight(recPlayer.right()-10);
                currentPlayer.move(3,10);
                lastDirection = 3;
                step++;
                moveDirection = 0;
            }break;
            case 4:{
                recPlayer.setLeft(recPlayer.left()+10);
                recPlayer.setRight(recPlayer.right()+10);
                currentPlayer.move(4,10);
                lastDirection = 4;
                step++;
                moveDirection = 0;
            }break;
            default :
            ;
        }
    }


    // if game over, set IsOver = true
    update();//paintEvent update
}

QRect MainWindow::CreateRect(int x, int y)//generate random rect
{
    QRect rect(x,y,10,10); // in the (10,35,460,227)
    return rect;
}

void MainWindow::spawnMob(int m_class){
    int x = qrand()%42;
    int y = qrand()%18;
    x = 20+x*10;
    y = 40+y*10;
    while(x==currentPlayer.get_x()&&y==currentPlayer.get_y()){
        x = qrand()%42;
        y = qrand()%18;
        x = 20+x*10;
        y = 40+y*10;
    }// dont let mob spawn at the pos of player
    QRect r = CreateRect(x,y);
    rec_Mob.push_back(r);
    mob m = mob("slime",m_class, 1, 2, 5, x, y);
    mob_list.push_back(m);
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    QKeyEvent *key = (QKeyEvent*)event;
    switch(key->key())
    {
        case Qt::Key_Up : moveDirection=1;break;
        case Qt::Key_Down : moveDirection=2;break;
        case Qt::Key_Left : moveDirection=3;break;
        case Qt::Key_Right : moveDirection=4;break;
        default: ;
    }
}

//! [3]

void MainWindow::save()
{
    //infoLabel->setText(tr("<b>File|Save</b>"));
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About Menu"),
            tr("The Game is made by Tianhe Lei.\n"
               "It is a Nethack Lite."));
}

//! [4]
void MainWindow::createActions()
{
//! [5]

    saveAct = new QAction(tr("&Save"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    connect(saveAct, &QAction::triggered, this, &MainWindow::save);

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    connect(exitAct, &QAction::triggered, this, &QWidget::close);

    aboutAct = new QAction(tr("&About"), this);
    connect(aboutAct, &QAction::triggered, this, &MainWindow::about);

//! [6]
}
//! [7]

//! [8]
void MainWindow::createMenus()
{
//! [9] //! [10]
    fileMenu = menuBar()->addMenu(tr("&File"));

    fileMenu->addAction(saveAct);
//! [11]
    fileMenu->addSeparator();
//! [11]
    fileMenu->addAction(exitAct);

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
//! [8]

}
//! [12]
int MainWindow::isBlock()
{
    //check boundary
    int x = currentPlayer.get_x();
    int y = currentPlayer.get_y();

    switch(moveDirection){
    case 1:{
        if(y<=30)
            return 1;
        y-=10;
    }break;
    case 2:{
        if(y>=250)
            return 1;
        y+=10;
    }break;
    case 3:{
        if(x<=10)
            return 1;
        x-=10;
    }break;
    case 4:{
        if(x>=460)
            return 1;
        x+=10;
    }break;
    default:break;
    }

    //check if run into mob
    for (mob m : mob_list) {
        if(x==m.get_x()&&y==m.get_y())
            return 2;          // blocked by mob
    }

    return 0; // non-block
}
