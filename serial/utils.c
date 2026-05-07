
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>

#include "mnist.h"

#define TRAIN_IMG "./dataset/MNIST/train-images.idx3-ubyte"
#define TRAIN_LBL "./dataset/MNIST/train-labels.idx1-ubyte"

#define TEST_IMG "./dataset/MNIST/t10k-images.idx3-ubyte"
#define TEST_LBL "./dataset/MNIST/t10k-labels.idx1-ubyte"

/*
 *
 * https://www.fon.hum.uva.nl/praat/manual/IDX_file_format.html
 *
 ***/

// clang-format off
FILE* _open_or_die(const char* path) {
    FILE* f = fopen(path, "rb");
    if (!f) { perror("fopen"); exit(1); }
    return f;
}

uint32_t _read4bytes(FILE* f) {
    uint32_t buf;
    if (fread(&buf, sizeof(buf), 1, f) != 1) {
      fprintf(stderr, "short read\n"); exit(1);
    } return ((buf << 24) & 0xFF000000) |
             ((buf << 8)  & 0x00FF0000) |
             ((buf >> 24) & 0x000000FF) |
             ((buf >> 8)  & 0x0000FF00);
}

uint8_t _readbyte(FILE* f) {
    uint8_t buf;
    if (fread(&buf, sizeof(buf), 1, f) != 1) {
      fprintf(stderr, "short read\n"); exit(1);
    } return buf;
}

Dataset* _scratch(const char* lbl, const char* img) {

    FILE* fl = _open_or_die(lbl);
    printf("\nreading %s...\n\n", lbl);

    printf("first_byte=%u\n", _readbyte(fl));
    printf("second_byte=%u\n", _readbyte(fl));
    printf("dtype=%u\n", _readbyte(fl));
    printf("dims=%u\n", _readbyte(fl));

    uint32_t nb_labels = _read4bytes(fl);
    printf("\nnb_labels=%u\n", nb_labels);

    FILE* fi = _open_or_die(img);
    printf("\nreading %s...\n\n", img);

    printf("first_byte=%u\n", _readbyte(fi));
    printf("second_byte=%u\n", _readbyte(fi));
    printf("dtype=%u\n", _readbyte(fi));
    printf("dims=%u\n", _readbyte(fi));

    uint32_t nb_imgs = _read4bytes(fi);  //  uint32_t → size_t always safe
    printf("\nnb_imgs=%u\n", nb_imgs);
    uint32_t dim1 = _read4bytes(fi);
    printf("dim1=%u\n", dim1);
    uint32_t dim2 = _read4bytes(fi);
    printf("dim2=%u\n", dim2);

    Dataset* dataset = Dataset_ctor(nb_imgs, (Shape){.rows = dim1, .cols = dim2});

    size_t training_imgs = nb_imgs * dim1 * dim2;
    if (fread(dataset->content, 1, training_imgs, fi) != training_imgs) {
        fprintf(stderr, "\nshort read\n"); exit(1);
    } if (fread(dataset->labels, 1, nb_labels, fl) != nb_labels) {
        fprintf(stderr, "\nshort read\n"); exit(1);
    }

    Image img1 = dataset->index(dataset, 1);
    img1.print(img1);

    Image img2 = dataset->index(dataset, 2);
    img2.print(img2);

    Image img3 = dataset->index(dataset, 3);
    img3.print(img3);

    fclose(fl);
    fclose(fi);

    return NULL;
}

Dataset* test() {
    return _scratch(TEST_LBL, TEST_IMG);
}

Dataset* train() {
    return _scratch(TRAIN_LBL, TRAIN_IMG);
}
