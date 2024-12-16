#include <stdio.h>
#include <stdlib.h>

struct admin {
    char username[25];
    char password[25];
};

int main() {
    FILE *ptr;
    int i;
    struct admin a;

    // Open the file in binary write mode
    ptr = fopen("AdminData.txt", "wb");
    if (ptr == NULL) {
        printf("Error opening file.\n");
        exit(1);
    }

    // Input and write admin data
    for (i = 0; i < 3; i++) { // Adjust the number of records as needed
        printf("\nEnter Username:");
        scanf("%s", a.username);
        printf("\nEnter Password:");
        scanf("%s", a.password);

        // Write admin data to the file
        fwrite(&a, sizeof(struct admin), 1, ptr);
    }

    // Close the file
    fclose(ptr);

    printf("Admin data has been written to AdminData.\n");
    return 0;
}

