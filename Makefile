CC = g++
OUTPUT = bin/debug/application.exe

IPATH = include
SPATH = src
CPATH = bin/intermediates

IFLAGS = -IC:/SDL2/include -IC:/JSON_Parse -I$(IPATH)
LFLAGS = -LC:/SDL2/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image
OFLAGS = -Wall
FLAGS = $(IFLAGS) $(LFLAGS) $(OFLAGS)

# --------------------------------

default: FLAGS += -g
default: application.exe

release: FLAGS += -O3
release: OUTPUT = bin/release/application.exe
release: application.exe

# --------------------------------

application.exe: $(CPATH)/main.o $(CPATH)/game.o $(CPATH)/window.o
	$(CC) $(CPATH)/main.o $(CPATH)/game.o $(CPATH)/window.o $(FLAGS) -o $(OUTPUT)

$(CPATH)/main.o: $(SPATH)/main.cpp $(IPATH)/game.h
	$(CC) -c $(SPATH)/main.cpp $(FLAGS) -o $(CPATH)/main.o

$(CPATH)/game.o: $(SPATH)/game.cpp $(IPATH)/game.h $(IPATH)/window.h
	$(CC) -c $(SPATH)/game.cpp $(FLAGS) -o $(CPATH)/game.o

$(CPATH)/window.o: $(SPATH)/window.cpp $(IPATH)/window.h $(IPATH)/vector.h $(IPATH)/utility.h
	$(CC) -c $(SPATH)/window.cpp $(FLAGS) -o $(CPATH)/window.o

clean:
	-rm $(CPATH)/*.o