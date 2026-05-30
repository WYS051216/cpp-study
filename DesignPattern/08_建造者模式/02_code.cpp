#include<iostream>
#include<vector>
#include<map>
#include<string>
using namespace std;

//1.定义船相关的类
//2.定义生成器相关的类
//3.定义管理者类->建造不同规格的船
//4.编写测试程序

// 定义桑尼号船
// 存储已经建造好的零部件
class SunnyShip {
public:
    void addParts(string name)
    {
        m_parts.push_back(name);
    }
    void showParts()
    {
        for(const auto& item : m_parts)
        {
            cout << item << " ";
        }
        cout << endl;
    }
private:
    vector<string> m_parts; // 存储零部件的容器

};

// 定义梅利号船
class MerryShip {
public:
    void assemble(string name, string parts)
    {
        m_parts.insert(make_pair(parts, name));
    }
    void showParts()
    {
        for(const auto& item : m_parts)
        {
            cout << item.first << ":" << item.second << " ";
        }
        cout << endl;
    }
private:
    map<string, string> m_parts; // 存储零部件的容器

};

//定义生成器类
class ShipBuilder {
public:
    virtual void reset() = 0; 
    virtual void buildBody() = 0;
    virtual void bulidEngine() = 0;
    virtual void buildWeapon() = 0;
    virtual void buildInterior() = 0;
    virtual ~ShipBuilder() {}
};

class SunnyBuilder : public ShipBuilder {
public:
    SunnyBuilder()
    {
        reset();   
    }
    void reset() override
    {
        m_sunny = new SunnyShip;
    }
    SunnyShip* getSunnyShip()
    {
        SunnyShip* ship = m_sunny;
        m_sunny = nullptr;
        return ship;
    }
    void buildBody() override
    {
        m_sunny->addParts("船体是神树亚当");
    }
    void bulidEngine() override
    {
        m_sunny->addParts("引擎是可乐驱动的内燃机");
    }
    void buildWeapon() override
    {
        m_sunny->addParts("武器是狮吼炮");
    }
    void buildInterior() override
    {
        m_sunny->addParts("内部装饰是豪华内饰");
    }
    ~SunnyBuilder() 
    {
        if(m_sunny)
        {
            delete m_sunny;
            m_sunny = nullptr;
        }
    }
private:
    SunnyShip* m_sunny = nullptr;
};

// 定义梅利号生成器
class MerryBuilder : public ShipBuilder {
public:
    MerryBuilder()
    {
        reset();   
    }
    void reset() override
    {
        m_merry = new MerryShip;
    }
    MerryShip* getMerryShip()
    {
        MerryShip* ship = m_merry;
        m_merry = nullptr;
        return ship;
    }
    void buildBody() override
    {
        m_merry->assemble("优质木材", "船体");
    }
    void bulidEngine() override
    {
        m_merry->assemble("烧煤的蒸汽机", "引擎");
    }
    void buildWeapon() override
    {
        m_merry->assemble("普通的四门大炮", "武器");
    }
    void buildInterior() override
    {
        m_merry->assemble("非常豪华的内饰", "内部装饰");
    }
    ~MerryBuilder() 
    {
        if(m_merry)
        {
            delete m_merry;
            m_merry = nullptr;
        }
    }
private:
    MerryShip* m_merry = nullptr;
};

//定义管理者
class Director {
public:
    void setBuilder(ShipBuilder *builder)
    {
        m_builder = builder;
    }

    //简约型
    void buildSimpleShip()
    {
        m_builder->buildBody();
        m_builder->bulidEngine();
    }

    //标准型
    void buildStandardShip()
    {
        buildSimpleShip();
        m_builder->buildWeapon();
    }

    //豪华型
    void buildLuxuryShip()
    {
        buildStandardShip();
        m_builder->buildInterior();
    }
private:
    ShipBuilder* m_builder = nullptr;
};

//测试代码
//建造桑尼号的函数
void buildSunnyShip()
{
    Director* director = new Director;
    SunnyBuilder* builder = new SunnyBuilder;
    //简约型
    director->setBuilder(builder);
    director->buildSimpleShip();
    SunnyShip* ship = builder->getSunnyShip();
    ship->showParts();
    delete ship;
    //标准型
    builder->reset();
    director->buildStandardShip();
    ship = builder->getSunnyShip();
    ship->showParts();
    delete ship;
    //豪华型
    builder->reset();
    director->buildLuxuryShip();
    ship = builder->getSunnyShip();
    ship->showParts();
    delete ship;
    delete builder;
    delete director;
}

//建造梅利号的函数
void buildMerryShip()
{
    Director* director = new Director;
    MerryBuilder* builder = new MerryBuilder;
    //简约型
    director->setBuilder(builder);
    director->buildSimpleShip();
    MerryShip* ship = builder->getMerryShip();
    ship->showParts();
    delete ship;
    //标准型
    builder->reset();
    director->buildStandardShip();
    ship = builder->getMerryShip();
    ship->showParts();
    delete ship;
    //豪华型
    builder->reset();
    director->buildLuxuryShip();
    ship = builder->getMerryShip();
    ship->showParts();
    delete ship;
    delete builder;
    delete director;
}


int main()
{
    buildMerryShip();
    cout << "-----------------------------" << endl;
    buildSunnyShip();
    return 0;
}