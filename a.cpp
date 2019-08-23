#include "kotsubu_time.h"
#include <iostream>
using namespace std;



// kotsubu_timeのテストプログラム
int main()
{
    unsigned long ms = KotsubuTime::getTickCount();
    cout << KotsubuTime::toMinSecMS(ms) << endl;


    size_t s = KotsubuTime::getTime();
    cout << KotsubuTime::toDate(s) << endl;


    KotsubuTime& time = KotsubuTime::getInstance();
    time.stopwatch.start();
    time.sleep(1000);
    cout << time.stopwatch.getMS() << endl;


    return 0;
}
