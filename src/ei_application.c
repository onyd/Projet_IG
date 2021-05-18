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

    widget_compt = 0;
    create_vector(&pick_vector, 1);

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

    // offscreen for picking
    picking_offscreen = hw_surface_create(main_window, hw_surface_get_size(main_window), EI_FALSE);
}

void ei_app_free(void) {
    ei_widget_list_t to_delete = {NULL, NULL, NULL};
    widget_breadth_list(&(root->widget), &to_delete);
    ei_linked_widget_t *current = to_delete.head;
    while (current != NULL) {
        current->widget->wclass->releasefunc(current->widget);
        current = current->next;
    }
    free_vector(&pick_vector);
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
        if (event.type == ei_ev_keydown) {
            ei_app_quit_request();
        }
        // Draw
        hw_surface_lock(main_window);

        draw_window(root);

        /*ei_widget_list_t children = {NULL, NULL, NULL};
        widget_breadth_list(root, &children);
        ei_linked_widget_t *current = children.head;
        //ei_linked_rect_t *rect_to_update;
        //rect_to_update->rect = *(current->widget->content_rect);
        while (current != NULL) {
            //rect_to_update = rect_to_update->next;
            //rect_to_update->rect = *(current->widget->content_rect);
            ei_placer_run(current->widget);
            current->widget->wclass->drawfunc(current->widget, main_window, picking_offscreen, clipping_window);
            current = current->next;
        }
        //rect_to_update->next = NULL;
        free_linked_widget(children.head);
        */
        hw_surface_unlock(main_window);
        hw_surface_update_rects(main_window, NULL);

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
    ei_surface_t surface = hw_surface_create(main_window, hw_surface_get_size(main_window), EI_FALSE);
    return surface;
}


