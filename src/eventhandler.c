#include "widgets.h"
#include "ei_application.h"

ei_default_handle_func_t *default_handle_func;
ei_widget_t *active_widget;

void event_propagation(ei_widget_t *widget, ei_event_t *event) {
    ei_bool_t succes = widget->wclass->handlefunc(widget, event);

    if (!succes) {
        (*default_handle_func)(event);
    }

    // Event propagation on children
    ei_widget_t *current = widget->children_head;
    while (current != NULL) {
        event_propagation(current, event);
        current = current->next_sibling;
    }
}

void handle_event(ei_event_t *event) {
    ei_widget_t *root = ei_app_root_widget();
    event_propagation(root, event);
}

void identity(ei_event_t *event) {
    return;
}