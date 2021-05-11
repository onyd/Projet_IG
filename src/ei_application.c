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

    widget_class = malloc(sizeof(ei_widgetclass_t));
    button_class = malloc(sizeof(ei_widgetclass_t));
    frame_class = malloc(sizeof(ei_widgetclass_t));

    strcpy(widget_class->name, "widget");
    strcpy(button_class->name, "button");
    strcpy(frame_class->name, "frame");
    ei_widgetclass_register(button_class);
    ei_widgetclass_register(widget_class);
    ei_widgetclass_register(frame_class);

    root = frame_allocfunc();

    if (fullscreen == false) {
        main_window = hw_create_window(main_window_size, EI_FALSE);
    }
    /* else{
         main_window = hw_create_window(hw_surface_get_size(ei_app_root_surface()), EI_TRUE);
    } */
}

void ei_app_free(void) {
    ei_widget_list_t *to_delete;
    widget_deep_list(&(root->widget), to_delete);
    ei_linked_widget_t *current = to_delete->head;
    while(current != NULL){
        current->widget->wclass->releasefunc(current);
        current = current->next;
    }
    frame_releasefunc(&(root->widget));
    free(frame_class);
    free(button_class);
    free(widget_class);
    free(default_color);
    free(default_text_color);
    free(widget_class);
    hw_quit();
}

void ei_app_run(void) {
    ei_rect_t root_clipper = hw_surface_get_rect(main_window);
    root->widget.wclass->drawfunc(root, main_window, NULL, &root_clipper);



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


