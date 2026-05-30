#include<iostream>
#include<string>
using namespace std;

//父类
class GermaSoldier
{
public:
    virtual GermaSoldier* clone() = 0;
    virtual string whoAmI() = 0;
    virtual ~GermaSoldier() {}
};

//子类
class Solider66 : public GermaSoldier
{
public:
    GermaSoldier* clone() override
    {
        return new Solider66(*this);
    }
    string whoAmI() override
    {
        return "我是Soldier66的士兵!!!\n";
    }
};

class Solider67 : public GermaSoldier
{
public:
    GermaSoldier* clone() override
    {
        return new Solider67(*this);
    }
    string whoAmI() override
    {
        return "我是Soldier67的士兵!!!\n";
    }
};

int main()
{   
    GermaSoldier* obj = new Solider66;
    GermaSoldier* soldier = obj->clone();
    cout << soldier->whoAmI();
    delete soldier;
    delete obj;

    obj = new Solider67;
    soldier = obj->clone();
    cout << soldier->whoAmI();
    delete soldier;
    delete obj;
    return 0;
}