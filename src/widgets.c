#include "widgets.h"
#include "stdlib.h"
#include "utils.h"
#include "hw_interface.h"
#include "geometry.h"
#include "eventhandler.h"

// Class declarations
ei_widgetclass_t *frame_class;
ei_widgetclass_t *button_class;
ei_widgetclass_t *widget_class;
ei_widgetclass_t *toplevel_class;

// Default declaration
ei_surface_t main_window;
ei_frame_t *root;
ei_surface_t picking_offscreen;
uint32_t widget_compt;
vector pick_vector;
chained_list *chainedList;

ei_size_t *toplevel_default_size;
ei_size_t *toplevel_default_min_size;
int *toplevel_default_border_width;
ei_color_t *default_color;
ei_color_t *default_text_color;
ei_size_t *default_size;
ei_relief_t *default_relief;
ei_anchor_t *default_anchor;

bool quit_request;

ei_rect_t *clipping_window;

void draw_window(ei_widget_t *current) {
    if (current != NULL) {
        while (current != NULL) {
            ei_placer_run(current);
            current->wclass->drawfunc(current, main_window, picking_offscreen, clipping_window);
            draw_window(current->children_head);
            current = current->next_sibling;
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
    ei_button_t *button = (ei_button_t *) widget;
    button->img_rect = calloc(1, sizeof(ei_rect_t *));
    button->callback = NULL;
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

    // Button
    ei_widgetclass_t *class = ei_widgetclass_from_name("button");
    toplevel->button = class->allocfunc();

    // Widget init
    toplevel->button->widget.wclass = class;
    toplevel->button->widget.parent = toplevel;

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
    to_release->button->widget.wclass->releasefunc(to_release->button);

    if (to_release->title != NULL) {
        free(to_release->title);
    }
    if (widget->content_rect != NULL) {
        free(widget->content_rect);
    }
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
    ei_color_t color = button->color;
    int radius = button->corner_radius;
    // If etat is true the button is up else he is down
    ei_color_t darker = {0.1 * 255 + 0.9 * button->color.red, 0.1 * 255 + 0.9 * color.green,
                         0.1 * 255 + 0.9 * color.blue,
                         color.alpha};
    ei_color_t lighter = {0.9 * color.red, 0.9 * color.green, 0.9 * color.blue, color.alpha};

    // The two part of the button
    ei_linked_point_t *top = rounded_frame(widget->screen_location, radius, 10, 1);
    ei_linked_point_t *bot = rounded_frame(widget->screen_location, radius, 10, 2);
    // The button
    ei_rect_t inside_button;
    int border_size = button->border_width;
    inside_button.top_left.x = widget->screen_location.top_left.x + border_size;
    inside_button.top_left.y = widget->screen_location.top_left.y + border_size;
    inside_button.size.width = widget->screen_location.size.width - 2 * border_size;
    inside_button.size.height = widget->screen_location.size.height - 2 * border_size;
    radius = radius - border_size;
    ei_linked_point_t *points_button = rounded_frame(inside_button, radius, 15, 0);

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
        ei_point_t topleft = anchor_target_pos(button->text_anchor, ei_size(width, height), widget->screen_location);
        ei_rect_t clipper_text;
        // We crop text in the button
        intersection(&widget->screen_location, clipper, &clipper_text);
        ei_draw_text(surface, &topleft, button->text, button->text_font, button->text_color, &clipper_text);
    }
    // Image eventually inside the button
    if (button->img != NULL) {
        ei_size_t img_size = (button->img_rect != NULL) ? (*button->img_rect)->size : hw_surface_get_size(button->img);
        ei_point_t topleft = anchor_target_pos(button->text_anchor, img_size, widget->screen_location);
        ei_rect_t clipper_img;
        // We crop image in the button
        intersection(&widget->screen_location, clipper, &clipper_img);
        draw_image(surface, button->img, &topleft, *button->img_rect, &clipper_img);
    }
}

void frame_drawfunc(ei_widget_t *widget,
                    ei_surface_t surface,
                    ei_surface_t pick_surface,
                    ei_rect_t *clipper) {
    ei_frame_t *frame = (ei_frame_t *) widget;
    ei_color_t color = frame->color;
    ei_color_t darker = {0.1 * 255 + 0.9 * color.red, 0.1 * 255 + 0.9 * color.green, 0.1 * 255 + 0.9 * color.blue,
                         color.alpha};
    ei_color_t lighter = {0.9 * color.red, 0.9 * color.green, 0.9 * color.blue, color.alpha};
    ei_rect_t inside_frame;
    int border_size = frame->border_width;
    inside_frame.top_left.x = widget->screen_location.top_left.x + border_size;
    inside_frame.top_left.y = widget->screen_location.top_left.y + border_size;
    inside_frame.size.width = widget->screen_location.size.width - 2 * border_size;
    inside_frame.size.height = widget->screen_location.size.height - 2 * border_size;
    if (frame->relief == ei_relief_none) {
        draw_rectangle(surface, widget->screen_location, color, clipper);
    } else {
        int direction = (frame->relief == ei_relief_raised) ? 0 : 1;
        draw_rect_triangle(surface, widget->screen_location, lighter, clipper, direction);
        draw_rect_triangle(surface, widget->screen_location, darker, clipper, 1 - direction);
        draw_rectangle(surface, inside_frame, color, clipper);
    }
    draw_rectangle(pick_surface, widget->screen_location, *(widget->pick_color), clipper);
    // Text eventually inside the frame
    if (frame->text != NULL) {
        int width, height;
        hw_text_compute_size(frame->text, frame->text_font, &width, &height);
        ei_point_t topleft = anchor_target_pos(frame->text_anchor, ei_size(width, height), widget->screen_location);
        ei_rect_t clipper_text;
        //in case the clipper is NULL, clipper_text must be widget->screen_location to avoid having the text outside the button
        intersection(&widget->screen_location, clipper, &clipper_text);
        ei_draw_text(surface, &topleft, frame->text, frame->text_font, frame->text_color, &clipper_text);
    }
    // Image eventually inside the frame
    if (frame->img != NULL) {
        ei_size_t img_size = (*frame->img_rect != NULL) ? (*frame->img_rect)->size : hw_surface_get_size(frame->img);
        ei_point_t topleft = anchor_target_pos(frame->text_anchor, img_size, widget->screen_location);
        ei_rect_t clipper_img;
        //in case the clipper is NULL, clipper_text must be widget->screen_location to avoid having the text outside the button
        intersection(&widget->screen_location, clipper, &clipper_img);
        draw_image(surface, frame->img, &topleft, *frame->img_rect, &clipper_img);
    }
}

void toplevel_drawfunc(ei_widget_t *widget,
                       ei_surface_t surface,
                       ei_surface_t pick_surface,
                       ei_rect_t *clipper) {
    ei_toplevel_t *toplevel = (ei_toplevel_t *) widget;
    ei_color_t color = toplevel->color;
    char *title = toplevel->title;

    //Draw all the top level
    draw_rectangle(surface, widget->screen_location, *default_color, clipper);
    draw_rectangle(pick_surface, widget->screen_location, *(widget->pick_color), clipper);
    //Draw the toplevel without border and top bar
    draw_rectangle(surface, *widget->content_rect, color, clipper);

    ei_rect_t clipper_text;
    intersection(&widget->screen_location, clipper, &clipper_text);
    if (toplevel->closable == EI_FALSE) {
        ei_point_t point_text = ei_point(widget->screen_location.top_left.x + toplevel->border_width,
                                         widget->screen_location.top_left.y);
        ei_draw_text(surface, &point_text, title, ei_default_font, toplevel->color, &clipper_text);
    } else {
        ei_point_t point_text = ei_point(widget->screen_location.top_left.x + 2 * toplevel->border_width + 20,
                                         widget->screen_location.top_left.y);
        ei_draw_text(surface, &point_text, title, ei_default_font, toplevel->color, &clipper_text);
        toplevel->button->widget.wclass->drawfunc(toplevel->button, surface, pick_surface, clipper);
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
    ei_toplevel_t *toplevel = (ei_toplevel_t *) widget;
    ei_color_t background = {0xA0, 0xA0, 0xA0, 0xff};
    char *title = "Toplevel";
    ei_bool_t default_closable = EI_TRUE;
    ei_axis_set_t default_resizable = ei_axis_both;

    // Toplevel default settings
    ei_toplevel_configure(widget,
                          toplevel_default_size,
                          &background,
                          toplevel_default_border_width,
                          &title,
                          &default_closable,
                          &default_resizable,
                          &toplevel_default_min_size);

    // Toplevel's button default settings
    int button_width = 2;
    ei_size_t size = ei_size(20, 20);
    int corner_size = 10;
    ei_color_t color_button = {200, 0, 0, 255};
    ei_callback_t close_callback = destroy_widget_callback;
    ei_button_configure((ei_widget_t *) toplevel->button, &size, &color_button, &button_width,
                        &corner_size, NULL, NULL, NULL, NULL,
                        NULL, NULL, NULL, NULL, &close_callback, NULL);
    ei_point_t point_button = ei_point(widget->screen_location.top_left.x + toplevel->border_width,
                                       widget->screen_location.top_left.y + toplevel->border_width);
    toplevel->button->widget.screen_location.top_left = point_button;

    // Picking
    toplevel->button->widget.pick_color = malloc(sizeof(ei_color_t));
    toplevel->button->widget.pick_id = widget_compt;
    ei_color_t pick_color = ei_map_rgba_inverse(picking_offscreen, toplevel->button->widget.pick_id);
    *(toplevel->button->widget.pick_color) = pick_color;
    append(&pick_vector, toplevel->button);
    widget_compt++;
}

/* geomnotifyfunc */

void widget_geomnotifyfunc(ei_widget_t *widget, ei_rect_t rect) {
    widget->screen_location = rect;
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
    toplevel->button->widget.screen_location.top_left.x = widget->screen_location.top_left.x + toplevel->border_width;
    toplevel->button->widget.screen_location.top_left.y = widget->screen_location.top_left.y + toplevel->border_width;
}

/* handlefunc */

ei_bool_t widget_handlefunc(ei_widget_t *widget, struct ei_event_t *event) {
    return false;
}

ei_bool_t button_handlefunc(ei_widget_t *widget, ei_event_t *event) {
    ei_button_t *button = (ei_button_t *) widget;
    switch (event->type) {
        case ei_ev_mouse_buttondown:
            if (inside(event->param.mouse.where, button->widget.content_rect)) {
                button->relief = ei_relief_sunken;
                return true;
            }
            break;
        case ei_ev_mouse_buttonup:
            button->relief = ei_relief_raised;
            if (button->callback != NULL && inside(event->param.mouse.where, button->widget.content_rect)) {
                button->callback(widget->parent, event, NULL);
            }
            return true;
    }
    return false;
}

ei_bool_t frame_handlefunc(ei_widget_t *widget, ei_event_t *event) {
    return false;
}

ei_bool_t toplevel_handlefunc(ei_widget_t *widget, struct ei_event_t *event) {
    ei_toplevel_t *toplevel = (ei_toplevel_t *) widget;
    switch (event->type) {
        case ei_ev_mouse_buttondown:
            ei_event_set_active_widget(toplevel);
            toplevel->active = event->param.mouse.where;
            return true;
        case ei_ev_mouse_move:
            if (ei_event_get_active_widget() == widget) {
                int topleft_x = widget->content_rect->top_left.x + (event->param.mouse.where.x - toplevel->active.x);
                int topleft_y = widget->content_rect->top_left.y + (event->param.mouse.where.y - toplevel->active.y);
                ei_place(toplevel, NULL, &topleft_x, &topleft_y, NULL, NULL, NULL, NULL,
                         NULL, NULL);
                toplevel->active = event->param.mouse.where;
                return true;
            }
            break;
        case ei_ev_mouse_buttonup:
            ei_event_set_active_widget(NULL);
            toplevel->button->widget.wclass->handlefunc(toplevel->button, event);
            return true;
    }
}


