#include "ei_widget.h"
#include "widgets.h"
#include "string.h"

ei_widget_t *ei_widget_create(ei_widgetclass_name_t class_name, ei_widget_t *parent, void *user_data,
                              ei_widget_destructor_t destructor) {
    if (strcmp(class_name, "frame") == 0 || strcmp(class_name, "button") == 0 || strcmp(class_name, "widget") == 0
        || strcmp(class_name, "toplevel") == 0) {
        ei_widgetclass_t *class = ei_widgetclass_from_name(class_name);
        ei_widget_t *new_widget = class->allocfunc();

        // Widget init
        new_widget->wclass = class;
        new_widget->parent = parent;
        class->setdefaultsfunc(new_widget);

        if (parent->children_head == NULL) {
            parent->children_head = new_widget;
            parent->children_tail = new_widget;
        } else {
            parent->children_tail->next_sibling = new_widget;
            parent->children_tail = new_widget;
        }
        return new_widget;
    }
    return NULL;
}

void ei_widget_destroy(ei_widget_t *widget) {
    ei_widget_list_t children = {NULL, NULL, NULL};
    widget_breadth_list(widget, &children);

    ei_placer_forget(widget);
    ei_linked_widget_t *current;
    current = children.head;
    while (current != children.tail) {
        ei_widget_t *tmp = current->widget;
        current = current->next;
        tmp->wclass->releasefunc(tmp);
    }
}


ei_widget_t *ei_widget_pick(ei_point_t *where) {
    uint32_t *pixels = (uint32_t *) hw_surface_get_buffer(picking_offscreen);
    int width = hw_surface_get_size(picking_offscreen).width;
    uint32_t current_pick_color = pixels[where->x + width * (where->y)];

    ei_widget_list_t widgetList = {NULL, NULL, NULL};
    widget_breadth_list(&(root->widget), &widgetList);
    ei_linked_widget_t *current = widgetList.head;
    while ((ei_map_rgba(picking_offscreen, *(current->widget->pick_color)) != current_pick_color) &&
           current->widget != NULL) {
        current = current->next;
    }
    return current->widget;
}

void ei_frame_configure(ei_widget_t *widget,
                        ei_size_t *requested_size,
                        const ei_color_t *color,
                        int *border_width,
                        ei_relief_t *relief,
                        char **text,
                        ei_font_t *text_font,
                        ei_color_t *text_color,
                        ei_anchor_t *text_anchor,
                        ei_surface_t *img,
                        ei_rect_t **img_rect,
                        ei_anchor_t *img_anchor) {
    ei_frame_t *frame = (ei_frame_t *) widget;
    widget->requested_size = (requested_size != NULL) ? *requested_size : widget->requested_size;
    if (widget->parent != NULL) {
        ei_rect_t rect = ei_rect(widget->parent->screen_location.top_left, widget->requested_size);
        intersection(&rect, &widget->parent->screen_location, &widget->screen_location);
        widget->content_rect = &widget->screen_location;
    } else { // Then it is root widget
        widget->screen_location = ei_rect(ei_point_zero(), widget->requested_size);
        widget->content_rect = &widget->screen_location;
    }

    frame->color = (color != NULL) ? *color : frame->color;
    frame->border_width = (border_width != NULL) ? *border_width : frame->border_width;
    frame->relief = (relief != NULL) ? *relief : frame->relief;
    if (text != NULL) {
        free(frame->text);
        frame->text = calloc(strlen(*text) + 1, sizeof(char));
        strcpy(frame->text, *text);
    }
    frame->text_font = (text_font != NULL) ? *text_font : frame->text_font;
    frame->text_color = (text_color != NULL) ? (*text_color) : frame->text_color;
    frame->text_anchor = (text_anchor != NULL) ? *text_anchor : frame->text_anchor;

    frame->img = (img != NULL) ? *img : frame->img;
    *(frame->img_rect) = (img_rect != NULL) ? *img_rect : *frame->img_rect;
    frame->img_anchor = (img_anchor != NULL) ? *img_anchor : frame->img_anchor;
}

void ei_button_configure(ei_widget_t *widget,
                         ei_size_t *requested_size,
                         const ei_color_t *color,
                         int *border_width,
                         int *corner_radius,
                         ei_relief_t *relief,
                         char **text,
                         ei_font_t *text_font,
                         ei_color_t *text_color,
                         ei_anchor_t *text_anchor,
                         ei_surface_t *img,
                         ei_rect_t **img_rect,
                         ei_anchor_t *img_anchor,
                         ei_callback_t *callback,
                         void **user_param) {
    ei_button_t *button = (ei_button_t *) widget;
    widget->requested_size = (requested_size != NULL) ? *requested_size : widget->requested_size;
    ei_rect_t rect = ei_rect(widget->parent->screen_location.top_left, widget->requested_size);
    intersection(&rect, &widget->parent->screen_location, &widget->screen_location);
    widget->content_rect = &widget->screen_location;

    button->color = (color != NULL) ? *color : (button->color);
    button->border_width = (border_width != NULL) ? *border_width : button->border_width;
    button->corner_radius = *corner_radius;
    button->relief = (relief != NULL) ? *relief : button->relief;
    button->callback = (callback != NULL) ? *callback : button->callback;
    button->widget.user_data = (user_param != NULL) ? *user_param : button->widget.user_data;

    // Auto-size image
    if (img != NULL) {
        button->img = *img;

        if (requested_size == NULL) {
            int width, height;
            hw_text_compute_size(button->text, button->text_font, &width, &height);
            widget->requested_size = ei_size(width, height);
        }
    }
    *(button->img_rect) = (img_rect != NULL) ? *img_rect : *button->img_rect;
    button->img_anchor = (img_anchor != NULL) ? *img_anchor : button->img_anchor;

    // Auto-size text
    if (text != NULL) {
        free(button->text);
        button->text = calloc(strlen(*text) + 1, sizeof(char));
        strcpy(button->text, *text);

        if (requested_size == NULL) {
            int width, height;
            hw_text_compute_size(button->text, button->text_font, &width, &height);
            widget->requested_size = ei_size(width, height);
        }
    }
    button->text_font = (text_font != NULL) ? *text_font : button->text_font;
    button->text_color = (text_color != NULL) ? *text_color : button->text_color;
    button->text_anchor = (text_anchor != NULL) ? *text_anchor : button->text_anchor;
}


void ei_toplevel_configure(ei_widget_t *widget,
                           ei_size_t *requested_size,
                           ei_color_t *color,
                           int *border_width,
                           char **title,
                           ei_bool_t *closable,
                           ei_axis_set_t *resizable,
                           ei_size_t **min_size) {
    ei_toplevel_t *toplevel = (ei_toplevel_t *) widget;
    widget->requested_size = (requested_size != NULL) ? *requested_size : widget->requested_size;

    toplevel->color = (color != NULL) ? *color : (toplevel->color);
    toplevel->border_width = (border_width != NULL) ? *border_width : toplevel->border_width;

    if (title != NULL) {
        free(toplevel->title);
        toplevel->title = calloc(strlen(*title) + 1, sizeof(char));
        strcpy(toplevel->title, *title);
    }

    int width, height;
    hw_text_compute_size(toplevel->title, ei_default_font, &width, &height);
    *widget->content_rect = ei_rect(ei_point(widget->parent->screen_location.top_left.x + toplevel->border_width,
                                             widget->parent->screen_location.top_left.y + toplevel->border_width +
                                             height), widget->requested_size);
    widget->screen_location = ei_rect(widget->parent->screen_location.top_left,
                                      ei_size(widget->content_rect->size.width + 2 * toplevel->border_width,
                                              widget->content_rect->size.height + 2 * toplevel->border_width + height));

    toplevel->closable = (closable != NULL) ? *closable : toplevel->closable;
    toplevel->resizable = (resizable != NULL) ? *resizable : toplevel->resizable;
    if (min_size != NULL) {
        if (*min_size != NULL) {
            toplevel->min_size = **min_size;
        } else {
            toplevel->min_size = *toplevel_default_min_size;
        }
    }
}