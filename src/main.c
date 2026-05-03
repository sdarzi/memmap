#include <stdio.h>

typedef struct {
    unsigned long start;
    unsigned long end;
    char perms[5];
    char pathname[256];
} MemoryRegion;

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
        printf("START: %lx END: %lx PERMS: %s PATH: %s\n",
               region.start,
               region.end,
               region.perms,
               region.pathname);
    }
    // close file
    fclose(current_map);
    return 0;
}
