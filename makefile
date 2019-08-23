#################  My Makefile V1.1  ##################
# ��ɋL�q�� ### �L������ ### �̓��e�B
# PROG   --- Windows�����ł́A�K���u.exe�v��t���邱��
# RESRC  --- ���\�[�X�t�@�C�����g�p����Ȃ�u�t�@�C����.o�v
# SUB_*  --- ���ʃf�B���N�g���Ȃǂ̃I�u�W�F�N�g�t�@�C��
# CFLAFS --- �R���p�C���I�v�V�������w��
# .o�t�@�C������������.h�t�@�C���̈ˑ��֌W���ύX�ɂȂ����ꍇ�A
# �u�w�b�_�t�@�C���̈ˑ��֌W�v�̍��ڂŁA�ʂɎw�肷��B
########################################################


# �}�N��   ### �L������ ###
PROG     := a.exe
RESRC    :=
SUB_DIR  := 
SUB_OBJS := 
OBJS     := $(addsuffix .o, $(basename $(wildcard *.cpp)))
LIBS     := 
COMP     := g++
CFLAGS   := -g -Wall -std=c++17


# �f�t�H���g�^�[�Q�b�g�i�擪�ɂ���̂Ńf�t�H���g�Ŏ��s�j
# .PHONY: all
# all: clean $(PROG)


# �^�[�Q�b�g�F�����J
$(PROG): $(OBJS)
	$(COMP) $(CFLAGS) $(OBJS) $(LIBS) -o $(PROG)


# �^�[�Q�b�g�F���\�[�X�t�@�C���̃R���p�C��
$(RESRC):
	windres $*.rc  -o $@


# �^�[�Q�b�g�F���ԃt�@�C���̍폜
.PHONY: clean
clean:
	-rm *.o


# �T�t�B�b�N�X���[���i.o���R���p�C������Ƃ��̓R�`���j
# -o ����̕��́A�\�[�X���g�̃f�B���N�g���ɏo�͂����邽�߁i�����ƃJ�����g�ɏo�́j
.c.o:
	$(COMP) $(CFLAGS)  -c $<  -o $(basename $<).o

.cpp.o:
	$(COMP) $(CFLAGS)  -c $<  -o $(basename $<).o


# �w�b�_�t�@�C���̈ˑ��֌W   ### �L������ ###
a.o: kotsubu_time.h
