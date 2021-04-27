#ifndef PLAYER_H
#define PLAYER_H
#include <QVector>
#include <QRect>
#include <QRandomGenerator>
#include "bbbitem.h"
class player{
public:
    player();
    player(QString);
    player(QString name,int roleClass, int level, bbbitem wieldWeapon, QVector<bbbitem> backpackItems, int cash, int x,int y);
    QString get_name();
    int get_class(); // 0: Knight  1: Healer  2: Wizard
    int get_level();
    int get_x();
    int get_y();
    int get_cash();
    void set_class(int c);
    void set_level(int l);
    void level_up();
    int get_exp();
    void acc_exp(int i);
    bool take_dmg(int dmg);
    void get_heal(int heal);
    int get_hp();
    int get_maxhp();
    void pick_up_item(bbbitem i);
    void weild_weapon(bbbitem w);
    bbbitem get_wweapon();
    int get_atk(); // read from w_weapon's attr
    int get_bonus_atk();
    int atk_roll();
    void move(int dir,int dis);
    void set_face_dir(int dir);
    int get_face_dir();
    QRect get_rect();

    QVector<bbbitem> get_backpack();
private:
    QString name;
    QRect rec;
    int hitpoint = 3;
    int max_hp = 3;
    int x_pos = 0;
    int y_pos = 0;
    int p_class = 0;
    int p_level = 1;
    int exp = 0;
    int cash = 0;
    int face_dir = 4;
    bbbitem w_weapon = bbbitem("nothing",0,0,0,0);
    QVector<bbbitem> b_pack;
};
#endif // PLAYER_H
