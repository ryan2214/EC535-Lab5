#include "bbbitem.h"

bbbitem::bbbitem(){
    item_name = "";
    atk = 0;
    bonus_atk = 0;
    healing = 0;
    weight = 0;
}
bbbitem::bbbitem(QString i_name, int base_atk,int bon_atk, int heal, int w){
    item_name = i_name;
    atk = base_atk;
    bonus_atk = bon_atk;
    healing = heal;
    weight = w;
}

QString bbbitem::get_name(){
    return item_name;
}

int bbbitem::get_atk(){
    return atk;
}

int bbbitem::get_bonus_atk(){
    return bonus_atk;
}

int bbbitem::get_healing(){
    return healing;
}

int bbbitem::get_weight(){
    return weight;
}

int bbbitem::get_x(){
    return x_pos;
}

int bbbitem::get_y(){
    return y_pos;
}
