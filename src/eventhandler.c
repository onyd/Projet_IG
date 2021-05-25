#include "widgets.h"
#include "ei_application.h"
#include "eventhandler.h"
#include "defaults.h"
#include "geometry.h"

ei_default_handle_func_t default_handle_func;
ei_widget_t *active_widget = NULL;

ei_bool_t event_propagation(ei_widget_t *widget, ei_event_t *event) {
    ei_widget_list_t l = {NULL};
    inverse_depth_widget_list(widget, &l);

    // Event propagation on children
    ei_linked_widget_t *current = l.head;
    while (current != NULL) {
        if (current->widget->wclass->handlefunc(current->widget, event)) {
            free_linked_widget(l.head);
            return true;
        }
        current = current->next;
    }

    free_linked_widget(l.head);
    return false;
}

void handle_event(ei_event_t *event) {
    ei_widget_t *root = ei_app_root_widget();

    switch (event->type) {
        case ei_ev_mouse_move:
            set_prev_mouse_pos(get_mouse_pos());
            set_mouse_pos(event->param.mouse.where);
        case ei_ev_mouse_buttondown :
        case ei_ev_mouse_buttonup:
            if (active_widget != NULL) {
                if (active_widget->wclass->handlefunc(active_widget, event))
                    (*ei_event_get_default_handle_func())(event);
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
                // Used for minimize_square unshow
                case toplevel_param: {
                    user_param_t *user_params = event->param.application.user_param;
                    toplevel_app_event_t *toplevel_params = ((user_param_t *) event->param.application.user_param)->data;
                    ei_toplevel_t *toplevel = toplevel_params->caller;
                    if (!inside(get_mouse_pos(), &toplevel->grab_event.param.minimize_square)) {
                        toplevel->grab_event.param.show_minimize_square = false;
                        ei_rect_t inter_rect;
                        intersection_rect(get_clipper_window(), &(toplevel->widget.screen_location), &inter_rect);
                        append_left_linked_rect(inter_rect, get_updated_rects());
                        free(user_params->data);
                        free(user_params);
                    }
                    toplevel->grab_event.param.unshow_minimize_square_event_sent = false;
                    break;
                }
                case user_param:
                    break;
            }
            break;
    }
}

void close_toplevel_callback(ei_widget_t *widget,
                             struct ei_event_t *event,
                             void *user_param) {
    if (((ei_toplevel_t *) widget)->grab_event.grab_type == clicked){
        ei_widget_destroy(widget);
    }
}

ei_bool_t always_true(ei_event_t *event) {
    return true;
}

void inverse_depth_widget_list(ei_widget_t *widget, ei_widget_list_t *result) {
    // Add current
    append_left_linked_widget(widget, result);

    // Recursively add children
    ei_widget_t *current_child = widget->children_head;
    while (current_child != NULL) {
        inverse_depth_widget_list(current_child, result);
        current_child = current_child->next_sibling;
    }
}



