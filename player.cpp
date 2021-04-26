#include "player.h"
#include "bbbitem.h"

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
    if(exp>(2*p_level+1)){
        p_level++;
        exp = exp - 2*p_level -1;
    }
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
    return get_atk()+qrand()%get_bonus_atk();
}
QVector<bbbitem> player::get_backpack(){
    return b_pack;
}
