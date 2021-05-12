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
    //Initialisation of the placer_params
    if (widget->placer_params == NULL) {
        widget->placer_params = calloc(1, sizeof(ei_placer_params_t));
        ei_placer_params_t *params = widget->placer_params;
        //Default values
        params->anchor_data = ei_anc_none;
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

    if (widget->placer_params->x) {

    }
}

void ei_placer_forget(struct ei_widget_t *widget) {

}

