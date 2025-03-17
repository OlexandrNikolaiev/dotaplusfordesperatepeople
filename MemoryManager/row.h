#ifndef ROW_H
#define ROW_H

#include <vector>

struct row {
    int moduleID;
    uintptr_t moduleOffset;
    std::vector<uintptr_t> offsets;
};

#endif // ROW_H
