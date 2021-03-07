LIBS=`pkg-config --cflags --libs allegro-5 allegro_acodec-5 allegro_audio-5 allegro_color-5 allegro_dialog-5 allegro_font-5 allegro_image-5 allegro_main-5 allegro_memfile-5 allegro_physfs-5 allegro_primitives-5 allegro_ttf-5` -lm

all: tank.o collision.o combat clean

combat: combat.o
	gcc -o combat combat.o tank.o collision.o $(LIBS)

combat.o: combat.c
	gcc -c combat.c $(LIBS)

tank.o: tank.c
	gcc -c tank.c $(LIBS)

collision.o: collision.c
	gcc -c collision.c $(LIBS)

clean:
	rm -r tank.o
	rm -r combat.o
	rm -r collision.o