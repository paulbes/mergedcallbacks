#include "MergedCallbacks.hpp"
#include <iostream>

void f()
{
    static int counter = 0;
    std::cout << "f(), call counter: " << counter << std::endl;
    ++counter;
}

class A
{
public:
    void f(int a)
    {
        static int counter = 0;
        std::cout << "A::f(int), call counter: "  << counter << std::endl;
        counter++;
        a++;
    }

    virtual void g(int b, int c)
    {
        b++; 
        c++;
    }
};

class B : public A
{
public:
    void g(int b, int c)
    {
        static int counter = 0;
        std::cout << "B::g(int, int), call counter: " << counter << std::endl;
        counter++;
    }
};

int main()
{

    A a;
    B b;

    MergedCallbacks mc;
    
    mc.registerUniqueCallback(MergedCallbacks::NORMAL, boost::bind(&f));
    mc.registerUniqueCallback(MergedCallbacks::NORMAL, boost::bind(&f));

    mc.registerUniqueCallback(MergedCallbacks::NORMAL, boost::bind(&A::f, &a, 1));
    mc.registerUniqueCallback(MergedCallbacks::NORMAL, boost::bind(&A::f, &a, 1));
    mc.registerUniqueCallback(MergedCallbacks::NORMAL, boost::bind(&A::f, &a, 2));

    mc.registerUniqueCallback(MergedCallbacks::NORMAL, boost::bind(&B::g, &b, 0, 0));
    mc.registerUniqueCallback(MergedCallbacks::NORMAL, boost::bind(&B::g, &b, 0, 0));
    mc.registerUniqueCallback(MergedCallbacks::NORMAL, boost::bind(&B::g, &b, 0, 1));
    mc.registerUniqueCallback(MergedCallbacks::NORMAL, boost::bind(&B::g, &b, 0, 1));
    mc.registerUniqueCallback(MergedCallbacks::NORMAL, boost::bind(&B::g, &b, 1, 0));
    mc.registerUniqueCallback(MergedCallbacks::NORMAL, boost::bind(&B::g, &b, 1, 0));

    mc.executeCallbackWithPriority(MergedCallbacks::NORMAL);

    return 0;
}