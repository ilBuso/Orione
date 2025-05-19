#ifndef PARSER_H
#define PARSER_H

#include <stdint.h>
#include <stdbool.h>

#define ENCODER_COUNT 3
#define MAX_LAYERS 10

typedef struct {
    const char* os;
    bool active;
    int rows;
    int cols;
    int num_layers;           // Number of layers actually loaded
    uint8_t** layers;         // Array of pointers to layers
    char** layer_names;       // Names of the layers
    uint8_t* encoder;
} Parse;

// Function prototypes
uint8_t parse_get_layer(const Parse* parse, int layer_index, int row, int col);
uint8_t parse_get_encoder(const Parse* parse, int index);
void parse_free(Parse* parse);
Parse* parse_toml(const char* filename);

// Helper functions to get specific layers (for backward compatibility)
static inline uint8_t parse_get_base(const Parse* parse, int row, int col) {
    return parse_get_layer(parse, 1, row, col);  // Base is layer 0
}

#endif // PARSER_H