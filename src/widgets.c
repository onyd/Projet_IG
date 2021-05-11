#include "widgets.h"
#include "stdlib.h"

void append_left(ei_widget_t *widget, ei_widget_list_t *l) {
    ei_linked_widget_t *linked_widget = malloc(sizeof(ei_linked_widget_t));
    linked_widget->widget = widget;
    linked_widget->next = NULL;

    // Empty
    if (l->head == NULL) {
        l->head = linked_widget;
        l->tail = linked_widget;
        return;
    }

    // 2 elements
    if (l->pre_tail == NULL && l->head != l->tail) {
        l->pre_tail = l->head;
    }

    linked_widget->next = l->head;
    l->head = linked_widget;
}

void append(ei_widget_t *widget, ei_widget_list_t *l) {
    ei_linked_widget_t *linked_widget = malloc(sizeof(ei_linked_widget_t));
    linked_widget->widget = widget;
    linked_widget->next = NULL;

    // Empty
    if (l->head == NULL) {
        l->head = linked_widget;
        l->tail = linked_widget;
        return;
    }

    // More than 1 elements
    if (l->tail != l->head) {
        l->pre_tail = l->tail;
    }

    l->tail->next = linked_widget;
    l->tail = linked_widget;
}

ei_linked_widget_t *pop(ei_widget_list_t *l) {
    // Empty
    if (l->head == NULL) {
        return NULL;
    }

    // 1 or 2 elements
    if (l->pre_tail == NULL) {
        // 1
        if (l->tail == l->head) {
            ei_linked_widget_t *popped = l->tail;
            l->head = NULL;
            l->tail = NULL;
            return popped;
        } else {
            ei_linked_widget_t *popped = l->tail;
            l->tail = l->head;
            return popped;
        }
    }

    // More
    ei_linked_widget_t *popped = l->tail;
    l->tail = l->pre_tail;
    l->tail->next = NULL;

    return popped;
}

void free_linked_widget(ei_linked_widget_t *start) {
    ei_linked_widget_t *current = start;
    while (current != NULL) {
        ei_linked_widget_t *tmp = current;
        current = current->next;
        free(tmp);
    }
}

void widget_breadth_list(ei_widget_t *start, ei_widget_list_t *result) {
    ei_widget_list_t to_see;
    to_see.tail = NULL;
    to_see.head = NULL;
    to_see.pre_tail = NULL;

    append_left(start, &to_see);

    ei_linked_widget_t *current;
    while (to_see.head != NULL) {
        current = pop(&to_see);
        append(current, result);

        // Add children for next children stage
        ei_widget_t *children = current->widget->children_head;
        while (children->next_sibling != NULL) {
            ei_linked_widget_t *linked = malloc(sizeof(ei_linked_widget_t));
            linked->widget = children;
            linked->next = NULL;
            append_left(linked, &to_see);

            children = children->next_sibling;
        }
    }
}

/* allocfunc */
ei_widget_t *widget_allocfunc() {
    ei_widget_t *widget = calloc(1, sizeof(ei_widget_t));
    return widget;
}

ei_widget_t *button_allocfunc() {
    ei_widget_t *widget = (ei_button_t *) calloc(1, sizeof(ei_button_t));
    return widget;
}

ei_widget_t *frame_allocfunc() {
    ei_widget_t *widget = (ei_frame_t *) calloc(1, sizeof(ei_frame_t));
    return widget;
}

/* releasefunc */
void widget_releasefunc(struct ei_widget_t *widget) {
    free(widget);
}

void button_releasefunc(struct ei_widget_t *widget) {
    free((ei_button_t *) widget);
}

void frame_releasefunc(struct ei_widget_t *widget) {
    free((ei_frame_t *) widget);
}

/* drawfunc */
void widget_drawfunc(ei_widget_t *widget,
                     ei_surface_t surface,
                     ei_surface_t pick_surface,
                     ei_rect_t *clipper) {}

// For the button
void button_drawfunc(ei_widget_t *widget,
                     ei_surface_t surface,
                     ei_surface_t pick_surface,
                     ei_rect_t *clipper) {
    ei_button_t *button = (ei_button_t *) widget;
    ei_rect_t button_rect = widget->screen_location;
    ei_color_t color = *button->color;
    int radius = *button->corner_radius;
    // If etat is true the button is up else he is down
    ei_color_t darker = {0.1 * 255 + 0.9 * color.red, 0.1 * 255 + 0.9 * color.green, 0.1 * 255 + 0.9 * color.blue,
                         color.alpha};
    ei_color_t lighter = {0.9 * color.red, 0.9 * color.green, 0.9 * color.blue, color.alpha};

    // The two part of the button
    ei_linked_point_t *top = rounded_frame(button_rect, radius, 10, 1);
    ei_linked_point_t *bot = rounded_frame(button_rect, radius, 10, 2);
    // The button
    ei_rect_t inside_button;
    int border_size = *button->border_width;
    inside_button.top_left.x = button_rect.top_left.x + border_size;
    inside_button.top_left.y = button_rect.top_left.y + border_size;
    inside_button.size.width = button_rect.size.width - 2 * border_size;
    inside_button.size.height = button_rect.size.height - 2 * border_size;
    radius = radius - border_size;
    ei_linked_point_t *points_button = rounded_frame(inside_button, radius, 10, 0);

    if (*button->relief == ei_relief_raised) {
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

void frame_drawfunc(ei_widget_t *widget,
                    ei_surface_t surface,
                    ei_surface_t pick_surface,
                    ei_rect_t *clipper) {
    ei_frame_t *frame = (ei_frame_t *) widget;
    ei_rect_t frame_rect = widget->screen_location;
    ei_color_t color = *frame->color;
    ei_color_t darker = {0.1 * 255 + 0.9 * color.red, 0.1 * 255 + 0.9 * color.green, 0.1 * 255 + 0.9 * color.blue,
                         color.alpha};
    ei_color_t lighter = {0.9 * color.red, 0.9 * color.green, 0.9 * color.blue, color.alpha};
    ei_rect_t inside_frame;
    int border_size = *frame->border_width;
    inside_frame.top_left.x = frame_rect.top_left.x + border_size;
    inside_frame.top_left.y = frame_rect.top_left.y + border_size;
    inside_frame.size.width = frame_rect.size.width - 2 * border_size;
    inside_frame.size.height = frame_rect.size.height - 2 * border_size;
    if (*frame->relief == ei_relief_none) {
        draw_rectangle(surface, frame_rect, color, clipper);
    } else {
        if (*frame->relief == ei_relief_raised) {
            rect_to_triangle(surface, frame_rect, lighter, clipper, 0);
            rect_to_triangle(surface, frame_rect, darker, clipper, 1);
            draw_rectangle(surface, inside_frame, color, clipper);
        } else {
            rect_to_triangle(surface, frame_rect, darker, clipper, 0);
            rect_to_triangle(surface, frame_rect, lighter, clipper, 1);
            draw_rectangle(surface, inside_frame, color, clipper);
        }
    }
}

/* setdefaultsfunc */

void widget_setdefaultsfunc(ei_widget_t *widget) {
}

void button_setdefaultsfunc(ei_widget_t *widget) {
    ei_button_configure(widget,
                        default_size,
                        default_color,
                        &k_default_button_border_width,
                        &k_default_button_corner_radius,
                        default_relief,
                        NULL,
                        &ei_default_font,
                        default_text_color,
                        NULL,
                        NULL,
                        NULL,
                        NULL,
                        NULL,
                        NULL
    );
}

void frame_setdefaultsfunc(ei_widget_t *widget) {
    ei_frame_configure(widget,
                       default_size,
                       default_color,
                       &k_default_button_border_width,
                       default_relief,
                       NULL,
                       &ei_default_font,
                       default_text_color,
                       NULL,
                       NULL,
                       NULL,
                       NULL
    );
}

/* geomnotifyfunc */

void widget_geomnotifyfunc(ei_widget_t *widget, ei_rect_t rect) {

}

void button_geomnotifyfunc(ei_widget_t *widget, ei_rect_t rect) {

}

void frame_geomnotifyfunc(ei_widget_t *widget, ei_rect_t rect) {

}

/* handlefunc */

ei_bool_t widget_handlefunc(ei_widget_t *widget, struct ei_event_t *event) {

}

ei_bool_t button_handlefunc(ei_widget_t *widget, struct ei_event_t *event) {

}

ei_bool_t frame_handlefunc(ei_widget_t *widget, struct ei_event_t *event) {

}


