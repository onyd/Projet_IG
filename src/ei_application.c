#include "hw_interface.h"
#include "ei_widgetclass.h"
#include "stdbool.h"
#include "string.h"
#include "ei_application.h"
#include "stdlib.h"
#include "widgets.h"
#include "eventhandler.h"
#include "vector.h"

void ei_app_create(ei_size_t main_window_size, ei_bool_t fullscreen) {
    hw_init();
    quit_request = false;
    // Class init
    widget_class = malloc(sizeof(ei_widgetclass_t));
    button_class = malloc(sizeof(ei_widgetclass_t));
    frame_class = malloc(sizeof(ei_widgetclass_t));
    toplevel_class = malloc(sizeof(ei_widgetclass_t));

    strcpy(widget_class->name, "widget");
    strcpy(button_class->name, "button");
    strcpy(frame_class->name, "frame");
    strcpy(toplevel_class->name, "toplevel");

    ei_widgetclass_register(widget_class);
    ei_widgetclass_register(button_class);
    ei_widgetclass_register(frame_class);
    ei_widgetclass_register(toplevel_class);

    // Defaults init
    ei_default_font = hw_text_font_create(ei_default_font_filename, ei_style_normal, ei_font_default_size);

    default_color = malloc(sizeof(ei_color_t));
    *default_color = (ei_color_t) {100, 100, 100, 255};

    default_text_color = malloc(sizeof(ei_color_t));
    *default_text_color = (ei_color_t) {0, 0, 0, 255};

    default_size = malloc(sizeof(ei_size_t));
    *default_size = (ei_size_t) {100, 100};

    default_relief = malloc(sizeof(ei_relief_t));
    *default_relief = ei_relief_raised;

    default_anchor = malloc((sizeof(ei_anchor_t)));
    *default_anchor = ei_anc_center;

    toplevel_default_size = malloc(sizeof(ei_size_t));
    *toplevel_default_size = ei_size(320, 240);

    toplevel_default_min_size = malloc(sizeof(ei_size_t));
    *toplevel_default_min_size = ei_size(160, 120);

    toplevel_default_border_width = malloc(sizeof(ei_size_t));
    *toplevel_default_border_width = 4;

    clipping_window = malloc(sizeof(ei_rect_t));
    *clipping_window = ei_rect(ei_point(0, 0), main_window_size);

    default_handle_func = malloc(sizeof(ei_default_handle_func_t));
    default_handle_func = &always_true;

    // Picking init
    widget_counter = 0;
    pick_vector = create_vector(1);

    updated_rects = calloc(1, sizeof(ei_linked_rect_t));
    updated_rects->rect = *get_clipper_window();

    mouse_pos = calloc(1, sizeof(ei_point_t));
    *mouse_pos = ei_point_zero();

    last_event = malloc(sizeof(ei_event_t));

    // root init
    root = frame_allocfunc();
    root->widget.wclass = frame_class;
    root->widget.wclass->setdefaultsfunc(root);
    root->widget.requested_size = main_window_size;
    ei_rect_t *root_rect = malloc(sizeof(ei_rect_t));
    *root_rect = ei_rect(ei_point(0, 0), main_window_size);
    root->widget.screen_location = *root_rect;
    root->widget.content_rect = root_rect;
    root->widget.pick_id = 0;
    root->widget.pick_color = malloc(sizeof(ei_color_t));
    *(root->widget.pick_color) = (ei_color_t) {0, 0, 0, 0};

    if (fullscreen == false) {
        main_window = hw_create_window(main_window_size, EI_FALSE);
    } else {
        main_window = hw_create_window(ei_size(900, 700), EI_TRUE);
    }

    pick_surface = hw_surface_create(get_main_window(), hw_surface_get_size(main_window), EI_FALSE);
}

void ei_app_free(void) {
    // Clear all widgets
    ei_widget_t *current = ei_app_root_widget()->children_head;
    while (current != NULL) {
        ei_widget_destroy(current);
        current = current->next_sibling;
    }

    free_vector(pick_vector);
    free(updated_rects);
    free(mouse_pos);

    // Free classes
    free(frame_class);
    free(button_class);
    free(widget_class);
    free(toplevel_class);

    // Free defaults
    free(default_color);
    free(default_text_color);
    free(default_size);
    free(default_relief);
    free(default_anchor);
    free(toplevel_default_border_width);
    free(toplevel_default_min_size);
    free(toplevel_default_size);
    free(clipping_window);
    //hw_text_font_free(ei_default_font); seems to be already freed by hw_quit()
    hw_quit();
}

void ei_app_run(void) {
    ei_event_t event;

    event.type = ei_ev_none;
    while (quit_request == false) {
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
        hw_surface_lock(main_window);
        draw_window();

        hw_surface_unlock(get_main_window());
        hw_surface_update_rects(get_main_window(), NULL);
        ei_linked_rect_t *current_rect = updated_rects->next;
        while (current_rect != NULL) {
            ei_linked_rect_t *tmp = current_rect;
            current_rect = current_rect->next;
            free(tmp);
        }
        updated_rects->rect = ei_rect_zero();
        updated_rects->next = NULL;

        // Event handling
        hw_event_wait_next(&event);
        handle_event(&event);
    }
}

void ei_app_invalidate_rect(ei_rect_t *rect) {

}

void ei_app_quit_request(void) {
    quit_request = true;
}

ei_widget_t *ei_app_root_widget(void) {
    return root;
}

ei_surface_t ei_app_root_surface(void) {
    ei_surface_t surface = hw_surface_create(get_main_window(), hw_surface_get_size(main_window), EI_FALSE);
    return surface;
}


