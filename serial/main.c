#include <stddef.h>
#include <stdint.h>

#include "idxfile.h"


int main() {

    IDXFile labels = IDXFile_ctor("./dataset/train-labels.idx1-ubyte", LABEL);
    IDXFile imgs = IDXFile_ctor("./dataset/train-images.idx3-ubyte", IMG);

    print("---- LABELS ----", labels, 0);
    print("---- IMGS ----", imgs, 0);

    return 0;
}


// int main() {
//     Dataset* train_set = train();
//     Dataset* test_set = test();
//
//     Dataset_dtor(train_set);
//     Dataset_dtor(test_set);
//     return 0;
// }
