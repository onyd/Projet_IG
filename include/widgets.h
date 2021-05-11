#ifndef PROJETC_IG_WIDGETS_H
#define PROJETC_IG_WIDGETS_H

#include "ei_widget.h"
#include "ei_widgetclass.h"
#include "geometry.h"
#include "ei_types.h"
#include "ei_application.h"
#include "ei_event.h"
#include "draw.h"


typedef struct ei_button_t {
    ei_widget_t widget;
    // Specific field
    ei_color_t *color;
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
    ei_widget_t widget;
    // Specific field
    ei_color_t *color;
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

typedef struct ei_linked_widget_t {
    ei_widget_t *widget;
    struct ei_linked_widget_t *next;
} ei_linked_widget_t;

typedef struct ei_widget_list_t {
    ei_linked_widget_t *head;
    ei_linked_widget_t *pre_tail;
    ei_linked_widget_t *tail;
} ei_widget_list_t;

// Class declarations
ei_widgetclass_t *frame_class;
ei_widgetclass_t *button_class;
ei_widgetclass_t *widget_class;

// Default declaration
ei_surface_t main_window;
ei_frame_t *root;

ei_color_t *default_color;
ei_color_t *default_text_color;
ei_size_t *default_size;
ei_relief_t *default_relief;

void append_left(ei_widget_t *widget, ei_widget_list_t *l);

void append(ei_widget_t *widget, ei_widget_list_t *l);

ei_linked_widget_t *pop(ei_widget_list_t *l);

void free_linked_widget(ei_linked_widget_t *start);

void widget_breadth_list(ei_widget_t *start, ei_widget_list_t *result);

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
void widget_releasefunc(ei_widget_t *widget);

/**
 * \brief	Allow to free a widget of type \ref ei_button_t to zero.
 *
 * @param	widget		The widget which resources are to be freed.
 */
void button_releasefunc(ei_widget_t *widget);

/**
 * \brief	Allow to free a widget of type \ref ei_frame_t to zero.
 *
 * @param	widget		The widget which resources are to be freed.
 */
void frame_releasefunc(ei_widget_t *widget);

/**
 * \brief	Allow to draw a widget of type \ref ei_widget_t to zero.
 *
 * @param	widget		The widget we want to draw.
 * @param	surface		The surface on which we want to draw.
 * @param	pick_surface		The picking surface.
 * @param	clipper		The clipper that restrain the drawing.
 */
void widget_drawfunc(ei_widget_t *widget,
                     ei_surface_t surface,
                     ei_surface_t pick_surface,
                     ei_rect_t *clipper);

/**
 * \brief	Allow to draw a widget of type \ref ei_button_t to zero.
 *
 * @param	widget		The widget we want to draw.
 * @param	surface		The surface on which we want to draw.
 * @param	pick_surface		The picking surface.
 * @param	clipper		The clipper that restrain the drawing.
 */
void button_drawfunc(ei_widget_t *widget,
                     ei_surface_t surface,
                     ei_surface_t pick_surface,
                     ei_rect_t *clipper);

/**
 * \brief	Allow to draw a widget of type \ref ei_frame_t to zero.
 *
 * @param	widget		The widget we want to draw.
 * @param	surface		The surface on which we want to draw.
 * @param	pick_surface		The picking surface.
 * @param	clipper		The clipper that restrain the drawing.
 */
void frame_drawfunc(ei_widget_t *widget,
                    ei_surface_t surface,
                    ei_surface_t pick_surface,
                    ei_rect_t *clipper);

/**
 * \brief	Allow to apply defaults values to a widget of type \ref ei_widget_t.
 *
 * @param	widget		The widget which we want to set.
 */
void widget_setdefaultsfunc(ei_widget_t *widget);

/**
 * \brief	Allow to apply defaults values to a widget of type \ref ei_button_t.
 *
 * @param	widget		The widget which we want to set.
 */
void button_setdefaultsfunc(ei_widget_t *widget);

/**
 * \brief	Allow to apply defaults values to a widget of type \ref ei_frame_t.
 *
 * @param	widget		The widget which we want to set.
 */
void frame_setdefaultsfunc(ei_widget_t *widget);

/* geomnotifyfunc */

void widget_geomnotifyfunc(ei_widget_t *widget, ei_rect_t rect);

void button_geomnotifyfunc(ei_widget_t *widget, ei_rect_t rect);

void frame_geomnotifyfunc(ei_widget_t *widget, ei_rect_t rect);

/* handlefunc */

ei_bool_t widget_handlefunc(ei_widget_t *widget, ei_event_t *event);

ei_bool_t button_handlefunc(ei_widget_t *widget, ei_event_t *event);

ei_bool_t frame_handlefunc(ei_widget_t *widget, ei_event_t *event);

#endif //PROJETC_IG_WIDGETS_H
