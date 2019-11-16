CC = g++.exe

INC_DIR = -I"./include"
LIB_DIR = -L"./lib"

CFLAG = -g -Wall -Wl,-subsystem=windows
LFLAG = -lmingw32 -lSDL2main -lSDL2

SRC = main.cpp
OUT = -o main.exe

all : $(SRC)
	$(CC) $(SRC) $(INC_DIR) $(LIB_DIR) $(CFLAG) $(LFLAG) $(OUT)
    