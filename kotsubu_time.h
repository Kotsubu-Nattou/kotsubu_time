/**************************************************************************************************
【ヘッダオンリークラス】kotsubu_time

〇概要
  時間に関する処理を集めたシングルトン。
  一般的な時間に関するメソッド、ウェイトシステム、ストップウォッチ、など。
  解放は不要（アプリケーション終了時に自動）
  内部クラス以外のメソッドは「静的メソッド」のためインクルードだけで利用可能。

〇使い方
  #include "kotsubu_time.h"
  // インスタンスを取得。これで全てのメンバにアクセス可能
  KotsubuTime &time = KotsubuTime::getInstance();
  // 利用する
  time.sleep(1000);

〇使い方 - ウェイトシステム
  1. time.wait.anchor();
  2. 何らかの処理（この処理は300msかかるとする）
  3. time.wait.execute(1000);
  ・「何らかの処理」を挟む形で利用する
  ・実際にウェイトされるタイミングは上記の3.
  ・実際にウェイトされる時間は700ms（1000ms - 300ms）

〇注意等
  ・Windows依存
  ・DWORD型は、unsigned longと同じ（キャスト不要）
  ・時間関数の「49日問題」には、簡易的な対策しかしていない
  ・C言語から、C++に移植＆改良
**************************************************************************************************/



#pragma once

#include <ctime>      // time_t型、tm構造体型、time(), localtime(), strftime()
#include <cstdio>     // C言語からの移植のため
#include <string>     // クラスstring（C++）
#include <windows.h>  // DWORD型、GetTickCount(), Sleep()





///////////////////////////////////////////////////////////////////////////////////////////////
// 【クラス】KotsubuTime
//
class KotsubuTime
{
public:
    // 【メソッド】唯一のインスタンスの参照を返す
    static KotsubuTime& getInstance()
    {
        static KotsubuTime inst;
        return inst;
    }



    // 【メソッド】システムの時刻を返す
    // <戻り値> 時間（秒）
    static time_t getTime()
    {
        return time(NULL);
    }



    // 【メソッド】システムが起動してからの時間を返す
    // <戻り値> 時間（ミリ秒）
    static unsigned long getTickCount()
    {
        return GetTickCount();
    }



    // 【メソッド】スリープする
    // <引数> 時間（ミリ秒）
    static void sleep(unsigned long ms)
    {
        Sleep(ms);
    }



    // 【メソッド】秒を日付文字列にして返す
    // <引数>   秒（time()の値など）
    // <戻り値> 文字列データ
    static std::string toDate(time_t sec)
    {
        putenv("TZ=JST-9");              // 環境変数にタイムゾーンを設定
        struct tm* t = localtime(&sec);  // 時間構造体にデータを取得

        return std::string(std::to_string(1900 + t->tm_year) + "/" +
                           std::to_string(1 + t->tm_mon) + "/" +
                           std::to_string(t->tm_mday) + " " +
                           std::to_string(t->tm_hour) + ":" +
                           std::to_string(t->tm_min) + ":" +
                           std::to_string(t->tm_sec));
    }



    // 【メソッド】ミリ秒を文字列"min sec ms"にして返す
    // <引数>   ミリ秒（GetTickCount()の値など）
    // <戻り値> 文字列データ
    static std::string toMinSecMS(unsigned long ms)
    {
        unsigned long m = ms / 60000;
        ms %= 60000;
        unsigned long s = ms / 1000;
        ms %= 1000;

        return std::string(std::to_string(m)  + "m " +
                           std::to_string(s)  + "s " +
                           std::to_string(ms) + "ms");
    }





    ///////////////////////////////////////////////////////////////////////////////////////////////
    // 【内部クラス】ウェイトシステム
    //
    class Wait
    {
        unsigned long anchorTime = 0;

    public:
        // 【メソッド】ウェイトを適用したい処理の「基点」をセット
        void anchor()
        {
            anchorTime = GetTickCount();
        }



        // 【メソッド】ウェイトを実行
        // 実際にウェイトする時間 = 引数に指定した時間 - 基点からの経過時間。
        // 上記、0以下の場合は何もしない。
        // <引数> 最長でウェイトする時間（ミリ秒）
        void execute(unsigned long maxWaitMS)
        {
            unsigned long nowTime = GetTickCount();
            unsigned long elapseTime;


            // 49日問題の簡易対策
            if( nowTime < anchorTime ) nowTime = anchorTime;

            // 符号無し型なので、結果が0未満にならないようアンダーフロー対策して演算。
            elapseTime = nowTime - anchorTime;
            if( maxWaitMS > elapseTime ) Sleep( maxWaitMS - elapseTime );

            // 基点時刻をリセット
            anchorTime = 0;
        }
    } wait;





    ///////////////////////////////////////////////////////////////////////////////////////////////
    // 【内部クラス】ストップウォッチ
    //
    class Stopwatch
    {
        unsigned long startTime = 0;

    public:
        // 【メソッド】ストップウォッチを計測開始
        void start()
        {
            startTime = GetTickCount();
        }



        // 【メソッド】ストップウォッチの経過時間を返す
        // <戻り値> 時間（ミリ秒）
        unsigned long getMS()
        {
            unsigned long nowTime = GetTickCount();

            // 49日問題及びアンダーフロー対策
            if( nowTime < startTime ) nowTime = startTime;

            return nowTime - startTime;
        }
    } stopwatch;





private:
    // 【隠しメソッド】
    KotsubuTime(){}                              // 隠しコンストラクタ
    ~KotsubuTime(){}                             // 隠しデストラクタ
    KotsubuTime(const KotsubuTime&);             // 隠しコピーコンストラクタ
    KotsubuTime& operator=(const KotsubuTime&);  // 隠しコピー代入演算子
};