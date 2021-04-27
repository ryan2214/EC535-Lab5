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
    QFile file("/root/map.txt");                        // open map file, prepare map info
    file.open(QIODevice::ReadOnly | QIODevice::Text);   // not good when elements are sparse
        QByteArray t = file.readAll();
        QString raw_map = QString(t);
        file.close();
        QStringList list = raw_map.split("p");
        map_x = list[0].toInt();
        map_y = list[1].toInt();
        QString map_content = list[2];
        auto iter = map_content.begin();
        d_map_init(map_x,map_y);

        for(int x=0;x<map_x;x++){
            for(int y=0;y<map_y;y++){
                if(*iter=='\n')
                    iter++;
                d_map[x][y]=QString((*iter)).toInt(); // .digitValue()
                iter++;
            }
        }

//! [2]
    createActions();
    createMenus();
    setWindowTitle(tr("NetSnake"));
    setMinimumSize(160, 160);
    resize(480,272);

    bool bOk = false;
    username = QInputDialog::getText(this,"Name","What's your name?",QLineEdit::Normal,"",&bOk);
    if (bOk) {
        playerClass = QInputDialog::getText(this,"Class","0:Knight 1:Healer 2:Wizard",QLineEdit::Normal,"",&bOk);
    }
}
//! [2]

void MainWindow::paintEvent(QPaintEvent *)   // called by update() and if QT wants
{
    QPainter painter(this);
    if(!IsStart)
    {
        InitGame();
    }
    // background , always the absolute pos
    painter.setPen(Qt::black);
    painter.setBrush(Qt::black);
    painter.drawRect(5,30,screenXWidth,screenYWidth);
    d_map_draw(&painter);
    // draw status
    generateStatusStr(); // prepare statusStr
    QFont font2("Courier",10);
    painter.setFont(font2);
    painter.setPen(Qt::white);
    painter.setBrush(Qt::blue);
    painter.drawText(10,260,statusStr);
    // draw voiceover
    int transparent = 150;
    int voiceover_linepos = voiceover_height;
    for(auto line:voiceover){
        painter.setPen(QColor(248,248,255,transparent));
        painter.drawText(10,voiceover_linepos,line);
        voiceover_linepos+=voiceover_interval;
        transparent+=30;
    }

    // draw player
    switch(currentPlayer.get_class()){
        case 0:painter.setBrush(QColor(255,192,203));break;
        case 1:painter.setBrush(Qt::white);break;
        case 2:painter.setBrush(QColor(0,255,255));break;
        default:painter.setBrush(QColor(211,211,211));break;
    }
    painter.setPen(Qt::black);
    QRect t = currentPlayer.get_rect();
    t.setHeight(t.height()-screenAnchorY+30);
    t.setTop(t.top()-screenAnchorY+30);
    t.setLeft(t.left()-screenAnchorX);
    t.setRight(t.right()-screenAnchorX);
    painter.drawRect(t);
    // draw mobs

    for(auto m:mob_list){
        if(m.get_x()>screenAnchorX&&m.get_x()<screenAnchorX+screenXWidth)
            if(m.get_y()>screenAnchorY&&m.get_y()<screenAnchorY+screenYWidth){
                switch(m.get_class()){
                case 1:painter.setBrush(QColor(0,255,0));break;
                case 2:painter.setBrush(QColor(220,20,60));break;
                default:painter.setBrush(Qt::red);break;
                }
                QRect t = m.get_rect();
                t.setHeight(t.height()-screenAnchorY+30);
                t.setTop(t.top()-screenAnchorY+30);
                t.setLeft(t.left()-screenAnchorX);
                t.setRight(t.right()-screenAnchorX);
                painter.drawRect(t);
            }
    }
    if(IsOver)
    {
        timer->stop();
    }
}

void MainWindow::InitGame() // parameter reset, used for first setup and restart
{
    IsStart = true;
    freezeForAttack = false;
    atkDirection = 0;
    moveDirection = 0;
    lastDirection = 0;
    screenAnchorX = 5;
    screenAnchorY = 30;
    screenXWidth = 470;
    screenYWidth = 237;
    statusStr = "";
    push_voiceover("Welcome to the bbb world!");
    step = 0;
    currentPlayer = player(username,playerClass.toInt());
    while(mob_list.size()>0)
        mob_list.pop_back(); // clear all the mob
    spawnMob(1);//spawn 1 slime 
    speed = 50;
    timer = new QTimer(this);
    timer->start(speed); // timer for update
    connect(timer,SIGNAL(timeout()),SLOT(game_update()));
    IsOver = false;
}
void MainWindow::game_update()   // timer timeout handler, called by time pass
{
    while(voiceover.size()>3)
        voiceover.pop_front();
    // spawn mob if not reach the limit
    spawnByTick();
    mob_move();
    // if nothing block the player
    int p_x = currentPlayer.get_x();
    int p_y = currentPlayer.get_y();
    int check_block = isBlock(p_x,p_y,moveDirection,true);

    if(check_block==0){
        player_move();

    }else if(check_block==1){
        hit_environment(p_x,p_y,moveDirection);
    }else if(check_block==2){
        process_battle(moveDirection);
    }

    // if game over, set IsOver = true
    if(currentPlayer.get_hp()<=0){
        push_voiceover("Time freezed from the danger tick,");
        push_voiceover("press R to restart.");
        IsOver = true;
    }
    update();//paintEvent update
}

void MainWindow::hit_environment(int x,int y,int dir){ // can't move because something neutral ahead
    QString thisline;                                  // append some voiceover
    int x_limit = 10*map_x;
    int y_limit = 10*map_y;
    switch(dir){
    case 1:{
        if(y<=30)
            thisline = "That's the end of world";
        y-=10;
    }break;
    case 2:{
        if(y>=y_limit)
            thisline = "That's the end of world";
        y+=10;
    }break;
    case 3:{
        if(x<=10)
            thisline = "That's the end of world";
        x-=10;
    }break;
    case 4:{
        if(x>=x_limit)
            thisline = "That's the end of world";
        x+=10;
    }break;
    default:break;
    }
    if(thisline.isEmpty()){
        int obj = d_map[(x-5)/10][(y-30)/10];
        switch(obj){
        case 3:thisline = "You wish you know how to swim.";break;  // water
        case 4:thisline = "You found a solid wall ahead.";break; // wall
        case 5:thisline = "You found a door, nice.";break;  // door
        case 6:thisline = "You don't even look at the gold.";break;  // gold
        case 7:thisline = "A big tree, let's take a snap.";break;  // tree
        default:thisline = "You don't know what is ahead.";
        }
    }
    push_voiceover(thisline);
}

void MainWindow::keyPressEvent(QKeyEvent *event)   // receive all key events
{
    QKeyEvent *key = (QKeyEvent*)event;
    switch(key->key())
    {
        case Qt::Key_Up : {
            moveDirection = 1;
            atkDirection = 1;
        }break;
        case Qt::Key_Down : {
            moveDirection = 2;
            atkDirection = 2;
        }break;
        case Qt::Key_Left : {
            moveDirection = 3;
            atkDirection = 3;
        }break;
        case Qt::Key_Right : {
            moveDirection = 4;
            atkDirection = 4;
        }break;
        case Qt::Key_A : attackifPossible();break;
        case Qt::Key_R :{
            if(IsOver==true){
                restart();
            }
        }break;
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
               "tianhel@bu.edu"));
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
void MainWindow::createMenus()   // menu function from QT tutorial
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
int MainWindow::isBlock(int x,int y,int dir,bool isPlayer)
{                                       // in dir, is there anything? 0 clear, 1 building, 2 enemy
    //check boundary
    int x_limit = 10*map_x;
    int y_limit = 10*map_y;
    switch(dir){
    case 1:{
        if(y<=30)
            return 1;
        y-=10;
    }break;
    case 2:{
        if(y>=y_limit)
            return 1;
        y+=10;
    }break;
    case 3:{
        if(x<=10)
            return 1;
        x-=10;
    }break;
    case 4:{
        if(x>=x_limit)
            return 1;
        x+=10;
    }break;
    default:break;
    }
    // map item
    int map_element = d_map[(x-5)/10][(y-30)/10];
    if(map_element>2)
        return 1;
    if(isPlayer){
        //check if run into mob
        for (mob m : mob_list) {
            if(x==m.get_x()&&y==m.get_y())
                return 2;          // blocked by mob
        }
    }else{
        //check if run into player
        if(x==currentPlayer.get_x()&&y==currentPlayer.get_y())
            return 2;
    }


    return 0; // non-block
}

void MainWindow::attack_voiceover_append(QString m_name, int p_dmg, QString w_name){
    while(voiceover.size()>0)                           // output dmg done log
        voiceover.pop_back();   // reset the voiceover
    QString thisline = "You attemp to attack ";
    thisline.append(m_name);
    thisline.append(" with ");
    thisline.append(w_name);
    thisline.append(".");
    voiceover.push_back(thisline);
    if(p_dmg==0){
        thisline = "You didn't hit the ";
        thisline.append(m_name);
        thisline.append(".");
        voiceover.push_back(thisline);
    }else if(p_dmg<10){
        thisline = "You barely hit the ";
        thisline.append(m_name);
        thisline.append(", dealt ");
        thisline.append(QString::number(p_dmg));
        thisline.append(" dmg.");
        voiceover.push_back(thisline);
    }else if(p_dmg<100){
        thisline = "You hit the ";
        thisline.append(m_name);
        thisline.append("hard, dealt ");
        thisline.append(QString::number(p_dmg));
        thisline.append(" dmg.");
        voiceover.push_back(thisline);
    }else{
        thisline = "You made a lengendary attack on ";
        thisline.append(m_name);
        thisline.append(", dealt ");
        thisline.append(QString::number(p_dmg));
        thisline.append(" dmg.");
        voiceover.push_back(thisline);
    }
}

void MainWindow::got_hit_voiceover_append(QString m_name, int m_dmg){   // output take dmg log
    QString thisline = ""; // keep the voiceover
    if(m_dmg==0){
        thisline.append(m_name);
        thisline.append(" didn't hit you.");
        voiceover.push_back(thisline);
    }else if(m_dmg<10){
        thisline.append(m_name);
        thisline.append(" barely hit you, dealt ");
        thisline.append(QString::number(m_dmg));
        thisline.append(" dmg.");
        voiceover.push_back(thisline);
    }else if(m_dmg<100){
        thisline.append(m_name);
        thisline.append(" hit you hard, dealt ");
        thisline.append(QString::number(m_dmg));
        thisline.append(" dmg.");
        voiceover.push_back(thisline);
    }else{
        thisline.append(m_name);
        thisline.append(" made a lengendary attack on you, dealt ");
        thisline.append(QString::number(m_dmg));
        thisline.append(" dmg.");
        voiceover.push_back(thisline);
    }
}

void MainWindow::restart(){   // if hp reach 0, timer freezes until restart
    while(voiceover.size()>0)
        voiceover.pop_back();
    push_voiceover("Time roll back to the beginning,");
    push_voiceover("enjoy your life!");
    InitGame();
}

void MainWindow::generateStatusStr(){  // prepare the status string at the bottom of screen
    statusStr = currentPlayer.get_name();
    statusStr.append(", the ");
    switch(currentPlayer.get_class()){
        //0: Knight  1: Healer  2: Wizard
        case 0:statusStr.append("Knight");break;
        case 1:statusStr.append("Healer");break;
        case 2:statusStr.append("Wizard");break;
    }
    statusStr.append(" Hp: ");
    statusStr.append(QString::number(currentPlayer.get_hp()));
    statusStr.append("/");
    statusStr.append(QString::number(currentPlayer.get_maxhp()));
    statusStr.append(" Lvl: ");
    statusStr.append(QString::number(currentPlayer.get_level()));
    statusStr.append(" Atk: 1d");
    statusStr.append(QString::number(currentPlayer.get_wweapon().get_atk()));
    //statusStr.append(" XA: ");
    //statusStr.append(QString::number(d_map[6][10]));
    //statusStr.append(" YA: ");
    //statusStr.append(QString::number(screenAnchorY));
    int bonus_atk = currentPlayer.get_wweapon().get_bonus_atk(); // if weapon have bonus atk
    if(bonus_atk>0){
        statusStr.append("+");
        statusStr.append(QString::number(bonus_atk));
    }
}

int MainWindow::calculateDistoPlayer(int x, int y){ // 2D distance to player
    qreal distx = qPow(x-currentPlayer.get_x(),2);
    qreal disty = qPow(y-currentPlayer.get_y(),2);
    qreal dist = qSqrt(distx+disty);
    return  qFloor(dist);
}

void MainWindow::spawnMob(int m_class){   // before create mob, make sure the rand pos is clear
    int x = QRandomGenerator::global()->bounded(42);
    int y = QRandomGenerator::global()->bounded(18);
    x = 20+x*10;
    y = 40+y*10;
    while(calculateDistoPlayer(x,y)<30||isBlock(x,y,0,false)==1){ // not too close to player, not on solid terrain
        x = QRandomGenerator::global()->bounded(42);
        y = QRandomGenerator::global()->bounded(18);
        x = 20+x*10;
        y = 40+y*10;
    }// dont let mob spawn at the pos of player
    QString m_name;
    switch(m_class){
    case 1:m_name = "slime";break;
    case 2:m_name = "wolf";break;
    default:m_name = "void";
    }

    mob m = mob(m_name,m_class, 1, 2, 5, x, y);
    mob_list.push_back(m);
}

void MainWindow::spawnByTick(){  // every spawntick, spawn a new random mob if not reach the limit
    if(tick<spawnTick){
        tick++;
    }else{
        tick = 0;
        if(mob_list.size()<maxMobAmount){
            int m_class = QRandomGenerator::global()->bounded(1,3);
            spawnMob(m_class);
        }
    }
}

void MainWindow::attackifPossible(){ // look if there is enemy to attack in dir
    int p_x = currentPlayer.get_x();
    int p_y = currentPlayer.get_y();
    int check_block = isBlock(p_x,p_y,lastDirection,true);
    if(check_block==2)
        process_battle(lastDirection);
    else{
        push_voiceover("Nothing here to attack.");
    }
}
void MainWindow::process_battle(int dir){ // the battle process, dmg, get hurt, and exp
    int p_x = currentPlayer.get_x();
    int p_y = currentPlayer.get_y();
    mob rabbit = mob("rabbit",0, 1, 0, 1,p_x,p_y); // invisible rabbit: name, class, level, exp, hp, x, y
    switch(dir){
        case 1:{
            rabbit.set_y(p_y-10);
        }break;
        case 2:{
            rabbit.set_y(p_y+10);
        }break;
        case 3:{
            rabbit.set_x(p_x-10);
        }break;
        case 4:{
            rabbit.set_x(p_x+10);
        }break;
        default:{
            rabbit.set_x(0);
            rabbit.set_y(0);
        }
    }// set a rabbit in the mob pos

        auto iter = mob_list.begin();
        while(iter!=mob_list.end()){
            if((*iter).get_x()==rabbit.get_x()&&(*iter).get_y()==rabbit.get_y()){
                // m is the engaging mob
                // calculate engaging
                int p_dmg = currentPlayer.atk_roll();
                (*iter).take_dmg(p_dmg);

                attack_voiceover_append((*iter).get_name(), p_dmg, currentPlayer.get_wweapon().get_name());
                if((*iter).get_hp()>0){
                    int m_dmg = (*iter).atk_roll();
                    currentPlayer.take_dmg(m_dmg);
                    got_hit_voiceover_append((*iter).get_name(),m_dmg);
                }
                moveDirection = 0;
            }

            if((*iter).get_hp()<=0){ // mob dies, get exp from it
                currentPlayer.acc_exp((*iter).get_exp());
                iter = mob_list.erase(iter);
            }
            else
                ++iter;
        }
}

void MainWindow::player_move(){ // change player pos on move
    int x_limit = 5+10*map_x;
    int y_limit = 30+10*map_y;
    switch(moveDirection){
    case 0: break;
    case 1:{
        if(screenAnchorY>30&&currentPlayer.get_y()<=70+screenAnchorY){
            currentPlayer.move(1,10);
            screenAnchorY-=10;  // move the screen anchor if needed
        }else{
            currentPlayer.move(1,10);
        }
        lastDirection = 1;
        step++;
    }break;
    case 2:{
        if((screenAnchorY+screenYWidth+30<y_limit)&&(currentPlayer.get_y()>=screenAnchorY+screenYWidth-70)){
            currentPlayer.move(2,10);
            screenAnchorY+=10;
        }else{
            currentPlayer.move(2,10);
        }
        lastDirection = 2;
        step++;
    }break;
    case 3:{
        if(screenAnchorX>10&&currentPlayer.get_x()<=100+screenAnchorX){
            currentPlayer.move(3,10);
            screenAnchorX-=10;
        }else{
            currentPlayer.move(3,10);
        }
        lastDirection = 3;
        step++;
    }break;
    case 4:{
        if((screenAnchorX+screenXWidth<x_limit)&&(currentPlayer.get_x()>=screenAnchorX+screenXWidth-100)){
            currentPlayer.move(4,10);
            screenAnchorX+=10;
        }else{
            currentPlayer.move(4,10);
        }
        lastDirection = 4;
        step++;
    }break;
    default:;
    }
    if(moveDirection!=0){
        if(step_t<15)
            step_t++;
        else{
            step_t = 0;
            //TODO: accuire player pos and say something related.
            push_voiceover("You are walking through a silent place.");
        }
        moveDirection = 0;
    }

}

void MainWindow::mob_move(){          // let mobs randomly move
    if(wanderTick<mobWanderTick)
        wanderTick++;
    else{
        wanderTick=0;
        for(auto &m:mob_list){ // for each mob in mob_list
            int dir = QRandomGenerator::global()->bounded(1,5);
            int check_block = isBlock(m.get_x(),m.get_y(),dir,false);
            if(check_block==0){ // clear to move
                m.move(dir,10);
            }
        }
    }
}

void MainWindow::d_map_init(int xscale,int yscale){ // init d_map before fill from file
    d_map.resize(xscale);
    for(int j=0;j<d_map.size();j++){
           d_map[j].resize(yscale);
    }
    // 2D empty vector ready
}

void MainWindow::d_map_draw(QPainter* q){    // draw everything from d_map
    int x_start = (screenAnchorX-5)/10;
    int x_width = 47;
    int y_start = (screenAnchorY-30)/10;
    int y_width = 24;
    q->setPen(QColor(47,79,79));
    for(int i = x_start;i<x_start+x_width;i++){
        for(int j = y_start;j<y_start+y_width;j++){
            // for every tile in screen
            switch(d_map[i][j]){
            case 0:q->setBrush(QColor(47,79,79));break;  // dirt
            case 1:q->setBrush(QColor(0,100,0));break;  // grass
            case 2:q->setBrush(QColor(112,128,144));break;  // floor
            case 3:q->setBrush(QColor(30,144,255));break;  // water
            case 4:q->setBrush(QColor(169,169,169));break; // wall
            case 5:q->setBrush(QColor(205,133,63));break;  // door
            case 6:q->setBrush(QColor(255,215,0));break;  // gold
            case 7:q->setBrush(QColor(85,107,47));break;  // tree
            default:q->setBrush(QColor(205,133,63));
            }
            // paint the tile
            q->drawRect((i-x_start)*10+5,(j-y_start)*10+30,10,10);
        }
    }
}

void MainWindow::push_voiceover(QString s){   // push voiceover
    if(voiceover.size()==0)
        voiceover.push_back(s);
    else if(voiceover[voiceover.size()-1]!=s)
        voiceover.push_back(s);
}
