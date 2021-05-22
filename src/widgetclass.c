#include "widgetclass.h"

#include <string.h>

// Class declarations
ei_widgetclass_t *frame_class;
ei_widgetclass_t *button_class;
ei_widgetclass_t *toplevel_class;
ei_widgetclass_t *radiobutton_class;

ei_bool_t is_button(ei_widget_t *widget) {
    return strcmp(widget->wclass->name, "button");
}

ei_bool_t is_frame(ei_widget_t *widget) {
    return strcmp(widget->wclass->name, "frame");
}

ei_bool_t is_toplevel(ei_widget_t *widget) {
    return strcmp(widget->wclass->name, "toplevel");
}
