#include <stdio.h>
#include <allegro5/allegro.h>
#include "mandelbrot.h"

void paint(ALLEGRO_BITMAP *bitmap, int width, int height, double xmin, double xmax, double ymin, double ymax)
{
    ALLEGRO_LOCKED_REGION *region = al_lock_bitmap(bitmap, ALLEGRO_PIXEL_FORMAT_ANY, ALLEGRO_LOCK_READWRITE);
    unsigned int *pPixelBuffer = (unsigned int*) region->data;
    pPixelBuffer -= width * (height-1);
    
    mandelbrot(pPixelBuffer, width, height, xmin, xmax, ymin, ymax);
    
    al_unlock_bitmap(bitmap);
    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_draw_bitmap(bitmap, 0, 0, 0);
    al_flip_display();
}

int main()
{
  ALLEGRO_BITMAP *bitmap;
  ALLEGRO_DISPLAY *display;
  ALLEGRO_EVENT_QUEUE *queue;
  int width = 1000;
  int height = 1000;
  double zoom = 2.0;
  double xmin = -2.0;
  double xmax = 2.0;
  double ymin = -2.0;
  double ymax = 2.0;
  double tmp, tmp2;
  if(!al_init())
  {
    fprintf(stderr, "failed to initialize allegro!\n");
    return -1;
  }
  
  al_set_app_name("Mandelbrot");
  
  if(!al_install_keyboard())
  {
    fprintf(stderr, "failed to install keyboard\n");
    return -1;
  }
  
  if(!al_install_mouse())
  {
    fprintf(stderr, "failed to install mouse!\n");
    return -1;
  }
  
  bitmap = al_create_bitmap(width, height);
  
  if (!bitmap)
  {
    fprintf(stderr, "failed to initialize bitmap!\n");
    return -1;
  }

  display = al_create_display(width, height);
    
  if(!display)
  {
    fprintf(stderr, "failed to create display!\n");
    al_destroy_bitmap(bitmap);
    return -1;
  }

  queue = al_create_event_queue();
  
  if(!queue)
  {
    fprintf(stderr, "failed to create event_queue!\n");
    al_destroy_bitmap(bitmap);
    al_destroy_display(display);
    return -1;
  }
  
  al_register_event_source(queue, al_get_keyboard_event_source());
  al_register_event_source(queue, al_get_mouse_event_source());
  al_register_event_source(queue, al_get_display_event_source(display));
  al_set_target_bitmap(al_get_backbuffer(display));
  paint(bitmap, width, height, xmin, xmax, ymin, ymax);
  for(;;)
  {
    ALLEGRO_EVENT event;
    al_wait_for_event(queue, &event);
    if(event.type == ALLEGRO_EVENT_KEY_DOWN && event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
    {
      al_destroy_bitmap(bitmap);
      al_destroy_display(display);
      al_destroy_event_queue(queue);
      break;
    }
    else if(event.type == ALLEGRO_EVENT_KEY_DOWN && event.keyboard.keycode == ALLEGRO_KEY_SPACE)
    {
      xmin = -2;
      xmax = 2;
      ymin = -2;
      ymax = 2;
      printf("Enter width, height and zoom:\n");
      scanf("%d%d%lf", &width, &height, &zoom);
      al_destroy_bitmap(bitmap);
      bitmap = al_create_bitmap(width, height);
      if (!bitmap)
      {
        fprintf(stderr, "failed to initialize bitmap!\n");
        al_destroy_event_queue(queue);
        return -1;
      }
      al_resize_display(display, width, height);
      paint(bitmap, width, height, xmin, xmax, ymin, ymax);
    }
    else if(event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
    {
      ALLEGRO_MOUSE_STATE button;
      al_get_mouse_state(&button);
		  if(button.buttons & 1)
      {
    	 	tmp = xmin;
        tmp2 = xmax;
      	xmin = ((tmp2 - tmp) * event.mouse.x/width + tmp) - (tmp2 - tmp)/(2 * zoom);
	    	xmax = ((tmp2 - tmp) * event.mouse.x/width + tmp) + (tmp2 - tmp)/(2 * zoom);
        tmp = ymin;
        tmp2 = ymax;
	    	ymin = ((tmp2 - tmp) * (height - event.mouse.y)/height + tmp) - (tmp2 - tmp)/(2 * zoom);
	    	ymax = ((tmp2 - tmp) * (height - event.mouse.y)/height + tmp) + (tmp2 - tmp)/(2 * zoom);
        paint(bitmap, width, height, xmin, xmax, ymin, ymax);
      }
      else
      {
        tmp = xmin;
        tmp2 = xmax;
      	xmin = ((tmp2 - tmp) * event.mouse.x/width + tmp) - (tmp2 - tmp)*zoom/2;
	    	xmax = ((tmp2 - tmp) * event.mouse.x/width + tmp) + (tmp2 - tmp)*zoom/2;
        tmp = ymin;
        tmp2 = ymax;
	    	ymin = ((tmp2 - tmp) * (height - event.mouse.y)/height + tmp) - (tmp2 - tmp)*zoom/2;
	    	ymax = ((tmp2 - tmp) * (height - event.mouse.y)/height + tmp) + (tmp2 - tmp)*zoom/2;
        paint(bitmap, width, height, xmin, xmax, ymin, ymax);
      }
      printf("xmin: %lf, xmax: %lf, ymin: %lf, ymax: %lf\n", xmin, xmax, ymin, ymax);
    }
  }
  
  return 0;
}
