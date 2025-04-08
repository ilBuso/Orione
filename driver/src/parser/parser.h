#ifndef PARSER_H
#define PARSER_H

    typedef struct {
        int rows;
        int cols;
        int* data;
    } Matrix;

    int matrix_get(const Matrix* matrix, int row, int col);
    void matrix_set(Matrix* matrix, int row, int col, int value);
    void matrix_free(Matrix* matrix);

    Matrix* parse_toml(const char* filename);

#endif // PARSER_H
