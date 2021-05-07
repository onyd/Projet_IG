#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "ei_draw.h"
#include "ei_utils.h"
#include "ei_event.h"


int main() {
    ei_size_t		win_size	= ei_size(800, 600);
    ei_surface_t		main_window	= NULL;
    ei_color_t		white		= { 0x0, 0xff, 0xaa, 0x8a };
    ei_rect_t*		clipper_ptr	= NULL;
    hw_init();
    main_window = hw_create_window(win_size, EI_FALSE);
    hw_surface_lock	(main_window);
    //test clipper for ei_fill
    ei_rect_t clipper;
    clipper.top_left.x = 500;
    clipper.top_left.y = 400;
    clipper.size.width = 200;
    clipper.size.height = 100;

    ei_fill		(main_window, &white, &clipper);
    hw_surface_unlock(main_window);
    hw_surface_update_rects(main_window, NULL);

    //Creation of the surface to copy
    ei_surface_t window2 = hw_create_window(win_size, EI_FALSE);
    // creation of the clipper
    ei_rect_t clipper_test;
    clipper_test.top_left.x = 100;
    clipper_test.top_left.y = 100;
    clipper_test.size.width = 200;
    clipper_test.size.height = 100;
    ei_copy_surface(window2, &clipper_test, main_window, &clipper, true);
    hw_surface_update_rects(window2, NULL);

    /* Wait for a character on command line. */
    ei_event_t event;
    event.type = ei_ev_none;
    while (event.type != ei_ev_keydown)
        hw_event_wait_next(&event);
    hw_quit();
    return EXIT_SUCCESS;
}
