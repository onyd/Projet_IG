#include "hw_interface.h"
#include "widgets.h"
#include "ei_widgetclass.h"
#include "stdbool.h"
#include "string.h"
#include "ei_application.h"
#include "stdlib.h"
#include "stdio.h"

void ei_app_create(ei_size_t main_window_size, ei_bool_t fullscreen) {
    hw_init();

    // Class init
    widget_class = malloc(sizeof(ei_widgetclass_t));
    button_class = malloc(sizeof(ei_widgetclass_t));
    frame_class = malloc(sizeof(ei_widgetclass_t));

    strcpy(widget_class->name, "widget");
    strcpy(button_class->name, "button");
    strcpy(frame_class->name, "frame");

    ei_widgetclass_register(widget_class);
    ei_widgetclass_register(button_class);
    ei_widgetclass_register(frame_class);

    // Defaults init
    ei_default_font = hw_text_font_create(ei_default_font_filename, ei_style_normal, ei_font_default_size);
    default_color = malloc(sizeof(ei_color_t));
    *default_color = (ei_color_t) {100, 100, 100, 255};
    default_text_color = malloc(sizeof(ei_color_t));
    *default_text_color = (ei_color_t) {0, 0, 0, 255};
    default_size = malloc(sizeof(ei_size_t));
    *default_size = ei_size(100, 100);
    default_relief = malloc(sizeof(ei_relief_t));
    *default_relief = ei_relief_raised;

    // root init
    root = frame_allocfunc();
    root->widget.wclass = frame_class;
    root->widget.wclass->setdefaultsfunc(root);

    if (fullscreen == false) {
        main_window = hw_create_window(main_window_size, EI_FALSE);
    }
    /* else{
         main_window = hw_create_window(hw_surface_get_size(ei_app_root_surface()), EI_TRUE);
    } */
}

void ei_app_free(void) {

}

void ei_app_run(void) {
    ei_rect_t root_clipper = hw_surface_get_rect(main_window);
    root->widget.wclass->drawfunc(root, main_window, NULL, &root_clipper);

    ei_widget_list_t children;
    widget_breadth_list(root, &children);
    ei_linked_widget_t *current = children.head;
    while (current != NULL) {
        current->widget->wclass->drawfunc(current->widget, main_window, NULL, current->widget->parent->content_rect);
    }
    free_linked_widget(children.head);
    getchar();
}

void ei_app_invalidate_rect(ei_rect_t *rect) {

}

void ei_app_quit_request(void) {

}

ei_widget_t *ei_app_root_widget(void) {
    return root;
}

ei_surface_t ei_app_root_surface(void) {
    return hw_surface_create(main_window, hw_surface_get_size(root), EI_FALSE);
}


