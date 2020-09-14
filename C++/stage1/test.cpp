#include "lib/monster.h"
#include "lib/monster_factory.h"
#include <QtCore>
#include <QtGlobal>
#include<iostream>
#include <QDebug>


using namespace std;

int main(){
    Monster *u = MonsterFactory::produce("Charizard", 1);
    qDebug() << "宠物小精灵的名称：" << u -> name();
    qDebug() << "宠物小精灵的昵称：" << u -> nickname();
    qDebug() << "宠物小精灵的等级：" << u -> level() ;
    qDebug() << "宠物小精灵的最大生命：" << u -> max_hp() ;
    qDebug() << "宠物小精灵的攻击力：" << u -> attack() ;
    qDebug() << "宠物小精灵的防御力：" << u -> defence() ;
    qDebug() << "宠物小精灵的攻击速度：" << u -> speed() ;
    qDebug() << "宠物小精灵的当前HP：" << u -> hp() ;

    Monster *nb = MonsterFactory::produce("Charizard", 15);
    int damage = nb->monster_attack(u);
    u->attacked(damage);
    if (u->is_died()){
        qDebug() << u->nickname()  << "失去战斗能力";
        qDebug() << nb->nickname()  << "击杀了" << u->nickname();
        if (nb->should_level_up()){
            nb->level_up();
            qDebug() << nb->nickname()  << "升级了！";
        }
    }
}
