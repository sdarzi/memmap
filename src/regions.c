#define _FILE_OFFSET_BITS 64
#include "regions.h"
#include <stdio.h>
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

void dump_region(int pid, MemoryRegion *region, size_t max_bytes) {
    char path[64];
    if (region->perms[0] == 'r') {

        snprintf(path, sizeof(path), "/proc/%d/mem", pid);

        FILE *mem = fopen(path, "rb");

        if (mem == NULL) {
            printf("Error: could not open %s - check PID exists and you have permission\n", path);
            return;
        }

        fseeko(mem, (off_t)region->start, SEEK_SET);

        unsigned char buffer[4096];
        size_t to_read = max_bytes < sizeof(buffer) ? max_bytes : sizeof(buffer);
        size_t bytes_read = fread(buffer, 1, to_read, mem);
        if (bytes_read == 0){
            fclose(mem);
            return;
        }

        for (size_t i = 0; i < bytes_read; i++) {
            printf("%02x ", buffer[i]);
            if ((i + 1) % 16 == 0)
                printf("\n");
        }
        fclose(mem);
    }
}
