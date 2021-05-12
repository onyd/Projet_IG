#include "ei_placer.h"
#include "ei_utils.h"
#include "utils.h"
#include "ei_widget.h"
#include "stdlib.h"

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
    ei_size_t used_size = ei_size(used_width, used_height);
    // Initialization of the placer_params
    if (widget->placer_params == NULL) {
        widget->placer_params = calloc(1, sizeof(ei_placer_params_t));
        ei_placer_params_t *params = widget->placer_params;
        //Default values
        params->anchor_data = ei_anc_northwest;
        params->anchor = &params->anchor_data;
        params->x = &params->x_data;
        params->y = &params->y_data;
        params->rx = &params->rx_data;
        params->ry = &params->ry_data;
        params->rw = &params->rw_data;
        params->rh = &params->rh_data;
        params->w = &params->w_data;
        params->h = &params->h_data;
    }
    ei_placer_params_t *params = widget->placer_params;
    params->w_data = used_width;
    params->h_data = used_height;
    params->anchor_data = (anchor != NULL) ? *anchor : params->anchor_data;
    params->x_data = (x != NULL) ? *x : params->x_data;
    params->y_data = (y != NULL) ? *y : params->x_data;
    params->w_data = (width != NULL) ? *x : params->w_data;
    params->h_data = (height != NULL) ? *x : params->h_data;
    params->rx_data = (rel_x != NULL) ? *rel_x : params->rx_data;
    params->ry_data = (rel_y != NULL) ? *rel_y : params->ry_data;
    params->rw_data = (rel_width != NULL) ? *rel_width : params->rw_data;
    params->rw_data = (rel_width != NULL) ? *rel_width : params->rw_data;
    params->rh_data = (rel_height != NULL) ? *rel_height : params->rh_data;
    ei_placer_run(widget);
}

void ei_placer_run(struct ei_widget_t *widget) {
    if (widget->placer_params == NULL) {
        return;
    }

    int x = widget->parent->screen_location.top_left.x;
    if (widget->placer_params->rx != NULL){
        x += (widget->placer_params->rx_data) * (widget->parent->screen_location.size.width);
    }
    if (widget->placer_params->x != NULL){
        x += (widget->placer_params->x_data);
    }

    int y = widget->parent->screen_location.top_left.y;
    if (widget->placer_params->ry != NULL){
        y += (widget->placer_params->ry_data) * (widget->parent->screen_location.size.width);
    }
    if (widget->placer_params->y != NULL){
        y += (widget->placer_params->y_data);
    }

    int w;
    if (widget->placer_params->rw == NULL && widget->placer_params->w == NULL){
        w = widget->requested_size.width;
    }
    else{
        w = 0;
        if (widget->placer_params->rw != NULL){
            w += (widget->placer_params->rw_data) * (widget->parent->screen_location.size.width);
        }
        if (widget->placer_params->w){
            w += (widget->placer_params->w_data);
        }
    }

    int h;
    if (widget->placer_params->rh == NULL && widget->placer_params->h == NULL){
        h = widget->requested_size.height;
    }
    else{
        h = 0;
        if (widget->placer_params->rh != NULL){
            h += (widget->placer_params->rh_data) * (widget->parent->screen_location.size.height);
        }
        if (widget->placer_params->h){
            h += (widget->placer_params->h_data);
        }
    }

    ei_point_t top_left_point = topleft(ei_point(x, y), widget->screen_location.size, widget->placer_params->anchor);
    widget->screen_location.top_left = top_left_point;

    ei_size_t size = ei_size(w, h);
    widget->screen_location.size = size;
}

void ei_placer_forget(struct ei_widget_t *widget) {
    free(widget->placer_params);
    widget->placer_params = NULL;

    ei_widget_t *parent = widget->parent;
    ei_widget_t *current_child = parent->children_head;
    ei_widget_t *previous_child;
    while (current_child != widget){
        previous_child = current_child;
        current_child = current_child->next_sibling;
    }
    previous_child->next_sibling = current_child->next_sibling;

    widget->parent = NULL;
}

