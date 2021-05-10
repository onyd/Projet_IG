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

/**
 * \brief	Allows to allocate a widget of type \ref ei_widget_t to zero.
 */
ei_widget_t *widget_allocfunc();

/**
 * \brief	Allows to allocate a widget of type \ref ei_button_t to zero.
 */
ei_widget_t *button_allocfunc();

/**
 * \brief	Allows to allocate a widget of type \ref ei_frame_t to zero.
 */
ei_widget_t *frame_allocfunc();

/**
 * \brief	Allow to free a widget of type \ref ei_widget_t to zero.
 *
 * @param	widget		The widget which resources are to be freed.
 */
void widget_releasefunc(struct ei_widget_t *widget);

/**
 * \brief	Allow to free a widget of type \ref ei_widget_t to zero.
 *
 * @param	widget		The widget which resources are to be freed.
 */
void button_releasefunc(struct ei_widget_t *widget);

/**
 * \brief	Allow to free a widget of type \ref ei_widget_t to zero.
 *
 * @param	widget		The widget which resources are to be freed.
 */
void frmae_releasefunc(struct ei_widget_t *widget);

/**
 * \brief	Allow to draw a widget of type \ref ei_button_t to zero.
 *
 */
void draw_button(ei_widget_t*	widget,
                 ei_surface_t		surface,
                 ei_surface_t		pick_surface,
                 ei_rect_t*		clipper);

#endif //PROJETC_IG_WIDGETS_H
