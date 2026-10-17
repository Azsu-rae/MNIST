#ifndef IDXFILE_H
#define IDXFILE_H

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum {
    LABEL,
    IMG,
} IDXContentType;

typedef struct IDXFile IDXFile;

struct IDXFile {
    uint8_t firstbyte;
    uint8_t secondbyte;
    uint8_t dtype;
    uint8_t dims;
    uint32_t* shape;
    uint32_t item_size;
    uint8_t* content;
    void (*str)(char*, IDXFile, size_t);

};

void _label_to_str(char* buffer, IDXFile idxfile, size_t index) {

}

void _mnistdigit_to_str(char* buffer, IDXFile idxfile, size_t index) {
    const char* shades = " .:-=+*#%@";
    for (int i=0;i<idxfile.shape[1];i++) {
        for (int j=0;j<idxfile.shape[2];j++) {
            // divide by 26 to get roughly 10 range of values
            sprintf(buffer, "%c ", shades[idxfile.content[idxfile.item_size*index +  28*i + j ] / 26]);
        } sprintf(buffer, "\n");
    } sprintf(buffer, "\n");
}

IDXFile IDXFile_ctor(const char* filepath, IDXContentType type) {
    FILE* f = fopen(filepath, "rb");
    if (!f) {
        // fprintf(stderr, "short read\n");
        printf("couldn't fopen: %s", filepath);
        exit(1);
    }

    IDXFile idxfile;
    fread(&idxfile.firstbyte, sizeof(idxfile.firstbyte), 1, f);
    fread(&idxfile.secondbyte, sizeof(idxfile.secondbyte), 1, f);
    fread(&idxfile.dtype, sizeof(idxfile.dtype), 1, f);
    fread(&idxfile.dims, sizeof(idxfile.dims), 1, f);

    size_t total = 1;
    idxfile.shape = malloc(sizeof(uint32_t)*idxfile.dims);
    for (int i=0;i<idxfile.dims;i++) {
        uint32_t unknown_name;
        fread(&unknown_name, sizeof(uint32_t), 1, f);
        idxfile.shape[i] = ((unknown_name << 24) & 0xFF000000) |
                           ((unknown_name << 8) & 0x00FF0000) |
                           ((unknown_name >> 24) & 0x000000FF) |
                           ((unknown_name >> 8) & 0x0000FF00);
        total *= idxfile.shape[i];
    }

    idxfile.content = malloc(total);
    fread(idxfile.content, 1, total, f);
    idxfile.item_size = total / idxfile.shape[0];

    switch (type) {
        case LABEL:
            idxfile.str = _mnistdigit_to_str;
            break;
        case IMG:
            idxfile.str = _label_to_str;
            break;
    }

    return idxfile;
}

void IDXFile_dtor(IDXFile idxfile) {
    free(idxfile.shape);
    free(idxfile.content);
}

void print(const char* title, IDXFile idxfile, int sample_index) {

    printf("\n\n%s", title);

    printf("\n\nFirst Byte: %u", idxfile.firstbyte);
    printf("\nSecond Byte: %u", idxfile.secondbyte);
    printf("\n\nDType: %u", idxfile.dtype);
    printf("\nDims: %u", idxfile.dims);
    printf("\nShape: (");
    for (int i=0;i<idxfile.dims;i++) {
        printf(" %d ", idxfile.shape[i]);
    } printf(")");

    printf("\n\nSample N%d:", sample_index);
    char buffer[idxfile.item_size];
    idxfile.str(buffer, idxfile, sample_index);
    printf("%s", buffer);
}

#endif
