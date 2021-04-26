#ifndef BBBITEM_H
#define BBBITEM_H
#include <QString>
class bbbitem{
public:
    bbbitem();
    bbbitem(QString item_name, int base_atk,int bon_atk, int healing, int weight);
    QString get_name();
    int get_atk();
    int get_bonus_atk();
    int get_healing();
    int get_weight();
    int get_x();
    int get_y();
private:
    QString item_name;
    int atk = 0;
    int bonus_atk = 0;
    int hungry = 0;
    int healing = 0;
    int weight = 0;
    int x_pos = 0;
    int y_pos = 0;
};
#endif // BBBITEM_H
