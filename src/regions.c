#include "regions.h"
#include <string.h>


RegionType classify_region(MemoryRegion *region) {
    // take the memory region as input and
    // output the region type
    if (*region->pathname == '[') {
        if (strcmp(region->pathname, "[stack]") == 0) {
            return REGION_STACK;
        } else if (strcmp(region->pathname, "[heap]") == 0) {
            return REGION_HEAP;
        } else {
            return REGION_VDSO;
        }
    }

    if ((strstr(region->pathname, ".so") != NULL) || (strstr(region->perms, "x") != NULL))
        return REGION_EXECUTABLE;

    if (region->pathname[0] == '\0')
        return REGION_ANONYMOUS;

    return REGION_OTHER;
}


const char *region_label(RegionType type) {
    // return a string like "STACK" or "HEAP"
    switch (type) {
    case REGION_STACK:
        return "STACK";
    case REGION_HEAP:
        return "HEAP";
    case REGION_EXECUTABLE:
        return "EXECUTABLE";
    case REGION_VDSO:
        return "VDSO";
    case REGION_ANONYMOUS:
        return "ANONYMOUS";
    default:
        return "OTHER";
    }
}
