#include <stdio.h>
#include <stdlib.h>

#include "ei_application.h"
#include "ei_event.h"
#include "hw_interface.h"
#include "ei_widget.h"


/*
 * ei_main --
 *
 *	Main function of the application.
 */
int main(int argc, char **argv) {
    ei_size_t screen_size = {600, 600};
    ei_color_t root_bgcol = {0x52, 0x7f, 0xb4, 0xff};

    ei_widget_t *frame;
    ei_size_t frame_size = {300, 200};
    int frame_x = 200;
    int frame_y = 200;
    ei_color_t frame_color = {0x88, 0x88, 0x88, 0xff};
    ei_relief_t frame_relief = ei_relief_raised;
    int frame_border_width = 6;

    /* Create the application and change the color of the background. */
    ei_app_create(screen_size, EI_FALSE);
    ei_frame_configure(ei_app_root_widget(), NULL, &root_bgcol, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

    /*create a toplevel */
    int toplevel_x = 100;
    int toplevel_y = 150;
    ei_widget_t *toplevel = ei_widget_create("toplevel", ei_app_root_widget(), NULL, NULL);
    ei_toplevel_configure(toplevel, &frame_size, NULL,
                          NULL, NULL, NULL, NULL, NULL);
    ei_place(toplevel, NULL, &toplevel_x, &toplevel_y, NULL, NULL, NULL, NULL, NULL, NULL);
    int width = 300;
    int height = 200;
    ei_place(toplevel, NULL, NULL, NULL, &width, &height, NULL, NULL, NULL, NULL);
    /* Create, configure and place the frame on screen. */
    // text on the frame
    char *text = "Toto";
    ei_anchor_t text_anchor = ei_anc_center;
    frame = ei_widget_create("frame", toplevel, NULL, NULL);
    ei_frame_configure(frame, &frame_size, &frame_color,
                       &frame_border_width, &frame_relief, &text, NULL, NULL, &text_anchor,
                       NULL, NULL, NULL);
    float relative = 0;
    float rel_size = 0.2;
    int x = 5;
    int y = 5;
    ei_place(frame, NULL, &x, &y, NULL, NULL, NULL, NULL, &rel_size, &rel_size);

    ei_anchor_t text_anchor2 = ei_anc_center;
    ei_widget_t *frame2 = ei_widget_create("frame", toplevel, NULL, NULL);
    ei_frame_configure(frame2, &frame_size, &frame_color,
                       &frame_border_width, &frame_relief, NULL, NULL, NULL, NULL,
                       NULL, NULL, NULL);
    int x2 = 150;
    int y2 = 100;
    ei_place(frame2, NULL, &x2, &y2, NULL, NULL, NULL, NULL, &rel_size, &rel_size);

    /*second toplevel */
    int toplevel_x_2 = 200;
    int toplevel_y_2 = 300;
    ei_widget_t *toplevel_2 = ei_widget_create("toplevel", ei_app_root_widget(), NULL, NULL);
    ei_place(toplevel_2, NULL, &toplevel_x_2, &toplevel_y_2, NULL, NULL, NULL, NULL, NULL, NULL);

    float rel_3 = 0;
    float rel_size_3 = 1;
    ei_widget_t *toplevel_3 = ei_widget_create("toplevel", toplevel_2, NULL, NULL);
    ei_size_t toplevel_size_3 = {100, 50};
    ei_size_t **min_size = malloc(sizeof(ei_size_t *));
    *min_size = &toplevel_size_3;
    ei_toplevel_configure(toplevel_3, &toplevel_size_3, NULL,
                          NULL, NULL, NULL, NULL, min_size);
    ei_place(toplevel_3, NULL, NULL, NULL, NULL, NULL, &rel_3, &rel_3, &rel_size_3, &rel_size_3);
    /* Run the application's main loop. */
    ei_app_run();

    /* We just exited from the main loop. Terminate the application (cleanup). */
    ei_app_free();

    return (EXIT_SUCCESS);
}
