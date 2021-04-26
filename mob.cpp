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
}

QString mob::get_name(){
    return name;
}

int mob::atk_roll(){
    return b_atk+qrand()%r_atk;
}

int mob::get_exp(){
    return exp_drop;
}

int mob::get_level(){
    return c_level;
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
    x_pos = l_b + qrand()%u_b;
}
void mob::rand_y(int l_b, int u_b){
    y_pos = l_b + qrand()%u_b;
}
void mob::take_dmg(int dmg){
    hitpoint-=dmg;
}
int mob::get_hp(){
    return hitpoint;
}
