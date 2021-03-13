SOURCE_DIR=./core/src

LIBS=`pkg-config --cflags --libs allegro-5 allegro_acodec-5 allegro_audio-5 allegro_color-5 allegro_dialog-5 allegro_font-5 allegro_image-5 allegro_main-5 allegro_memfile-5 allegro_physfs-5 allegro_primitives-5 allegro_ttf-5` -lm

all: tank.o collision.o drawer.o score.o combat clean execute

execute:
	./combat

combat: combat.o
	gcc -o combat combat.o tank.o collision.o score.o drawer.o $(LIBS)

combat.o:
	gcc -c $(SOURCE_DIR)/combat.c $(LIBS)

score.o:
	gcc -c $(SOURCE_DIR)/score.c $(LIBS)

tank.o:
	gcc -c ./modules/tank/src/tank.c $(LIBS)

collision.o:
	gcc -c ./modules/collision/src/collision.c $(LIBS)

drawer.o:
	gcc -c ./modules/drawer/src/drawer.c $(LIBS)

clean:
	rm -r tank.o
	rm -r combat.o
	rm -r collision.o
	rm -r drawer.o
	rm -r score.o