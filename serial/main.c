#include <stddef.h>
#include <stdint.h>

#include "mnist.h"
#include "utils.h"

int main() {
    Dataset* train_set = train();
    Dataset* test_set = test();

    Dataset_dtor(train_set);
    Dataset_dtor(test_set);
    return 0;
}
