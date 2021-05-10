#include "ei_widget.h"
#include "widgets.h"

ei_widget_t*		ei_widget_create		(ei_widgetclass_name_t	class_name,
                                             ei_widget_t*		parent,
                                             void*			user_data,
                                             ei_widget_destructor_t destructor){
    return NULL;
}

void			ei_widget_destroy		(ei_widget_t*		widget){

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
    frame->widget.requested_size = *requested_size;
    frame->color = color;
    frame->border_width = border_width;
    frame->relief = relief;
    frame->text = text;
    frame->text_font = text_font;
    frame->text_color = text_color;
    frame->text_anchor;
    frame->img = img;
    frame->img_rect = img_rect;
    frame->img_anchor = img_anchor;
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
    button->widget.requested_size = *requested_size;
    button->color = color;
    button->border_width = border_width;
    button->corner_radius = corner_radius;
    button->relief = relief;
    button->text = text;
    button->text_font = text_font;
    button->text_color = text_color;
    button->text_anchor = text_anchor;
    button->img = img;
    button->img_rect = img_rect;
    button->img_anchor = img_anchor;
    button->callback = callback;
    button->widget.user_data = *user_param;
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