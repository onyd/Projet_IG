#ifndef PROJETC_IG_UTILS_H
#define PROJETC_IG_UTILS_H

#include "stdint.h"
#include "ei_types.h"
#include "hw_interface.h"
#include "eventhandler.h"

typedef struct linked_edges {
    int ymax;
    int x_ymin;
    int x_ymax;
    int E;
    int dx;
    int dy;
    int sign_dx;
    struct linked_edges *next;
} linked_edges;

typedef struct linked_acive_edges {
    struct linked_edges *head;
} linked_acive_edges;

typedef struct ei_linked_widget_t {
    ei_widget_t *widget;
    struct ei_linked_widget_t *next;
} ei_linked_widget_t;

typedef struct ei_point_list_t {
    struct ei_linked_point_t *head;
} ei_point_list_t;

typedef struct ei_rect_list_t {
    struct ei_linked_rect_t *head;
} ei_rect_list_t;

typedef struct ei_widget_list_t {
    struct ei_linked_widget_t *head;
} ei_widget_list_t;

/**
 * \brief	Swap the variable content pointed by a and b
 *
 * @param	a 	uint32_t variable.
 * @param	b   uint32_t variable.
 */
void swap(int *a, int *b);

void append_left_lae(linked_edges *e, linked_acive_edges *lae);

void delete_y(int y, linked_acive_edges *lae);

void display(linked_acive_edges *lae);

void sorting_insert(linked_edges *le, linked_acive_edges *lae);

/**
 * \brief	free the structure linked_edges
 *
 * @param	 	struct linked_edges*.
 */
void le_free(linked_edges *);

/**
 * \brief	free the structure linked_acive_edges
 *
 * @param	 	struct linked_acive_edges*.
 */
void lae_free(linked_acive_edges *);

/**
 * \brief	Return the point which have the minimum y coordinates
 *
 * @param	a 	struct ei_linked_point_t the first point.
 * @param	b   struct ei_linked_point_t the other point.
 */
struct ei_linked_point_t *y_argmax(ei_linked_point_t *a, ei_linked_point_t *b);

/**
 * \brief	Return the point which have the maximum y coordinates
 *
 * @param	a 	struct ei_linked_point_t the first point.
 * @param	b   struct ei_linked_point_t the other point.
 */
struct ei_linked_point_t *y_argmin(ei_linked_point_t *a, ei_linked_point_t *b);

void append_linked_point(ei_point_t p, ei_point_list_t *l);

void free_linked_point(ei_linked_point_t *l);

void append_linked_rect(ei_rect_t rect, ei_rect_list_t *l);

void free_linked_rect(ei_linked_rect_t *l);

void append_linked_widget(ei_widget_t *widget, ei_widget_list_t *l);

void free_linked_widget(ei_linked_widget_t *l);

ei_point_t topleft(ei_point_t anchor_point, ei_size_t size, const ei_anchor_t *anchor);

ei_point_t anchor_point(ei_point_t topleft, ei_size_t size, const ei_anchor_t *anchor);

ei_point_t anchor_target_pos(ei_anchor_t anchor, ei_size_t target_size, ei_rect_t parent_rect, int radius, int border);

ei_color_t ei_map_rgba_inverse(ei_surface_t surface, uint32_t color_id);

#endif //PROJETC_IG_UTILS_H
