#ifndef PROJETC_IG_DEFAULTS_H
#define PROJETC_IG_DEFAULTS_H

#include "hw_interface.h"
#include "widgets.h"
#include "utils.h"

// App values
static ei_surface_t _main_window;
static ei_frame_t *_root;
static ei_surface_t _pick_surface;
static vector *_pick_vector;
static ei_rect_list_t _updated_rects;
static ei_point_t _mouse_pos;
static ei_point_t _prev_mouse_pos;
static ei_rect_t *_clipping_window;
static bool _quit_request;

// Default values
static ei_size_t _toplevel_default_size;
static ei_size_t _toplevel_default_min_size;
static int _toplevel_default_border_width;
static ei_color_t _default_color;
static ei_color_t _default_text_color;
static ei_size_t _default_size;
static ei_relief_t _default_relief;
static ei_anchor_t _default_anchor;
static ei_color_t _default_button_color;
static ei_color_t _default_selected_color;

// Getter
/**
 * @brief Return the main_window surface
 */
ei_surface_t get_main_window();

/**
 * @brief Return the root widget as ei_frame_t
 */
ei_frame_t *get_root_widget();

/**
 * @brief Return the surface used for picking
 */
ei_surface_t get_pick_surface();

/**
 * @brief Return the vector used to retrieve widget by picking
 */
vector *get_pick_vector();

/**
 * @brief Return the rects which have been modified as a linked list
 */
ei_rect_list_t *get_updated_rects();

/**
 * @brief Return the mouse position surface
 */
ei_point_t get_mouse_pos();

/**
 * @brief Return the previous mouse position surface
 */
ei_point_t get_prev_mouse_pos();

/**
 * @brief Return the rect of main_window
 */
ei_rect_t *get_clipper_window();

/**
 * @brief Return quit_window boolean
 */
ei_bool_t get_quit_request();

/**
 * @brief Return the default toplevel size
 */
ei_size_t *get_toplevel_default_size();

/**
 * @brief Return the default toplevel minimum size
 */
ei_size_t *get_toplevel_default_min_size();

/**
 * @brief Return the default toplevel border width
 */
int *get_toplevel_default_border_width();

/**
 * @brief Return the default widget color
 */
ei_color_t *get_default_color();

/**
 * @brief Return the default text color
 */
ei_color_t *get_default_text_color();

/**
 * @brief Return the default widget size
 */
ei_size_t *get_default_size();

/**
 * @brief Return the default widget relief
 */
ei_relief_t *get_default_relief();

/**
 * @brief Return the default anchor (see \ref ei_anchor_t)
 */
ei_anchor_t *get_default_anchor();

/**
 * @brief Return the default button color
 */
ei_color_t *get_default_button_color();

/**
 * @brief Return the default selected color for ei_radiobutton_t
 */
ei_color_t *get_default_selected_color();

// Setter
/**
 * @brief Set the main_window surface
 */
void set_main_window(ei_surface_t main_window);

/**
 * @brief Set the root widget as ei_frame_t
 */
void set_root_widget(ei_widget_t *root);

/**
 * @brief Set the surface used for picking
 */
void set_pick_surface(ei_surface_t pick_surface);

/**
 * @brief Set the vector used to retrieve widget by picking
 */
void set_pick_vector(vector *pick_vector);

/**
 * @brief Set the rects which have been modified as a linked list
 */
void set_updated_rects(ei_rect_list_t updated_rects);

/**
 * @brief Return the mouse position surface
 */
void set_mouse_pos(ei_point_t pos);

/**
 * @brief Return the previous mouse position surface
 */
void set_prev_mouse_pos(ei_point_t pos);

/**
 * @brief Return the rect of main_window
 */
void set_clipper_window(ei_rect_t *clipping_window);

/**
 * @brief Return quit_window boolean
 */
void set_quit_request(ei_bool_t quit_request);

#endif //PROJETC_IG_DEFAULTS_H
