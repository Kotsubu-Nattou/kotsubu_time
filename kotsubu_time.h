/**************************************************************************************************
�y�w�b�_�I�����[�N���X�zkotsubu_time

�Z�T�v
  ���ԂɊւ��鏈�����W�߂��V���O���g���B
  ��ʓI�Ȏ��ԂɊւ��郁�\�b�h�A�E�F�C�g�V�X�e���A�X�g�b�v�E�H�b�`�A�ȂǁB
  ����͕s�v�i�A�v���P�[�V�����I�����Ɏ����j
  �����N���X�ȊO�̃��\�b�h�́u�ÓI���\�b�h�v�̂��߃C���N���[�h�����ŗ��p�\�B

�Z�g����
  #include "kotsubu_time.h"
  // �C���X�^���X���擾�B����őS�Ẵ����o�ɃA�N�Z�X�\
  KotsubuTime &time = KotsubuTime::getInstance();
  // ���p����
  time.sleep(1000);

�Z�g���� - �E�F�C�g�V�X�e��
  1. time.wait.anchor();
  2. ���炩�̏����i���̏�����300ms������Ƃ���j
  3. time.wait.execute(1000);
  �E�u���炩�̏����v�����ތ`�ŗ��p����
  �E���ۂɃE�F�C�g�����^�C�~���O�͏�L��3.
  �E���ۂɃE�F�C�g����鎞�Ԃ�700ms�i1000ms - 300ms�j

�Z���ӓ�
  �EWindows�ˑ�
  �EDWORD�^�́Aunsigned long�Ɠ����i�L���X�g�s�v�j
  �E���Ԋ֐��́u49�����v�ɂ́A�ȈՓI�ȑ΍􂵂����Ă��Ȃ�
  �EC���ꂩ��AC++�ɈڐA������
**************************************************************************************************/



#pragma once

#include <ctime>      // time_t�^�Atm�\���̌^�Atime(), localtime(), strftime()
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



    // �y���\�b�h�z�V�X�e���̎�����Ԃ�
    // <�߂�l> ���ԁi�b�j
    static time_t getTime()
    {
        return time(NULL);
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



    // �y���\�b�h�z�b����t������ɂ��ĕԂ�
    // <����>   �b�itime()�̒l�Ȃǁj
    // <�߂�l> ������f�[�^
    static std::string toDate(time_t sec)
    {
        putenv("TZ=JST-9");              // ���ϐ��Ƀ^�C���]�[����ݒ�
        struct tm* t = localtime(&sec);  // ���ԍ\���̂Ƀf�[�^���擾

        return std::string(std::to_string(1900 + t->tm_year) + "/" +
                           std::to_string(1 + t->tm_mon) + "/" +
                           std::to_string(t->tm_mday) + " " +
                           std::to_string(t->tm_hour) + ":" +
                           std::to_string(t->tm_min) + ":" +
                           std::to_string(t->tm_sec));
    }



    // �y���\�b�h�z�~���b�𕶎���"min sec ms"�ɂ��ĕԂ�
    // <����>   �~���b�iGetTickCount()�̒l�Ȃǁj
    // <�߂�l> ������f�[�^
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
    // �y�����N���X�z�E�F�C�g�V�X�e��
    //
    class Wait
    {
        unsigned long anchorTime = 0;

    public:
        // �y���\�b�h�z�E�F�C�g��K�p�����������́u��_�v���Z�b�g
        void anchor()
        {
            anchorTime = GetTickCount();
        }



        // �y���\�b�h�z�E�F�C�g�����s
        // ���ۂɃE�F�C�g���鎞�� = �����Ɏw�肵������ - ��_����̌o�ߎ��ԁB
        // ��L�A0�ȉ��̏ꍇ�͉������Ȃ��B
        // <����> �Œ��ŃE�F�C�g���鎞�ԁi�~���b�j
        void execute(unsigned long maxWaitMS)
        {
            unsigned long nowTime = GetTickCount();
            unsigned long elapseTime;


            // 49�����̊ȈՑ΍�
            if( nowTime < anchorTime ) nowTime = anchorTime;

            // ���������^�Ȃ̂ŁA���ʂ�0�����ɂȂ�Ȃ��悤�A���_�[�t���[�΍􂵂ĉ��Z�B
            elapseTime = nowTime - anchorTime;
            if( maxWaitMS > elapseTime ) Sleep( maxWaitMS - elapseTime );

            // ��_���������Z�b�g
            anchorTime = 0;
        }
    } wait;





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
        unsigned long getMS()
        {
            unsigned long nowTime = GetTickCount();

            // 49�����y�уA���_�[�t���[�΍�
            if( nowTime < startTime ) nowTime = startTime;

            return nowTime - startTime;
        }
    } stopwatch;





private:
    // �y�B�����\�b�h�z
    KotsubuTime(){}                              // �B���R���X�g���N�^
    ~KotsubuTime(){}                             // �B���f�X�g���N�^
    KotsubuTime(const KotsubuTime&);             // �B���R�s�[�R���X�g���N�^
    KotsubuTime& operator=(const KotsubuTime&);  // �B���R�s�[������Z�q
};