#include "ei_widget.h"
#include "widgets.h"

ei_widget_t*		ei_widget_create		(ei_widgetclass_name_t	class_name,
                                             ei_widget_t*		parent,
                                             void*			user_data,
                                             ei_widget_destructor_t destructor){
//    if (class_name == "frame_class"){
//        frame_allocfunc();
//    }
//    else if (class_name == "button_class"){
//        button_allocfunc()
//    }
//    else if (class_name == "widget_class"){
//
//    }
//    return NULL;
}

void			ei_widget_destroy		(ei_widget_t*		widget){
    ei_widget_list_t children;
    widget_breadth_list(widget, &children);
    ei_linked_widget_t *current;
    current = children.head;
    while (current != children.tail) {
        ei_widget_t *tmp = current->widget;
        current = current->next;
        tmp->wclass->releasefunc(tmp);
    }
}


ei_widget_t*		ei_widget_pick			(ei_point_t*		where){
    return NULL;
}

void			ei_frame_configure		(ei_widget_t*		widget,
                                           ei_size_t*		requested_size,
                                           const ei_color_t*	color,
                                           int*			border_width,
                                           ei_relief_t*		relief,
                                           char**			text,
                                           ei_font_t*		text_font,
                                           ei_color_t*		text_color,
                                           ei_anchor_t*		text_anchor,
                                           ei_surface_t*		img,
                                           ei_rect_t**		img_rect,
                                           ei_anchor_t*		img_anchor){
    ei_frame_t *frame = (ei_frame_t *) widget;
    frame->widget.requested_size = (requested_size != NULL) ? *requested_size : frame->widget.requested_size;
    frame->color = (color != NULL) ?  color : frame->color;
    frame->border_width = (border_width != NULL) ? border_width : frame->border_width;
    frame->relief = (relief != NULL) ? relief : frame->relief;
    frame->text = (text != NULL) ? text : frame->text;
    frame->text_font = (text_font != NULL) ? text_font : frame->text_font;
    frame->text_color = (text_color != NULL) ? text_color : frame->text_color;
    frame->text_anchor = (text_anchor != NULL) ? text_anchor : frame->text_anchor;
    frame->img = (img != NULL) ? img : frame->img;
    frame->img_rect = (img_rect != NULL) ? img_rect : frame->img_rect;
    frame->img_anchor = (img_anchor != NULL) ? img_anchor : frame->img_anchor;
}
void			ei_button_configure		(ei_widget_t*		widget,
                                            ei_size_t*		requested_size,
                                            const ei_color_t*	color,
                                            int*			border_width,
                                            int*			corner_radius,
                                            ei_relief_t*		relief,
                                            char**			text,
                                            ei_font_t*		text_font,
                                            ei_color_t*		text_color,
                                            ei_anchor_t*		text_anchor,
                                            ei_surface_t*		img,
                                            ei_rect_t**		img_rect,
                                            ei_anchor_t*		img_anchor,
                                            ei_callback_t*		callback,
                                            void**			user_param){
    ei_button_t *button = (ei_button_t *) widget;
    button->widget.requested_size = (requested_size != NULL) ? *requested_size : button->widget.requested_size;
    button->color = (color != NULL) ?  color : button->color;
    button->border_width = (border_width != NULL) ? border_width : button->border_width;
    button->corner_radius = corner_radius;
    button->relief = (relief != NULL) ? relief : button->relief;
    button->text = (text != NULL) ? text : button->text;
    button->text_font = (text_font != NULL) ? text_font : button->text_font;
    button->text_color = (text_color != NULL) ? text_color : button->text_color;
    button->text_anchor = (text_anchor != NULL) ? text_anchor : button->text_anchor;
    button->img = (img != NULL) ? img : button->img;
    button->img_rect = (img_rect != NULL) ? img_rect : button->img_rect;
    button->img_anchor = (img_anchor != NULL) ? img_anchor : button->img_anchor;
    button->callback = (callback !=NULL) ? callback : button->callback;
    button->widget.user_data = (user_param != NULL) ? *user_param : button->widget.user_data;
}


void			ei_toplevel_configure		(ei_widget_t*		widget,
                                              ei_size_t*		requested_size,
                                              ei_color_t*		color,
                                              int*			border_width,
                                              char**			title,
                                              ei_bool_t*		closable,
                                              ei_axis_set_t*		resizable,
                                              ei_size_t**		min_size){

}