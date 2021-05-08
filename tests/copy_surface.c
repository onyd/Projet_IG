#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "ei_draw.h"
#include "ei_utils.h"
#include "ei_event.h"

void test_line(ei_surface_t surface, ei_rect_t *clipper) {
    ei_color_t color = {255, 0, 255, 255};
    ei_linked_point_t pts[2];

    pts[0].point.x = 300;
    pts[0].point.y = 300;
    pts[0].next = &pts[1];
    pts[1].point.x = 700;
    pts[1].point.y = 500;
    pts[1].next = NULL;

    ei_draw_polyline(surface, pts, color, clipper);
}

int main() {
    ei_size_t		win_size	= ei_size(800, 600);
    ei_surface_t		main_window	= NULL;
    ei_color_t		color		= { 0x0, 0xff, 0xaa, 0x8a };
    ei_rect_t*		clipper_ptr	= NULL;

    ei_color_t		color2		= { 0xff, 0x0, 0xaa, 0x8a };

    hw_init();
    main_window = hw_create_window(win_size, EI_FALSE);
    hw_surface_lock	(main_window);
    //test clipper for ei_fill
    ei_rect_t clipper;
    clipper.top_left.x = 500;
    clipper.top_left.y = 400;
    clipper.size.width = 200;
    clipper.size.height = 100;

    ei_fill		(main_window, &color, &clipper);
    //test_line(main_window, clipper_ptr);

    hw_surface_unlock(main_window);
    hw_surface_update_rects(main_window, NULL);

    //Creation of the surface to copy
    ei_surface_t surface = hw_surface_create(main_window, win_size, false);
    //ei_fill		(surface, &color2, clipper_ptr);
    test_line(surface, clipper_ptr);
    // creation of the clipper
    ei_rect_t clipper_test;
    clipper_test.top_left.x = 250;
    clipper_test.top_left.y = 250;
    clipper_test.size.width = 200;
    clipper_test.size.height = 100;
    ei_copy_surface(main_window, &clipper_test, surface, &clipper, false);
    hw_surface_update_rects(main_window, NULL);

    /* Wait for a character on command line. */
    ei_event_t event;
    event.type = ei_ev_none;
    while (event.type != ei_ev_keydown)
        hw_event_wait_next(&event);
    hw_quit();
    return EXIT_SUCCESS;
}
