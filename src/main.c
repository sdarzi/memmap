#include <stdio.h>

int main(){
    // 1. Open /proc/self/maps with fopen
    FILE *current_map = fopen ("/proc/self/maps", "r");
    if (current_map==NULL){
        printf("Failed to open file\n");
        return 1;
    }
    // 2. Read it line by line with fgets w
    char line[256];
    while (fgets(line,sizeof(line),current_map) != NULL){
        // 3. Print each line with printf
        printf("%s",line);
    }
        // 4. Close the file
    fclose(current_map);
    return 0;
}
