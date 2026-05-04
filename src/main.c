#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "regions.h"

int main(int argc, char *argv[]) {


    int pid;

    if (argc < 2) {
        pid = getpid();
    } else {
        pid = atoi(argv[1]);
    }

    MemoryRegion *snap1 = NULL;
    MemoryRegion *snap2 = NULL;

    printf("taking first snapshot...\n");
    int count1 = take_snapshots(pid, &snap1, 512);

    printf("sleeping for 5 seconds...\n");
    sleep(5);

    printf("taking second snapsshot...\n");
    int count2 = take_snapshots(pid, &snap2, 512);

    printf("\n--- DIFF ---\n");
    diff_snapshots(snap1, count1, snap2, count2);

    free(snap1);
    free(snap2);



    return 0;
}
