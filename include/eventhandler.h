#ifndef PROJETC_IG_EVENTHANDLER_H
#define PROJETC_IG_EVENTHANDLER_H

#include "ei_event.h"

extern ei_default_handle_func_t default_handle_func;
extern ei_widget_t *active_widget;

void event_propagation(ei_widget_t *widget, ei_event_t *event);

void handle_event(ei_event_t *event);

void identity(ei_event_t *event);

ei_bool_t always_true(ei_event_t *event);

#endif //PROJETC_IG_EVENTHANDLER_H
