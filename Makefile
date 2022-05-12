CC = g++
OUTPUT = bin/debug/application.exe

IPATH = include
SPATH = src
CPATH = bin/intermediates

IFLAGS = -IC:/SDL2/include -IC:/JSON_Parse -I$(IPATH)
LFLAGS = -LC:/SDL2/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image
DFLAGS = -static-libgcc -static-libstdc++ -Wl,-Bstatic -lstdc++ -lpthread
OFLAGS = -Wall -std=c++20
EFLAGS = -sUSE_SDL=2 -sUSE_SDL_IMAGE=2 -sSDL2_IMAGE_FORMATS='png'
EFFLAGS = -sASYNCIFY=1 --preload-file data
FFLAGS =
FLAGS = $(OFLAGS) $(IFLAGS)

# --------------------------------

# Debug build
default: FLAGS += -g
default: FLAGS += $(LFLAGS)
default: application.exe

# Release build
release: FLAGS += $(LFLAGS)
release: FLAGS += $(DFLAGS)
release: FLAGS += -O3
release: OUTPUT = bin/release/application.exe
release: application.exe

# Emscripten build
emscripten: CC = em++
emscripten: FLAGS += -g
emscripten: FLAGS += $(EFLAGS)
emscripten: FFLAGS += $(EFFLAGS)
emscripten: OUTPUT = bin/web/game.html
emscripten: application.exe

# --------------------------------

application.exe: $(CPATH)/main.o $(CPATH)/game.o $(CPATH)/window.o $(CPATH)/player.o $(CPATH)/interactable.o $(CPATH)/base.o $(CPATH)/platform.o $(CPATH)/building.o $(CPATH)/silverStorage.o
	$(CC) $(CPATH)/main.o $(CPATH)/game.o $(CPATH)/window.o $(CPATH)/player.o $(CPATH)/interactable.o $(CPATH)/base.o $(CPATH)/platform.o $(CPATH)/building.o $(CPATH)/silverStorage.o $(FLAGS) $(FFLAGS) -o $(OUTPUT)

$(CPATH)/main.o: $(SPATH)/main.cpp $(IPATH)/game.h
	$(CC) -c $(SPATH)/main.cpp $(FLAGS) -o $(CPATH)/main.o

$(CPATH)/game.o: $(SPATH)/game.cpp $(IPATH)/game.h $(IPATH)/window.h $(IPATH)/player.h $(IPATH)/vector.h $(IPATH)/base.h $(IPATH)/save.h $(IPATH)/utility.h
	$(CC) -c $(SPATH)/game.cpp $(FLAGS) -o $(CPATH)/game.o

$(CPATH)/window.o: $(SPATH)/window.cpp $(IPATH)/window.h $(IPATH)/vector.h $(IPATH)/utility.h
	$(CC) -c $(SPATH)/window.cpp $(FLAGS) -o $(CPATH)/window.o

$(CPATH)/player.o: $(SPATH)/player.cpp $(IPATH)/player.h $(IPATH)/vector.h $(IPATH)/window.h $(IPATH)/utility.h $(IPATH)/base.h $(IPATH)/interactable.h
	$(CC) -c $(SPATH)/player.cpp $(FLAGS) -o $(CPATH)/player.o

$(CPATH)/interactable.o: $(SPATH)/interactable.cpp $(IPATH)/interactable.h $(IPATH)/vector.h $(IPATH)/window.h $(IPATH)/utility.h $(IPATH)/base.h
	$(CC) -c $(SPATH)/interactable.cpp $(FLAGS) -o $(CPATH)/interactable.o

$(CPATH)/base.o: $(SPATH)/base.cpp $(IPATH)/base.h $(IPATH)/vector.h $(IPATH)/window.h $(IPATH)/interactable.h $(IPATH)/platform.h $(IPATH)/building.h $(IPATH)/utility.h
	$(CC) -c $(SPATH)/base.cpp $(FLAGS) -o $(CPATH)/base.o

$(CPATH)/platform.o: $(SPATH)/platform.cpp $(IPATH)/platform.h $(IPATH)/base.h $(IPATH)/vector.h $(IPATH)/interactable.h $(IPATH)/window.h $(IPATH)/utility.h
	$(CC) -c $(SPATH)/platform.cpp $(FLAGS) -o $(CPATH)/platform.o

$(CPATH)/building.o: $(SPATH)/building.cpp $(IPATH)/building.h $(IPATH)/base.h $(IPATH)/vector.h $(IPATH)/interactable.h $(IPATH)/utility.h
	$(CC) -c $(SPATH)/building.cpp $(FLAGS) -o $(CPATH)/building.o

$(CPATH)/silverStorage.o: $(SPATH)/silverStorage.cpp $(IPATH)/silverStorage.h $(IPATH)/building.h $(IPATH)/interactable.h $(IPATH)/vector.h
	$(CC) -c $(SPATH)/silverStorage.cpp $(FLAGS) -o $(CPATH)/silverStorage.o

clean:
	-rm $(CPATH)/*.o