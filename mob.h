#ifndef MOB_H
#define MOB_H
#include <QString>
#include <QRect>
#include <QRandomGenerator>
class QRect;

class mob{
public:
    mob();
    mob(QString name,int mobClass, int level, int exp, int ,int x,int y);
    QString get_name();
    int atk_roll();
    int get_exp();
    int get_level();
    int get_class();
    int get_x();
    int get_y();
    void set_x(int x);
    void set_y(int y);
    void rand_x(int l_b, int u_b);
    void rand_y(int l_b, int u_b);
    void take_dmg(int dmg);
    int get_hp();
    QRect get_rect();
private:
    QString name;
    int hitpoint = 1;
    int x_pos = 0;
    int y_pos = 0;
    int c_class = 0;
    int c_level = 1;
    int exp_drop = 1;
    int bonus_atk = 0;
    int raw_atk = 1;
    QRect rec;
};
#endif // MOB_H
