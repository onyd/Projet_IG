#include "widgets.h"
#include "ei_application.h"
#include "eventhandler.h"

ei_default_handle_func_t default_handle_func;
ei_widget_t *active_widget = NULL;

ei_bool_t event_propagation(ei_widget_t *widget, ei_event_t *event) {
    if (widget->wclass->handlefunc(widget, event)) {
        return true;
    }

    // Event propagation on children
    ei_widget_t *current = widget->children_head;
    while (current != NULL) {
        if (event_propagation(current, event))
            return true;
        current = current->next_sibling;
    }

    return false;
}

void handle_event(ei_event_t *event) {
    ei_widget_t *root = ei_app_root_widget();

    switch (event->type) {
        case ei_ev_mouse_move:
            *mouse_pos = event->param.mouse.where;
        case ei_ev_mouse_buttondown :
        case ei_ev_mouse_buttonup:
            if (active_widget != NULL) {
                if (active_widget->wclass->handlefunc(active_widget, event))
                    (*default_handle_func)(event);
            } else {
                ei_widget_t *picked = ei_widget_pick(&event->param.mouse.where);
                if (picked->wclass->handlefunc(picked, event))
                    (*ei_event_get_default_handle_func())(event);
            }
            break;
        case ei_ev_keydown:
        case ei_ev_keyup:
            if (!event_propagation(root, event))
                (*default_handle_func)(event);
            break;
        case ei_ev_app:
            switch (((user_param_t *) event->param.application.user_param)->app_event_type) {
                case toplevel_param:
                    user_param_t *user_params = event->param.application.user_param;
                    toplevel_app_event_t *toplevel_params = ((user_param_t *) event->param.application.user_param)->data;
                    ei_toplevel_t *toplevel = toplevel_params->caller;
                    if (!inside(get_mouse_pos(), &toplevel->grab_event.param.minimize_square)) {
                        toplevel->grab_event.param.show_minimize_square = false;
                        free(user_params->data);
                        free(user_params);
                    }
                    toplevel->grab_event.param.unshow_minimize_square_event_sent = false;

                    break;
                case user_param:
                    break;
            }
            break;
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
