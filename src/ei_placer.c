#include "ei_placer.h"
#include "ei_utils.h"
#include "utils.h"
#include "ei_widget.h"
#include <stdlib.h>
#include "vector.h"
#include "widgets.h"
#include "defaults.h"

void ei_place(struct ei_widget_t *widget,
              ei_anchor_t *anchor,
              int *x,
              int *y,
              int *width,
              int *height,
              float *rel_x,
              float *rel_y,
              float *rel_width,
              float *rel_height) {
    int used_width = (width != NULL) ? *width : widget->requested_size.width;
    int used_height = (height != NULL) ? *height : widget->requested_size.height;
    // Initialization of the placer_params
    if (widget->placer_params == NULL) {
        widget->placer_params = calloc(1, sizeof(ei_placer_params_t));
        ei_placer_params_t *params = widget->placer_params;
        //Default values
        params->anchor_data = ei_anc_northwest;
        params->anchor = &params->anchor_data;
    }
    ei_placer_params_t *params = widget->placer_params;
    params->w_data = used_width;
    params->h_data = used_height;
    params->anchor_data = (anchor != NULL) ? *anchor : params->anchor_data;
    params->x_data = (x != NULL) ? *x : params->x_data;
    params->x = (x != NULL) ? &params->x_data : params->x;
    params->y_data = (y != NULL) ? *y : params->y_data;
    params->y = (y != NULL) ? &params->y_data : params->y;
    params->w_data = (width != NULL) ? *width : params->w_data;
    params->w = (width != NULL) ? &params->w_data : params->w;
    params->h_data = (height != NULL) ? *height : params->h_data;
    params->h = (height != NULL) ? &params->h_data : params->h;
    params->rx_data = (rel_x != NULL) ? *rel_x : params->rx_data;
    params->rx = (rel_x != NULL) ? &params->rx_data : params->rx;
    params->ry_data = (rel_y != NULL) ? *rel_y : params->ry_data;
    params->ry = (rel_y != NULL) ? &params->ry_data : params->ry;
    params->rw_data = (rel_width != NULL) ? *rel_width : params->rw_data;
    params->rw = (rel_width != NULL) ? &params->rw_data : params->rw;
    params->rh_data = (rel_height != NULL) ? *rel_height : params->rh_data;
    params->rh = (rel_height != NULL) ? &params->rh_data : params->rh;
}

void ei_placer_run(ei_widget_t *widget) {
    if (widget == NULL) {
        return;
    }

    if (widget->placer_params != NULL) {
        int x = widget->parent->content_rect->top_left.x;
        if (widget->placer_params->rx != NULL) {
            x +=(int) ((widget->placer_params->rx_data) * (float) (widget->parent->content_rect->size.width));
        }
        if (widget->placer_params->x != NULL) {
            x += (widget->placer_params->x_data);
        }

        int y = widget->parent->content_rect->top_left.y;
        if (widget->placer_params->ry != NULL) {
            y += (int) ((widget->placer_params->ry_data) * (float) (widget->parent->content_rect->size.height));
        }
        if (widget->placer_params->y != NULL) {
            y += (widget->placer_params->y_data);
        }

        int w;
        if (widget->placer_params->rw == NULL && widget->placer_params->w == NULL) {
            w = widget->content_rect->size.width;
        } else {
            w = 0;
            if (widget->placer_params->rw != NULL) {
                w += (int) ((widget->placer_params->rw_data) * (float) (widget->parent->content_rect->size.width));
            }
            if (widget->placer_params->w != NULL) {
                w += (widget->placer_params->w_data);
            }
        }

        int h;
        if (widget->placer_params->rh == NULL && widget->placer_params->h == NULL) {
            h = widget->content_rect->size.height;
        } else {
            h = 0;
            if (widget->placer_params->rh != NULL) {
                h += (int) ((widget->placer_params->rh_data) * (float) (widget->parent->content_rect->size.height));
            }
            if (widget->placer_params->h != NULL) {
                h += (widget->placer_params->h_data);
            }
        }

        ei_point_t top_left_point = topleft(ei_point(x, y), widget->content_rect->size, widget->placer_params->anchor);
        ei_rect_t rect = ei_rect(top_left_point, ei_size(w, h));
        if (!((rect.size.width == widget->content_rect->size.width) &&
              (rect.size.height == widget->content_rect->size.height) &&
              (rect.top_left.x == widget->content_rect->top_left.x) &&
              (rect.top_left.y == widget->content_rect->top_left.y))) {
            widget->wclass->geomnotifyfunc(widget, rect);
        }
    }

    ei_widget_t *current = widget->children_head;
    while (current != NULL) {
        ei_placer_run(current);
        current = current->next_sibling;
    }
}

void ei_placer_forget(ei_widget_t *widget) {
    free(widget->placer_params);
    widget->placer_params = NULL;
    remove_vector(get_pick_vector(), widget);

    ei_widget_t *parent = widget->parent;
    ei_widget_t *current_child = parent->children_head;
    ei_widget_t *previous_child = NULL;
    while (current_child != widget) {
        previous_child = current_child;
        current_child = current_child->next_sibling;
    }

    // Pop widget from its parent
    //if the widget is the head
    if (parent->children_tail != parent->children_head) {
        //if the widget is the head
        if (previous_child == NULL) {
            parent->children_head = current_child->next_sibling;
        }
        else if (parent->children_tail == current_child) {
            previous_child->next_sibling = NULL;
            parent->children_tail = previous_child;
        }
        else {
            previous_child->next_sibling = widget->next_sibling;
        }
    }
    else {
        parent->children_tail = NULL;
        parent->children_head = NULL;
    }
    widget->next_sibling = NULL;
    widget->parent = NULL;
}

