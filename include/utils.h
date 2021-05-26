#ifndef PROJETC_IG_UTILS_H
#define PROJETC_IG_UTILS_H

#include "stdint.h"
#include "ei_types.h"
#include "ei_widget.h"

/**
 * \brief	linked structure that stores data for scanline algorithm
 */
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

/**
 * \brief	list structure for \ref linked_edges used to store active edges in scanline algorithm
 */
typedef struct linked_acive_edges {
    struct linked_edges *head;
} linked_acive_edges;

/**
 * \brief	Linked list of widget
 */
typedef struct ei_linked_widget_t {
    ei_widget_t *widget;
    struct ei_linked_widget_t *next;
} ei_linked_widget_t;

/**
 * @brief Linked list of float error
 */
typedef struct ei_linked_error_t {
    float error;
    struct ei_linked_error_t *next;
} ei_linked_error_t;

/**
 * @brief Linked list of float error
 */
typedef struct ei_linked_text_t {
    char* text;
    struct ei_linked_text_t *next;
} ei_linked_text_t;

/**
 * \brief	List structure for \ref ei_point_t
 */
typedef struct ei_point_list_t {
    ei_linked_point_t *head;
    ei_linked_point_t *tail;
} ei_point_list_t;

/**
 * \brief	List structure for \ref ei_rect_t
 */
typedef struct ei_rect_list_t {
    ei_linked_rect_t *head;
} ei_rect_list_t;

/**
 * \brief	List structure for \ref ei_widget_t
 */
typedef struct ei_widget_list_t {
    ei_linked_widget_t *head;
    ei_linked_widget_t *tail;
} ei_widget_list_t;

/**
 * \brief	List structure for \ref float
 */
typedef struct ei_error_list_t {
    ei_linked_error_t *head;
    ei_linked_error_t *tail;
} ei_error_list_t;


/**
 * \brief	List structure for \ref char*
 */
typedef struct ei_text_list_t {
    ei_linked_text_t *head;
    ei_linked_text_t *tail;
} ei_text_list_t;

/**
 * \brief	Swap the variable content pointed by a and b
 *
 * @param	a 	uint32_t variable.
 * @param	b   uint32_t variable.
 */
void swap(int *a, int *b);

/**
 * @brief	Append left the linked_edges e to the linked_active_edges lae.
 */
void append_left_lae(linked_edges *e, linked_acive_edges *lae);

/**
 * @brief	Delete all linked_edges structures contained in lae that have ymax = y.
 */
void delete_y(int y, linked_acive_edges *lae);

/**
 * @brief	Display ymax and x_ymin of all linked_edges structures contained in lae.
 */
void display(linked_acive_edges *lae);

/**
 * @brief	Insert an linked_edges structure le in a linked_acive_edges structure lae with insertion sort.
 */
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

/**
 * \brief	Append to the right p in the ei_point_list_t structure l.
 */
void append_linked_point(ei_point_t p, ei_point_list_t *l);

/**
 * @brief	Free the ei_linked_point_t structure l.
 */
void free_linked_point(ei_linked_point_t *l);

/**
 * \brief	Append to the right rect in the ei_rect_list_t structure l.
 */
void append_left_linked_rect(ei_rect_t rect, ei_rect_list_t *l);

/**
 * @brief	Free the ei_linked_rect_t structure l.
 */
void free_linked_rect(ei_linked_rect_t *l);

/**
 * \brief	Append to the left a widget in the ei_widget_list_t structure l.
 */
void append_left_linked_widget(ei_widget_t *widget, ei_widget_list_t *l);

/**
 * \brief	Append to the right a widget in the ei_widget_list_t structure l.
 */
void append_linked_widget(ei_widget_t *widget, ei_widget_list_t *l);

/**
 * @brief	Free the ei_widget_rect_t structure l.
 */
void free_linked_widget(ei_linked_widget_t *l);

/**
 * \brief	Append to the left a float error in the ei_error_list_t structure l.
 */
void append_linked_error(float error, ei_error_list_t *l);

/**
 * @brief	Free the ei_widget_rect_t structure l.
 */
void free_linked_error(ei_linked_error_t *l);

/**
 * @brief	Compute the top left point of an anchor_point knowing the size and the anchor.
 */
ei_point_t topleft(ei_point_t anchor_point, ei_size_t size, const ei_anchor_t *anchor);

/**
 * @brief	Compute the anchor point of a top left knowing the size and the anchor.
 */
ei_point_t anchor_point(ei_point_t topleft, ei_size_t size, const ei_anchor_t *anchor);

/**
 * @brief	Compute the target position knowing the size, the anchor, the parent_rect, the radius and the border.
 */
ei_point_t anchor_target_pos(ei_anchor_t anchor, ei_size_t target_size, ei_rect_t parent_rect, int radius, int border);

/**
 * \brief	Converts 32 bits integer into a color composed of the red, green, blue and alpha components.
 * 		The surface parameter provides the channel order.
 *
 * @param	surface		The surface where to store this pixel, provides the channels order.
 * @param	color_id	The 32 bit integer corresponding to the color to convert.
 *
 * @return 		The ei_color_t corresponding to the color.
 */
ei_color_t ei_map_rgba_inverse(ei_surface_t surface, uint32_t color_id);

#endif //PROJETC_IG_UTILS_H
