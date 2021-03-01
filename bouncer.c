#include <stdio.h>
#include <allegro5/allegro.h>

//este exemplo foi tirado da pagina http://wiki.allegro.cc/index.php?title=Allegro_5_Tutorial/Bitmaps
//veja a pagina para mais detalhes
 
const float FPS = 60;
const int SCREEN_W = 640;
const int SCREEN_H = 480;
//variavel global que armazena o tamanho do bouncer (quadrado que vai navegar pela tela)
const int BOUNCER_SIZE = 32;
 
int main(int argc, char **argv){
   ALLEGRO_DISPLAY *display = NULL;
   ALLEGRO_EVENT_QUEUE *event_queue = NULL;
   ALLEGRO_TIMER *timer = NULL;
   ALLEGRO_BITMAP *bouncer = NULL;
   
   //posicoes x e y iniciais do bouncer
   float bouncer_x = SCREEN_W / 2.0 - BOUNCER_SIZE / 2.0;
   float bouncer_y = SCREEN_H / 2.0 - BOUNCER_SIZE / 2.0;
   //o quanto as posicoes x e y vao variar ao longo do tempo. No t=1, se x do bouncer eh 40, no t=2, x do bouncer eh 40 + bouncer_dx = 36
   float bouncer_dx = -4.0, bouncer_dy = 4.0;
   bool redraw = true;
 
	//----------------------- rotinas de inicializacao ---------------------------------------
   if(!al_init()) {
      fprintf(stderr, "failed to initialize allegro!\n");
      return -1;
   }
 
   timer = al_create_timer(1.0 / FPS);
   if(!timer) {
      fprintf(stderr, "failed to create timer!\n");
      return -1;
   }
 
   display = al_create_display(SCREEN_W, SCREEN_H);
   if(!display) {
      fprintf(stderr, "failed to create display!\n");
      al_destroy_timer(timer);
      return -1;
   }
 
	//cria um bitmap quadrangular de tamanho BOUNCER_SIZE (variavel global declarada acima)
   bouncer = al_create_bitmap(BOUNCER_SIZE, BOUNCER_SIZE);
   if(!bouncer) {
      fprintf(stderr, "failed to create bouncer bitmap!\n");
      al_destroy_display(display);
      al_destroy_timer(timer);
      return -1;
   }
 
	//avisa o allegro que eu quero modificar as propriedades do bouncer
   al_set_target_bitmap(bouncer);
 
	//altera a cor do bouncer para rgb(255,0,255)
   al_clear_to_color(al_map_rgb(255, 0, 255));
 
	//avisa o allegro que agora eu quero modificar as propriedades da tela
   al_set_target_bitmap(al_get_backbuffer(display));
	//colore a tela de preto (rgb(0,0,0))
   al_clear_to_color(al_map_rgb(0,0,0));   
 
	//cria a fila de eventos
   event_queue = al_create_event_queue();
   if(!event_queue) {
      fprintf(stderr, "failed to create event_queue!\n");
      al_destroy_bitmap(bouncer);
      al_destroy_display(display);
      al_destroy_timer(timer);
      return -1;
   }
 
	//registra na fila de eventos que eu quero identificar quando a tela foi alterada
   al_register_event_source(event_queue, al_get_display_event_source(display));
	//registra na fila de eventos que eu quero identificar quando o tempo alterou de t para t+1
   al_register_event_source(event_queue, al_get_timer_event_source(timer));
 

	//reinicializa a tela
   al_flip_display();
	//inicia o temporizador
   al_start_timer(timer);
 
	//loop infinito, ou seja, enquanto 1 for verdadeiro, faca:
   while(1)
   {
      ALLEGRO_EVENT ev;
	  //espera por um evento e o armazena na variavel de evento ev
      al_wait_for_event(event_queue, &ev);
 
		//se o tipo de evento for um evento do temporizador, ou seja, se o tempo passou de t para t+1
      if(ev.type == ALLEGRO_EVENT_TIMER) {
			//verifica se a posicao x do bouncer passou dos limites da tela
         if(bouncer_x < 0 || bouncer_x > SCREEN_W - BOUNCER_SIZE) {
			//altera a direcao na qual o bouncer se move no eixo x
            bouncer_dx = -bouncer_dx;
         }
 
			//verifica se a posicao y do bouncer passou dos limites da tela
         if(bouncer_y < 0 || bouncer_y > SCREEN_H - BOUNCER_SIZE) {
			//altera a direcao na qual o bouncer se move no eixo y
            bouncer_dy = -bouncer_dy;
         }
 
		//faz o bouncer se mover no eixo x e y incrementando as suas posicoes de bouncer_dx e bouncer_dy, respectivamente
         bouncer_x += bouncer_dx;
         bouncer_y += bouncer_dy;
 
         redraw = true;
      }
	  //se o tipo de evento for o fechamento da tela (clique no x da janela)
      else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
		//interrompe o while(1)
         break;
      }
 
		//se eu alterei a posicao do bouncer, o redraw foi para true e eu nao tenho eventos na fila para ler
      if(redraw && al_is_event_queue_empty(event_queue)) {

         redraw = false;
		//limpo a tela
         al_clear_to_color(al_map_rgb(0,0,0));
		//desenho o bouncer nas novas posicoes x e y
         al_draw_bitmap(bouncer, bouncer_x, bouncer_y, 0);
		//reinicializo a tela
         al_flip_display();
      }
   } //fim do while
   
	//procedimentos de fim de jogo (fecha a tela, limpa a memoria, etc)
   al_destroy_bitmap(bouncer);
   al_destroy_timer(timer);
   al_destroy_display(display);
   al_destroy_event_queue(event_queue);
 
   return 0;
}