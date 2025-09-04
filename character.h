#pragma once
#ifndef CHARACTER_H
#define CHARACTER_H
#include<iostream>
#include<string>
#include<cstdlib>
#include<ctime>
#include<iomanip>  // 用于控制小数位
using namespace std;
extern int case2_skill, case3_skill, case4_skill;
class character
{
public:
    string name;
    int maxhealth;    // 最大生命值
    int health;       // 当前生命值
    int damage;       // 人物伤害值
    int money;        // 钱
    int exp;          // 经验值
    int maxexp;
    int rate;         // 等级
    int percent;      // 暴击率（百分制，0~100）
    float crit; // 暴击伤害转化比（例如 1.3 表示伤害×1.3）
    int blue;
    int maxblue;
    bool restrict = 0;//false->未被控制    true->被控制

    character(const string& n, int mh, int h, int dmg, int m, int e, int me, int r, int p, float cr, int b, int mb)
        : name(n), maxhealth(mh), health(h), damage(dmg),
        money(m), exp(e), maxexp(me), rate(r),
        percent(p), crit(cr), blue(b), maxblue(mb) {}//赋初值

    /*change("blue",-10)*/
        // 在原有值的基础上增加
    virtual void change(const string& attr, float delta) {
        if (attr == "maxhealth") maxhealth += (int)delta;
        else if (attr == "health") {
            health += (int)delta;
            if (health > maxhealth) health = maxhealth;
        }
        else if (attr == "damage") damage += (int)delta;
        else if (attr == "money") money += (int)delta;
        else if (attr == "exp") {
            exp += (int)delta;
            if (exp >= maxexp)
            {
                exp = exp - maxexp;
                change("maxhealth", 10.f * rate);
                change("health", 10.f * rate);
                change("blue", 10);
                change("maxblue", 10);
                change("maxexp", 0.1f * maxexp);
                change("damage", 5.f * rate);
                rate++;
                if (rate == 2) { case2_skill = 1; cout << "已解锁二技能千载幽咽！(千载幽咽：扣除对方1%的总血量并眩晕敌方一回合)" << endl; }
                if (rate == 5) { case3_skill = 1; cout << "已解锁三技能弑君突刺！(弑君突刺：扣除对方(自身攻击力*2)的血量并治疗自己(自身攻击力*0.5)的血量)" << endl; }
                if (rate == 9) { case4_skill = 1; cout << "已解锁四技能穷途末路！(穷途末路: 若敌方血量低于总血量的10%则直接斩杀，否则扣除对方总血量的5%)" << endl; }

            }
        }
        else if (attr == "rate") rate += (int)delta;
        else if (attr == "percent") percent += (int)delta;
        else if (attr == "crit") crit += delta;
        else cout << "属性 " << attr << " 不存在！" << endl;
    }

    // attack函数，返回攻击值 (考虑暴击)
    int attack() {
        int number = static_cast<int>(damage * 0.2);
        int num = static_cast<int>(0.9 * damage) + rand() % (number + 1);// 基础伤害

        // 生成一个 [0, 99] 的随机数，模拟百分比
        int chance = rand() % 100;

        if (chance < percent) {  // 触发暴击
            num = static_cast<int>(num * crit);  // 使用 crit 作为倍率
            cout << "暴击" << " ";
        }


        return num;
    }

    bool isalive()
    {
        return health > 0;
    }


    virtual void attackenemy(character& enemy) = 0;



    virtual void show() const {
        cout << name << " "
            << "生命值: " << health << " / " << maxhealth
            << " | 攻击力: " << damage
            << " | 暴击率: " << percent << "%"
            << " | 暴击伤害转化比: " << crit << "x"
            << " | 能量石: " << money
            << " | 经验值: " << exp << endl;
    }

};


class user : public character
{
public:
    // 带参数的构造函数
    user(const string& n, int mh, int h, int dmg, int m, int e, int me, int r, int p, float cr, int b, int mb)
        : character(n, mh, h, dmg, m, e, me, r, p, cr, b, mb) {};

    user() : user("灵脉守望者", 100, 100, 20, 100, 0, 100, 1, 20, 1.5f, 60, 60) {};

    // 复制构造函数
    user(const user& other)
        : character(other.name, other.maxhealth, other.health, other.damage,
            other.money, other.exp, other.maxexp, other.rate,
            other.percent, other.crit, other.blue, other.maxblue) {}

    // 赋值运算符重载
    user& operator=(const user& other)
    {
        if (this != &other) {
            name = other.name;
            maxhealth = other.maxhealth;
            health = other.health;
            damage = other.damage;
            money = other.money;
            exp = other.exp;
            maxexp = other.maxexp;
            rate = other.rate;
            percent = other.percent;
            crit = other.crit;
            blue = other.blue;
            maxblue = other.maxblue;
        }
        return *this;
    }


    // 覆盖 change，增加升级逻辑
    virtual void change(const string& attr, float delta) override {
        if (attr == "maxhealth") maxhealth += static_cast<int>(delta);
        else if (attr == "health") {
            health += static_cast<int>(delta);
            if (health > maxhealth) health = maxhealth;
        }
        else if (attr == "blue") {
            blue += static_cast<int>(delta);
            if (blue > maxblue) blue = maxblue;
        }
        else if (attr == "maxblue") maxblue += static_cast<int>(delta);
        else if (attr == "damage") damage += static_cast<int>(delta);
        else if (attr == "money") money += static_cast<int>(delta);
        else if (attr == "exp") {
            exp += static_cast<int>(delta);
            // 升级逻辑
            while (exp >= maxexp) {
                exp -= maxexp;

                change("maxhealth", 10.f * rate);
                change("health", 10.f * rate);
                change("blue", 10);
                change("maxblue", 10);
                change("maxexp", maxexp * 0.1f);
                change("damage", 5.f * rate);
                rate++;
                cout << "你升级了，当前的等级是：" << rate << endl;
                if (rate == 2) { case2_skill = 1; cout << "已解锁二技能千载幽咽！(千载幽咽：扣除对方1%的总血量并眩晕敌方一回合)" << endl; }
                if (rate == 5) { case3_skill = 1; cout << "已解锁三技能弑君突刺！(弑君突刺：扣除对方(自身攻击力*2)的血量并治疗自己(自身攻击力*0.5)的血量)" << endl; }
                if (rate == 9) { case4_skill = 1; cout << "已解锁四技能穷途末路！(穷途末路: 若敌方血量低于总血量的10%则直接斩杀，否则扣除对方总血量的5%)" << endl; }

            }
        }
        else if (attr == "maxexp") maxexp += static_cast<int>(delta);
        else if (attr == "rate") rate += static_cast<int>(delta);
        else if (attr == "percent") percent += static_cast<int>(delta);
        else if (attr == "crit") crit += delta;
        else cout << "属性 " << attr << " 不存在！" << endl;
    }


    virtual void attackenemy(character& enemy)
    {
        int dealt = attack();
        int temp_health = enemy.health;
        enemy.health -= dealt;

        if (!enemy.isalive())
        {
            cout << name << " 造成 " << enemy.name << " " << temp_health << " HP" << endl;
            cout << "打败 " << enemy.name << endl;
            cout << name << " 获得 " << enemy.money << " 能量石   " << enemy.exp << " 经验值" << endl;
            money += enemy.money;  // 奖励加到自己身上
            change("exp", enemy.exp);
            return;
        }
        cout << name << " 造成 " << enemy.name << " " << dealt << " HP" << endl;
    }



    void attackenemy_skill(character& enemy, int dealt)
    {
        enemy.health -= dealt;

        if (!enemy.isalive())
        {
            cout << "打败 " << enemy.name << endl;
            cout << name << " 获得 " << enemy.money << " 能量石   " << enemy.exp << " 经验值" << endl;
            money += enemy.money;  // 奖励加到自己身上
            change("exp", enemy.exp);
            return;
        }

    }


    // 覆盖 show，显示蓝量
    virtual void show() const override {
        cout << name << " "
            << "生命值: " << health << " / " << maxhealth
            << " | 蓝量: " << blue << " / " << maxblue
            << " | 攻击力: " << damage
            << " | 等级: " << rate
            << " | 暴击率: " << percent << "%"
            << " | 暴击伤害转化比: " << crit << "x"
            << " | 能量石: " << money
            << " | 经验值: " << exp
            << endl;
    }
};



class monster : public character
{
public:

    monster(const string& n, int mh, int h, int dmg, int m, int e, int me, int r, int p, float cr, int b, int mb)
        : character(n, mh, h, dmg, m, e, me, r, p, cr, b, mb) {};

    /*  name(n), maxhealth(mh), health(h), damage(dmg),
        money(m), exp(e), maxexp(me), rate(r),
        percent(p), crit(cr), blue(b), maxblue(mb) */


    virtual void attackenemy(character& enemy)
    {
        int dealt = attack();
        int temp_health = enemy.health;
        enemy.health -= dealt;

        if (!enemy.isalive())
        {
            cout << name << " 造成 " << enemy.name << " " << temp_health << " HP" << endl;
            cout << "打败 " << enemy.name << endl;
            return;
        }
        cout << name << " 造成 " << enemy.name << " " << dealt << " HP" << endl;
    }


};
//monster room1_monster()  room1_monster_pro
#endif//CHARACTER_H