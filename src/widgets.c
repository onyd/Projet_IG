#include "widgets.h"
#include "stdlib.h"

/* allocfunc */
ei_widget_t *widget_allocfunc() {
    ei_widget_t *widget = calloc(1, sizeof(ei_widget_t));
    return widget;
}

ei_widget_t *button_allocfunc() {
    ei_widget_t *widget = (ei_button_t *) calloc(1, sizeof(ei_button_t));
    return widget;
}

ei_widget_t *frame_allocfunc() {
    ei_widget_t *widget = (ei_frame_t *) calloc(1, sizeof(ei_frame_t));
    return widget;
}

/* releasefunc */
void widget_releasefunc(struct ei_widget_t *widget) {
    free(widget);
}

void button_releasefunc(struct ei_widget_t *widget) {
    free((ei_button_t *) widget);
}

void frame_releasefunc(struct ei_widget_t *widget) {
    free((ei_frame_t *) widget);
}

/* drawfunc */

/* setdefaultsfunc */