#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "ei_draw.h"
#include "ei_utils.h"
#include "ei_event.h"


int main() {
    ei_size_t		win_size	= ei_size(800, 600);
    ei_surface_t		main_window	= NULL;
    ei_color_t		white		= { 0xf0, 0xf0, 0xff, 0xff };
    ei_rect_t*		clipper_ptr	= NULL;
    hw_init();
    main_window = hw_create_window(win_size, EI_FALSE);
    hw_surface_lock	(main_window);
    ei_fill		(main_window, &white, clipper_ptr);
    hw_surface_unlock(main_window);
    hw_surface_update_rects(main_window, NULL);
    ei_surface_t window2 = hw_create_window(win_size, EI_FALSE);
    ei_copy_surface(window2, NULL, main_window, NULL, false);
    hw_surface_update_rects(window2, NULL);

    /* Wait for a character on command line. */
    ei_event_t event;
    event.type = ei_ev_none;
    while (event.type != ei_ev_keydown)
        hw_event_wait_next(&event);
    hw_quit();
    return EXIT_SUCCESS;
}
