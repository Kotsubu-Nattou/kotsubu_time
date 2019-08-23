#################  My Makefile V1.1  ##################
# 主に記述は ### 記入項目 ### の内容。
# PROG   --- Windows環境下では、必ず「.exe」を付けること
# RESRC  --- リソースファイルを使用するなら「ファイル名.o」
# SUB_*  --- 下位ディレクトリなどのオブジェクトファイル
# CFLAFS --- コンパイルオプションを指定
# .oファイルが増えたり.hファイルの依存関係が変更になった場合、
# 「ヘッダファイルの依存関係」の項目で、個別に指定する。
########################################################


# マクロ   ### 記入項目 ###
PROG     := a.exe
RESRC    :=
SUB_DIR  := 
SUB_OBJS := 
OBJS     := $(addsuffix .o, $(basename $(wildcard *.cpp)))
LIBS     := 
COMP     := g++
CFLAGS   := -g -Wall -std=c++17


# デフォルトターゲット（先頭にあるのでデフォルトで実行）
# .PHONY: all
# all: clean $(PROG)


# ターゲット：リンカ
$(PROG): $(OBJS)
	$(COMP) $(CFLAGS) $(OBJS) $(LIBS) -o $(PROG)


# ターゲット：リソースファイルのコンパイル
$(RESRC):
	windres $*.rc  -o $@


# ターゲット：中間ファイルの削除
.PHONY: clean
clean:
	-rm *.o


# サフィックスルール（.oをコンパイルするときはコチラ）
# -o からの文は、ソース自身のディレクトリに出力させるため（無いとカレントに出力）
.c.o:
	$(COMP) $(CFLAGS)  -c $<  -o $(basename $<).o

.cpp.o:
	$(COMP) $(CFLAGS)  -c $<  -o $(basename $<).o


# ヘッダファイルの依存関係   ### 記入項目 ###
a.o: kotsubu_time.h
