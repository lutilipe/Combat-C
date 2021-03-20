SOURCE_DIR=./core/src

ifeq ($(OS),Windows_NT)

ALLEGRO_VERSION=5.0.10
MINGW_VERSION=4.7.0
FOLDER=C:

FOLDER_NAME=\allegro-$(ALLEGRO_VERSION)-mingw-$(MINGW_VERSION)
PATH_ALLEGRO=$(FOLDER)$(FOLDER_NAME)
LIB_ALLEGRO=\lib\liballegro-$(ALLEGRO_VERSION)-monolith-mt.a
INCLUDE_ALLEGRO=\include

all: tank.o obstacle.o collision.o drawer.o score.o game.o combat clean execute

execute:
	./combat

combat: combat.o
	gcc -o combat obstacle.o combat.o tank.o collision.o score.o drawer.o game.o

combat.o:
	gcc -I $(PATH_ALLEGRO)$(INCLUDE_ALLEGRO) -c $(SOURCE_DIR)/combat.c

score.o:
	gcc -I $(PATH_ALLEGRO)$(INCLUDE_ALLEGRO) -c ./screens/src/score.c
	
game.o:
	gcc -I $(PATH_ALLEGRO)$(INCLUDE_ALLEGRO) -c ./screens/src/game.c

obstacle.o:
	gcc -I $(PATH_ALLEGRO)$(INCLUDE_ALLEGRO) -c ./modules/src/obstacle.c

tank.o:
	gcc -I $(PATH_ALLEGRO)$(INCLUDE_ALLEGRO) -c ./modules/src/tank.c

collision.o:
	gcc -I $(PATH_ALLEGRO)$(INCLUDE_ALLEGRO) -c ./modules/src/collision.c

drawer.o:
	gcc -I $(PATH_ALLEGRO)$(INCLUDE_ALLEGRO) -c ./modules/src/drawer.c

clean:
	del tank.o
	del obstacle.o
	del combat.o
	del collision.o
	del drawer.o
	del score.o
	del game.o

else

LIBS=`pkg-config --cflags --libs allegro-5 allegro_acodec-5 allegro_audio-5 allegro_color-5 allegro_dialog-5 allegro_font-5 allegro_image-5 allegro_main-5 allegro_memfile-5 allegro_physfs-5 allegro_primitives-5 allegro_ttf-5` -lm

all: tank.o collision.o drawer.o score.o game.o obstacle.o combat clean execute

execute:
	./combat

combat: combat.o
	gcc -o combat combat.o tank.o collision.o score.o drawer.o obstacle.o game.o $(LIBS)

combat.o:
	gcc -c $(SOURCE_DIR)/combat.c $(LIBS)

score.o:
	gcc -c ./screens/src/score.c $(LIBS)

game.o:
	gcc -c ./screens/src/game.c $(LIBS)

obstacle.o:
	gcc -c ./modules/src/obstacle.c $(LIBS)

tank.o:
	gcc -c ./modules/src/tank.c $(LIBS)

collision.o:
	gcc -c ./modules/src/collision.c $(LIBS)

drawer.o:
	gcc -c ./modules/src/drawer.c $(LIBS)

clean:
	rm -f tank.o
	rm -f obstacle.o
	rm -f combat.o
	rm -f collision.o
	rm -f drawer.o
	rm -f score.o
	rm -f game.o
	
endif