#include "defaults.h"

// App values
static ei_surface_t _main_window;
static ei_frame_t *_root;
static ei_surface_t _pick_surface;
static vector *_pick_vector;
static ei_linked_rect_t *_updated_rects;
static ei_point_t _mouse_pos;
static ei_point_t _prev_mouse_pos;
static ei_rect_t *_clipping_window;
static bool _quit_request = true;

// Default values
static ei_size_t _toplevel_default_size = {320, 240};
static ei_size_t _toplevel_default_min_size = {160, 120};
static int _toplevel_default_border_width = 4;

static ei_color_t _default_color = {100, 100, 100, 255};
static ei_color_t _default_text_color = {0, 0, 0, 255};
static ei_color_t _default_button_color = {200, 200, 200, 255};
static ei_color_t _default_selectioned_color = {255, 0, 0, 255};

static ei_size_t _default_size = {100, 100};
static ei_relief_t _default_relief = ei_relief_raised;
static ei_anchor_t _default_anchor = ei_anc_center;

// Getter
ei_surface_t get_main_window() {
    return _main_window;
}

ei_frame_t *get_root_widget() {
    return _root;
}

ei_surface_t get_pick_surface() {
    return _pick_surface;
}

vector *get_pick_vector() {
    return _pick_vector;
}

ei_linked_rect_t *get_updated_rects() {
    return _updated_rects;
}

ei_point_t get_mouse_pos() {
    return _mouse_pos;
}

ei_point_t get_prev_mouse_pos() {
    return _prev_mouse_pos;
}

ei_rect_t *get_clipper_window() {
    return _clipping_window;
}

ei_bool_t get_quit_request() {
    return _quit_request;
}

ei_size_t *get_toplevel_default_size() {
    return &_toplevel_default_size;
}

ei_size_t *get_toplevel_default_min_size() {
    return &_toplevel_default_min_size;
}

int *get_toplevel_default_border_width() {
    return &_toplevel_default_border_width;
}

ei_color_t *get_default_color() {
    return &_default_color;
}

ei_color_t *get_default_text_color() {
    return &_default_text_color;
}

ei_size_t *get_default_size() {
    return &_default_size;
}

ei_relief_t *get_default_relief() {
    return &_default_relief;
}

ei_anchor_t *get_default_anchor() {
    return &_default_anchor;
}

ei_color_t *get_default_button_color() {
    return &_default_button_color;
}

ei_color_t *get_default_selected_color() {
    return &_default_selectioned_color;
}

// Setter
void set_main_window(ei_surface_t main_window) {

    _main_window = main_window;
}

void set_root_widget(ei_widget_t *root) {
    _root = (ei_frame_t *) root;
}

void set_pick_surface(ei_surface_t pick_surface) {
    _pick_surface = pick_surface;
}

void set_pick_vector(vector *pick_vector) {
    _pick_vector = pick_vector;
}

void set_updated_rects(ei_linked_rect_t *updated_rects) {
    _updated_rects = updated_rects;
}

void set_mouse_pos(ei_point_t pos) {
    _mouse_pos = pos;
}

void set_prev_mouse_pos(ei_point_t pos) {
    _prev_mouse_pos = pos;
}

void set_clipper_window(ei_rect_t *clipper_window) {
    _clipping_window = clipper_window;
}

void set_quit_request(ei_bool_t quit_request) {
    _quit_request = quit_request;
}
