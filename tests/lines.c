#include <stdlib.h>
#include <math.h>

#include "hw_interface.h"
#include "ei_utils.h"
#include "ei_draw.h"
#include "ei_types.h"
#include "ei_event.h"
#include "geometry.h"
#include <time.h>

#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) < (b)) ? (a) : (b))


/* test_line --
 *
 *	Draws a simple line in the canonical octant, that is, x1>x0 and y1>y0, with
 *	dx > dy. This can be used to test a first implementation of Bresenham
 *	algorithm, for instance.
 */
void test_line(ei_surface_t surface, ei_rect_t *clipper) {
    ei_color_t color = {255, 0, 255, 255};
    ei_linked_point_t pts[2];

    pts[0].point.x = 200;
    pts[0].point.y = 200;
    pts[0].next = &pts[1];
    pts[1].point.x = 600;
    pts[1].point.y = 400;
    pts[1].next = NULL;

    ei_draw_polyline(surface, pts, color, clipper);
}


/* test_octogone --
 *
 *	Draws an octogone in the middle of the screen. This is meant to test the
 *	algorithm that draws a polyline in each of the possible octants, that is,
 *	in each quadrant with dx>dy (canonical) and dy>dx (steep).
 */
void test_octogone(ei_surface_t surface, ei_rect_t *clipper) {
    ei_color_t color = {0, 255, 0, 255};
    ei_linked_point_t pts[9];
    int i, xdiff, ydiff;

    /* Initialisation */
    pts[0].point.x = 400;
    pts[0].point.y = 90;

    /* Draw the octogone */
    for (i = 1; i <= 8; i++) {
        /*	Add or remove 70/140 pixels for next point
           The first term of this formula gives the sign + or - of the operation
           The second term is 2 or 1, according to which coordinate grows faster
           The third term is simply the amount of pixels to skip */
        xdiff = pow(-1, (i + 1) / 4) * pow(2, (i / 2) % 2 == 0) * 70;
        ydiff = pow(-1, (i - 1) / 4) * pow(2, (i / 2) % 2) * 70;

        pts[i].point.x = pts[i - 1].point.x + xdiff;
        pts[i].point.y = pts[i - 1].point.y + ydiff;
        pts[i - 1].next = &(pts[i]);
    }

    /* End the linked list */
    pts[i - 1].next = NULL;

    /* Draw the form with polylines */
    ei_draw_polyline(surface, pts, color, clipper);
}

void test_polygone(ei_surface_t surface, ei_rect_t *clipper) {
    ei_color_t color = {0, 255, 0, 255};
    ei_linked_point_t pts[9];
    int i, xdiff, ydiff;

    /* Initialisation */
    pts[0].point.x = 400;
    pts[0].point.y = 90;

    /* Draw the polygone */
    for (i = 1; i <= 8; i++) {
        /*	Add or remove 70/140 pixels for next point
           The first term of this formula gives the sign + or - of the operation
           The second term is 2 or 1, according to which coordinate grows faster
           The third term is simply the amount of pixels to skip */
        xdiff = pow(-1, (i + 1) / 4) * pow(2, (i / 2) % 2 == 0) * 70;
        ydiff = pow(-1, (i - 1) / 4) * pow(2, (i / 2) % 2) * 70;

        pts[i].point.x = pts[i - 1].point.x + xdiff;
        pts[i].point.y = pts[i - 1].point.y + ydiff;
        pts[i - 1].next = &(pts[i]);
    }

    /* End the linked list */
    pts[i - 1].next = NULL;

    /* Draw the form with polylines */

    ei_draw_polygon(surface, pts, color, clipper);
    ei_color_t cl = {0, 0, 255, 255};
    ei_draw_polyline(surface, pts, cl, clipper);

}

/* test_square --
 *
 *	Draws a square in the middle of the screen. This is meant to test the
 *	algorithm for the special cases of horizontal and vertical lines, where
 *	dx or dy are zero
 */
void test_square(ei_surface_t surface, ei_rect_t *clipper) {
    ei_color_t color = {255, 0, 0, 255};
    ei_linked_point_t pts[5];
    int i, xdiff, ydiff;

    /* Initialisation */
    pts[0].point.x = 300;
    pts[0].point.y = 400;

    /* Draw the square */
    for (i = 1; i <= 4; i++) {
        /*	Add or remove 200 pixels or 0 for next point
            The first term of this formula gives the sign + or - of the operation
            The second term is 0 or 1, according to which coordinate grows
            The third term is simply the side of the square */
        xdiff = pow(-1, i / 2) * (i % 2) * 200;
        ydiff = pow(-1, i / 2) * (i % 2 == 0) * 200;

        pts[i].point.x = pts[i - 1].point.x + xdiff;
        pts[i].point.y = pts[i - 1].point.y + ydiff;
        pts[i - 1].next = &(pts[i]);
    }

    /* End the linked list */
    pts[i - 1].next = NULL;

    /* Draw the form with polygon */
    ei_draw_polygon(surface, pts, color, clipper);
}


/* test_dot --
 *
 *	Draws a dot in the middle of the screen. This is meant to test the special
 *	case when dx = dy = 0
 */
void test_dot(ei_surface_t surface, ei_rect_t *clipper) {
    ei_color_t color = {0, 0, 0, 255};
    ei_linked_point_t pts[3];

    pts[0].point.x = 400;
    pts[0].point.y = 300;
    pts[0].next = &(pts[1]);
    pts[1].point.x = 400;
    pts[1].point.y = 300;
    pts[1].next = NULL;

    ei_draw_polyline(surface, pts, color, clipper);
}

/* test_ei_draw_text --
 *
 *	Draws text at the center of the window with default font
 */
void test_ei_draw_text(ei_surface_t surface, ei_rect_t *clipper) {
    ei_point_t where;
    where.x = 400;
    where.y = 400;
    char *text = "afflict";
    ei_color_t color = {255, 0, 255, 255};
    ei_font_t default_font;
    default_font = hw_text_font_create(ei_default_font_filename, ei_style_normal, ei_font_default_size);
    ei_draw_text(surface, &where, text, default_font, color, clipper);
}

/* test_rounded_frame --
 *
 *	Draws square with rounded corners at the center of the window
 */

void test_rounded_frame(ei_surface_t surface, ei_rect_t* clipper) {
    ei_color_t color = { 0, 100, 255, 255 };
    ei_color_t color2 = { 0, 100, 0, 255 };
    ei_rect_t rect = ei_rect(ei_point(200, 200), ei_size(450, 350));
    ei_linked_point_t *pts = rounded_frame(rect, 50, 30, 0);

    ei_draw_polygon(surface, pts, color, clipper);
    ei_draw_polyline(surface, pts, color2, clipper);
}

void test_draw_button(ei_surface_t surface, ei_rect_t* clipper, bool etat) {
    ei_color_t color = {83, 200, 150, 255};
    ei_rect_t rect = ei_rect(ei_point(100, 100), ei_size(200, 100));
    draw_button(surface, clipper, rect, color, 25, etat);
}

/* test_rounded_frame --
 *
 *	Draws random polygon with N points
 */

void test_random_polygon(ei_surface_t surface, uint32_t N, ei_rect_t *clipper) {
    ei_color_t color = {0, 255, 0, 255};
    ei_color_t color2 = {0, 0, 255, 255};
    ei_size_t size = hw_surface_get_size(surface);
    ei_linked_point_t *pts = calloc(N, sizeof(ei_linked_point_t));
    int i;


    /* Initialisation */

    int max_r = min(size.width, size.height) / 2 ;
    int da = 360 / N;
    float a = 0;
    float angle = a + rand() % da;
    int r = rand() % max_r;
    pts[0].point.x = size.width / 2 + r * cos(angle * (3.14f / 180.0f));
    pts[0].point.y = size.height / 2 - r * sin(angle * (3.14f / 180.0f));

    /* Draw the square */

    for (i = 1; i < N; i++) {
        a += da;

        angle = a + rand() % da;
        r = rand() % max_r;
        pts[i].point.x = size.width / 2 + r * cos(angle * (3.14f / 180.0f));
        pts[i].point.y = size.height / 2 - r * sin(angle * (3.14f / 180.0f));

        pts[i - 1].next = &(pts[i]);
    }

    /* End the linked list */

    ei_linked_point_t end;
    end.point = ei_point(pts[0].point.x, pts[0].point.y);
    end.next = NULL;
    pts[i - 1].next = &end;

    ei_draw_polygon(surface, pts, color, clipper);
    ei_draw_polyline(surface, pts, color2, clipper);
    free(pts);
}

/*
 * ei_main --
 *
 *	Main function of the application.
 */
int main(int argc, char **argv) {
    srand(time(NULL));

    ei_size_t win_size = ei_size(800, 600);
    ei_surface_t main_window = NULL;
    ei_color_t white = {0xf0, 0xf0, 0xff, 0xff};
    ei_rect_t *clipper_ptr = NULL;

    ei_rect_t clipper_test;
    clipper_test.top_left.x = 0;
    clipper_test.top_left.y = 0;
    clipper_test.size.width = 400;
    clipper_test.size.height = 600;

//	ei_rect_t		clipper		= ei_rect(ei_point(200, 150), ei_size(400, 300));
//	clipper_ptr		= &clipper;
    ei_event_t event;

    hw_init();

    main_window = hw_create_window(win_size, EI_FALSE);

    /* Lock the drawing surface, paint it white. */
    hw_surface_lock(main_window);
    ei_fill(main_window, &white, clipper_ptr);

    /* Draw polylines. */
//    test_line(main_window, clipper_ptr);
//	test_octogone	(main_window, clipper_ptr);
//    test_octogone(main_window, &clipper_test);
//	test_square	(main_window, clipper_ptr);
//	test_dot	(main_window, clipper_ptr);
//  test_polygone(main_window, clipper_ptr);
//    test_polygone(main_window, &clipper_test);
//    test_random_polygon(main_window, 10, clipper_ptr);

    /* Rounded polygon */
//    test_rounded_frame(main_window, clipper_ptr);
    // test button
    /*
    hw_surface_unlock(main_window);
    bool etat = true;
    test_draw_button(main_window, clipper_ptr, etat);
    hw_surface_update_rects(main_window, NULL);
    event.type = ei_ev_none;
    while (event.type != ei_ev_mouse_buttondown)
        hw_event_wait_next(&event);
    etat = !etat;
    hw_surface_lock(main_window);
    test_draw_button(main_window, clipper_ptr, etat);
    hw_surface_unlock(main_window);
    hw_surface_update_rects(main_window, NULL);
     */
    /* Draw text. */
//    test_ei_draw_text(main_window, clipper_ptr);
    /* Unlock and update the surface. */
    hw_surface_unlock(main_window);
    hw_surface_update_rects(main_window, NULL);

    /* Wait for a character on command line. */
    event.type = ei_ev_none;
    while (event.type != ei_ev_keydown)
        hw_event_wait_next(&event);

    hw_quit();
    return (EXIT_SUCCESS);
}
