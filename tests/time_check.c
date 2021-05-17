#include <stdlib.h>
#include <math.h>

#include "hw_interface.h"
#include "ei_utils.h"
#include "ei_draw.h"
#include "ei_types.h"
#include "ei_event.h"
#include "geometry.h"

/* test_rounded_frame --
 *
 *	Draws circle at the given position
 */
void draw_circle(ei_surface_t surface, ei_point_t pos, ei_rect_t *clipper) {
    ei_color_t color = {0, 100, 255, 255};
    ei_color_t color2 = {0, 100, 0, 255};

    ei_linked_point_t *pts = arc(&pos, 30, 0, 360, 20);
    ei_linked_point_t *end = malloc(sizeof(ei_linked_point_t));
    end->point.x = pts->point.x;
    end->point.y = pts->point.y;
    pts[19].next = end;
    end->next = NULL;

    ei_draw_polygon(surface, pts, color, clipper);
    ei_draw_polyline(surface, pts, color2, clipper);
}

/*
 * ei_main --
 *
 *	Main function of the application.
 */
int main(int argc, char **argv) {
    ei_size_t win_size = ei_size(800, 600);
    ei_surface_t main_window = NULL;
    ei_color_t white = {0xf0, 0xf0, 0xff, 0xff};
    ei_rect_t *clipper_ptr = NULL;

    ei_rect_t clipper_test;
    clipper_test.top_left.x = 0;
    clipper_test.top_left.y = 0;
    clipper_test.size.width = 800;
    clipper_test.size.height = 600;


    ei_event_t event;
    hw_init();
    main_window = hw_create_window(win_size, EI_FALSE);

    uint32_t N = 100;
    int dx = 1;
    int dy = 2;
    int x = win_size.width / 2;
    int y = win_size.height / 2;

    event.type = ei_ev_none;
    while (event.type != ei_ev_keydown) {
        //hw_event_wait_next(&event);

        /* Lock the drawing surface, paint it white. */
        hw_surface_lock(main_window);

        ei_fill(main_window, &white, clipper_ptr);


        draw_circle(main_window, ei_point(x, y), &clipper_test);


        if (y - 30 < 0) {
            dy = abs(dy);
        } else if (y + 30 > win_size.height) {
            dy = -abs(dy);
        }
        if (x - 30 < 0) {
            dx = abs(dx);
        } else if (x + 30 > win_size.width) {
            dx = -abs(dx);
        }


        x += dx;
        y += dy;

        hw_surface_unlock(main_window);
        hw_surface_update_rects(main_window, NULL);
    }
    hw_quit();
    return (EXIT_SUCCESS);
}
