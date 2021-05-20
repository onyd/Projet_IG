#ifndef PROJETC_IG_EVENTHANDLER_H
#define PROJETC_IG_EVENTHANDLER_H

#include "ei_event.h"
#include "widgets.h"

typedef struct ei_linked_widget_t {
    ei_widget_t *widget;
    struct ei_linked_widget_t *next;
} ei_linked_widget_t;

typedef struct ei_widget_list_t {
    struct ei_linked_widget_t *head;
    struct ei_linked_widget_t *tail;
} ei_widget_list_t;

typedef enum {
    toplevel_param = 0,
    user_param
} user_param_type_t;

typedef struct user_param_t {
    user_param_type_t app_event_type;
    void *data;
} user_param_t;

typedef struct toplevel_app_event_t {
    ei_toplevel_t *caller;
} toplevel_app_event_t;

extern ei_default_handle_func_t default_handle_func;
extern ei_widget_t *active_widget;

ei_bool_t event_propagation(ei_widget_t *widget, ei_event_t *event);

void handle_event(ei_event_t *event);

ei_bool_t always_true(ei_event_t *event);

void destroy_widget_callback(ei_widget_t *widget,
                    struct ei_event_t *event,
                    void *user_param);

void inverse_depth_widget_list(ei_widget_t *widget, struct ei_widget_list_t *result);

void free_linked_widget(struct ei_linked_widget_t *l);

#endif //PROJETC_IG_EVENTHANDLER_H
