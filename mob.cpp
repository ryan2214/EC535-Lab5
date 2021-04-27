#include "mob.h"

mob::mob(){
    name = "slime";
    c_class = 1;//1 for slime, 2 for wolf
    c_level = 1;
    hitpoint = 5;
    rand_x(5,400);
    rand_y(30,200);
}

mob::mob(QString n,int mobClass, int level, int exp, int h,int x,int y){
    name = n;
    c_class = mobClass;//1 for slime, 2 for wolf
    c_level = level;
    exp_drop = exp;
    hitpoint = h;
    x_pos = x;
    y_pos = y;
    QRect r(x,y,10,10);
    rec = r;
}

QString mob::get_name(){
    return name;
}

int mob::atk_roll(){
    return bonus_atk+QRandomGenerator::global()->bounded(raw_atk+1);
}

int mob::get_exp(){
    return exp_drop;
}

int mob::get_level(){
    return c_level;
}
int mob::get_class(){
    return c_class;
}

void mob::set_x(int x){
    x_pos = x;
}
void mob::set_y(int y){
    y_pos = y;
}
int mob::get_x(){
    return x_pos;
}
int mob::get_y(){
    return y_pos;
}
void mob::rand_x(int l_b, int u_b){
    x_pos = QRandomGenerator::global()->bounded(l_b,u_b);
}
void mob::rand_y(int l_b, int u_b){
    y_pos = QRandomGenerator::global()->bounded(l_b,u_b);
}
void mob::take_dmg(int dmg){
    hitpoint-=dmg;
}
int mob::get_hp(){
    return hitpoint;
}
QRect mob::get_rect(){
    return rec;
}
void mob::move(int dir,int dis){
        switch (dir) {
        case 1:{
            y_pos-=dis;
            rec.setHeight(rec.height()-10);
            rec.setTop(rec.top()-10);
        }break;
        case 2:{
            y_pos+=dis;
            rec.setHeight(rec.height()+10);
            rec.setTop(rec.top()+10);
        }break;
        case 3:{
            x_pos-=dis;
            rec.setLeft(rec.left()-10);
            rec.setRight(rec.right()-10);
        }break;
        case 4:{
            x_pos+=dis;
            rec.setLeft(rec.left()+10);
            rec.setRight(rec.right()+10);
        }break;
        default:break;
        }
    }
