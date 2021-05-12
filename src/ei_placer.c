#include "ei_placer.h"
#include "ei_utils.h"
#include "utils.h"
#include "ei_widget.h"

void		ei_place	(struct ei_widget_t*	widget,
                         ei_anchor_t*		anchor,
                         int*			x,
                         int*			y,
                         int*			width,
                         int*			height,
                         float*			rel_x,
                         float*			rel_y,
                         float*			rel_width,
                         float*			rel_height){
    int used_width = (width != NULL) ? *width : widget->requested_size.width;
    int used_height = (height != NULL) ? *height : widget->requested_size.height;
    ei_size_t used_size = ei_size(used_width, used_height);
    //Initialisation of the placer_params
    if (widget->placer_params == NULL) {
        widget->placer_params = calloc(1, sizeof(ei_placer_params_t));
        ei_placer_params_t *params = widget->placer_params;
        //Default values
        params->x_data = 0;
        params->x = &params->x_data;
        params->y_data = 0;
        params->y = &params->y_data;
        params->w_data = used_width;
        params->w = &params->w_data;
        params->w_data = used_width;
        params->w = &params->w_data;
    }
    if (anchor != NULL) {

    }

    ei_point_t anchor_point;
    switch (*anchor) {
        case ei_anc_none:
            break;
        case ei_anc_center:
            anchor_point = absolute_coords(ei_rect(ei_point(*x, *y), used_size), 0.5, 0.5);
            break;
        case ei_anc_north:
            break;
        case ei_anc_northeast:
            break;
        case ei_anc_east:
            break;
        case ei_anc_southeast:
            break;
        case ei_anc_south:
            break;
        case ei_anc_southwest:
            break;
        case ei_anc_west:
            break;
        case ei_anc_northwest:
            break;

    }
}

void ei_placer_run(struct ei_widget_t *widget) {

}

void ei_placer_forget(struct ei_widget_t *widget) {

}

