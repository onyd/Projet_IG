#ifndef PROJETC_IG_DEFAULTS_H
#define PROJETC_IG_DEFAULTS_H

#include "hw_interface.h"
#include "widgets.h"

// App values
static ei_surface_t _main_window;
static ei_frame_t *_root;
static ei_surface_t _pick_surface;
static vector *_pick_vector;
static ei_linked_rect_t *_updated_rects;
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
static ei_color_t _default_selectioned_color;

// Getter
ei_surface_t get_main_window();

ei_frame_t *get_root_widget();

ei_surface_t get_pick_surface();

vector *get_pick_vector();

ei_linked_rect_t *get_updated_rects();

ei_point_t get_mouse_pos();

ei_point_t get_prev_mouse_pos();

ei_rect_t *get_clipper_window();

ei_bool_t get_quit_request();

ei_size_t *get_toplevel_default_size();

ei_size_t *get_toplevel_default_min_size();

int *get_toplevel_default_border_width();

ei_color_t *get_default_color();

ei_color_t *get_default_text_color();

ei_size_t *get_default_size();

ei_relief_t *get_default_relief();

ei_anchor_t *get_default_anchor();

ei_color_t *get_default_button_color();

ei_color_t *get_default_selectioned_color();

// Setter
void set_main_window(ei_surface_t main_window);

void set_root_widget(ei_widget_t *root);

void set_pick_surface(ei_surface_t pick_surface);

void set_pick_vector(vector *pick_vector);

void set_updated_rects(ei_linked_rect_t *updated_rects);

void set_mouse_pos(ei_point_t pos);

void set_prev_mouse_pos(ei_point_t pos);

void set_clipper_window(ei_rect_t *clipping_window);

void set_quit_request(ei_bool_t quit_request);

#endif //PROJETC_IG_DEFAULTS_H
