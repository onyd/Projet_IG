#include "widgets.h"

/* allocfunc */

/* releasefunc */

/* drawfunc */

// For the button
void draw_button(ei_widget_t*	widget,
                 ei_surface_t		surface,
                 ei_surface_t		pick_surface,
                 ei_rect_t*		clipper) {
    ei_button_t *button = (ei_button_t *) widget;
    ei_rect_t button_rect = widget->screen_location;
    ei_color_t color = button->color;
    int radius = button->corner_radius;
    // If etat is true the button is up else he is down
    ei_color_t darker = {0.1*255 + 0.9*color.red, 0.1*255 + 0.9*color.green, 0.1*255 + 0.9*color.blue, color.alpha};
    ei_color_t lighter = {0.9*color.red, 0.9*color.green, 0.9*color.blue, color.alpha};

    // The two part of the button
    ei_linked_point_t *top = rounded_frame(button_rect, radius, 10, 1);
    ei_linked_point_t *bot = rounded_frame(button_rect, radius, 10, 2);
    // The button
    ei_rect_t inside_button;
    int border_size = button->border_width;
    inside_button.top_left.x = button_rect.top_left.x + border_size;
    inside_button.top_left.y = button_rect.top_left.y + border_size;
    inside_button.size.width = button_rect.size.width - 2 * border_size;
    inside_button.size.height = button_rect.size.height - 2 * border_size;
    radius = radius - border_size;
    ei_linked_point_t *points_button = rounded_frame(inside_button, radius, 10, 0);

    if (button->relief == ei_relief_raised) {
        ei_draw_polygon(surface, top, lighter, clipper);
        ei_draw_polygon(surface, bot, darker, clipper);
    } else {
        ei_draw_polygon(surface, top, darker, clipper);
        ei_draw_polygon(surface, bot, lighter, clipper);
    }
    ei_draw_polygon(surface, points_button, color, clipper);

    free_rounded_frame(top);
    free_rounded_frame(bot);
    free_rounded_frame(points_button);
}

/* setdefaultsfunc */