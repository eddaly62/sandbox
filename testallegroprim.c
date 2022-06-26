// testallegroprim.c


#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <errno.h>

#include "stdafx.h"
#include "allegro.h"
#include <allegro5allegro_primitives.h>

int main()  {
    ALLEGRO_DISPLAY * display;
    al_init();

    display = al_create_display(640, 480);
    al_init_primitives_addon();
    float points[8] = {0.0f, 0.0f, 100.00f, 100.00f, 200.00f, 100.00f, 640.00f, 150.00f};
    float polygon[8] = { 640.0f, 100.0f, 640.0f, 300.0f, 380.0f, 350.0f, 200.0f, 200.0f };

    bool running = true;

    while (running) {
        al_draw_line(0, 0, al_get_display_width(display), al_get_display_height(display), al_map_rgb(255, 0, 0),5.0);
        al_draw_rectangle(100, 100, 300, 300, al_map_rgb(0, 255, 0), 1);
        al_draw_ellipse(300, 300, 120, 50, al_map_rgb(0, 0, 255), 3);
        al_draw_spline(points, al_map_rgb(128, 128, 0), 8);

        //al_draw_polygon(polygon, 8, ALLEGRO_LINE_JOIN_BEVEL, al_map_rgb(255, 15, 15),3,1);
        al_draw_filled_polygon(polygon, 8, al_map_rgb(255, 0, 0));
        al_flip_display();
        al_destroy_display(display);
    }

    return 0;
}