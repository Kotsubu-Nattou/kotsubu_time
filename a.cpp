#include "kotsubu_time.h"
#include <iostream>
using namespace std;



// kotsubu_timeのテストプログラム
int main()
{
    // @ 静的メソッドを利用
    // GetTickCountの取得, "分 秒 ミリ秒"変換
    unsigned long ms = KotsubuTime::getTickCount();
    cout << KotsubuTime::toMinSecMs(ms) << endl;


    // GMT秒の取得, 日付と時刻の文字列, それの現在ver
    size_t s = KotsubuTime::getGmtSec();
    cout << KotsubuTime::toDate(s) << " " << KotsubuTime::toTime(s) << endl;
    cout << KotsubuTime::currentDate() << " " << KotsubuTime::currentTime() << endl;


    // ctime()書式の文字列, それの現在ver
    cout << KotsubuTime::toCtime(s) << endl;
    cout << KotsubuTime::currentCtime() << endl;


    // @ インスタンスのメソッドを利用
    // ストップウォッチ, スリープ
    KotsubuTime& time = KotsubuTime::getInstance();
    time.stopwatch.start();
    time.sleep(1000);
    cout << time.stopwatch.getMs() << endl;


    return 0;
}
