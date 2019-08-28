/**************************************************************************************************
【ヘッダオンリークラス】kotsubu_time

〇 概要
  時間に関する処理を集めたシングルトン。
  一般的な時間に関するメソッド、ウェイトシステム、ストップウォッチ、など。
  解放は不要（アプリケーション終了時に自動）
  内部クラス以外のメソッドは「静的メソッド」のためインクルードだけで利用可能。


〇 使い方
  #include "kotsubu_time.h"
  // インスタンスを取得。これで全てのメンバにアクセス可能
  KotsubuTime &time = KotsubuTime::getInstance();
  // 利用する
  time.sleep(1000);


〇 使い方 - ウェイトシステム
  1. time.wait.anchor();
  2. 何らかの処理（この処理は300msかかるとする）
  3. time.wait.execute(1000);
  ・「何らかの処理」を挟む形で利用する
  ・実際にウェイトされるタイミングは上記の3.
  ・実際にウェイトされる時間は700ms（1000ms - 300ms）


〇 補足
  ・Windows依存
  ・DWORD型は、unsigned longと同じ（キャスト不要）
  ・時間関数の「49日問題」には、簡易的な対策しかしていない
  ・このライブラリは、C言語からC++に移植＆改良したもの
  ・c++で時間を扱うベターな方法は、<ctime>をインクルードして、c言語の
    やり方で記述する（c++からtime(),localtime(),tm構造体などを利用）


〇 おまけ。<time.h>または<ctime>について
  ・time_t型
      これを利用する関数は、-1をエラーとしている。
      ISO Cでは「数値型」としか定義していない。符号化方式も定められていないため算術演算に注意。
      VCにおいて、VS2005より前はlong int（32bit）と同じ、現在は_time64_t（64bit）と同じ

  ・tm構造体型
      暦時刻（calendar time）の要素を格納するための構造体型。
      struct tm {
          int  tm_year;   // 年（1900年からの年数）
          int  tm_mon;    // 月（1月=0 ～ 12月=11）
          int  tm_mday;   // 日
          int  tm_hour;   // 時
          int  tm_min;    // 分
          int  tm_sec;    // 秒
          int  tm_wday;   // 曜日（日曜=0 ～ 土曜=6）
          int  tm_yday;   // 1月1日からの経過日数（0 ～ 355）
          int  tm_isdst;  // 夏時間フラグ
      };

  ・ベターな使い方
      1.   time()でGMT秒（time_t）を取得
      2-a. ctime()にGMT秒を渡し、日付文字列を取得
      2-b. localtime()にGMT秒を渡し、暦時刻（tm）を取得
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



    ////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 〇 時間処理一般

    // 【メソッド】システムの時刻を返す
    // <戻り値> グリニッチ標準時（GMT）の1970/1/1からの経過秒数。エラーは-1
    // <補足> 経過秒数はlocaltime()やctime()などで、文字列に変換して利用するのがベター
    static time_t getTime()
    {
        return time(NULL);  // timeは戻り値と引数に同じ値を返す。NULLの指定で「戻り値のみ」を利用
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



    ////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 〇 文字列を出力する時間処理
    // この機能は、汎用性よりも学習や簡単なログの出力用。

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