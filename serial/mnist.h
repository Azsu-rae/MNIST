#ifndef MNIST_H
#define MNIST_H

#include <stddef.h>
#include <stdint.h>

typedef struct Shape {
    size_t rows;
    size_t cols;
} Shape;

typedef struct Image Image;
typedef struct Dataset Dataset;

typedef struct Image {
    Shape shape;
    uint8_t* lbl;
    uint8_t* content;
    void (*label)(Image img, uint8_t label);
    uint8_t (*get)(Image img, size_t row, size_t col);
    void (*set)(Image img, size_t row, size_t col, uint8_t value);

    void (*print)(Image img);
} Image;

typedef struct Dataset {
    size_t size;
    Shape shape;
    uint8_t* labels;
    uint8_t* content;
    Image (*index)(Dataset* dataset, size_t index);
} Dataset;

Dataset* Dataset_ctor(size_t size, Shape shape);
void Dataset_dtor(Dataset* dataset);

#endif
