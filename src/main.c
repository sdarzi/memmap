#include <stdio.h>

int main(){
    // open file 
    FILE *current_map = fopen ("/proc/self/maps", "r");
    if (current_map==NULL){
        printf("Failed to open file\n");
        return 1;
    }
    // read and print each line of the opened file
    char line[256];
    while (fgets(line,sizeof(line),current_map) != NULL){
        printf("%s",line);
    }
    // close file
    fclose(current_map);
    return 0;
}
