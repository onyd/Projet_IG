#include "hw_interface.h"
#include "ei_widgetclass.h"
#include "stdbool.h"
#include <string.h>
#include "ei_application.h"
#include <stdlib.h>
#include "widgets.h"
#include "eventhandler.h"
#include "vector.h"
#include "widgetclass.h"
#include "defaults.h"
#include "utils.h"

void ei_app_create(ei_size_t main_window_size, ei_bool_t fullscreen) {
    hw_init();

    // Class init
    button_class = malloc(sizeof(ei_widgetclass_t));
    frame_class = malloc(sizeof(ei_widgetclass_t));
    toplevel_class = malloc(sizeof(ei_widgetclass_t));

    strcpy(button_class->name, "button");
    strcpy(frame_class->name, "frame");
    strcpy(toplevel_class->name, "toplevel");

    ei_widgetclass_register(button_class);
    ei_widgetclass_register(frame_class);
    ei_widgetclass_register(toplevel_class);

    // Defaults init
    ei_default_font = hw_text_font_create(ei_default_font_filename, ei_style_normal, ei_font_default_size);

    ei_rect_t *clipping_window = malloc(sizeof(ei_rect_t));
    *clipping_window = ei_rect(ei_point(0, 0), main_window_size);
    set_clipper_window(clipping_window);

    default_handle_func = malloc(sizeof(ei_default_handle_func_t));
    default_handle_func = &always_true;

    set_quit_request(false);

    // Picking init
    set_pick_vector(create_vector(1));

    ei_linked_rect_t *updated_rects = calloc(1, sizeof(ei_linked_rect_t));
    updated_rects->rect = *get_clipper_window();
    set_updated_rects(updated_rects);

    set_mouse_pos(ei_point_zero());
    set_prev_mouse_pos(ei_point_zero());

    // root init
    set_root_widget(frame_allocfunc());
    get_root_widget()->widget.wclass = frame_class;
    get_root_widget()->widget.wclass->setdefaultsfunc(get_root_widget());
    get_root_widget()->widget.requested_size = main_window_size;
    ei_rect_t *root_rect = malloc(sizeof(ei_rect_t));
    *root_rect = ei_rect(ei_point(0, 0), main_window_size);
    get_root_widget()->widget.screen_location = *root_rect;
    get_root_widget()->widget.content_rect = root_rect;
    get_root_widget()->widget.pick_id = 0;
    get_root_widget()->widget.pick_color = malloc(sizeof(ei_color_t));
    *(get_root_widget()->widget.pick_color) = (ei_color_t) {0, 0, 0, 0};

    ei_surface_t main_window;
    if (fullscreen == false) {
        main_window = hw_create_window(main_window_size, EI_FALSE);
    } else {
        main_window = hw_create_window(ei_size(900, 700), EI_TRUE);
    }
    set_main_window(main_window);

    set_pick_surface(hw_surface_create(get_main_window(), hw_surface_get_size(get_main_window()), EI_FALSE));
}

void ei_app_free(void) {
    // Clear all widgets
    ei_widget_t *current = ei_app_root_widget()->children_head;
    while (current != NULL) {
        ei_widget_t *tmp = current;
        current = current->next_sibling;
        ei_widget_destroy(tmp);
    }

    free_vector(get_pick_vector());
    free(get_updated_rects());

    // Free classes
    free(frame_class);
    free(button_class);
    free(toplevel_class);

    // Free defaults
    free(get_clipper_window());
    //hw_text_font_free(ei_default_font); seems to be already freed by hw_quit()
    hw_quit();
}

void ei_app_run(void) {
    ei_event_t event;

    event.type = ei_ev_none;
    while (get_quit_request() == false) {
        if (event.type == ei_ev_keydown && event.param.key.key_code == 27) {
            ei_app_quit_request();
        }

        // Recursively place children of root widget ie all widgets
        ei_widget_t *current = ei_app_root_widget()->children_head;
        while (current != NULL) {
            ei_placer_run(current);
            current = current->next_sibling;
        }

        // Draw
        hw_surface_lock(get_main_window());
        draw_window();
        hw_surface_unlock(get_main_window());
        hw_surface_update_rects(get_main_window(), get_updated_rects());

        free_linked_rect(get_updated_rects());
        append_linked_rect(ei_rect_zero(), get_updated_rects());

        // Event handling
        hw_event_wait_next(&event);
        handle_event(&event);
    }
}

void ei_app_invalidate_rect(ei_rect_t *rect) {

}

void ei_app_quit_request(void) {
    set_quit_request(true);
}

ei_widget_t *ei_app_root_widget(void) {
    return (ei_widget_t *) get_root_widget();
}

ei_surface_t ei_app_root_surface(void) {
    ei_surface_t surface = hw_surface_create(get_main_window(), hw_surface_get_size(get_main_window()), EI_FALSE);
    return surface;
}


