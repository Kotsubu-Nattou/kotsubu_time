/**************************************************************************************************
�y�w�b�_�I�����[�N���X�zkotsubu_time v1.0

�Z �T�v
  ���ԂɊւ��鏈�����W�߂��V���O���g���B
  ��ʓI�Ȏ��ԂɊւ��郁�\�b�h�A�E�F�C�g�V�X�e���A�X�g�b�v�E�H�b�`�A�ȂǁB
  ����͕s�v�i�A�v���P�[�V�����I�����Ɏ����j
  �����N���X�ȊO�̃��\�b�h�́u�ÓI���\�b�h�v�̂��߃C���N���[�h�����ŗ��p�\�B
  �������Ԃ����\�b�h�́A���ׂ�std::string�l��Ԃ�


�Z �g����
  #include "kotsubu_time.h"
  // �C���X�^���X���擾�B����őS�Ẵ����o�ɃA�N�Z�X�\
  KotsubuTime &time = KotsubuTime::getInstance();
  // ���p����
  time.sleep(1000);


�Z �g���� - �E�F�C�g�V�X�e���i�����N���X�j
  1. time.waitSection.begin();
  2. ���炩�̏����i���̏�����300ms������Ƃ���j
  3. time.waitSection.end(1000);
  �E��{�́u���炩�̏����v��begin�`end�ŋ���
  �E���ۂɃE�F�C�g�����^�C�~���O��.end
  �E���ۂɃE�F�C�g����鎞�Ԃ�700ms�i1000ms - 300ms�j


�Z �⑫
  �EWindows�ˑ�
  �EDWORD�^�́Aunsigned long�Ɠ����i�L���X�g�s�v�j
  �E���Ԋ֐��́u49�����v�ɂ́A�ȈՓI�ȑ΍􂵂����Ă��Ȃ��B
  �E���̃��C�u�����́AC���ꂩ��C++�ɈڐA�����ǂ�������
  �Ec++�Ŏ��Ԃ������x�^�[�ȕ��@�́A<ctime>���C���N���[�h���āAc�����
    �����ŋL�q����ic++����time(),localtime(),tm�\���̂Ȃǂ𗘗p�j
  �E�u49�����v�Ƃ�
    unsigned long�i32bit�j�Ń~���b���J�E���g����ƁA��49���Łu�����ӂ�v����B
    �ȒP�Ȑ��l�͈̔͏C�����s���A���s���G���[�͖h�����Ƃ��Ă��A�ێ����Ă���u��ׂ鎞�ԁv�Ȃǂ�
    ���낪����Ȃ��Ȃ�A�l�X�ȕs�蓮��������N�����B�e���͈͂�������ɂ����f�o�b�O������B
    �^��f�[�^�̐��x��ς��ĊȈՓI�ɖ����y���ł���B���̖���OS����Ȃ�


�Z <time.h>�����<ctime>�ɂ���
  �Etime_t�^
      ����𗘗p����֐��́A-1���G���[�Ƃ��Ă���B
      ��ʓI��time_t�l�́Alocaltime()��ctime()�Ȃǂŕ�����ɕϊ����ė��p����B
      ISO C�ł́u���l�^�v�Ƃ�����`���Ă��Ȃ��B��������������߂��Ă��Ȃ����ߎZ�p���Z�ɒ��ӁB
      VC�ɂ����ẮAVS2005���O��long int�i32bit�j�Ɠ����A���݂�_time64_t�i64bit�j�Ɠ���

  �Etm�\���̌^
      ����icalendar time�j�̗v�f���i�[���邽�߂̍\���̌^�B
      struct tm {
          int  tm_year;   // �N�i1900�N����̔N���j
          int  tm_mon;    // ���i1��=0 �` 12��=11�j
          int  tm_mday;   // ��
          int  tm_hour;   // ��
          int  tm_min;    // ��
          int  tm_sec;    // �b
          int  tm_wday;   // �j���i���j=0 �` �y�j=6�j
          int  tm_yday;   // 1��1������̌o�ߓ����i0 �` 355�j
          int  tm_isdst;  // �Ď��ԃt���O
      };

  �E���\�b�h
      time(NULL)                  --- GMT�b��Ԃ��itime_t�j
      ctime(time_t)               --- �������Ԃ��i�j�� �� ��  ��:��:�b  �N\n\0�j
      localtime(time_t)           --- �����Ԃ��itm�j
      mktime(*tm)                 --- tm��time_t�ɕϊ��i�s���S��tm�ł��������������j
      difftime(time_t2, time_t1)  --- time_t1����time_t2�̎��ԍ���Ԃ��itime_t�j
      strftime(*str, size, ����, *tm) --- �����ɏ]��tm�𕶎��񉻁B�|�C���^��str�ɕԂ�
**************************************************************************************************/



#pragma once

#include <ctime>      // time_t�^�Atm�\���̌^�Atime(), localtime()
#include <cstdio>     // C���ꂩ��̈ڐA�̂���
#include <string>     // �N���Xstring�iC++�j
#include <windows.h>  // DWORD�^�AGetTickCount(), Sleep()





///////////////////////////////////////////////////////////////////////////////////////////////
// �y�N���X�zKotsubuTime
//
class KotsubuTime
{
public:
    // �y���\�b�h�z�B��̃C���X�^���X�̎Q�Ƃ�Ԃ�
    static KotsubuTime& getInstance()
    {
        static KotsubuTime inst;
        return inst;
    }



    ////////////////////////////////////////////////////////////////////////////////////////////////////////
    // �Z ���ԏ������

    // �y���\�b�h�z�V�X�e���̎�����Ԃ�
    // <�߂�l> �O���j�b�`�W�����iGMT�j��1970/1/1����̕b���B�G���[��-1
    // <time_t�ɂ���>
    // ��ʓI��time_t�l�́Alocaltime()��ctime()�Ȃǂŕ�����ɕϊ����ė��p����B
    // ISO C�ł́u���l�^�v�Ƃ�����`���Ă��Ȃ��B��������������߂��Ă��Ȃ����ߎZ�p���Z�ɒ��ӁB
    // VC�ɂ����ẮAVS2005���O��long int�i32bit�j�Ɠ����A���݂�_time64_t�i64bit�j�Ɠ���
    static time_t getGmtSec()
    {
        return time(NULL);  // time�͖߂�l�ƈ����ɓ����l��Ԃ��BNULL�̎w��Łu�߂�l�̂݁v�𗘗p
    }



    // �y���\�b�h�z�V�X�e�����N�����Ă���̎��Ԃ�Ԃ�
    // <�߂�l> ���ԁi�~���b�j
    static unsigned long getTickCount()
    {
        return GetTickCount();
    }



    // �y���\�b�h�z�X���[�v����
    // <����> ���ԁi�~���b�j
    static void sleep(unsigned long ms)
    {
        Sleep(ms);
    }



    ////////////////////////////////////////////////////////////////////////////////////////////////////////
    // �Z ��������o�͂��鎞�ԏ���
    // ���̋@�\�́A�ėp�������w�K��ȒP�ȃ��O�����Ɏ���

    // �y���\�b�h�zGMT�b�𕶎���uyear/mon/day�v�ɂ��ĕԂ�
    // <����> GMT�b�itime()�̒l�j
    static std::string toDate(time_t gmtSec)
    {
        tm* t = localtime(&gmtSec);  // ����𐶐�
        return std::string(std::to_string(1900 + t->tm_year) + "/" +
                           std::to_string(1 + t->tm_mon)     + "/" +
                           std::to_string(t->tm_mday));
    }

    // �y���\�b�h�z���ݎ����̕�����uyear/mon/day�v��Ԃ�
    static std::string currentDate()
    {
        return toDate(getGmtSec());
    }



    // �y���\�b�h�zGMT�b�𕶎���uhour:min:sec�v�ɂ��ĕԂ�
    // <����> GMT�b�itime()�̒l�j
    static std::string toTime(time_t gmtSec)
    {
        tm* t = localtime(&gmtSec);  // ����𐶐�
        return std::string(std::to_string(t->tm_hour) + ":" +
                           std::to_string(t->tm_min)  + ":" +
                           std::to_string(t->tm_sec));
    }

    // �y���\�b�h�z���ݎ����̕�����uhour:min:sec�v��Ԃ�
    static std::string currentTime()
    {
        return toTime(getGmtSec());
    }



    // �y���\�b�h�zGMT�b��ctime������ɂ��ĕԂ�
    // <����> �j�� �� ��  ��:��:�b  �N\n\0 �i\n�ɒ��Ӂj
    // <����> GMT�b�itime()�̒l�j
    static std::string toCtime(time_t t)
    {
        return std::string(ctime(&t));
    }

    // �y���\�b�h�z���ݎ�����ctime�������Ԃ�
    // <����> �j�� �� ��  ��:��:�b  �N\n\0 �i\n�ɒ��Ӂj
    static std::string currentCtime()
    {
        return toCtime(getGmtSec());
    }



    // �y���\�b�h�z�~���b�𕶎���umin sec ms�v�ɂ��ĕԂ�
    // <����> �~���b�iGetTickCount()�̒l�Ȃǁj
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
    // �y�����N���X�z�E�F�C�g�V�X�e��
    //
    class WaitSection
    {
        unsigned long beginTime = 0;

    public:
        // �y���\�b�h�z�E�F�C�g��K�p�����Ԃ̎n�܂�
        void begin()
        {
            beginTime = GetTickCount();
        }



        // �y���\�b�h�z�E�F�C�g��K�p�����Ԃ̏I���i���ۂɃE�F�C�g����j
        // ���ۂɃE�F�C�g���鎞�� = �����Ɏw�肵������ - ��_����̌o�ߎ��ԁB
        // ��L�A0�ȉ��̏ꍇ�͉������Ȃ��B
        // <����> �Œ��ŃE�F�C�g���鎞�ԁi�~���b�j
        void end(unsigned long maxWaitMs)
        {
            unsigned long nowTime = GetTickCount();
            unsigned long elapseTime;


            // 49�����ƃA���_�[�t���[�̗}��
            if( nowTime < beginTime ) nowTime = beginTime;

            // �E�F�C�g���鎞�Ԃ����߂Ď��s
            elapseTime = nowTime - beginTime;
            if( maxWaitMs > elapseTime ) Sleep( maxWaitMs - elapseTime );

            // ��_���������Z�b�g
            beginTime = 0;
        }
    } waitSection;





    ///////////////////////////////////////////////////////////////////////////////////////////////
    // �y�����N���X�z�X�g�b�v�E�H�b�`
    //
    class Stopwatch
    {
        unsigned long startTime = 0;

    public:
        // �y���\�b�h�z�X�g�b�v�E�H�b�`���v���J�n
        void start()
        {
            startTime = GetTickCount();
        }



        // �y���\�b�h�z�X�g�b�v�E�H�b�`�̌o�ߎ��Ԃ�Ԃ�
        // <�߂�l> ���ԁi�~���b�j
        unsigned long getMs()
        {
            unsigned long nowTime = GetTickCount();

            // 49�����ƃA���_�[�t���[�̗}��
            if( nowTime < startTime ) nowTime = startTime;

            return nowTime - startTime;
        }
    } stopwatch;





private:
    // �y�B�����\�b�h�z
    // �B���R���X�g���N�^
    KotsubuTime()
    {
        putenv("TZ=JST-9");  // ���ϐ��Ƀ^�C���]�[����ݒ�
    }
    ~KotsubuTime(){}                             // �B���f�X�g���N�^
    KotsubuTime(const KotsubuTime&);             // �B���R�s�[�R���X�g���N�^
    KotsubuTime& operator=(const KotsubuTime&);  // �B���R�s�[������Z�q
};