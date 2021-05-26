#include <stdlib.h>
#include <math.h>

#include "hw_interface.h"
#include "ei_utils.h"
#include "ei_draw.h"
#include "ei_types.h"
#include "ei_event.h"
#include "geometry.h"
#include "ei_application.h"
#include "defaults.h"

typedef struct ball {
    ei_point_t pos;
    ei_point_t speed;
    uint32_t size;
    ei_color_t color;
    ei_color_t outline_color;
    ei_rect_t boundaries;
    double last_move;
} ball;

/* test_rounded_frame --
 *
 *	Draws circle at the given position
 */
void draw_circle(ei_surface_t surface, ball *b, ei_rect_t *clipper) {
    ei_color_t color = {0, 100, 255, 255};
    ei_color_t color2 = {0, 100, 0, 255};

    ei_linked_point_t *pts = arc(ei_point_add(b->pos, ei_point(b->size / 2, b->size / 2)), b->size, 0, 360, 20);
    ei_draw_polygon(surface, pts, color, clipper);
    ei_draw_polyline(surface, pts, color2, clipper);
}

void move_ball(ei_event_t *event) {
    ei_frame_t *ball_frame = (ei_frame_t *) event->param.application.user_param;
    ball *b = ball_frame->widget.user_data;

    if (b->pos.y - b->size < b->boundaries.top_left.y) {
        b->speed.y = abs(b->speed.y);
    } else if (b->pos.y + b->size > b->boundaries.top_left.y + b->boundaries.size.height) {
        b->speed.y = -abs(b->speed.y);
    }
    if (b->pos.x - b->size < b->boundaries.top_left.x) {
        b->speed.x = abs(b->speed.x);
    } else if (b->pos.x + b->size > b->boundaries.top_left.x + b->boundaries.size.width) {
        b->speed.x = -abs(b->speed.x);
    }

    b->pos.x += b->speed.x;
    b->pos.y += b->speed.y;
    ei_place(ball_frame, NULL, &b->pos.x, &b->pos.y, &b->size, &b->size, NULL, NULL, NULL, NULL);

    hw_event_schedule_app(16.0 - (hw_now() * 1000 - b->last_move), ball_frame);
    b->last_move = hw_now() * 1000;
    draw_circle(get_main_window(), b, NULL);
}

/*
 * ei_main --
 *
 *	Main function of the application.
 */
int main(int argc, char **argv) {
    ei_size_t win_size = ei_size(800, 600);
    ei_surface_t main_window = NULL;
    ei_color_t bg_color = {0xa0, 0xa0, 0xa0, 0xff};
    ei_color_t ball_color = {0xf0, 0xf0, 0xff, 0xff};
    ei_color_t ball_outline_color = {0xf0, 0xf0, 0xff, 0xff};

    ei_app_create(win_size, EI_FALSE);
    ei_event_set_default_handle_func(&move_ball);

    ei_frame_configure(ei_app_root_widget(),
                       NULL,
                       &bg_color,
                       NULL,
                       NULL,
                       NULL,
                       NULL,
                       NULL,
                       NULL,
                       NULL,
                       NULL,
                       NULL);
    ball b = {ei_point(win_size.width / 2, win_size.height / 2),
              ei_point(1, 1),
              40,
              ball_color,
              ball_outline_color,
              ei_rect(ei_point_zero(), win_size),
              hw_now() * 1000};
    ei_widget_t *ball_frame = ei_widget_create("frame", ei_app_root_widget(), &b, NULL);
    ei_size_t size = ei_size(b.size, b.size);
    ei_surface_t ball_img = hw_surface_create(get_main_window(), size, EI_FALSE);
    ei_frame_configure(ball_frame,
                       &size,
                       NULL,
                       NULL,
                       NULL,
                       NULL,
                       NULL,
                       NULL,
                       NULL,
                       &ball_img,
                       NULL,
                       NULL);
    hw_event_schedule_app(16.0, ball_frame);
    ei_app_run();

    ei_app_free();
    return (EXIT_SUCCESS);
}
