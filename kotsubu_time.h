/**************************************************************************************************
【ヘッダオンリークラス】kotsubu_time v1.0

〇 概要
  時間に関する処理を集めたシングルトン。
  一般的な時間に関するメソッド、ウェイトシステム、ストップウォッチ、など。
  解放は不要（アプリケーション終了時に自動）
  内部クラス以外のメソッドは「静的メソッド」のためインクルードだけで利用可能。
  文字列を返すメソッドは、すべてstd::string値を返す


〇 使い方
  #include "kotsubu_time.h"
  // インスタンスを取得。これで全てのメンバにアクセス可能
  KotsubuTime &time = KotsubuTime::getInstance();
  // 利用する
  time.sleep(1000);


〇 使い方 - ウェイトシステム（内部クラス）
  1. time.waitSection.begin();
  2. 何らかの処理（この処理は300msかかるとする）
  3. time.waitSection.end(1000);
  ・基本は「何らかの処理」をbegin～endで挟む
  ・実際にウェイトされるタイミングは.end
  ・実際にウェイトされる時間は700ms（1000ms - 300ms）


〇 補足
  ・Windows依存
  ・DWORD型は、unsigned longと同じ（キャスト不要）
  ・時間関数の「49日問題」には、簡易的な対策しかしていない。
  ・このライブラリは、C言語からC++に移植＆改良したもの
  ・c++で時間を扱うベターな方法は、<ctime>をインクルードして、c言語の
    やり方で記述する（c++からtime(),localtime(),tm構造体などを利用）
  ・「49日問題」とは
    unsigned long（32bit）でミリ秒をカウントすると、約49日で「桁あふれ」する。
    簡単な数値の範囲修正を行い、実行時エラーは防げたとしても、保持している「比べる時間」などと
    辻褄が合わなくなり、様々な不定動作を引き起こす。影響範囲が分かりにくくデバッグが困難。
    型やデータの精度を変えて簡易的に問題を軽減できる。この問題はOSを問わない


〇 <time.h>および<ctime>について
  ・time_t型
      これを利用する関数は、-1をエラーとしている。
      一般的にtime_t値は、localtime()やctime()などで文字列に変換して利用する。
      ISO Cでは「数値型」としか定義していない。符号化方式も定められていないため算術演算に注意。
      VCにおいては、VS2005より前はlong int（32bit）と同じ、現在は_time64_t（64bit）と同じ

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

  ・メソッド
      time(NULL)                  --- GMT秒を返す（time_t）
      ctime(time_t)               --- 文字列を返す（曜日 月 日  時:分:秒  年\n\0）
      localtime(time_t)           --- 暦時刻を返す（tm）
      mktime(*tm)                 --- tmをtime_tに変換（不完全なtmでも自動調整される）
      difftime(time_t2, time_t1)  --- time_t1からtime_t2の時間差を返す（time_t）
      strftime(*str, size, 書式, *tm) --- 書式に従いtmを文字列化。ポインタをstrに返す
**************************************************************************************************/



#pragma once

#include <ctime>      // time_t型、tm構造体型、time(), localtime()
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
    // <戻り値> グリニッチ標準時（GMT）の1970/1/1からの秒数。エラーは-1
    // <time_tについて>
    // 一般的にtime_t値は、localtime()やctime()などで文字列に変換して利用する。
    // ISO Cでは「数値型」としか定義していない。符号化方式も定められていないため算術演算に注意。
    // VCにおいては、VS2005より前はlong int（32bit）と同じ、現在は_time64_t（64bit）と同じ
    static time_t getGmtSec()
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
    // この機能は、汎用性よりも学習や簡単なログ向けに実装

    // 【メソッド】GMT秒を文字列「year/mon/day」にして返す
    // <引数> GMT秒（time()の値）
    static std::string toDate(time_t gmtSec)
    {
        tm* t = localtime(&gmtSec);  // 暦時刻を生成
        return std::string(std::to_string(1900 + t->tm_year) + "/" +
                           std::to_string(1 + t->tm_mon)     + "/" +
                           std::to_string(t->tm_mday));
    }

    // 【メソッド】現在時刻の文字列「year/mon/day」を返す
    static std::string currentDate()
    {
        return toDate(getGmtSec());
    }



    // 【メソッド】GMT秒を文字列「hour:min:sec」にして返す
    // <引数> GMT秒（time()の値）
    static std::string toTime(time_t gmtSec)
    {
        tm* t = localtime(&gmtSec);  // 暦時刻を生成
        return std::string(std::to_string(t->tm_hour) + ":" +
                           std::to_string(t->tm_min)  + ":" +
                           std::to_string(t->tm_sec));
    }

    // 【メソッド】現在時刻の文字列「hour:min:sec」を返す
    static std::string currentTime()
    {
        return toTime(getGmtSec());
    }



    // 【メソッド】GMT秒をctime文字列にして返す
    // <書式> 曜日 月 日  時:分:秒  年\n\0 （\nに注意）
    // <引数> GMT秒（time()の値）
    static std::string toCtime(time_t t)
    {
        return std::string(ctime(&t));
    }

    // 【メソッド】現在時刻のctime文字列を返す
    // <書式> 曜日 月 日  時:分:秒  年\n\0 （\nに注意）
    static std::string currentCtime()
    {
        return toCtime(getGmtSec());
    }



    // 【メソッド】ミリ秒を文字列「min sec ms」にして返す
    // <引数> ミリ秒（GetTickCount()の値など）
    static std::string toMinSecMs(unsigned long ms)
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
    class WaitSection
    {
        unsigned long beginTime = 0;

    public:
        // 【メソッド】ウェイトを適用する区間の始まり
        void begin()
        {
            beginTime = GetTickCount();
        }



        // 【メソッド】ウェイトを適用する区間の終わり（実際にウェイトする）
        // 実際にウェイトする時間 = 引数に指定した時間 - 基点からの経過時間。
        // 上記、0以下の場合は何もしない。
        // <引数> 最長でウェイトする時間（ミリ秒）
        void end(unsigned long maxWaitMs)
        {
            unsigned long nowTime = GetTickCount();
            unsigned long elapseTime;


            // 49日問題とアンダーフローの抑制
            if( nowTime < beginTime ) nowTime = beginTime;

            // ウェイトする時間を求めて実行
            elapseTime = nowTime - beginTime;
            if( maxWaitMs > elapseTime ) Sleep( maxWaitMs - elapseTime );

            // 基点時刻をリセット
            beginTime = 0;
        }
    } waitSection;





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
        unsigned long getMs()
        {
            unsigned long nowTime = GetTickCount();

            // 49日問題とアンダーフローの抑制
            if( nowTime < startTime ) nowTime = startTime;

            return nowTime - startTime;
        }
    } stopwatch;





private:
    // 【隠しメソッド】
    // 隠しコンストラクタ
    KotsubuTime()
    {
        putenv("TZ=JST-9");  // 環境変数にタイムゾーンを設定
    }
    ~KotsubuTime(){}                             // 隠しデストラクタ
    KotsubuTime(const KotsubuTime&);             // 隠しコピーコンストラクタ
    KotsubuTime& operator=(const KotsubuTime&);  // 隠しコピー代入演算子
};