#include "ei_widgetclass.h"
#include <string.h>
#include "widgets.h"
#include "widgetclass.h"

void ei_widgetclass_register(ei_widgetclass_t *widgetclass) {
    if (strcmp(widgetclass->name, "button") == 0) {
        widgetclass->allocfunc = &button_allocfunc;
        widgetclass->releasefunc = &button_releasefunc;
        widgetclass->drawfunc = &button_drawfunc;
        widgetclass->setdefaultsfunc = &button_setdefaultsfunc;
        widgetclass->geomnotifyfunc = &button_geomnotifyfunc;
        widgetclass->handlefunc = &button_handlefunc;
    }
    else if (strcmp(widgetclass->name, "frame") == 0) {
        widgetclass->allocfunc = &frame_allocfunc;
        widgetclass->releasefunc = &frame_releasefunc;
        widgetclass->drawfunc = &frame_drawfunc;
        widgetclass->setdefaultsfunc = &frame_setdefaultsfunc;
        widgetclass->geomnotifyfunc = &frame_geomnotifyfunc;
        widgetclass->handlefunc = &frame_handlefunc;
    }
    else if (strcmp(widgetclass->name, "toplevel") == 0) {
        widgetclass->allocfunc = &toplevel_allocfunc;
        widgetclass->releasefunc = &toplevel_releasefunc;
        widgetclass->drawfunc = &toplevel_drawfunc;
        widgetclass->setdefaultsfunc = &toplevel_setdefaultsfunc;
        widgetclass->geomnotifyfunc = &toplevel_geomnotifyfunc;
        widgetclass->handlefunc = &toplevel_handlefunc;
    }
    if (strcmp(widgetclass->name, "radiobutton") == 0) {
        widgetclass->allocfunc = &radiobutton_allocfunc;
        widgetclass->releasefunc = &radiobutton_releasefunc;
        widgetclass->drawfunc = &radiobutton_drawfunc;
        widgetclass->setdefaultsfunc = &radiobutton_setdefaultsfunc;
        widgetclass->geomnotifyfunc = &radiobutton_geomnotifyfunc;
        widgetclass->handlefunc = &radiobutton_handlefunc;
    }
}


ei_widgetclass_t *ei_widgetclass_from_name(ei_widgetclass_name_t name) {
    if (strcmp(name, "button") == 0) {
        return button_class;
    }
    if (strcmp(name, "frame") == 0) {
        return frame_class;
    }
    if (strcmp(name, "toplevel") == 0) {
        return toplevel_class;
    }
    if (strcmp(name, "radiobutton") == 0) {
        return radiobutton_class;
    }

    return NULL;
}
