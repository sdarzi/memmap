#include <stdlib.h>
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
        if (bytes_read == 0) {
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

int take_snapshots(int pid, MemoryRegion **regions_out, int max_regions) {
    char path[64];
    // 1. Allocate an array of MemoryRegion with malloc

    MemoryRegion *regions = malloc(sizeof(MemoryRegion) * max_regions);
    if (regions == NULL) {
        printf("Error: malloc failed\n");
        return 0;
    }
    // 2. Build the /proc/[pid]/maps path
    snprintf(path, sizeof(path), "/proc/%d/maps", pid);
    // 3. Open the file
    FILE *current_map = fopen(path, "r");

    printf("opening: /proc/%d/maps\n", pid);
    if (current_map == NULL) {
        printf("Error: could not open %s - check PID exists and you have permission\n", path);
        free(regions);
        return 0;
    }
    // 4. Read each line with fgets, parse with sscanf — exactly like main does

    int i = 0;
    char line[256];
    MemoryRegion region_temp;
    while (fgets(line, sizeof(line), current_map) != NULL) {
        region_temp.pathname[0] = '\0';
        sscanf(line, "%lx-%lx %4s %*x %*x:%*x %*d %255s",
               &region_temp.start,
               &region_temp.end,
               region_temp.perms,
               region_temp.pathname);
        // 5. Store each parsed region into the array
        regions[i] = region_temp;
        i++;
        if (i >= max_regions)
            break;
    }
    // 6. Count how many regions you parsed
    // 7. Point *regions_out at your allocated array
    *regions_out = regions;
    // 8. Close the file
    fclose(current_map);
    // 9. Return the count

    printf("snapshot:  found %d regions\n", i);
    return i;
}

void diff_snapshots(MemoryRegion *snap1, int count1,
                    MemoryRegion *snap2, int count2) {
    int i;
    int j;
    for (i = 0; i < count2; i++) {
        int found = 0;
        for (j = 0; j < count1; j++) {
            if (snap2[i].start == snap1[j].start) {
                found = 1;
                if (strcmp(snap2[i].perms, snap1[j].perms) != 0) {
                    printf("\033[33m%-8s\033[0m %lx-%lx %s -> %s %s\n",
                           "CHANGED:",
                           snap1[j].start,
                           snap1[j].end,
                           snap1[j].perms,
                           snap2[i].perms,
                           snap2[i].pathname);
                }

                break;
            }
        }

        if (found == 0)
            printf("\033[32m%-5s %lx-%lx %s %s\033[0m\n",
                   "NEW:",
                   snap2[i].start,
                   snap2[i].end,
                   snap2[i].perms,
                   snap2[i].pathname);
    }

    for (i = 0; i < count1; i++) {
        int found = 0;
        for (j = 0; j < count2; j++) {
            if (snap1[i].start == snap2[j].start) {
                found = 1;
                break;
            }
        }
        if (found == 0)
            printf("\033[31m%-5s %lx-%lx %s %s\033[0m\n",
                   "GONE:",
                   snap1[i].start,
                   snap1[i].end,
                   snap1[i].perms,
                   snap1[i].pathname);
    }

}
