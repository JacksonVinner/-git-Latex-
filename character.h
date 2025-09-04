#pragma once
#ifndef CHARACTER_H
#define CHARACTER_H
#include<iostream>
#include<string>
#include<cstdlib>
#include<ctime>
#include<iomanip>  // ���ڿ���С��λ
using namespace std;
extern int case2_skill, case3_skill, case4_skill;
class character
{
public:
    string name;
    int maxhealth;    // �������ֵ
    int health;       // ��ǰ����ֵ
    int damage;       // �����˺�ֵ
    int money;        // Ǯ
    int exp;          // ����ֵ
    int maxexp;
    int rate;         // �ȼ�
    int percent;      // �����ʣ��ٷ��ƣ�0~100��
    float crit; // �����˺�ת���ȣ����� 1.3 ��ʾ�˺���1.3��
    int blue;
    int maxblue;
    bool restrict = 0;//false->δ������    true->������

    character(const string& n, int mh, int h, int dmg, int m, int e, int me, int r, int p, float cr, int b, int mb)
        : name(n), maxhealth(mh), health(h), damage(dmg),
        money(m), exp(e), maxexp(me), rate(r),
        percent(p), crit(cr), blue(b), maxblue(mb) {}//����ֵ

    /*change("blue",-10)*/
        // ��ԭ��ֵ�Ļ���������
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
                if (rate == 2) { case2_skill = 1; cout << "�ѽ���������ǧ�����ʣ�(ǧ�����ʣ��۳��Է�1%����Ѫ����ѣ�εз�һ�غ�)" << endl; }
                if (rate == 5) { case3_skill = 1; cout << "�ѽ���������߱��ͻ�̣�(߱��ͻ�̣��۳��Է�(��������*2)��Ѫ���������Լ�(��������*0.5)��Ѫ��)" << endl; }
                if (rate == 9) { case4_skill = 1; cout << "�ѽ����ļ�����;ĩ·��(��;ĩ·: ���з�Ѫ��������Ѫ����10%��ֱ��նɱ������۳��Է���Ѫ����5%)" << endl; }

            }
        }
        else if (attr == "rate") rate += (int)delta;
        else if (attr == "percent") percent += (int)delta;
        else if (attr == "crit") crit += delta;
        else cout << "���� " << attr << " �����ڣ�" << endl;
    }

    // attack���������ع���ֵ (���Ǳ���)
    int attack() {
        int number = static_cast<int>(damage * 0.2);
        int num = static_cast<int>(0.9 * damage) + rand() % (number + 1);// �����˺�

        // ����һ�� [0, 99] ���������ģ��ٷֱ�
        int chance = rand() % 100;

        if (chance < percent) {  // ��������
            num = static_cast<int>(num * crit);  // ʹ�� crit ��Ϊ����
            cout << "����" << " ";
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
            << "����ֵ: " << health << " / " << maxhealth
            << " | ������: " << damage
            << " | ������: " << percent << "%"
            << " | �����˺�ת����: " << crit << "x"
            << " | ����ʯ: " << money
            << " | ����ֵ: " << exp << endl;
    }

};


class user : public character
{
public:
    // �������Ĺ��캯��
    user(const string& n, int mh, int h, int dmg, int m, int e, int me, int r, int p, float cr, int b, int mb)
        : character(n, mh, h, dmg, m, e, me, r, p, cr, b, mb) {};

    user() : user("����������", 100, 100, 20, 100, 0, 100, 1, 20, 1.5f, 60, 60) {};

    // ���ƹ��캯��
    user(const user& other)
        : character(other.name, other.maxhealth, other.health, other.damage,
            other.money, other.exp, other.maxexp, other.rate,
            other.percent, other.crit, other.blue, other.maxblue) {}

    // ��ֵ���������
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


    // ���� change�����������߼�
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
            // �����߼�
            while (exp >= maxexp) {
                exp -= maxexp;

                change("maxhealth", 10.f * rate);
                change("health", 10.f * rate);
                change("blue", 10);
                change("maxblue", 10);
                change("maxexp", maxexp * 0.1f);
                change("damage", 5.f * rate);
                rate++;
                cout << "�������ˣ���ǰ�ĵȼ��ǣ�" << rate << endl;
                if (rate == 2) { case2_skill = 1; cout << "�ѽ���������ǧ�����ʣ�(ǧ�����ʣ��۳��Է�1%����Ѫ����ѣ�εз�һ�غ�)" << endl; }
                if (rate == 5) { case3_skill = 1; cout << "�ѽ���������߱��ͻ�̣�(߱��ͻ�̣��۳��Է�(��������*2)��Ѫ���������Լ�(��������*0.5)��Ѫ��)" << endl; }
                if (rate == 9) { case4_skill = 1; cout << "�ѽ����ļ�����;ĩ·��(��;ĩ·: ���з�Ѫ��������Ѫ����10%��ֱ��նɱ������۳��Է���Ѫ����5%)" << endl; }

            }
        }
        else if (attr == "maxexp") maxexp += static_cast<int>(delta);
        else if (attr == "rate") rate += static_cast<int>(delta);
        else if (attr == "percent") percent += static_cast<int>(delta);
        else if (attr == "crit") crit += delta;
        else cout << "���� " << attr << " �����ڣ�" << endl;
    }


    virtual void attackenemy(character& enemy)
    {
        int dealt = attack();
        int temp_health = enemy.health;
        enemy.health -= dealt;

        if (!enemy.isalive())
        {
            cout << name << " ��� " << enemy.name << " " << temp_health << " HP" << endl;
            cout << "��� " << enemy.name << endl;
            cout << name << " ��� " << enemy.money << " ����ʯ   " << enemy.exp << " ����ֵ" << endl;
            money += enemy.money;  // �����ӵ��Լ�����
            change("exp", enemy.exp);
            return;
        }
        cout << name << " ��� " << enemy.name << " " << dealt << " HP" << endl;
    }



    void attackenemy_skill(character& enemy, int dealt)
    {
        enemy.health -= dealt;

        if (!enemy.isalive())
        {
            cout << "��� " << enemy.name << endl;
            cout << name << " ��� " << enemy.money << " ����ʯ   " << enemy.exp << " ����ֵ" << endl;
            money += enemy.money;  // �����ӵ��Լ�����
            change("exp", enemy.exp);
            return;
        }

    }


    // ���� show����ʾ����
    virtual void show() const override {
        cout << name << " "
            << "����ֵ: " << health << " / " << maxhealth
            << " | ����: " << blue << " / " << maxblue
            << " | ������: " << damage
            << " | �ȼ�: " << rate
            << " | ������: " << percent << "%"
            << " | �����˺�ת����: " << crit << "x"
            << " | ����ʯ: " << money
            << " | ����ֵ: " << exp
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
            cout << name << " ��� " << enemy.name << " " << temp_health << " HP" << endl;
            cout << "��� " << enemy.name << endl;
            return;
        }
        cout << name << " ��� " << enemy.name << " " << dealt << " HP" << endl;
    }


};
//monster room1_monster()  room1_monster_pro
#endif//CHARACTER_H