#ifndef PARSER_H
#define PARSER_H

    #include <stdint.h>
    #include <stdbool.h>

    #define ENCODER_COUNT 3

    typedef struct {
        const char* os;
        bool active;
        int rows;
        int cols;
        uint8_t* base;
        uint8_t* layer2;
        uint8_t* layer3;
        uint8_t* layer4;
        uint8_t* layer5;
        uint8_t* layer6;
        uint8_t* layer7;
        uint8_t* layer8;
        uint8_t* layer9;
        uint8_t* layer0;
        uint8_t* encoder;
    } Parse;

    // Function prototypes
    uint8_t parse_get_base(const Parse* parse, int row, int col);
    uint8_t parse_get_layer2(const Parse* parse, int row, int col);
    uint8_t parse_get_layer3(const Parse* parse, int row, int col);
    uint8_t parse_get_layer4(const Parse* parse, int row, int col);
    uint8_t parse_get_layer5(const Parse* parse, int row, int col);
    uint8_t parse_get_layer6(const Parse* parse, int row, int col);
    uint8_t parse_get_layer7(const Parse* parse, int row, int col);
    uint8_t parse_get_layer8(const Parse* parse, int row, int col);
    uint8_t parse_get_layer9(const Parse* parse, int row, int col);
    uint8_t parse_get_layer0(const Parse* parse, int row, int col);
    uint8_t parse_get_encoder(const Parse* parse, int index);
    void parse_free(Parse* parse);
    Parse* parse_toml(const char* filename);

#endif // PARSER_H