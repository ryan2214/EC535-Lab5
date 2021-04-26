#ifndef MOB_H
#define MOB_H
#include <QString>
class mob{
public:
    mob();
    mob(QString name,int mobClass, int level, int exp, int ,int x,int y);
    QString get_name();
    int atk_roll();
    int get_exp();
    int get_level();
    int get_x();
    int get_y();
    void set_x(int x);
    void set_y(int y);
    void rand_x(int l_b, int u_b);
    void rand_y(int l_b, int u_b);
    void take_dmg(int dmg);
    int get_hp();
private:
    QString name;
    int hitpoint = 1;
    int x_pos = 0;
    int y_pos = 0;
    int c_class = 0;
    int c_level = 1;
    int exp_drop = 1;
    int b_atk = 1;
    int r_atk = 1;
};
#endif // MOB_H
