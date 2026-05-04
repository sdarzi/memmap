#include <stdio.h>
#include <stdlib.h>
#include "regions.h"

int main(int argc, char *argv[]) {


    char path[64];
    
    // if no second argument, run on curreent pid
    if (argc < 2) {
        snprintf(path, sizeof(path), "/proc/self/maps");
    } else {
        int pid = atoi(argv[1]);
        snprintf(path, sizeof(path), "/proc/%d/maps", pid);
    }


    // open file based on path above
    FILE *current_map = fopen(path, "r");
    if (current_map == NULL) {
        printf("Error: could not open %s - check PID exists and you have permissions\n", path);
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
