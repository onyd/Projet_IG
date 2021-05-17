#include "widgets.h"
#include "stdlib.h"
#include "utils.h"
#include "hw_interface.h"

// Class declarations
ei_widgetclass_t *frame_class;
ei_widgetclass_t *button_class;
ei_widgetclass_t *widget_class;
ei_widgetclass_t *toplevel_class;

// Default declaration
ei_surface_t main_window;
ei_frame_t *root;
ei_surface_t *picking_offscreen;

ei_size_t *toplevel_default_size;
ei_size_t *toplevel_default_min_size;
int *toplevel_default_border_width;
ei_color_t *default_color;
ei_color_t *default_text_color;
ei_size_t *default_size;
ei_relief_t *default_relief;
ei_anchor_t *default_anchor;

bool quit_request;


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

    append_linked(linked_widget, l);
}

void append_linked(ei_linked_widget_t *e, ei_widget_list_t *l) {
    // Empty
    if (l->head == NULL) {
        l->head = e;
        l->tail = e;
        return;
    }

    // More than 1 element
    if (l->tail != l->head) {
        l->pre_tail = l->tail;
    }

    l->tail->next = e;
    l->tail = e;
}

void move(ei_widget_list_t *src, ei_widget_list_t *dst) {
    // Empty
    if (src->head == NULL) {
        return;
    }
    append_linked(src->tail, dst);
    // 1 or 2 elements
    if (src->pre_tail == NULL) {
        // 1
        if (src->tail == src->head) {
            src->head = NULL;
            src->tail = NULL;
            return;
        } else { // 2
            src->tail = src->head;
            src->tail->next = NULL;
            return;
        }
    }
    // More
    src->tail = src->pre_tail;
    src->tail->next = NULL;
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
    ei_linked_widget_t *linked_start = malloc(sizeof(ei_linked_widget_t));
    linked_start->widget = start;
    linked_start->next = NULL;
    ei_widget_list_t to_see = {linked_start, NULL, linked_start};

    ei_linked_widget_t *current;
    while (to_see.head != NULL) {
        move(&to_see, result);

        // Add children for next children stage
        ei_widget_t *children = result->tail->widget->children_head;
        if (children != NULL) {
            while (children != NULL) {
                append_left(children, &to_see);
                children = children->next_sibling;
            }
        }
    }
    result->tail->next = NULL;
}

/* allocfunc */
ei_widget_t *widget_allocfunc() {
    ei_widget_t *widget = calloc(1, sizeof(ei_widget_t));
    return widget;
}

ei_widget_t *button_allocfunc() {
    ei_widget_t *widget = (ei_button_t *) calloc(1, sizeof(ei_button_t));
    ei_button_t *button = (ei_button_t *) widget;
    button->img_rect = calloc(1, sizeof(ei_rect_t *));
    return widget;
}

ei_widget_t *frame_allocfunc() {
    ei_widget_t *widget = (ei_frame_t *) calloc(1, sizeof(ei_frame_t));
    ei_frame_t *frame = (ei_frame_t *) widget;
    frame->img_rect = calloc(1, sizeof(ei_rect_t *));
    return widget;
}

ei_widget_t *toplevel_allocfunc() {
    ei_widget_t *widget = (ei_toplevel_t *) calloc(1, sizeof(ei_toplevel_t));
    ei_toplevel_t *toplevel = (ei_toplevel_t *) widget;
    widget->content_rect = calloc(1, sizeof(ei_rect_t));
    toplevel->button->widget.wclass->allocfunc();

    return widget;
}

/* releasefunc */
void widget_releasefunc(struct ei_widget_t *widget) {
    free(widget);
}

void button_releasefunc(ei_widget_t *widget) {
    ei_button_t *to_release = (ei_button_t *) widget;
    if (to_release->text != NULL) {
        free(to_release->text);
    }
    if (to_release->img_rect != NULL) {
        free(to_release->img_rect);
    }
}

void frame_releasefunc(ei_widget_t *widget) {
    ei_frame_t *to_release = (ei_frame_t *) widget;
    if (to_release->text != NULL) {
        free(to_release->text);
    }
    if (to_release->img_rect != NULL) {
        free(to_release->img_rect);
    }
}

void toplevel_releasefunc(ei_widget_t *widget) {
    ei_toplevel_t *to_release = (ei_toplevel_t *) widget;
    if (to_release->title != NULL) {
        free(to_release->title);
    }
    if (widget->content_rect != NULL) {
        free(widget->content_rect);
    }
    to_release->button->widget.wclass->releasefunc(to_release->button);
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
    ei_color_t color = button->color;
    int radius = button->corner_radius;
    // If etat is true the button is up else he is down
    ei_color_t darker = {0.1 * 255 + 0.9 * color.red, 0.1 * 255 + 0.9 * color.green, 0.1 * 255 + 0.9 * color.blue,
                         color.alpha};
    ei_color_t lighter = {0.9 * color.red, 0.9 * color.green, 0.9 * color.blue, color.alpha};

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

    //ei_draw_polygon(pick_surface, points_button, *(widget->pick_color), clipper);
    //ei_draw_polygon(pick_surface, top, *(widget->pick_color), clipper);
    //ei_draw_polygon(pick_surface, bot, *(widget->pick_color), clipper);

    free_rounded_frame(top);
    free_rounded_frame(bot);
    free_rounded_frame(points_button);

    // Text eventually inside the button
    if (button->text != NULL) {
        int width, height;
        hw_text_compute_size(button->text, button->text_font, &width, &height);
        ei_point_t topleft = anchor_target_pos(button->text_anchor, ei_size(width, height), button_rect);
        ei_rect_t clipper_text;
        // We crop text in the button
        intersection(&button_rect, clipper, &clipper_text);
        ei_draw_text(surface, &topleft, button->text, button->text_font, button->text_color, &clipper_text);
    }
    // Image eventually inside the button
    if (button->img != NULL) {
        ei_size_t img_size = (button->img_rect != NULL) ? (*button->img_rect)->size : hw_surface_get_size(button->img);
        ei_point_t topleft = anchor_target_pos(button->text_anchor, img_size, button_rect);
        ei_rect_t clipper_img;
        // We crop image in the button
        intersection(&button_rect, clipper, &clipper_img);
        draw_image(surface, button->img, &topleft, *button->img_rect, &clipper_img);
    }
}

void frame_drawfunc(ei_widget_t *widget,
                    ei_surface_t surface,
                    ei_surface_t pick_surface,
                    ei_rect_t *clipper) {
    ei_frame_t *frame = (ei_frame_t *) widget;
    ei_rect_t frame_rect = widget->screen_location;
    ei_color_t color = frame->color;
    ei_color_t darker = {0.1 * 255 + 0.9 * color.red, 0.1 * 255 + 0.9 * color.green, 0.1 * 255 + 0.9 * color.blue,
                         color.alpha};
    ei_color_t lighter = {0.9 * color.red, 0.9 * color.green, 0.9 * color.blue, color.alpha};
    ei_rect_t inside_frame;
    int border_size = frame->border_width;
    inside_frame.top_left.x = frame_rect.top_left.x + border_size;
    inside_frame.top_left.y = frame_rect.top_left.y + border_size;
    inside_frame.size.width = frame_rect.size.width - 2 * border_size;
    inside_frame.size.height = frame_rect.size.height - 2 * border_size;
    if (frame->relief == ei_relief_none) {
        draw_rectangle(surface, frame_rect, color, clipper);
    } else {
        if (frame->relief == ei_relief_raised) {
            draw_rect_triangle(surface, frame_rect, lighter, clipper, 0);
            draw_rect_triangle(surface, frame_rect, darker, clipper, 1);
            draw_rectangle(surface, inside_frame, color, clipper);
        } else {
            draw_rect_triangle(surface, frame_rect, darker, clipper, 0);
            draw_rect_triangle(surface, frame_rect, lighter, clipper, 1);
            draw_rectangle(surface, inside_frame, color, clipper);
        }
    }
    //draw_rectangle(pick_surface, frame_rect, *(widget->pick_color), clipper);
    // Text eventually inside the frame
    if (frame->text != NULL) {
        int width, height;
        hw_text_compute_size(frame->text, frame->text_font, &width, &height);
        ei_point_t topleft = anchor_target_pos(frame->text_anchor, ei_size(width, height), frame_rect);
        ei_rect_t clipper_text;
        //in case the clipper is NULL, clipper_text must be button_rect to avoid having the text outside the button
        intersection(&frame_rect, clipper, &clipper_text);
        ei_draw_text(surface, &topleft, frame->text, frame->text_font, frame->text_color, &clipper_text);
    }
    // Image eventually inside the frame
    if (frame->img != NULL) {
        ei_size_t img_size = (*frame->img_rect != NULL) ? (*frame->img_rect)->size : hw_surface_get_size(frame->img);
        ei_point_t topleft = anchor_target_pos(frame->text_anchor, img_size, frame_rect);
        ei_rect_t clipper_img;
        //in case the clipper is NULL, clipper_text must be button_rect to avoid having the text outside the button
        intersection(&frame_rect, clipper, &clipper_img);
        draw_image(surface, frame->img, &topleft, *frame->img_rect, &clipper_img);
    }
}

void toplevel_drawfunc(ei_widget_t *widget,
                       ei_surface_t surface,
                       ei_surface_t pick_surface,
                       ei_rect_t *clipper) {
    ei_toplevel_t *toplevel = (ei_toplevel_t *) widget;
    ei_rect_t inside_toplevel_rect = *widget->content_rect;
    ei_rect_t toplevel_rect = widget->screen_location;
    ei_color_t color = toplevel->color;
    char *title = toplevel->title;

    //Draw all the top level
    draw_rectangle(surface, toplevel_rect, *default_color, clipper);

    //Draw the toplevel without border and top bar
    draw_rectangle(surface, inside_toplevel_rect, color, clipper);

    ei_rect_t clipper_text;
    intersection(&toplevel_rect, clipper, &clipper_text);
    if (toplevel->closable == EI_FALSE) {
        ei_point_t point_text = ei_point(toplevel_rect.top_left.x + toplevel->border_width,
                                         toplevel_rect.top_left.y + toplevel->border_width);
        ei_draw_text(surface, &point_text, title, ei_default_font, toplevel->color, &clipper_text);
    }
    else {
        ei_widget_t *button_widget = ei_widget_create("button", widget, NULL, NULL);
        ei_button_t *button = (ei_button_t *) button_widget;
        int button_width = 2;
        ei_size_t size = ei_size(20, 20);
        int corner_size = 10;
        ei_color_t color_button = {255, 0, 0, 255};
        ei_button_configure(button_widget ,&size, &color_button, &button_width,
                            &corner_size, NULL, NULL, NULL, NULL,
                            NULL, NULL, NULL, NULL, NULL, NULL);
        ei_point_t point_button = ei_point(toplevel_rect.top_left.x + toplevel->border_width,
                                         toplevel_rect.top_left.y + toplevel->border_width);
        ei_point_t point_text = ei_point(toplevel_rect.top_left.x + 2*toplevel->border_width + 20,
                                           toplevel_rect.top_left.y + toplevel->border_width);
        button_widget->screen_location.top_left = point_button;
        ei_draw_text(surface, &point_text, title, ei_default_font, toplevel->color, &clipper_text);
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
                        default_anchor,
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
                       ei_relief_none,
                       NULL,
                       &ei_default_font,
                       default_text_color,
                       NULL,
                       NULL,
                       NULL,
                       NULL
    );
}

void toplevel_setdefaultsfunc(ei_widget_t *widget) {
    ei_color_t background = {0xA0, 0xA0, 0xA0, 0xff};
    char *title = "Toplevel";
    ei_bool_t default_closable = EI_TRUE;
    ei_axis_set_t default_resizable = ei_axis_both;
    ei_toplevel_configure(widget,
                          toplevel_default_size,
                          &background,
                          toplevel_default_border_width,
                          &title,
                          &default_closable,
                          &default_resizable,
                          &toplevel_default_min_size);
}

/* geomnotifyfunc */

void widget_geomnotifyfunc(ei_widget_t *widget, ei_rect_t rect) {

}

void button_geomnotifyfunc(ei_widget_t *widget, ei_rect_t rect) {
    widget->screen_location = rect;
}

void frame_geomnotifyfunc(ei_widget_t *widget, ei_rect_t rect) {
    widget->screen_location = rect;
}

void toplevel_geomnotifyfunc(ei_widget_t *widget, ei_rect_t rect) {
    *widget->content_rect = rect;

    ei_toplevel_t *toplevel = (ei_toplevel_t *) widget;
    int width, height;
    hw_text_compute_size(toplevel->title, ei_default_font, &width, &height);

    widget->screen_location = ei_rect(ei_point(widget->content_rect->top_left.x - toplevel->border_width,
                                               widget->content_rect->top_left.y - toplevel->border_width - height),
                                      ei_size(widget->content_rect->size.width + 2 * toplevel->border_width,
                                              widget->content_rect->size.height + 2 * toplevel->border_width + height));
}

/* handlefunc */

ei_bool_t widget_handlefunc(ei_widget_t *widget, struct ei_event_t *event) {
    return false;
}

ei_bool_t button_handlefunc(ei_widget_t *widget, struct ei_event_t *event) {
    ei_button_t *button = (ei_button_t *) widget;
    switch (event->type) {

        case ei_ev_mouse_buttondown:
            button->relief = ei_relief_sunken;
            return true;
            break;
        case ei_ev_mouse_buttonup:
            button->relief = ei_relief_raised;
            return true;
            break;
    }

    return false;
}

ei_bool_t frame_handlefunc(ei_widget_t *widget, struct ei_event_t *event) {
    return false;
}

ei_bool_t toplevel_handlefunc(ei_widget_t *widget, struct ei_event_t *event) {
    return false;
}


