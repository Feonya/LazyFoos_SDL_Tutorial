CC = g++.exe

INC_DIR = -I"./include"
LIB_DIR = -L"./lib"

CFLAG = -g -Wall -Wl,-subsystem,console
LFLAG = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf

SRC = main.cc texture.cc
OUT = -o ./build/debug/main.exe

all : $(SRC)
	$(CC) $(SRC) $(INC_DIR) $(LIB_DIR) $(CFLAG) $(LFLAG) $(OUT)
    
