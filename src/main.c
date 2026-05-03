#include <stdio.h>
#include <string.h>

typedef enum {
    REGION_EXECUTABLE,
    REGION_STACK,
    REGION_HEAP,
    REGION_VDSO,
    REGION_ANONYMOUS,
    REGION_OTHER
} RegionType;

typedef struct {
    unsigned long start;
    unsigned long end;
    char perms[5];
    char pathname[256];
} MemoryRegion;

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

int main() {
    // open file
    FILE *current_map = fopen("/proc/self/maps", "r");
    if (current_map == NULL) {
        printf("Failed to open file\n");
        return 1;
    }
    // read and print each line of the opened file
    char line[256];
    MemoryRegion region;
    while (fgets(line, sizeof(line), current_map) != NULL) {
        region.pathname[0] = '\0';
        sscanf(line, "%lx-%lx %4s %*x %*x:%*x %*d %255s",
               &region.start,
               &region.end,
               region.perms,
               region.pathname);
        printf("START: %lx END: %lx PERMS: %s TYPE: %-10s PATH: %s\n",
               region.start,
               region.end,
               region.perms,
               region_label(classify_region(&region)),
               region.pathname);
    }
    // close file
    fclose(current_map);
    return 0;
}
