#include "ei_widgetclass.h"
#include "string.h"
#include "widgets.h"

void ei_widgetclass_register(ei_widgetclass_t *widgetclass) {
    if (strcmp(widgetclass->name, "widget") == 0) {
        widgetclass->allocfunc = &widget_allocfunc;
        widgetclass->releasefunc = &widget_releasefunc;
        widgetclass->drawfunc = &widget_drawfunc;
        widgetclass->setdefaultsfunc = &widget_setdefaultsfunc;
        widgetclass->geomnotifyfunc = &widget_geomnotifyfunc;
        widgetclass->handlefunc = &widget_handlefunc;
    }
    if (strcmp(widgetclass->name, "button") == 0) {
        widgetclass->allocfunc = &button_allocfunc;
        widgetclass->releasefunc = &button_releasefunc;
        widgetclass->drawfunc = &button_drawfunc;
        widgetclass->setdefaultsfunc = &button_setdefaultsfunc;
        widgetclass->geomnotifyfunc = &button_geomnotifyfunc;
        widgetclass->handlefunc = &button_handlefunc;
    }
    if (strcmp(widgetclass->name, "frame") == 0) {
        widgetclass->allocfunc = &frame_allocfunc;
        widgetclass->releasefunc = &frame_releasefunc;
        widgetclass->drawfunc = &frame_drawfunc;
        widgetclass->setdefaultsfunc = &frame_setdefaultsfunc;
        widgetclass->geomnotifyfunc = &frame_geomnotifyfunc;
        widgetclass->handlefunc = &frame_handlefunc;
    }
}


ei_widgetclass_t *ei_widgetclass_from_name(ei_widgetclass_name_t name) {
    if (strcmp(name, "button") == 0) {
        return button_class;
    }
    if (strcmp(name, "frame") == 0) {
        return frame_class;
    }
    if (strcmp(name, "widget") == 0) {
        return widget_class;
    }
}
