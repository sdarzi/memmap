#ifndef REGIONS_H
#define REGIONS_H

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

RegionType classify_region(MemoryRegion *region);
const char *region_label(RegionType type);

#endif
