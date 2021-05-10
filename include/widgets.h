#ifndef PROJETC_IG_WIDGETS_H
#define PROJETC_IG_WIDGETS_H

#include "widgets.h"
#include "ei_widgetclass.h"
#include "ei_widget.h"
#include "geometry.h"


typedef struct ei_button_t {
    struct ei_widget_t widget;
    // Specific field
    ei_color_t color;
    int *border_width;
    int *corner_radius;
    ei_relief_t *relief;
    char **text;
    ei_font_t *text_font;
    ei_color_t *text_color;
    ei_anchor_t *text_anchor;
    ei_surface_t *img;
    ei_rect_t **img_rect;
    ei_anchor_t *img_anchor;
    ei_callback_t *callback;
} ei_button_t;

typedef struct ei_frame_t {
    struct ei_widget_t widget;
    // Specific field
    const ei_color_t *color;
    int *border_width;
    ei_relief_t *relief;
    char **text;
    ei_font_t *text_font;
    ei_color_t *text_color;
    ei_anchor_t *text_anchor;
    ei_surface_t *img;
    ei_rect_t **img_rect;
    ei_anchor_t *img_anchor;
} ei_frame_t;

ei_widgetclass_t *frame_class;
ei_widgetclass_t *button_class;
ei_widgetclass_t *widget_class;

/* allocfunc */

/* releasefunc */

/* drawfunc */

// For the button
void draw_button(ei_widget_t*	widget,
                 ei_surface_t		surface,
                 ei_surface_t		pick_surface,
                 ei_rect_t*		clipper);

/* setdefaultsfunc */


#endif //PROJETC_IG_WIDGETS_H
