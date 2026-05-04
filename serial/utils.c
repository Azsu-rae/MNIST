
#include "utils.h"

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>

#include "mnist.h"

#define TRAIN_IMG "./dataset/MNIST/train-images.idx3-ubyte"
#define TRAIN_LBL "./dataset/MNIST/train-labels.idx1-ubyte"

#define TEST_IMG "./dataset/MNIST/t10k-images.idx3-ubyte"
#define TEST_LBL "./dataset/MNIST/t10k-labels.idx1-ubyte"

/*
 *
 * Image files (idx3):
 * [0..3]   magic number  = 0x00000803 (from byte 0 to byte 3 inclusive)
 * [4..7]   num_images
 * [8..11]  num_rows  (28)
 * [12..15] num_cols  (28)
 * [16..]   pixels: uint8, row-major, one byte per pixel
 *
 * Label files (idx1):
 * [0..3]   magic number = 0x00000801
 * [4..7]   num_labels
 * [8..]    labels: uint8, value 0-9
 *
 ***/

// clang-format off
FILE* open_or_die(const char* path) {
    FILE* f = fopen(path, "rb");
    if (!f) { perror("fopen"); exit(1); }
    return f;
}

uint32_t read4bytes(FILE* f) {
    uint32_t buf;
    if (fread(&buf, sizeof(buf), 1, f) != 1) {
      fprintf(stderr, "short read\n"); exit(1);
    } return ntohl(buf);
}

uint8_t readbyte(FILE* f) {
    uint8_t buf;
    if (fread(&buf, sizeof(buf), 1, f) != 1) {
      fprintf(stderr, "short read\n"); exit(1);
    } return buf;
}

Dataset* Dataset_test() {
    FILE* fl = open_or_die(TEST_LBL);
    FILE* fi = open_or_die(TEST_LBL);

    fclose(fl);
    fclose(fi);

    return NULL;
}

Dataset* Dataset_train() {
    FILE* fl = open_or_die(TRAIN_LBL);
    uint32_t magic_lbl = read4bytes(fl);
    printf("(label)magic_lbl=%u\n", magic_lbl);
    uint32_t nb_labels = read4bytes(fl);
    printf("(label)nb_labels=%u\n", nb_labels);

    FILE* fi = open_or_die(TRAIN_IMG);
    uint32_t magic_img = read4bytes(fi);
    printf("(image)magic_img=%u\n", magic_img);
    uint32_t size = read4bytes(fi);  //  uint32_t → size_t always safe
    printf("(image)nb_imgs=%u\n", size);
    uint32_t rows = read4bytes(fi);
    printf("(image)rows=%u\n", rows);
    uint32_t cols = read4bytes(fi);
    printf("(image)cols=%u\n", cols);

    if (nb_labels != size || magic_lbl != 2049 || magic_img != 2051) {
        fprintf(stderr, "There's a problem with the inputs!\n");
        exit(1);
    }

    Dataset* dataset = Dataset_ctor(size, (Shape){.rows = rows, .cols = cols});

    fread(dataset->content, 1, size * rows * cols, fi);
    fread(dataset->labels, 1, size, fl);

    Image img = dataset->index(dataset, 0);
    img.print(img);

    Image img1 = dataset->index(dataset, 1);
    img1.print(img1);

    Image img2 = dataset->index(dataset, 2);
    img2.print(img2);

    Image img3 = dataset->index(dataset, 3);
    img3.print(img3);

    // clang-format on

    fclose(fi);
    fclose(fl);

    return dataset;
}
