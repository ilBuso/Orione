#include "parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "../tomlc99/toml.h"

// Function to get element at specific coordinates with O(1) access
uint8_t parse_get_base(const Parse* parse, int row, int col) {
    if (row < 0 || row >= parse->rows || col < 0 || col >= parse->cols) {
        return 0; // Return 0 for out-of-bounds
    }
    return parse->base[row * parse->cols + col];
}

uint8_t parse_get_layer2(const Parse* parse, int row, int col) {
    if (row < 0 || row >= parse->rows || col < 0 || col >= parse->cols) {
        return 0; // Return 0 for out-of-bounds
    }
    return parse->layer2[row * parse->cols + col];
}

uint8_t parse_get_layer3(const Parse* parse, int row, int col) {
    if (row < 0 || row >= parse->rows || col < 0 || col >= parse->cols) {
        return 0; // Return 0 for out-of-bounds
    }
    return parse->layer3[row * parse->cols + col];
}

uint8_t parse_get_layer4(const Parse* parse, int row, int col) {
    if (row < 0 || row >= parse->rows || col < 0 || col >= parse->cols) {
        return 0; // Return 0 for out-of-bounds
    }
    return parse->layer4[row * parse->cols + col];
}

uint8_t parse_get_layer5(const Parse* parse, int row, int col) {
    if (row < 0 || row >= parse->rows || col < 0 || col >= parse->cols) {
        return 0; // Return 0 for out-of-bounds
    }
    return parse->layer5[row * parse->cols + col];
}

uint8_t parse_get_layer6(const Parse* parse, int row, int col) {
    if (row < 0 || row >= parse->rows || col < 0 || col >= parse->cols) {
        return 0; // Return 0 for out-of-bounds
    }
    return parse->layer6[row * parse->cols + col];
}

uint8_t parse_get_layer7(const Parse* parse, int row, int col) {
    if (row < 0 || row >= parse->rows || col < 0 || col >= parse->cols) {
        return 0; // Return 0 for out-of-bounds
    }
    return parse->layer7[row * parse->cols + col];
}

uint8_t parse_get_layer8(const Parse* parse, int row, int col) {
    if (row < 0 || row >= parse->rows || col < 0 || col >= parse->cols) {
        return 0; // Return 0 for out-of-bounds
    }
    return parse->layer8[row * parse->cols + col];
}

uint8_t parse_get_layer9(const Parse* parse, int row, int col) {
    if (row < 0 || row >= parse->rows || col < 0 || col >= parse->cols) {
        return 0; // Return 0 for out-of-bounds
    }
    return parse->layer9[row * parse->cols + col];
}

uint8_t parse_get_layer0(const Parse* parse, int row, int col) {
    if (row < 0 || row >= parse->rows || col < 0 || col >= parse->cols) {
        return 0; // Return 0 for out-of-bounds
    }
    return parse->layer0[row * parse->cols + col];
}

// Function to get encoder element at index
uint8_t parse_get_encoder(const Parse* parse, int index) {
    if (index < 0 || index >= ENCODER_COUNT) {
        return 0;
    }
    return parse->encoder[index];
}

// Function to free matrix memory
void parse_free(Parse* parse) {
    if (parse) {
        if (parse->base) {
            free(parse->base);
        }
        if (parse->layer2) {
            free(parse->layer2);
        }
        if (parse->layer3) {
            free(parse->layer3);
        }
        if (parse->layer4) {
            free(parse->layer4);
        }
        if (parse->layer5) {
            free(parse->layer5);
        }
        if (parse->layer6) {
            free(parse->layer6);
        }
        if (parse->layer7) {
            free(parse->layer7);
        }
        if (parse->layer8) {
            free(parse->layer8);
        }
        if (parse->layer9) {
            free(parse->layer9);
        }
        if (parse->layer0) {
            free(parse->layer0);
        }
        if (parse->encoder) {
            free(parse->encoder);
        }
        free(parse);
    }
}

// Helper function to parse a specific layer
static int parse_layer_data(toml_array_t* layer_array, uint8_t* layer_data, int rows, int cols, Parse* parse) {
    if (!layer_array) {
        // It's ok if some layers are missing
        memset(layer_data, 0, rows * cols * sizeof(uint8_t));
        return 1;
    }
    
    for (int i = 0; i < rows; i++) {
        toml_array_t* row_array = toml_array_at(layer_array, i);
        if (!row_array) {
            fprintf(stderr, "Cannot read row %d\n", i);
            return 0;
        }
        
        if (toml_array_nelem(row_array) != cols) {
            fprintf(stderr, "Row %d has incorrect number of columns\n", i);
            return 0;
        }
        
        for (int j = 0; j < cols; j++) {
            toml_datum_t cell = toml_int_at(row_array, j);
            if (!cell.ok) {
                fprintf(stderr, "Cannot read cell at [%d][%d]\n", i, j);
                return 0;
            }
            
            // Ensure value fits in uint8_t
            if (cell.u.i < 0 || cell.u.i > 255) {
                fprintf(stderr, "Value at [%d][%d] out of uint8_t range\n", i, j);
                return 0;
            }
            
            layer_data[i * cols + j] = (uint8_t)cell.u.i;
        }
    }
    
    return 1;
}

// Function to parse a TOML file and extract a matrix
Parse* parse_toml(const char* filename) {
    FILE* fp;
    char errbuf[200];
    
    fp = fopen(filename, "r");
    if (!fp) {
        fprintf(stderr, "Cannot open file %s\n", filename);
        return NULL;
    }
    
    toml_table_t* conf = toml_parse_file(fp, errbuf, sizeof(errbuf));
    fclose(fp);
    
    if (!conf) {
        fprintf(stderr, "Cannot parse TOML file: %s\n", errbuf);
        return NULL;
    }
    
    // Get the dimension table
    toml_table_t* dimension = toml_table_in(conf, "dimension");
    if (!dimension) {
        fprintf(stderr, "No dimension table found\n");
        toml_free(conf);
        return NULL;
    }
    
    // Get rows and columns
    toml_datum_t rows_datum = toml_int_in(dimension, "rows");
    if (!rows_datum.ok) {
        fprintf(stderr, "Cannot read rows\n");
        toml_free(conf);
        return NULL;
    }

    toml_datum_t cols_datum = toml_int_in(dimension, "columns");
    if (!cols_datum.ok) {
        fprintf(stderr, "Cannot read columns\n");
        toml_free(conf);
        return NULL;
    }
    
    int rows = rows_datum.u.i;
    int cols = cols_datum.u.i;
    
    // Get the matrix table
    toml_table_t* matrix = toml_table_in(conf, "matrix");
    if (!matrix) {
        fprintf(stderr, "No matrix table found\n");
        toml_free(conf);
        return NULL;
    }
    
    // Get encoder array
    toml_array_t* encoder_array = toml_array_in(matrix, "encoder");
    if (!encoder_array) {
        fprintf(stderr, "No encoder array found\n");
        toml_free(conf);
        return NULL;
    }
    
    if (toml_array_nelem(encoder_array) != ENCODER_COUNT) {
        fprintf(stderr, "Encoder array has incorrect number of elements\n");
        toml_free(conf);
        return NULL;
    }
    
    // Allocate our matrix structure
    Parse* parse = (Parse*)malloc(sizeof(Parse));
    if (!parse) {
        fprintf(stderr, "Memory allocation failed for matrix\n");
        toml_free(conf);
        return NULL;
    }
    
    parse->rows = rows;
    parse->cols = cols;
    
    // Allocate memory for data (array of uint8_t)
    parse->base = (uint8_t*)malloc(rows * cols * sizeof(uint8_t));
    parse->layer2 = (uint8_t*)malloc(rows * cols * sizeof(uint8_t));
    parse->layer3 = (uint8_t*)malloc(rows * cols * sizeof(uint8_t));
    parse->layer4 = (uint8_t*)malloc(rows * cols * sizeof(uint8_t));
    parse->layer5 = (uint8_t*)malloc(rows * cols * sizeof(uint8_t));
    parse->layer6 = (uint8_t*)malloc(rows * cols * sizeof(uint8_t));
    parse->layer7 = (uint8_t*)malloc(rows * cols * sizeof(uint8_t));
    parse->layer8 = (uint8_t*)malloc(rows * cols * sizeof(uint8_t));
    parse->layer9 = (uint8_t*)malloc(rows * cols * sizeof(uint8_t));
    parse->layer0 = (uint8_t*)malloc(rows * cols * sizeof(uint8_t));
    parse->encoder = (uint8_t*)malloc(ENCODER_COUNT * sizeof(uint8_t));
    
    if (!parse->base ||
        !parse->layer2 ||
        !parse->layer3 ||
        !parse->layer4 ||
        !parse->layer5 ||
        !parse->layer6 ||
        !parse->layer7 ||
        !parse->layer8 ||
        !parse->layer9 ||
        !parse->layer0 ||
        !parse->encoder) {
        fprintf(stderr, "Memory allocation failed\n");
        parse_free(parse);
        toml_free(conf);
        return NULL;
    }
    
    // Get all layer arrays
    toml_array_t* base_array = toml_array_in(matrix, "base");
    toml_array_t* layer2_array = toml_array_in(matrix, "layer2");
    toml_array_t* layer3_array = toml_array_in(matrix, "layer3");
    toml_array_t* layer4_array = toml_array_in(matrix, "layer4");
    toml_array_t* layer5_array = toml_array_in(matrix, "layer5");
    toml_array_t* layer6_array = toml_array_in(matrix, "layer6");
    toml_array_t* layer7_array = toml_array_in(matrix, "layer7");
    toml_array_t* layer8_array = toml_array_in(matrix, "layer8");
    toml_array_t* layer9_array = toml_array_in(matrix, "layer9");
    toml_array_t* layer0_array = toml_array_in(matrix, "layer0");
    
    // Base layer is required
    if (!base_array) {
        fprintf(stderr, "No base array found\n");
        parse_free(parse);
        toml_free(conf);
        return NULL;
    }
    
    // Parse base layer (required)
    if (!parse_layer_data(base_array, parse->base, rows, cols, parse)) {
        parse_free(parse);
        toml_free(conf);
        return NULL;
    }
    
    // Parse other layers (optional)
    if (!parse_layer_data(layer2_array, parse->layer2, rows, cols, parse) ||
        !parse_layer_data(layer3_array, parse->layer3, rows, cols, parse) ||
        !parse_layer_data(layer4_array, parse->layer4, rows, cols, parse) ||
        !parse_layer_data(layer5_array, parse->layer5, rows, cols, parse) ||
        !parse_layer_data(layer6_array, parse->layer6, rows, cols, parse) ||
        !parse_layer_data(layer7_array, parse->layer7, rows, cols, parse) ||
        !parse_layer_data(layer8_array, parse->layer8, rows, cols, parse) ||
        !parse_layer_data(layer9_array, parse->layer9, rows, cols, parse) ||
        !parse_layer_data(layer0_array, parse->layer0, rows, cols, parse)) {
        parse_free(parse);
        toml_free(conf);
        return NULL;
    }
    
    // Parse encoder array
    for (int i = 0; i < ENCODER_COUNT; i++) {
        toml_datum_t encoder_item = toml_int_at(encoder_array, i);
        if (!encoder_item.ok) {
            fprintf(stderr, "Cannot read encoder item %d\n", i);
            parse_free(parse);
            toml_free(conf);
            return NULL;
        }
        
        // Ensure value fits in uint8_t
        if (encoder_item.u.i < 0 || encoder_item.u.i > 255) {
            fprintf(stderr, "Encoder value at [%d] out of uint8_t range\n", i);
            parse_free(parse);
            toml_free(conf);
            return NULL;
        }
        
        parse->encoder[i] = (uint8_t)encoder_item.u.i;
    }
    
    // Free TOML resources
    toml_free(conf);
    return parse;
}