#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "mnist.h"
#include "utils.h"

int main() {
    Dataset* dataset = Dataset_train();
    // Dataset_dtor(dataset);
    return 0;
}
