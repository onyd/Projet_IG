#include "widgets.h"
#include <stdlib.h>
#include "utils.h"
#include "hw_interface.h"
#include "geometry.h"
#include "eventhandler.h"
#include "ei_application.h"
#include "SDL_keycode.h"
#include <string.h>
#include "widgetclass.h"
#include "defaults.h"


void ei_widget_destroy_rec(ei_widget_t *widget) {
    ei_widget_t *current = widget->children_head;
    while (current != NULL) {
        if (current->destructor != NULL) {
            current->destructor(current);
        }
        ei_placer_forget(current);
        ei_widget_destroy_rec(current);
        ei_widget_t *tmp = current;
        current = current->next_sibling;
        tmp->wclass->releasefunc(tmp);
        free(tmp->pick_color);
        free(tmp);
    }
    if (ei_event_get_active_widget() == widget) {
        ei_event_set_active_widget(NULL);
    }
}

ei_widget_t *search_for_toplevel(ei_widget_t *widget) {
    ei_widget_t *current = widget;
    ei_widget_t *first_top_level = widget;
    ei_widget_t *root_widget = ei_app_root_widget();
    while (current != root_widget) {
        if (!is_toplevel(current)) {
            first_top_level = current;
        }
        current = current->parent;
    }
    first_top_level = (!is_toplevel(first_top_level)) ? first_top_level : NULL;
    return first_top_level;
}

/* allocfunc */
ei_widget_t *button_allocfunc() {
    ei_widget_t *widget = calloc(1, sizeof(ei_button_t));
    ei_button_t *button = (ei_button_t *) widget;
    button->callback = NULL;
    return widget;
}

ei_widget_t *frame_allocfunc() {
    ei_widget_t *widget = calloc(1, sizeof(ei_frame_t));
    return widget;
}

ei_widget_t *toplevel_allocfunc() {
    ei_widget_t *widget = calloc(1, sizeof(ei_toplevel_t));
    ei_toplevel_t *toplevel = (ei_toplevel_t *) widget;
    widget->content_rect = calloc(1, sizeof(ei_rect_t));

    // Button
    ei_widgetclass_t *class = ei_widgetclass_from_name("button");
    toplevel->button = (ei_button_t *) class->allocfunc();

    // Widget init
    toplevel->button->widget.wclass = class;
    toplevel->button->widget.parent = (ei_widget_t *) toplevel;

    return widget;
}

ei_widget_t *radiobutton_allocfunc() {
    ei_widget_t *widget = calloc(1, sizeof(ei_radiobutton_t));
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
    to_release->button->widget.wclass->releasefunc((ei_widget_t *) to_release->button);
    free(to_release->button->widget.pick_color);
    free(to_release->button);

    if (to_release->title != NULL) {
        free(to_release->title);
    }
    if (widget->content_rect != NULL) {
        free(widget->content_rect);
    }
}

void radiobutton_releasefunc(ei_widget_t *widget) {
    ei_radiobutton_t *to_release = (ei_radiobutton_t *) widget;
    if (to_release->text != NULL) {
        free(to_release->text);
    }
}

/* drawfunc */
void button_drawfunc(ei_widget_t *widget,
                     ei_surface_t surface,
                     ei_surface_t pick_surface,
                     ei_rect_t *clipper) {
    if (widget == NULL) {
        return;
    }

    ei_button_t *button = (ei_button_t *) widget;
    ei_color_t color = button->color;
    int radius = button->corner_radius;
    // If etat is true the button is up else he is down
    ei_color_t darker = {(char) (0.1 * 255 + 0.9 * button->color.red), (char) (0.1 * 255 + 0.9 * color.green),
                         (char) (0.1 * 255 + 0.9 * color.blue),
                         color.alpha};
    ei_color_t lighter = {(char) (0.9 * color.red), (char) (0.9 * color.green), (char) (0.9 * color.blue), color.alpha};

    // The two part of the button
    ei_linked_point_t *top = rounded_frame(widget->screen_location, radius, 10, up);
    ei_linked_point_t *bot = rounded_frame(widget->screen_location, radius, 10, down);
    // The button
    ei_rect_t inside_button;
    int border_size = button->border_width;
    inside_button.top_left.x = widget->screen_location.top_left.x + border_size;
    inside_button.top_left.y = widget->screen_location.top_left.y + border_size;
    inside_button.size.width = widget->screen_location.size.width - 2 * border_size;
    inside_button.size.height = widget->screen_location.size.height - 2 * border_size;
    radius = (radius <= border_size) ? 0 : radius - border_size;
    ei_linked_point_t *points_button = rounded_frame(inside_button, radius, 15, both);

    if (button->relief == ei_relief_raised) {
        ei_draw_polygon(surface, top, lighter, clipper);
        ei_draw_polygon(surface, bot, darker, clipper);
    } else {
        ei_draw_polygon(surface, top, darker, clipper);
        ei_draw_polygon(surface, bot, lighter, clipper);
    }
    ei_draw_polygon(surface, points_button, color, clipper);

    ei_draw_polygon(pick_surface, points_button, *(widget->pick_color), clipper);
    ei_draw_polygon(pick_surface, top, *(widget->pick_color), clipper);
    ei_draw_polygon(pick_surface, bot, *(widget->pick_color), clipper);

    //Free all the points
    free_linked_point(top);
    free_linked_point(bot);
    free_linked_point(points_button);

    // Text eventually inside the button
    if (button->text != NULL) {
        int width, height;
        hw_text_compute_size(button->text, button->text_font, &width, &height);
        ei_point_t topleft = anchor_target_pos(button->text_anchor, ei_size(width, height), widget->screen_location,
                                               radius, button->border_width);
        ei_rect_t clipper_text;
        // We crop text in the button
        intersection_rect(&widget->screen_location, clipper, &clipper_text);
        ei_draw_text(surface, &topleft, button->text, button->text_font, button->text_color, &clipper_text);
    }
    // Image eventually inside the button
    if (button->img != NULL) {
        ei_rect_t rect = (button->img_rect != NULL) ? (*button->img_rect) : ei_rect(ei_point_zero(),
                                                                                    hw_surface_get_size(button->img));
        ei_point_t topleft = anchor_target_pos(button->img_anchor, rect.size, widget->screen_location, radius,
                                               button->border_width);
        ei_rect_t clipper_img;
        // We crop image in the button
        intersection_rect(&widget->screen_location, clipper, &clipper_img);
        draw_image(surface, button->img, &topleft, &rect, &clipper_img);
    }

    // Recursively draw children
    ei_widget_t *current = widget->children_head;
    while (current != NULL) {
        ei_rect_t clipping_widget;
        intersection_rect(get_clipper_window(), widget->content_rect, &clipping_widget);
        current->wclass->drawfunc(current, get_main_window(), get_pick_surface(),
                                  get_clipper_window());
        current = current->next_sibling;
    }
}

void frame_drawfunc(ei_widget_t *widget,
                    ei_surface_t surface,
                    ei_surface_t pick_surface,
                    ei_rect_t *clipper) {
    if (widget == NULL) {
        return;
    }

    ei_frame_t *frame = (ei_frame_t *) widget;
    ei_color_t color = frame->color;
    ei_color_t darker = {(char) (0.1 * 255 + 0.9 * color.red), (char) (0.1 * 255 + 0.9 * color.green),
                         (char) (0.1 * 255 + 0.9 * color.blue),
                         color.alpha};
    ei_color_t lighter = {(char) (0.9 * color.red), (char) (0.9 * color.green), (char) (0.9 * color.blue), color.alpha};
    ei_rect_t inside_frame;
    int border_size = frame->border_width;
    inside_frame.top_left.x = widget->screen_location.top_left.x + border_size;
    inside_frame.top_left.y = widget->screen_location.top_left.y + border_size;
    inside_frame.size.width = widget->screen_location.size.width - 2 * border_size;
    inside_frame.size.height = widget->screen_location.size.height - 2 * border_size;
    if (frame->relief == ei_relief_none) {
        draw_rectangle(surface, widget->screen_location, color, clipper);
    } else {
        if (frame->relief == ei_relief_raised) {
            draw_rect_triangle(surface, widget->screen_location, lighter, clipper, up);
            draw_rect_triangle(surface, widget->screen_location, darker, clipper, down);
        } else {
            draw_rect_triangle(surface, widget->screen_location, lighter, clipper, down);
            draw_rect_triangle(surface, widget->screen_location, darker, clipper, up);
        }
        draw_rectangle(surface, inside_frame, color, clipper);
    }

    draw_rectangle(pick_surface, widget->screen_location, *(widget->pick_color), clipper);
    // Text eventually inside the frame
    if (frame->text != NULL) {
        int width, height;
        hw_text_compute_size(frame->text, frame->text_font, &width, &height);
        ei_point_t topleft = anchor_target_pos(frame->text_anchor, ei_size(width, height), widget->screen_location, 0,
                                               0);
        ei_rect_t clipper_text;
        //in case the clipper is NULL, clipper_text must be widget->screen_location to avoid having the text outside the button
        intersection_rect(&widget->screen_location, clipper, &clipper_text);
        ei_draw_text(surface, &topleft, frame->text, frame->text_font, frame->text_color, &clipper_text);
    }
    // Image eventually inside the frame
    if (frame->img != NULL) {
        ei_rect_t rect = (frame->img_rect != NULL) ? (*frame->img_rect) : ei_rect(ei_point_zero(),
                                                                                  hw_surface_get_size(frame->img));
        ei_point_t topleft = anchor_target_pos(frame->img_anchor, rect.size, widget->screen_location, 0, 0);
        ei_rect_t clipper_img;
        // We crop image in the button
        intersection_rect(&widget->screen_location, clipper, &clipper_img);
        draw_image(surface, frame->img, &topleft, &rect, &clipper_img);
    }

    // Recursively draw children
    ei_widget_t *current = widget->children_head;
    while (current != NULL) {
        ei_rect_t clipping_widget;
        intersection_rect(get_clipper_window(), widget->content_rect, &clipping_widget);
        current->wclass->drawfunc(current, get_main_window(), get_pick_surface(),
                                  get_clipper_window());
        current = current->next_sibling;
    }
}

void toplevel_drawfunc(ei_widget_t *widget,
                       ei_surface_t surface,
                       ei_surface_t pick_surface,
                       ei_rect_t *clipper) {
    if (widget == NULL) {
        return;
    }
    ei_color_t white = {0xf0, 0xf0, 0xff, 0xff};

    ei_toplevel_t *toplevel = (ei_toplevel_t *) widget;
    ei_color_t color = toplevel->color;
    char *title = toplevel->title;

    //Draw all the top level
    draw_rectangle(surface, widget->screen_location, *get_default_color(), clipper);
    draw_rectangle(pick_surface, widget->screen_location, *(widget->pick_color), clipper);
    //Draw the toplevel without border and top bar
    draw_rectangle(surface, *widget->content_rect, color, clipper);


    ei_rect_t clipper_text;
    intersection_rect(&widget->screen_location, clipper, &clipper_text);
    if (toplevel->closable == EI_FALSE) {
        ei_point_t point_text = ei_point(widget->screen_location.top_left.x + toplevel->border_width,
                                         widget->screen_location.top_left.y);
        ei_draw_text(surface, &point_text, title, ei_default_font, toplevel->color, &clipper_text);
    } else {
        ei_point_t point_text = ei_point(widget->screen_location.top_left.x + 2 * toplevel->border_width + 20,
                                         widget->screen_location.top_left.y);
        ei_draw_text(surface, &point_text, title, ei_default_font, toplevel->color, &clipper_text);
        toplevel->button->widget.wclass->drawfunc((ei_widget_t *) toplevel->button, surface, pick_surface, clipper);
        float d = (float) toplevel->button->widget.content_rect->size.width;
        float sqrt_2 = sqrtf(2);
        draw_cross(surface,
                   ei_rect(ei_point((toplevel->button->widget.content_rect->top_left.x +
                                     (int) (d * (sqrt_2 - 1) / (2 * sqrt_2))),
                                    (toplevel->button->widget.content_rect->top_left.y +
                                     (int) (d * (sqrt_2 - 1) / (2 * sqrt_2)))),
                           ei_size((int) (d / sqrt(2)), (int) (d / sqrt_2))), white, clipper, 2);
    }

    // Recursively draw children
    ei_widget_t *current = widget->children_head;
    while (current != NULL) {
        ei_rect_t clipping_widget;
        intersection_rect(clipper, widget->content_rect, &clipping_widget);
        current->wclass->drawfunc(current, get_main_window(), get_pick_surface(),
                                  &clipping_widget);
        current = current->next_sibling;
    }

    // Minimize square
    ei_rect_t clipping_square;
    intersection_rect(clipper, &(toplevel->widget.screen_location), &clipping_square);
    if (toplevel->grab_event.param.show_minimize_square) {
        draw_rectangle(get_main_window(), toplevel->grab_event.param.minimize_square, *get_default_color(),
                       &clipping_square);
    }
    draw_rectangle(pick_surface, toplevel->grab_event.param.minimize_square, *toplevel->widget.pick_color,
                   &clipping_square);

}

void radiobutton_drawfunc(ei_widget_t *widget,
                          ei_surface_t surface,
                          ei_surface_t pick_surface,
                          ei_rect_t *clipper) {
    ei_radiobutton_t *radiobutton = (ei_radiobutton_t *) widget;
    draw_rectangle(surface, *(widget->content_rect), radiobutton->background_color, clipper);
    ei_point_t where = ei_point(widget->content_rect->top_left.x + 105, widget->content_rect->top_left.y);
    ei_draw_text(surface, &where, radiobutton->text, ei_default_font, radiobutton->text_color, clipper);
    draw_blank_rect(surface, *(widget->content_rect), radiobutton->text_color, clipper, 10, 100);
//    for (uint32_t i = 0; i <= radiobutton->button_list->; i++) {
//        if (get(radiobutton->button_list, i) != NULL) {
//            button_drawfunc(get(radiobutton->button_list, i), surface, NULL, clipper);
//        }
//    }
//    return;
}

/* setdefaultsfunc */
void button_setdefaultsfunc(ei_widget_t *widget) {

    ei_button_configure(widget,
                        get_default_size(),
                        get_default_color(),
                        get_toplevel_default_border_width(),
                        get_default_button_corner_radius(),
                        get_default_relief(),
                        NULL,
                        &ei_default_font,
                        get_default_text_color(),
                        get_default_anchor(),
                        NULL,
                        NULL,
                        NULL,
                        NULL,
                        NULL
    );
}

void frame_setdefaultsfunc(ei_widget_t *widget) {
    ei_relief_t relief = ei_relief_none;
    ei_frame_configure(widget,
                       get_default_size(),
                       get_default_color(),
                       get_default_button_border_width(),
                       &relief,
                       NULL,
                       &ei_default_font,
                       get_default_text_color(),
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
    ei_size_t *min_size = get_toplevel_default_min_size();

    // Toplevel default settings
    ei_toplevel_configure(widget,
                          get_toplevel_default_size(),
                          &background,
                          get_toplevel_default_border_width(),
                          &title,
                          &default_closable,
                          &default_resizable,
                          &min_size);

    // Picking
    toplevel->button->widget.pick_color = malloc(sizeof(ei_color_t));
    toplevel->button->widget.pick_id = append_vector(get_pick_vector(), toplevel->button);
    ei_color_t pick_color = ei_map_rgba_inverse(get_pick_surface(), toplevel->button->widget.pick_id);
    *(toplevel->button->widget.pick_color) = pick_color;
}

void radiobutton_setdefaultsfunc(ei_widget_t *widget) {
    ei_radiobutton_configure(widget,
                             get_default_size(),
                             get_default_color(),
                             get_default_button_color(),
                             get_default_selected_color(),
                             NULL,
                             &ei_default_font,
                             get_default_text_color(),
                             NULL
    );
}

/* geomnotifyfunc */
void button_geomnotifyfunc(ei_widget_t *widget, ei_rect_t rect) {
    ei_rect_t old_screen_location = widget->screen_location;
    widget->screen_location = rect;

    // Update
    add_widget_updated_rects(widget, old_screen_location);
}

void frame_geomnotifyfunc(ei_widget_t *widget, ei_rect_t rect) {
    ei_rect_t old_screen_location = widget->screen_location;
    widget->screen_location = rect;

    // Update
    add_widget_updated_rects(widget, old_screen_location);
}

void toplevel_geomnotifyfunc(ei_widget_t *widget, ei_rect_t rect) {
    ei_rect_t old_screen_location = widget->screen_location;

    *widget->content_rect = rect;
    ei_toplevel_t *toplevel = (ei_toplevel_t *) widget;
    int width, height;
    hw_text_compute_size(toplevel->title, ei_default_font, &width, &height);
    widget->requested_size = rect.size;
    widget->screen_location = ei_rect(ei_point(widget->content_rect->top_left.x - toplevel->border_width,
                                               widget->content_rect->top_left.y - toplevel->border_width - height),
                                      ei_size(widget->content_rect->size.width + 2 * toplevel->border_width,
                                              widget->content_rect->size.height + 2 * toplevel->border_width + height));
    toplevel->button->widget.screen_location.top_left.x =
            widget->screen_location.top_left.x + toplevel->border_width + toplevel->border_width;
    toplevel->button->widget.screen_location.top_left.y =
            widget->screen_location.top_left.y + toplevel->border_width + toplevel->border_width + height;

    toplevel->grab_event.param.minimize_square.top_left = ei_point(
            widget->screen_location.top_left.x + widget->screen_location.size.width - 20 + toplevel->border_width,
            widget->screen_location.top_left.y + widget->screen_location.size.height - 20 + toplevel->border_width +
            height);

    // Translation
    widget->screen_location.top_left = ei_point_add(widget->screen_location.top_left,
                                                    ei_point(toplevel->border_width, toplevel->border_width + height));
    widget->content_rect->top_left = ei_point_add(widget->content_rect->top_left,
                                                  ei_point(toplevel->border_width, toplevel->border_width + height));

    // Update
    add_widget_updated_rects(widget, old_screen_location);
}

void radiobutton_geomnotifyfunc(ei_widget_t *widget, ei_rect_t rect) {
    return;
}

void add_widget_updated_rects(ei_widget_t *widget, ei_rect_t rect) {
    ei_rect_t update_rect;
    union_rect(&rect, &(widget->screen_location), &update_rect);
    if (intersection_rect(&update_rect, get_clipper_window(), &update_rect)) {
        append_left_linked_rect(update_rect, get_updated_rects());
    }
}

/* handlefunc */
ei_bool_t button_handlefunc(ei_widget_t *widget, ei_event_t *event) {
    ei_button_t *button = (ei_button_t *) widget;
    ei_bool_t treated = false;
    switch (event->type) {
        case ei_ev_mouse_buttondown:
            if (inside(event->param.mouse.where, button->widget.content_rect)) {
                button->relief = ei_relief_sunken;
                ei_widget_t *toplevel = search_for_toplevel(widget);
                if (toplevel != NULL) {
                    toplevel->wclass->handlefunc(toplevel, event);
                }
                ei_event_set_active_widget(widget);
                treated = true;
            }
            break;
        case ei_ev_mouse_move:
            if (inside(event->param.mouse.where, button->widget.content_rect)) {
                break;
            }
        case ei_ev_mouse_buttonup:
            button->relief = ei_relief_raised;
            if (button->callback != NULL && ei_event_get_active_widget() == widget && inside(event->param.mouse.where, button->widget.content_rect)) {
                button->callback(widget->parent, event, button->widget.user_data);
            }
            ei_event_set_active_widget(NULL);
            treated = true;
            break;
        default:
            return false;
    }
    if (treated) {
        append_left_linked_rect(widget->screen_location, get_updated_rects());
    }
    return treated || false;
}

ei_bool_t frame_handlefunc(ei_widget_t *widget, struct ei_event_t *event) {
    switch (event->type) {
        case ei_ev_mouse_buttondown: {
            ei_widget_t *toplevel = search_for_toplevel(widget);
            if (toplevel != NULL) {
                toplevel->wclass->handlefunc(toplevel, event);
            }
            return true;
        }
    }
    return false;
}

ei_bool_t toplevel_handlefunc(ei_widget_t *widget, struct ei_event_t *event) {
    ei_toplevel_t *toplevel = (ei_toplevel_t *) widget;
    ei_bool_t treated = false;

    switch (event->type) {
        case ei_ev_mouse_buttondown: {
            ei_rect_t top_bar = ei_rect(widget->screen_location.top_left,
                                        ei_size(widget->screen_location.size.width,
                                                widget->screen_location.size.height -
                                                widget->content_rect->size.height));
            if (inside(event->param.mouse.where, &top_bar)) {
                ei_event_set_active_widget((ei_widget_t *) toplevel);
                toplevel->grab_event.grab_type = grabbed;
                treated = true;
            } else if (inside(event->param.mouse.where, &toplevel->grab_event.param.minimize_square)) {
                ei_event_set_active_widget((ei_widget_t *) toplevel);
                toplevel->grab_event.grab_type = resized;
                treated = true;
            }
            ei_widget_t *toplevel = search_for_toplevel(widget);
            if (toplevel != widget) {
                toplevel->wclass->handlefunc(toplevel, event);
            } else {
                //The top level is at the first plan
                ei_widget_t *current_child = widget->parent->children_head;
                ei_widget_t *previous_child = current_child;
                if (widget->parent->children_head != widget->parent->children_tail &&
                    widget != widget->parent->children_tail) {
                    while (current_child != widget) {
                        previous_child = current_child;
                        current_child = current_child->next_sibling;
                    }
                    //if the widget is the head
                    if (current_child == previous_child) {
                        widget->parent->children_head = widget->next_sibling;
                        widget->next_sibling = NULL;
                        widget->parent->children_tail->next_sibling = widget;
                        widget->parent->children_tail = widget;
                    } else {
                        previous_child->next_sibling = widget->next_sibling;
                        widget->next_sibling = NULL;
                        widget->parent->children_tail->next_sibling = widget;
                        widget->parent->children_tail = widget;
                    }
                    treated = true;
                }
            }
            break;
        }
            case ei_ev_mouse_move:
                switch (toplevel->grab_event.grab_type) {
                    case grabbed:
                        if (ei_event_get_active_widget() == widget) {
                            int topleft_x =
                                    widget->screen_location.top_left.x - widget->parent->content_rect->top_left.x +
                                    (event->param.mouse.where.x - get_prev_mouse_pos().x);
                            int topleft_y =
                                    widget->screen_location.top_left.y - widget->parent->content_rect->top_left.y +
                                    (event->param.mouse.where.y - get_prev_mouse_pos().y);
                            if (widget->placer_params->rx == NULL && widget->placer_params->ry == NULL) {
                                ei_place((ei_widget_t *) toplevel, NULL, &topleft_x, &topleft_y, NULL, NULL, NULL, NULL,
                                         NULL, NULL);
                            }
                            else if (widget->placer_params->rx == NULL) {
                                ei_place((ei_widget_t *) toplevel, NULL, &topleft_x, NULL, NULL, NULL, NULL, NULL,
                                         NULL, NULL);
                            }
                            else if (widget->placer_params->ry == NULL) {
                                ei_place((ei_widget_t *) toplevel, NULL, NULL, &topleft_y, NULL, NULL, NULL, NULL,
                                         NULL, NULL);
                            }
                            treated = true;
                        }
                        break;
                    case resized: {
                        ei_bool_t treated = false;
                        if (ei_event_get_active_widget() == widget && toplevel->resizable != ei_axis_none) {
                            int width = widget->content_rect->size.width +
                                        (event->param.mouse.where.x - get_prev_mouse_pos().x);
                            int height = widget->content_rect->size.height +
                                         (event->param.mouse.where.y - get_prev_mouse_pos().y);
                            // x-resize
                            if (toplevel->min_size.width <= width && toplevel->resizable == ei_axis_x &&
                                widget->placer_params->rw == NULL) {
                                ei_place((ei_widget_t *) toplevel, NULL, NULL, NULL, &width, NULL, NULL, NULL,
                                         NULL, NULL);
                            }
                                // y-resize
                            else if (toplevel->min_size.height <= height && toplevel->resizable == ei_axis_y &&
                                     widget->placer_params->rh == NULL) {
                                ei_place(toplevel, NULL, NULL, NULL, NULL, &height, NULL, NULL, NULL, NULL);
                            }

                                // x and y resize
                            else if (toplevel->min_size.height <= height && toplevel->min_size.width <= width &&
                                     toplevel->resizable == ei_axis_both) {
                                if (widget->placer_params->rw == NULL && widget->placer_params->rh == NULL) {
                                    ei_place(toplevel, NULL, NULL, NULL, &width, &height, NULL, NULL,
                                             NULL, NULL);
                                } else if (widget->placer_params->rw == NULL) {
                                    ei_place(toplevel, NULL, NULL, NULL, &width, NULL, NULL, NULL,
                                             NULL, NULL);
                                } else if (widget->placer_params->rh == NULL) {
                                    ei_place(toplevel, NULL, NULL, NULL, NULL, &height, NULL, NULL,
                                             NULL, NULL);
                                }
                                treated = true;
                            }


                        if (!toplevel->grab_event.param.unshow_minimize_square_event_sent) {
                            user_param_t *event = calloc(1, sizeof(user_param_t));
                            event->app_event_type = toplevel_param;
                            toplevel_app_event_t *data = calloc(1, sizeof(toplevel_app_event_t));
                            data->caller = toplevel;
                            event->data = data;
                            hw_event_schedule_app(200, event);
                            toplevel->grab_event.param.unshow_minimize_square_event_sent = true;
                            treated = true;
                        }
                    }
                    return treated;
                }
                case idle:
                    //Draw the square to minimize the toplevel
                    if (toplevel->resizable != ei_axis_none && !toplevel->grab_event.param.show_minimize_square) {
                        if (inside(event->param.mouse.where, &toplevel->grab_event.param.minimize_square)) {
                            toplevel->grab_event.param.show_minimize_square = true;
                            treated = true;
                        }
                    } else if (toplevel->resizable != ei_axis_none &&
                               !toplevel->grab_event.param.unshow_minimize_square_event_sent) {
                        user_param_t *event = calloc(1, sizeof(user_param_t));
                        event->app_event_type = toplevel_param;
                        toplevel_app_event_t *data = calloc(1, sizeof(toplevel_app_event_t));
                        data->caller = toplevel;
                        event->data = data;
                        hw_event_schedule_app(200, event);
                        toplevel->grab_event.param.unshow_minimize_square_event_sent = true;
                        treated = true;
                    }
            }
            break;
        case ei_ev_mouse_buttonup:
            ei_event_set_active_widget(NULL);
            toplevel->grab_event.grab_type = idle;
            //toplevel->button->widget.wclass->handlefunc((ei_widget_t *) toplevel->button, event);
            treated = true;

            break;

            case ei_ev_keydown:
                if (event->param.key.modifier_mask == 8 && event->param.key.key_code == 119) {
                    ei_widget_destroy(widget);
                    treated = true;
                    break;
                }

            if (treated) {
                ei_rect_t clipper;
                intersection_rect(&widget->screen_location, get_clipper_window(), &clipper);
                append_left_linked_rect(clipper, get_updated_rects());
            }

            // Button has to receive event
            return treated;
    }
}

ei_bool_t radiobutton_handlefunc(ei_widget_t *widget, ei_event_t *event) {
    return EI_TRUE;
}

void ei_radiobutton_configure(ei_widget_t *widget,
                              ei_size_t *requested_size,
                              ei_color_t *background_color,
                              ei_color_t *button_color,
                              ei_color_t *selected_color,
                              char **text,
                              ei_font_t *text_font,
                              ei_color_t *text_color,
                              ei_anchor_t *text_anchor) {
    ei_radiobutton_t *radiobutton = (ei_radiobutton_t *) widget;
    widget->requested_size = (requested_size != NULL) ? *requested_size : widget->requested_size;
    radiobutton->background_color = (background_color != NULL) ? *background_color : radiobutton->background_color;
    radiobutton->button_color = (button_color != NULL) ? *button_color : radiobutton->button_color;
    radiobutton->selected_color = (selected_color != NULL) ? *selected_color : radiobutton->selected_color;
    radiobutton->text_font = (text_font != NULL) ? *text_font : radiobutton->text_font;
    radiobutton->text_color = (text_color != NULL) ? *text_color : radiobutton->text_color;
    radiobutton->text_anchor = (text_anchor != NULL) ? *text_anchor : radiobutton->text_anchor;
    if (text != NULL) {
        free(radiobutton->text);
        radiobutton->text = calloc(strlen(*text) + 1, sizeof(char));
        strcpy(radiobutton->text, *text);
    }
}
