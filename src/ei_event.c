#include "ei_event.h"
#include "eventhandler.h"

void ei_event_set_active_widget(ei_widget_t *widget) {
    active_widget = widget;
}

ei_widget_t *ei_event_get_active_widget(void) {
    return active_widget;
}


void ei_event_set_default_handle_func(ei_default_handle_func_t func) {
    default_handle_func = func;
}


ei_default_handle_func_t ei_event_get_default_handle_func(void) {
    return default_handle_func;
}



