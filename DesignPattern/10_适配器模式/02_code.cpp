#include<iostream>
using namespace std;

#if 0
//适配器类和熊猫类是关联关系
// 外国人
class Foreigner
{
public:
    virtual string confession() = 0;
    void setResult(string res)
    {
        cout << "Panda say:" << res << endl;
    }
    virtual ~Foreigner() {}
};

//美国人
class American : public Foreigner
{
public:
    string confession() override
    {
        return "american,我有罪\n";
    }
    
};

//法国人
class French : public Foreigner
{
public:
    string confession() override
    {
        return "french,我有罪\n";
    }
};

//熊猫
class Panda
{
public:
    void recvMessage(string msg)
    {
        cout << msg << endl;
    }
    string sendMessage()
    {
        return string("强盗、罪人、凶手是不可能被宽恕和原谅的！\n");
    }
};

//适配器类的基类
class AbstractChopper
{
public:
    AbstractChopper(Foreigner* foreigner) : m_foreigner(foreigner) {}
    virtual void translateToPandas() = 0;
    virtual void translateToHuman() = 0;
    virtual ~AbstractChopper() {}
protected:
    Panda m_panda;
    Foreigner* m_foreigner;
};

class EnglishChopper : public AbstractChopper
{
public:
    // 继承父类的所有构造函数
    using AbstractChopper::AbstractChopper;
    void translateToPandas()
    {
        string msg = m_foreigner->confession();
        //解析熊猫的语言，并将数据传递给熊猫
        m_panda.recvMessage("美国人说：" + msg);
    }
    void translateToHuman()
    {
        string msg = m_panda.sendMessage();
        // 将熊猫语言转换成英语，再传递给外国人
        m_foreigner->setResult("美国佬：" + msg);
    }
};

class FrenchChopper : public AbstractChopper
{
public:
    using AbstractChopper::AbstractChopper;
    void translateToPandas()
    {
        string msg = m_foreigner->confession();
        //解析熊猫的语言，并将数据传递给熊猫
        m_panda.recvMessage("法国人说:" + msg);
    }
    void translateToHuman()
    {
        string msg = m_panda.sendMessage();
        // 将熊猫语言转换成英语，再传递给外国人
        m_foreigner->setResult("法国佬：" + msg);
    }
};
#endif

#if 1
//适配器和熊猫类是继承关系
class Foreigner
{
public:
    virtual string confession() = 0;
    void setResult(string res)
    {
        cout << "Panda say:" << res << endl;
    }
    virtual ~Foreigner() {}
};

//美国人
class American : public Foreigner
{
public:
    string confession() override
    {
        return "american,我有罪\n";
    }
    
};

//法国人
class French : public Foreigner
{
public:
    string confession() override
    {
        return "french,我有罪\n";
    }
};

//熊猫
class Panda
{
public:
    void recvMessage(string msg)
    {
        cout << msg << endl;
    }
    string sendMessage()
    {
        return string("强盗、罪人、凶手是不可能被宽恕和原谅的！\n");
    }
};

//适配器类的基类
class AbstractChopper : public Panda
{
public:
    AbstractChopper(Foreigner* foreigner) : m_foreigner(foreigner) {}
    virtual void translateToPandas() = 0;
    virtual void translateToHuman() = 0;
    virtual ~AbstractChopper() {}
protected:
    Foreigner* m_foreigner;
};

class EnglishChopper : public AbstractChopper
{
public:
    // 继承父类的所有构造函数
    using AbstractChopper::AbstractChopper;
    void translateToPandas()
    {
        string msg = m_foreigner->confession();
        //解析熊猫的语言，并将数据传递给熊猫
        recvMessage("美国人说：" + msg);
    }
    void translateToHuman()
    {
        string msg = sendMessage();
        // 将熊猫语言转换成英语，再传递给外国人
        m_foreigner->setResult("美国佬：" + msg);
    }
};

class FrenchChopper : public AbstractChopper
{
public:
    using AbstractChopper::AbstractChopper;
    void translateToPandas()
    {
        string msg = m_foreigner->confession();
        //解析熊猫的语言，并将数据传递给熊猫
        recvMessage("法国人说:" + msg);
    }
    void translateToHuman()
    {
        string msg = sendMessage();
        // 将熊猫语言转换成英语，再传递给外国人
        m_foreigner->setResult("法国佬：" + msg);
    }
};
#endif

int main()
{
    Foreigner* foreigner = new American();
    AbstractChopper* adapter = new EnglishChopper(foreigner);
    adapter->translateToPandas();
    adapter->translateToHuman();
    delete adapter;
    delete foreigner;
    cout << "-----------------------------" << endl;
    foreigner = new French();
    adapter = new FrenchChopper(foreigner);
    adapter->translateToPandas();
    adapter->translateToHuman();
    delete adapter;
    delete foreigner;
    return 0;
}