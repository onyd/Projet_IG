#include "utils.h"
#include "stdio.h"
#include "stdlib.h"
#include "ei_utils.h"
#include "hw_interface.h"

void append_left_lae(struct linked_edges *e, struct linked_acive_edges *lae) {
    if (lae->head == NULL) {
        lae->head = e;
        return;
    }
    e->next = lae->head;
    lae->head = e;
}

void delete_y(int y, struct linked_acive_edges *lae) {
    if (lae->head == NULL) {
        return;
    }

    struct linked_edges *previous = lae->head;
    while (previous != NULL && previous->ymax == y) {
        struct linked_edges *tmp = previous;
        previous = previous->next;
        free(tmp);
    }
    lae->head = previous;
    if (lae->head == NULL) {
        return;
    }

    struct linked_edges *current = lae->head->next;
    while (current != NULL) {
        if (current->ymax == y) {
            previous->next = current->next;
            free(current);
            current = previous->next;
            continue;
        }
        previous = current;
        current = current->next;
    }
}

void display(struct linked_acive_edges *lae) {
    struct linked_edges *current = lae->head;
    while (current != NULL) {
        printf("[%i, %i]->", current->ymax, current->x_ymin);
        current = current->next;
    }
    printf("\n");
}


void swap(int *a, int *b) {
    int tmp = *a;
    (*a) = *b;
    (*b) = tmp;
}


struct ei_linked_point_t *y_argmax(struct ei_linked_point_t *a, struct ei_linked_point_t *b) {
    if (a->point.y > b->point.y) {
        return a;
    } else {
        return b;
    }
}

struct ei_linked_point_t *y_argmin(struct ei_linked_point_t *a, struct ei_linked_point_t *b) {
    if (a->point.y < b->point.y) {
        return a;
    } else {
        return b;
    }
}

void sorting_insert(struct linked_edges *tc, struct linked_acive_edges *lae) {
    if (lae->head == NULL) {
        lae->head = tc;
        return;
    }
    struct linked_edges *previous = lae->head;
    struct linked_edges *current = previous;
    while (current != NULL) {
        // à savoir: il ne peut pas y avoir plus de deux côtés qui ont le même x_ymin
        if (current->x_ymin == tc->x_ymin) {
            //si c'est à la tête:
            if (current == previous) {
                if (current->x_ymax > tc->x_ymax) {
                    append_left_lae(tc, lae);
                } else {
                    tc->next = current->next;
                    current->next = tc;
                }
            } else {
                if (current->x_ymax > tc->x_ymax) {
                    previous->next = tc;
                    tc->next = current;
                } else {
                    tc->next = current->next;
                    current->next = tc;
                }
            }
            return;
        }
        if (current->x_ymin > tc->x_ymin) {
            //si c'est la tete
            if (current == previous) {
                append_left_lae(tc, lae);
            } else {
                previous->next = tc;
                tc->next = current;
            }
            return;
        }
        previous = current;
        current = current->next;
    }
    if (current == NULL) {
        previous->next = tc;
    }
}

void append_linked_point(struct ei_linked_point_t *p, struct ei_linked_point_t *l) {
    if (l == NULL) {
        l = p;
        return;
    }

    p->next = l;
    l = p;
}

void le_free(struct linked_edges *tc) {
    struct linked_edges *next_tc = tc;
    while (tc != NULL) {
        next_tc = tc->next;
        free(tc);
        tc = next_tc;
    }
}

void lae_free(struct linked_acive_edges *lae) {
    le_free(lae->head);
    free(lae);
}


ei_point_t topleft(ei_point_t anchor_point, ei_size_t size, const ei_anchor_t *anchor) {
    switch (*anchor) {
        case ei_anc_none:
            break;
        case ei_anc_center:
            anchor_point = ei_point_sub(anchor_point, ei_point(0.5 * size.width, 0.5 * size.height));
            break;
        case ei_anc_north:
            anchor_point = ei_point_sub(anchor_point, ei_point(0.5 * size.width, 0));
            break;
        case ei_anc_northeast:
            anchor_point = ei_point_sub(anchor_point, ei_point(size.width, 0));
            break;
        case ei_anc_east:
            anchor_point = ei_point_sub(anchor_point, ei_point(size.width, 0.5 * size.height));
            break;
        case ei_anc_southeast:
            anchor_point = ei_point_sub(anchor_point, ei_point(size.width, size.height));
            break;
        case ei_anc_south:
            anchor_point = ei_point_sub(anchor_point, ei_point(0.5 * size.width, size.height));
            break;
        case ei_anc_southwest:
            anchor_point = ei_point_sub(anchor_point, ei_point(0, size.height));
            break;
        case ei_anc_west:
            anchor_point = ei_point_sub(anchor_point, ei_point(0, 0.5 * size.height));
            break;
        case ei_anc_northwest:
            break;
    }
    return anchor_point;
}

ei_point_t anchor_point(ei_point_t topleft, ei_size_t size, const ei_anchor_t *anchor) {
    switch (*anchor) {
        case ei_anc_none:
            break;
        case ei_anc_center:
            topleft = ei_point_add(topleft, ei_point(0.5 * size.width, 0.5 * size.height));
            break;
        case ei_anc_north:
            topleft = ei_point_add(topleft, ei_point(0.5 * size.width, 0));
            break;
        case ei_anc_northeast:
            topleft = ei_point_add(topleft, ei_point(size.width, 0));
            break;
        case ei_anc_east:
            topleft = ei_point_add(topleft, ei_point(size.width, 0.5 * size.height));
            break;
        case ei_anc_southeast:
            topleft = ei_point_add(topleft, ei_point(size.width, size.height));
            break;
        case ei_anc_south:
            topleft = ei_point_add(topleft, ei_point(0.5 * size.width, size.height));
            break;
        case ei_anc_southwest:
            topleft = ei_point_add(topleft, ei_point(0, size.height));
            break;
        case ei_anc_west:
            topleft = ei_point_add(topleft, ei_point(0, 0.5 * size.height));
            break;
        case ei_anc_northwest:
            break;
    }
    return topleft;
}

ei_point_t anchor_target_pos(ei_anchor_t anchor, ei_size_t target_size, ei_rect_t parent_rect, int radius, int border) {
    int width = target_size.width;
    int height = target_size.height;
    radius = radius*(1 - sqrt(2)/2);
    ei_point_t topleft;
    switch (anchor) {
        case ei_anc_none:
            topleft.y = parent_rect.top_left.y + border + radius;
            topleft.x = parent_rect.top_left.x + border + radius;
            break;
        case ei_anc_center:
            topleft.y = parent_rect.top_left.y + parent_rect.size.height / 2 - height / 2;
            topleft.x = parent_rect.top_left.x + parent_rect.size.width / 2 - width / 2;
            break;
        case ei_anc_north:
            topleft.y = parent_rect.top_left.y + border;
            topleft.x = parent_rect.top_left.x + parent_rect.size.width / 2 - width / 2;
            break;
        case ei_anc_northeast:
            topleft.y = parent_rect.top_left.y + border + radius;
            topleft.x = parent_rect.top_left.x + parent_rect.size.width - width - border - radius;
            break;
        case ei_anc_east:
            topleft.y = parent_rect.top_left.y + parent_rect.size.height / 2 - height / 2;
            topleft.x = parent_rect.top_left.x + parent_rect.size.width - width - border;
            break;
        case ei_anc_southeast:
            topleft.y = parent_rect.top_left.y + parent_rect.size.height - height - border - radius;
            topleft.x = parent_rect.top_left.x + parent_rect.size.width - width - border - radius;
            break;
        case ei_anc_south:
            topleft.y = parent_rect.top_left.y + parent_rect.size.height - height - border;
            topleft.x = parent_rect.top_left.x + parent_rect.size.width / 2 - width / 2;
            break;
        case ei_anc_southwest:
            topleft.y = parent_rect.top_left.y + parent_rect.size.height - height - border - radius;
            topleft.x = parent_rect.top_left.x + border + radius;
            break;
        case ei_anc_west:
            topleft.y = parent_rect.top_left.y + parent_rect.size.height / 2 - height / 2;
            topleft.x = parent_rect.top_left.x + border;
            break;
        case ei_anc_northwest:
            topleft.y = parent_rect.top_left.y + border + radius;
            topleft.x = parent_rect.top_left.x + border + radius;
            break;
    }
    return topleft;
}

ei_color_t ei_map_rgba_inverse(ei_surface_t surface, uint32_t color_id){
    int ir, ig, ib, ia;
    hw_surface_get_channel_indices(surface, &ir, &ig, &ib, &ia);
    uint32_t r, g, b, a = 0;
    ei_color_t color;
    r = color_id & (255 << (8 * ir));
    color.red = r >> (8 * ir);
    g = color_id & (255 << (8 * ig));
    color.green = g >> (8 * ig);
    b = color_id & (255 << (8 * ib));
    color.blue = b >> (8 * ib);
    ia = 6 - ir - ig - ib;
    a = color_id & (255 << (8 * ia));
    color.alpha = a >> (8 * ia);
    return color;
}
