#include "hw_interface.h"
#include "widgets.h"
#include "ei_widgetclass.h"
#include "stdbool.h"
#include "string.h"
#include "ei_application.h"

void ei_app_create(ei_size_t main_window_size, ei_bool_t fullscreen){
    hw_init();
    strcpy(button_class -> name, "button");
    strcpy(widget_class -> name, "widget");
    strcpy(frame_class -> name, "frame");
    ei_widgetclass_register(button_class);
    ei_widgetclass_register(widget_class);
    ei_widgetclass_register(frame_class);

    if (fullscreen == false){
        hw_create_window(main_window_size, EI_FALSE);
    }
    /* else{
        hw_create_window(hw_surface_get_size(ei_app_root_surface()), EI_TRUE);
    } */
}

void ei_app_free(void){

}

void ei_app_run(void){

}

void ei_app_invalidate_rect(ei_rect_t* rect){

}

void ei_app_quit_request(void){

}

ei_widget_t* ei_app_root_widget(void){
    return NULL;
}

/* ei_surface_t ei_app_root_surface(void){
} */


