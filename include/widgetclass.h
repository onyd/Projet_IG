#ifndef PROJETC_IG_WIDGETCLASS_H
#define PROJETC_IG_WIDGETCLASS_H
#include "ei_widgetclass.h"
#include "widgets.h"


// Class declarations
/**
 * @brief	Declaration of different classes.
 */
extern ei_widgetclass_t *frame_class;
extern ei_widgetclass_t *button_class;
extern ei_widgetclass_t *toplevel_class;
extern ei_widgetclass_t *radiobutton_class;

/**
 * @brief	check if a widget is a button or not.
 */
ei_bool_t is_button(ei_widget_t *widget);

/**
 * @brief	check if a widget is a frame or not.
 */
ei_bool_t is_frame(ei_widget_t *widget);

/**
 * @brief	check if a widget is a toplevel or not.
 */
ei_bool_t is_toplevel(ei_widget_t *widget);

#endif //PROJETC_IG_WIDGETCLASS_H
