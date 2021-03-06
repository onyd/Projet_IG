#ifndef PROJETC_IG_WIDGETS_H
#define PROJETC_IG_WIDGETS_H

#include "ei_widget.h"
#include "ei_widgetclass.h"
#include "geometry.h"
#include "ei_types.h"
#include "ei_event.h"
#include "draw.h"
#include "vector.h"

/**
 * @brief The button widget struct
 */
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
    ei_rect_t *img_rect;
    ei_anchor_t img_anchor;
    ei_callback_t callback;
} ei_button_t;

/**
 * @brief The frame widget struct
 */
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
    ei_rect_t *img_rect;
    ei_anchor_t img_anchor;
} ei_frame_t;

/**
 * @brief Defines grab_event_type for toplevel
 */
typedef enum {
    idle = 0,
    grabbed,
    resized,
} grab_event_type;

/**
 * @brief Used to handle toplevel events
 */
typedef struct grab_event_t {
    grab_event_type grab_type; // Tells what the user do with toplevel
    struct param {
        ei_rect_t resize_square; // The ei_rect_t of minimize square for resizing
        ei_bool_t show_resize_square;
        ei_bool_t unshow_resize_square_event_sent;
    } param;
} grab_event_t;

/**
 * @brief The toplevel widget struct
 */
typedef struct ei_toplevel_t {
    ei_widget_t widget;
    //Specific field
    ei_color_t color;
    int border_width;
    char *title;
    ei_bool_t closable;
    ei_axis_set_t resizable;
    ei_size_t min_size;
    ei_button_t *button;
    grab_event_t grab_event;
} ei_toplevel_t;

/**
 * @brief The radio button struct to retrieve button's parameters
 */
typedef struct radiobutton_user_data_t {
    uint32_t idx;
    char *text;
    ei_callback_t callback;
} radiobutton_user_data_t;

/**
 * @brief The radio button widget struct
 */
typedef struct ei_radiobutton_t {
    ei_widget_t widget;
    // Specific field
    uint32_t margin;
    ei_color_t background_color;
    ei_color_t outline_color;
    ei_color_t button_color;
    ei_color_t selected_color;
    char *title;
    ei_font_t text_font;
    ei_color_t text_color;
    ei_anchor_t text_anchor;
    vector *button_list;
    int selected_id;
} ei_radiobutton_t;


/**
 * \brief	Allows to recursively destroy a widget of type \ref ei_widget_t.
 * The function is called by ei_widget_destroy
 * @param the widget to destroy
 */
void ei_widget_destroy_rec(ei_widget_t *widget);


/**
 * \brief	Search for the first parent of a widget which is a toplevel
 * @param a widget
 * @return the first widget's parent which is a toplevel or NULL if it doesn't exist
 */
ei_widget_t *search_for_toplevel(ei_widget_t *widget);

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
 * \brief	Allows to allocate a widget of type \ref ei_radiobutton_t to zero.
 */
ei_widget_t *radiobutton_allocfunc();

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
 * \brief	Allow to free a widget of type \ref ei_radiobutton_t to zero.
 *
 * @param	widget		The widget which resources are to be freed.
 */
void radiobutton_releasefunc(ei_widget_t *widget);

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
 * \brief	Allow to draw a widget of type \ref ei_radiobutton_t to zero.
 *
 * @param	widget		The widget we want to draw.
 * @param	surface		The surface on which we want to draw.
 * @param	pick_surface		The picking surface.
 * @param	clipper		The clipper that restrain the drawing.
 */
void radiobutton_drawfunc(ei_widget_t *widget,
                          ei_surface_t surface,
                          ei_surface_t pick_surface,
                          ei_rect_t *clipper);

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

/**
 * \brief	Allow to apply defaults values to a widget of type \ref ei_radiobutton_t.
 *
 * @param	widget		The widget which we want to set.
 */
void radiobutton_setdefaultsfunc(ei_widget_t *widget);

/* geomnotifyfunc */
/**
 * \brief	Allow to update geometry of a widget of type \ref ei_button_t.
 *
 * @param	widget		The subject widget.
 * @param	rect		The updated content_rect.
 */
void button_geomnotifyfunc(ei_widget_t *widget, ei_rect_t rect);

/**
 * \brief	Allow to update geometry of a widget of type \ref ei_frame_t.
 *
 * @param	widget		The subject widget.
 * @param	rect		The updated content_rect.
 */
void frame_geomnotifyfunc(ei_widget_t *widget, ei_rect_t rect);

/**
 * \brief	Allow to update geometry of a widget of type \ref ei_toplevel_t.
 *
 * @param	widget		The subject widget.
 * @param	rect		The updated content_rect.
 */
void toplevel_geomnotifyfunc(ei_widget_t *widget, ei_rect_t rect);

/**
 * \brief	Allow to update geometry of a widget of type \ref ei_radiobutton_t.
 *
 * @param	widget		The subject widget.
 * @param	rect		The updated content_rect.
 */
void radiobutton_geomnotifyfunc(ei_widget_t *widget, ei_rect_t rect);

/**
 * \brief	Allow to add the rect that cover the pixels eventually modified by the widget.
 *
 * @param	widget		The subject widget.
 * @param	rect		The old screen location.
 */
void add_widget_updated_rects(ei_widget_t *widget, ei_rect_t rect);

/* handlefunc */
/**
 * \brief	Handle the internal events for widget of type \ref ei_button_t .
 *
 * @param	widget		The widget which receive the event.
 * @param	event		The received event.
 */
ei_bool_t button_handlefunc(ei_widget_t *widget, ei_event_t *event);

/**
 * \brief	Handle the internal events for widget of type \ref ei_frame_t .
 *
 * @param	widget		The widget which receive the event.
 * @param	event		The received event.
 */
ei_bool_t frame_handlefunc(ei_widget_t *widget, ei_event_t *event);

/**
 * \brief	Handle the internal events for widget of type \ref ei_toplevel_t .
 *
 * @param	widget		The widget which receive the event.
 * @param	event		The received event.
 */
ei_bool_t toplevel_handlefunc(ei_widget_t *widget, ei_event_t *event);

/**
 * \brief	Handle the internal events for widget of type \ref ei_radiobutton_t .
 *
 * @param	widget		The widget which receive the event.
 * @param	event		The received event.
 */
ei_bool_t radiobutton_handlefunc(ei_widget_t *widget, ei_event_t *event);

/* radiobutton configure */
/**
 * \brief	Allows to configure a widget of type \ref ei_radiobutton_t.
 */
void ei_radiobutton_configure(ei_widget_t *widget,
                              ei_size_t *requested_size,
                              uint32_t *margin,
                              ei_color_t *background_color,
                              ei_color_t *outline_color,
                              ei_color_t *button_color,
                              ei_color_t *selected_color,
                              char **title,
                              ei_font_t *text_font,
                              ei_color_t *text_color,
                              ei_anchor_t *text_anchor
);

/**
 * \brief	Add a new button to the radiobutton with the text and the callback.
 */
void append_radiobutton(ei_radiobutton_t *radiobutton, char *text, ei_callback_t *callback);

/**
 * \brief	Free the radiobutton.
 */
void clear_radiobutton(ei_radiobutton_t *radiobutton);

#endif //PROJETC_IG_WIDGETS_H
