#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include<allegro5/allegro_primitives.h>
#include <stdlib.h>
#include <math.h>

const float FPS = 100;  
const int SCREEN_W = 640*1.5;
const int SCREEN_H = 540;

float dist(float x1, float x2, float y1, float y2) {
	return sqrt(pow(x1-x2,2) + pow(y1-y2,2));
}

typedef struct Bola {
	float x;
	float y;
	float raio;
	float dx;
	float dy;
	ALLEGRO_COLOR cor;
} Bola;

void criaBola(Bola *b) {
	b->y = SCREEN_H/2;
	b->x = SCREEN_W/2;
	b->cor = al_map_rgb(0,200,0);
	b->raio = 10;
	b->dx = 0;
	b->dy = 0;
}

void desenhaBola(Bola b) {
	al_draw_filled_circle(b.x, b.y, b.raio, b.cor);
}

int main() {
	
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;	
	int playing = 1;
	
	if(!al_init()) {
      fprintf(stderr, "failed to initialize allegro!\n");
      return -1;
   }
   
    if(!al_init_primitives_addon()){
		fprintf(stderr, "failed to initialize primitives!\n");
        return -1;
    }   
   
   display = al_create_display(SCREEN_W, SCREEN_H);
   if(!display) {
      fprintf(stderr, "failed to create display!\n");
      return -1;
   }
   
   timer = al_create_timer(1.0 / FPS);
   if(!timer) {
      fprintf(stderr, "failed to create timer!\n");
      return -1;
   }
 
   
   event_queue = al_create_event_queue();
    if(!event_queue) {
      fprintf(stderr, "failed to create event_queue!\n");
      return -1;
   }
   
   
	if(!al_install_mouse())
		fprintf(stderr, "failed to initialize mouse!\n");

   	//registra na fila de eventos que eu quero identificar quando a tela foi alterada
   al_register_event_source(event_queue, al_get_display_event_source(display));
	//registra na fila de eventos que eu quero identificar quando o tempo alterou de t para t+1
   al_register_event_source(event_queue, al_get_timer_event_source(timer));
	// Registra mouse na fila de eventos:
	al_register_event_source(event_queue, al_get_mouse_event_source());    
   
	ALLEGRO_COLOR BKG_COLOR = al_map_rgb(255,255,255);
   //avisa o allegro que agora eu quero modificar as propriedades da tela
   al_set_target_bitmap(al_get_backbuffer(display));
	//colore a tela de preto (rgb(0,0,0))
   al_clear_to_color(BKG_COLOR);     

   Bola bola;
   criaBola(&bola);
   

   
  ALLEGRO_EVENT ev;
  	//inicia o temporizador
   al_start_timer(timer);
  while(playing) {
	  
	al_wait_for_event(event_queue, &ev);
	  
	if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
		playing = 0;
	}	  
	else if(ev.type == ALLEGRO_EVENT_TIMER) {
	  
	  if(bola.x > SCREEN_W || bola.x < 0)
		  bola.dx = -bola.dx;
	  if(bola.y > SCREEN_H || bola.y < 0)
		  bola.dy = -bola.dy;
	  
	  
	   bola.x += bola.dx;
	   bola.y += bola.dy;
	   al_clear_to_color(BKG_COLOR); 
   	   desenhaBola(bola);
	   al_flip_display();
	   
	}
	else if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {

		bola.dx = (ev.mouse.x - bola.x)/dist(ev.mouse.x, bola.x, ev.mouse.y, bola.y);
		bola.dy = (ev.mouse.y - bola.y)/dist(ev.mouse.x, bola.x, ev.mouse.y, bola.y);
	}	
  }
   
   
   
	return 0;
	
}