#pragma once

#include "stdint.hpp"

namespace bear {
    void* kmalloc(bear::size_t sz);
    void kfree(void *mem);
}
