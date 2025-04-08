#include "parser.h"

#include <stdlib.h>
#include <stdlib.h>
#include <string.h>

#include "../tomlc99/toml.h"

Matrix* matrix;

// Function to get element at specific coordinates
int matrix_get(const Matrix* matrix, int row, int col) {
    if (row < 0 || row >= matrix->rows || col < 0 || col >= matrix->cols) {
        return -1;
    }
    return matrix->data[row * matrix->cols + col];
}

// Function to set element at specific coordinates
void matrix_set(Matrix* matrix, int row, int col, int value) {
    if (row < 0 || row >= matrix->rows || col < 0 || col >= matrix->cols) {
        return;
    }
    matrix->data[row * matrix->cols + col] = value;
}

// Function to free matrix memory
void matrix_free(Matrix* matrix) {
    if (matrix) {
        if (matrix->data) {
            free(matrix->data);
        }
        free(matrix);
    }
}

// Function to parse a TOML file and extract a matrix
Matrix* parse_toml(const char* filename) {
    FILE* fp;
    char errbuf[200];
    
    fp = fopen(filename, "r");
    if (!fp) {
        return NULL;
    }
    
    toml_table_t* conf = toml_parse_file(fp, errbuf, sizeof(errbuf));
    fclose(fp);
    
    if (!conf) {
        return NULL;
    }
    
    // Get the matrix table/array
    toml_table_t* toml_matrix = toml_table_in(conf, "matrix");
    if (!toml_matrix) {
        return NULL;
    }

    toml_array_t* matrix_array = toml_array_in(toml_matrix, "data");
    if (!matrix_array) {
        toml_free(conf);
        return NULL;
    }

    // Get dimensions
    toml_table_t* dimension = toml_table_in(conf, "dimensions");
    if (!dimension) {
        toml_free(conf);
        return NULL;
    }
    
    toml_datum_t rows =  toml_string_in(dimension, "rows");
    if (!rows.ok) {
        toml_free(conf);
        return NULL;
    }

    toml_datum_t cols =  toml_string_in(dimension, "cols");
    if (!cols.ok) {
        toml_free(conf);
        return NULL;
    }
    
    // Allocate our matrix structure
    Matrix* matrix = (Matrix*)malloc(sizeof(Matrix));
    if (!matrix) {
        toml_free(conf);
        return NULL;
    }
    
    matrix->rows = rows.u.i;
    matrix->cols = cols.u.i;
    matrix->data = (int*)malloc(rows.u.i * cols.u.i * sizeof(int));
    
    if (!matrix->data) {
        free(matrix);
        toml_free(conf);
        return NULL;
    }
    
    // Parse each row and column
    for (int i = 0; i < rows.u.i; i++) {
        toml_array_t* row_array = toml_array_at(matrix_array, i);
        if (!row_array || toml_array_nelem(row_array) != cols.u.i) {
            free(matrix->data);
            free(matrix);
            toml_free(conf);
            return NULL;
        }
        
        for (int j = 0; j < cols.u.i; j++) {
            toml_datum_t cell = toml_int_at(row_array, j);
            if (!cell.ok) {
                free(matrix->data);
                free(matrix);
                toml_free(conf);
                return NULL;
            }
            
            matrix_set(matrix, i, j, cell.u.i);
        }
    }
    
    // Free TOML resources
    toml_free(conf);
    
    return matrix;
}