#include "mnist.h"

#include <stdio.h>
#include <stdlib.h>

// clang-format off
void _print_img(Image img) {
    const char* shades = " .:-=+*#%@";
    printf("Label: %u", *img.lbl);
    for (int i=0;i<img.shape.rows;i++) {
        for (int j=0;j<img.shape.cols;j++) {
            // divide by 26 to get roughly 10 range of values
            printf("%c ", shades[img.get(img, i, j) / 26]);
        } printf("\n");
    } printf("\n");
}

uint8_t _get(Image img, size_t row, size_t col) {
    if (row >= img.shape.rows || col >= img.shape.cols) {
        fprintf(stderr, "bad indexing!\n");
        exit(1);
    } return img.content[row*img.shape.cols+col];
}

// curr_img[j*28 + i] = value;
void _set(Image img, size_t row, size_t col, uint8_t value) {
    if (row >= img.shape.rows || col >= img.shape.cols) {
        fprintf(stderr, "bad indexing!\n");
        exit(1);
    } img.content[row*img.shape.cols+col] = value;
}

// labels[k] = readbyte(fl);
void _label(Image img, uint8_t label) { *img.lbl = label; }

// uint8_t* curr_img = (dataset+(k*784));
Image _index(Dataset* dataset, size_t index) {
    if (index >= dataset->size) { fprintf(stderr, "bad indexing!\n"); exit(1); }
    return (Image) {
        .content = dataset->content + (index*dataset->shape.rows*dataset->shape.cols),
        .lbl = dataset->labels+index,
        .shape = dataset->shape,
        .label = _label,
        .get = _get,
        .set = _set,

        .print = _print_img
    };
}

// uint8_t dataset[60000][28][28]; // overflows on the stack
// uint8_t *dataset = malloc(nb_imgs * rows * cols); // 60000 * 784 ~= 47 mb
Dataset* Dataset_ctor(size_t size, Shape shape) {
    Dataset* buf = malloc(sizeof(Dataset));
    uint8_t* content = malloc(size*shape.rows*shape.cols);
    uint8_t* labels = malloc(size);
    if (buf == NULL || content == NULL || labels == NULL) {
        fprintf(stderr, "allocator returned NULL!\n");
        exit(1);
    } *buf = (Dataset) {
        .content = content,
        .labels = labels,
        .size = size,
        .shape = shape,
        .index = _index
    }; return buf;
}

void Dataset_dtor(Dataset* dataset) {
    free(dataset->content);
    free(dataset->labels);
    free(dataset);
}
