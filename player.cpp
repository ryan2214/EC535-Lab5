#include "player.h"
#include "bbbitem.h"

player::player(){
    name = "A";
    p_class = 0;
    p_level = 1;
    w_weapon = bbbitem("long_sword", 8,0, 0, 40);
    b_pack.push_back(w_weapon);
    cash = 0;
    x_pos = 50;
    y_pos = 110;
    QRect r(x_pos,y_pos,10,10);
    rec = r;
}
player::player(QString n){
    name = n;
    p_class = 0;
    p_level = 1;
    w_weapon = bbbitem("long_sword", 8,0, 0, 40);
    b_pack.push_back(w_weapon);
    cash = 0;
    x_pos = 50;
    y_pos = 110;
    QRect r(x_pos,y_pos,10,10);
    rec = r;
}

player::player(QString n,int roleClass, int level, bbbitem wieldWeapon, QVector<bbbitem> backpackItems, int c, int x,int y){
    name = n;
    p_class = roleClass;
    p_level = level;
    w_weapon = wieldWeapon;
    QVector<bbbitem> t(backpackItems);
    b_pack.swap(t);
    cash = c;
    x_pos = x;
    y_pos = y;
    QRect r(x_pos,y_pos,10,10);
    rec = r;
}

int player::get_class(){
    return p_class;
} // 0: Knight  1: Healer  3: Wizard
int player::get_level(){
    return p_level;
}
int player::get_x(){
    return x_pos;
}
int player::get_y(){
    return y_pos;
}
QRect player::get_rect(){
    return rec;
}
int player::get_cash(){
    return cash;
}
void player::set_class(int c){
    p_class = c;
}
void player::set_level(int l){
    p_level = l;
}
void player::level_up(){
    p_level++;
    exp = exp - 2*p_level -1;
    hitpoint = 2*p_level+1;
    max_hp = hitpoint;
}
int player::get_exp(){
    return exp;
}
bool player::take_dmg(int dmg){
    hitpoint -= dmg;
    if(hitpoint<=0){
        return false;
    }
    return true;
}
void player::get_heal(int heal){
    hitpoint+=heal;
    if(hitpoint>max_hp)
        hitpoint = max_hp;
}
int player::get_hp(){
    return hitpoint;
}
int player::get_maxhp(){
    return max_hp;
}
void player::pick_up_item(bbbitem i){
    b_pack.push_back(i);
}
void player::weild_weapon(bbbitem w){
    w_weapon = w;
}
int player::get_atk(){
    return w_weapon.get_atk();
} // read from w_weapon's attr
int player::get_bonus_atk(){
    return w_weapon.get_bonus_atk();
}
int player::atk_roll(){
    int dmg = get_bonus_atk()+QRandomGenerator::global()->bounded(get_atk()+1);
    return dmg;
}
QVector<bbbitem> player::get_backpack(){
    return b_pack;
}

void player::move(int dir,int dis){
    switch (dir) {
    case 1:{
        y_pos-=dis;
        set_face_dir(1);
        rec.setHeight(rec.height()-10);
        rec.setTop(rec.top()-10);
    }break;
    case 2:{
        y_pos+=dis;
        set_face_dir(2);
        rec.setHeight(rec.height()+10);
        rec.setTop(rec.top()+10);
    }break;
    case 3:{
        x_pos-=dis;
        set_face_dir(3);
        rec.setLeft(rec.left()-10);
        rec.setRight(rec.right()-10);
    }break;
    case 4:{
        x_pos+=dis;
        set_face_dir(4);
        rec.setLeft(rec.left()+10);
        rec.setRight(rec.right()+10);
    }break;
    default:break;
    }
}

void player::set_face_dir(int dir){
    face_dir = dir;
}
int player::get_face_dir(){
    return face_dir;
}

bbbitem player::get_wweapon(){
    return w_weapon;
}

void player::acc_exp(int i){
    exp+=i;
    if(exp>(2*p_level+1)){
        level_up();
    }
}
