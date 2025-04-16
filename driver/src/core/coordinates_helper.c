#include "coordinates_helper.h"
#include <stdio.h>

void parse_coord_string(const char* coord_str, int* x, int* y) {
    sscanf(coord_str, "%d_%d", x, y);
}

void coord_to_string(char* coord_str, int x, int y) {
    snprintf(coord_str, sizeof(coord_str), "%d_%d", x, y);
}