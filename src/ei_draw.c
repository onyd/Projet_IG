#include "ei_draw.h"
#include "utils.h"
#include "stdio.h"
#include "math.h"
#include "stdbool.h"

uint32_t ei_map_rgba(ei_surface_t surface, ei_color_t color) {
    int ir, ig, ib, ia;
    hw_surface_get_channel_indices(surface, &ir, &ig, &ib, &ia);
    uint32_t r, g, b, a = 0;
    r = (uint32_t) color.red << (8 * ir);
    g = (uint32_t) color.green << (8 * ig);
    b = (uint32_t) color.blue << (8 * ib);

    if (ia != -1) {
        return r | g | b | a;
    } else {
        return r | g | b;
    }
}

void ei_draw_polyline(ei_surface_t surface,
                      const ei_linked_point_t *first_point,
                      ei_color_t color,
                      const ei_rect_t *clipper) {
    const ei_linked_point_t *first = first_point;
    const ei_linked_point_t *second = first_point->next;

    uint32_t *pixels = (uint32_t *) hw_surface_get_buffer(surface);
    ei_size_t size = hw_surface_get_size(surface);

    // Clipper coordinates
    int top_left_x, top_right_x, top_left_y, bottom_left_y;
    if (clipper != NULL){
        top_left_x = clipper -> top_left.x;
        top_right_x = clipper -> top_left.x + clipper -> size.width;
        top_left_y = clipper -> top_left.y;
        bottom_left_y = clipper -> top_left.y + clipper -> size.height;
    }

    // Draw all lines between points
    while (first != NULL && second != NULL) {
        /* Bresenham */
        int dx = second->point.x - first->point.x;
        int dy = second->point.y - first->point.y;
        int x = first->point.x;
        int y = first->point.y;

        uint32_t c = ei_map_rgba(surface, color);
        int sign_x = (dx > 0) ? 1 : -1;
        int sign_y = (dy > 0) ? 1 : -1;

        if (dx == 0) {
            while (y != second->point.y) {
                pixels[x + size.width * y] = c;
                y += sign_y;
            }
            return;
        }
        if (dy == 0) {
            while (x != second->point.x) {
                pixels[x + size.width * y] = c;
                x += sign_x;
            }
            return;
        }

        // Swap variable for y-directed line
        bool swapped = abs(dx) < abs(dy);
        int x2 = second->point.x;
        int y2 = second->point.y;

        if (swapped) {
            swap(&x, &y);
            swap(&dx, &dy);
            swap(&sign_x, &sign_y);
            swap(&x2, &y2);
        }
        int E = dx/2;

        while (sign_x * x < sign_x * x2) {
            x += sign_x;
            E += abs(dy);

            if (2 * E > abs(dx)) {
                y += sign_y;
                E -= abs(dx);
            }
            // Draw pixel in the buffer
            if (clipper == NULL || x >= top_left_x && x <= top_right_x && y >= top_left_y && y <= bottom_left_y){
                if (!swapped) {
                    pixels[x + size.width * y] = c;
                } else {
                    pixels[y + size.width * x] = c;
                }
            }
        }
        first = second;
        second = second->next;
    }
}


void ei_draw_polygon(ei_surface_t surface,
                     const ei_linked_point_t *first_point,
                     ei_color_t color,
                     const ei_rect_t *clipper) {
    //Paramètres pour modifier l'image
    uint32_t *pixels = (uint32_t *) hw_surface_get_buffer(surface);
    ei_size_t size = hw_surface_get_size(surface);
    uint32_t c = ei_map_rgba(surface, color);
    // on crée la table des côtés
    const ei_linked_point_t *debut = first_point;
    const ei_linked_point_t *prec = first_point;
    const ei_linked_point_t *current = first_point->next;
    struct table_cote *tab_cote = malloc(sizeof(struct table_cote));
    struct table_cote *parcourt = tab_cote;
    //initialisation de la table de côté actif
    struct table_cote_actif *tab_cote_actif = malloc(sizeof(struct table_cote_actif));
    tab_cote_actif->head = NULL;
    // Donne le ymin du polygone et le côté correspondant à ce ymin
    int ycur = prec->point.y;
    do {
        //On ignore les côtés horizontaux qui sont inutiles
        if (prec->point.y == current->point.y) {
            int ymax = max(prec->point.y, current->point.y);
            parcourt->ymax = ymax;
            if (prec->point.y == ymax) {
                parcourt->xpmin = current->point.x;
                parcourt->xpmax = prec->point.x;
                parcourt->ymin = current->point.y;
            }
            else {
                parcourt->xpmin = prec->point.x;
                parcourt->xpmax = current->point.x;
                parcourt->ymin = prec->point.y;
            }
            int dx = current->point.x - prec->point.x;
            int dy = current->point.y - prec->point.y;
            parcourt->E = 0;
            parcourt->dx = dx;
            parcourt->dy = dy;
        }
        parcourt = parcourt->next;
        prec = prec->next;
        current = current->next;
        ycur = (ycur > prec->point.y) ? prec->point.y : ycur;
    } while(prec != debut);
    parcourt->next = NULL;

    /* On initialise la table des côté actifs en se
     * placant à la premiere scanline correspondant à
     * y = ymin avec ymin le côté minimum de tout les points du polygone
     * puis on parcourt les lignes jusqu'à ce que la TCA et la TC soient vides*/
    struct table_cote *parcourt_prec = tab_cote;
    while (tab_cote != NULL && tab_cote_actif->head != NULL) {
        parcourt = tab_cote;
        parcourt_prec = tab_cote;
        // On rajoute dans TCA les nouveaux points TC
        while(parcourt != NULL) {
            if (parcourt->ymin == ycur) {
                //On rajoute ce côté à la table des côtés actif de manière triés

                //Puis on supprime ce côté de TC
                //on regarde si c'est la tête qui est supprimé
                if (parcourt == parcourt_prec) {
                    tab_cote = parcourt->next;
                }
                else {
                    parcourt_prec->next = parcourt->next;
                }
            }
            parcourt_prec = parcourt;
            parcourt = parcourt->next;
        }

        // On supprime les coté de TCA tq y = ymax
        parcourt = tab_cote_actif->head;
        parcourt_prec = parcourt;
        while (parcourt != NULL) {
            if (parcourt->ymax == ycur) {
                //si on supprime la tête
                if (parcourt == parcourt_prec) {
                    tab_cote_actif->head = parcourt->next;
                }
                else {
                    parcourt_prec->next = parcourt->next;
                }
                parcourt = parcourt->next;
            }
            else {
                parcourt_prec = parcourt;
                parcourt = parcourt->next;
            }
        }

        //on remplit les pixels
        parcourt = tab_cote_actif->head;
        while (parcourt != NULL) {
            int x1 = parcourt->xpmin;
            parcourt = parcourt->next;
            int x2 = parcourt->xpmin;
            for (int i=x1; i<x2; i++) {
                pixels[i + size.width * ycur];
            }
        }

        // On incrémente y
        ycur += 1;

        // On fait Bresenham pour avoir les nouvelles valeurs de x
        parcourt = tab_cote_actif->head;
        while (parcourt != NULL) {
            int dx = parcourt->dx;
            int dy = parcourt->dy;
            int sign_x = (dx > 0) ? 1 : -1;
            int sign_y = (dy > 0) ? 1 : -1;
            parcourt->E += abs(dx);
            if (2 * parcourt->E > abs(dy)) {
                parcourt->xpmin += sign_x;
                parcourt->E -= abs(dy);
            }
        }
    }
    free(tab_cote);
    free(tab_cote_actif);
}

void ei_draw_text(ei_surface_t surface,
                  const ei_point_t *where,
                  const char *text,
                  ei_font_t font,
                  ei_color_t color,
                  const ei_rect_t *clipper) {

}

void ei_fill(ei_surface_t surface,
             const ei_color_t *color,
             const ei_rect_t *clipper) {
    hw_surface_lock(surface);
    uint32_t *pixels = (uint32_t *) hw_surface_get_buffer(surface);
    ei_size_t size = hw_surface_get_size(surface);
    uint32_t c = ei_map_rgba(surface, *color);
    for (uint32_t i = 0; i < size.width * size.height; i++) {
        pixels[i] = c;
    }
    hw_surface_unlock(surface);
}

int ei_copy_surface(ei_surface_t destination,
                    const ei_rect_t *dst_rect,
                    ei_surface_t source,
                    const ei_rect_t *src_rect,
                    ei_bool_t alpha) {

}

