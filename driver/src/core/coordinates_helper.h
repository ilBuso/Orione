//
// Created by Giulio Pimenoff Verdolin on 14/04/25.
//

#pragma once

/**
 * Helper function to parse coordinates from string
 */
void parse_coord_string(const char* coord_str, int* x, int* y);

/**
 * Helper function to convert coordinates to string
 */
void coord_to_string(char* coord_str, int x, int y);