#ifndef PROJETC_IG_WIDGETS_H
#define PROJETC_IG_WIDGETS_H

#include "ei_widget.h"
#include "ei_widgetclass.h"
#include "geometry.h"
#include "ei_types.h"
#include "ei_event.h"
#include "draw.h"


typedef struct ei_button_t {
    ei_widget_t widget;
    // Specific field
    ei_color_t color;
    int border_width;
    int corner_radius;
    ei_relief_t relief;
    char *text;
    ei_font_t text_font;
    ei_color_t text_color;
    ei_anchor_t text_anchor;
    ei_surface_t img;
    ei_rect_t **img_rect;
    ei_anchor_t img_anchor;
    ei_callback_t callback;
} ei_button_t;

typedef struct ei_frame_t {
    ei_widget_t widget;
    // Specific field
    ei_color_t color;
    int border_width;
    ei_relief_t relief;
    char *text;
    ei_font_t text_font;
    ei_color_t text_color;
    ei_anchor_t text_anchor;
    ei_surface_t img;
    ei_rect_t **img_rect;
    ei_anchor_t img_anchor;
} ei_frame_t;

typedef struct ei_toplevel_t {
    ei_widget_t widget;
    //Specific field
    ei_color_t color;
    int	border_width;
    char* title;
    ei_bool_t closable;
    ei_axis_set_t resizable;
    ei_size_t min_size;
    ei_button_t *button;
} ei_toplevel_t;

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
extern ei_widgetclass_t *frame_class;
extern ei_widgetclass_t *button_class;
extern ei_widgetclass_t *widget_class;
extern ei_widgetclass_t *toplevel_class;

// Default declaration
extern ei_surface_t main_window;
extern ei_frame_t *root;
extern ei_surface_t picking_offscreen;
extern uint32_t widget_compt;

extern ei_size_t *toplevel_default_size;
extern ei_size_t *toplevel_default_min_size;
extern int *toplevel_default_border_width;
extern ei_color_t *default_color;
extern ei_color_t *default_text_color;
extern ei_size_t *default_size;
extern ei_relief_t *default_relief;
extern ei_anchor_t *default_anchor;
extern bool quit_request;

void append_left(ei_widget_t *widget, ei_widget_list_t *l);

void append(ei_widget_t *widget, ei_widget_list_t *l);

void append_linked(ei_linked_widget_t *e, ei_widget_list_t *l);

void move(ei_widget_list_t *src, ei_widget_list_t *dst);

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
 * \brief	Allows to allocate a widget of type \ref ei_toplevel_t to zero.
 */
ei_widget_t *toplevel_allocfunc();

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
 * \brief	Allow to free a widget of type \ref ei_toplevel_t to zero.
 *
 * @param	widget		The widget which resources are to be freed.
 */
void frame_releasefunc(ei_widget_t *widget);


/**
 * \brief	Allow to free a widget of type \ref ei_button_t to zero.
 *
 * @param	widget		The widget which resources are to be freed.
 */
void toplevel_releasefunc(ei_widget_t *widget);

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
 * \brief	Allow to draw a widget of type \ref ei_toplevel_t to zero.
 *
 * @param	widget		The widget we want to draw.
 * @param	surface		The surface on which we want to draw.
 * @param	pick_surface		The picking surface.
 * @param	clipper		The clipper that restrain the drawing.
 */
void toplevel_drawfunc(ei_widget_t *widget,
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


/**
 * \brief	Allow to apply defaults values to a widget of type \ref ei_toplevel_t.
 *
 * @param	widget		The widget which we want to set.
 */
void toplevel_setdefaultsfunc(ei_widget_t *widget);

/* geomnotifyfunc */

void widget_geomnotifyfunc(ei_widget_t *widget, ei_rect_t rect);

void button_geomnotifyfunc(ei_widget_t *widget, ei_rect_t rect);

void frame_geomnotifyfunc(ei_widget_t *widget, ei_rect_t rect);

void toplevel_geomnotifyfunc(ei_widget_t *widget, ei_rect_t rect);

/* handlefunc */

ei_bool_t widget_handlefunc(ei_widget_t *widget, ei_event_t *event);

ei_bool_t button_handlefunc(ei_widget_t *widget, ei_event_t *event);

ei_bool_t frame_handlefunc(ei_widget_t *widget, ei_event_t *event);

ei_bool_t toplevel_handlefunc(ei_widget_t *widget, struct ei_event_t *event);

#endif //PROJETC_IG_WIDGETS_H
