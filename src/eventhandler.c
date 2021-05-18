#include "widgets.h"
#include "ei_application.h"

ei_default_handle_func_t default_handle_func;
ei_widget_t *active_widget = NULL;

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
    if (event->type == ei_ev_mouse_buttondown) {
        //if it's the top bar of a toplevel, then it's active and wait for the mouse to move
        /** Temporary **/
        ei_event_set_active_widget(root->children_head);
        ei_toplevel_t * active_toplevel = (ei_toplevel_t *) active_widget;
        active_toplevel->active = event->param.mouse.where;
    }
    if (active_widget == NULL) {
        event_propagation(root, event);
    }
    else {
        active_widget->wclass->handlefunc(active_widget, event);
    }
}


void destroy_widget_callback(ei_widget_t *widget,
                    struct ei_event_t *event,
                    void *user_param) {
    ei_widget_destroy(widget);
}

ei_bool_t always_true(ei_event_t *event) {
    return true;
}
