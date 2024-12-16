#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define the admin structure
struct admin {
    char username[25];
    char password[25];
};

// Define the room structure
struct room {
    int roomNumber;
    char roomType[25];
    float price;
    char availability[10];  // Added availability attribute
};

// Function prototypes
int validateAdminLogin(FILE *ptr);
void addRoom();
void viewRooms();
void updateRoomDetails();
void removeRoom();
int validateRoomType(const char *roomType);
void viewAvailableRooms();

// Function to validate admin login
int validateAdminLogin(FILE *ptr) {
    struct admin a;
    char inputUsername[25], inputPassword[25];
    int isValid = 0;

    while (1) {  // Infinite loop for login attempts
        // Prompt for username and password
        printf("Enter Admin Username: ");
        scanf("%s", inputUsername);
        printf("Enter Admin Password: ");
        scanf("%s", inputPassword);
        
        // Check credentials
        rewind(ptr);  // Reset file pointer to the beginning
        
        while (fread(&a, sizeof(struct admin), 1, ptr)) {
            if (strcmp(inputUsername, a.username) == 0 && strcmp(inputPassword, a.password) == 0) {
                isValid = 1;
                break;
            }
        }
        
        if (isValid) {
            break;  // Exit the loop if credentials are valid
        } else {
            printf("Invalid credentials. Please try again.\n");
        }
    }
    
    return isValid;
}

// Function to validate room type
int validateRoomType(const char *roomType) {
    return (strcmp(roomType, "Single") == 0 || 
            strcmp(roomType, "Double") == 0 || 
            strcmp(roomType, "Suite") == 0);
}

// Function to add a room to a text file
void addRoom() {
    FILE *roomFile;
    struct room r;
    char roomType[25];

    // Open the text file in append mode
    roomFile = fopen("RoomData.txt", "a");
    if (roomFile == NULL) {
        printf("Error opening room data file.\n");
        exit(1);
    }

    // Collect and validate room number
    do {
        printf("Enter Room Number (positive integer): ");
        scanf("%d", &r.roomNumber);
        if (r.roomNumber <= 0) {
            printf("Invalid room number. Please enter a positive integer.\n");
        }
    } while (r.roomNumber <= 0);

    // Collect and validate room type
    do {
        printf("Enter Room Type (Single, Double, Suite): ");
        scanf("%s", roomType);
        if (!validateRoomType(roomType)) {
            printf("Invalid room type. Please enter Single, Double, or Suite.\n");
        }
    } while (!validateRoomType(roomType));
    strcpy(r.roomType, roomType);

    // Collect price
    printf("Enter Price: ");
    scanf("%f", &r.price);

    // Set availability to "Yes" by default
    strcpy(r.availability, "Yes");

    // Write room details to the text file
    fprintf(roomFile, "Room Number: %d\n", r.roomNumber);
    fprintf(roomFile, "Room Type: %s\n", r.roomType);
    fprintf(roomFile, "Price: %.2f\n", r.price);
    fprintf(roomFile, "Availability: %s\n", r.availability);
    fprintf(roomFile, "-------------------------\n");

    // Close the file
    fclose(roomFile);

    printf("Room details have been added to RoomData.txt.\n");
}

// Function to view all rooms from the text file
void viewRooms() {
    FILE *roomFile;
    char line[256];  

    // Open the text file in read mode
    roomFile = fopen("RoomData.txt", "r");
    if (roomFile == NULL) {
        printf("Error opening room data file.\n");
        exit(1);
    }

    printf("\nRooms available:\n\n");
    // Read and display room details
    while (fgets(line, sizeof(line), roomFile)) {
        // Print each line read from the file
        printf("%s", line);
    }

    // Close the file
    fclose(roomFile);
}

// Function to update room details
void updateRoomDetails() {
    FILE *roomFile;
    struct room rooms[100];  // Assuming a maximum of 100 rooms
    int roomNumber, totalRooms = 0, found = 0;
    char newRoomType[25], newAvailability[10];
    float newPrice;

    roomFile = fopen("RoomData.txt", "r+");  // Open the file for reading and writing
    if (roomFile == NULL) {
        printf("Error opening room data file.\n");
        exit(1);
    }

    // Read all rooms into memory
    while (fscanf(roomFile, "Room Number: %d\n", &rooms[totalRooms].roomNumber) != EOF) {
        fscanf(roomFile, "Room Type: %s\n", rooms[totalRooms].roomType);
        fscanf(roomFile, "Price: %f\n", &rooms[totalRooms].price);
        fscanf(roomFile, "Availability: %s\n", rooms[totalRooms].availability);
        fscanf(roomFile, "-------------------------\n");
        totalRooms++;
    }

    // Get room number to update
    printf("Enter Room Number to update: ");
    scanf("%d", &roomNumber);

    // Search for the room
    for (int i = 0; i < totalRooms; i++) {
        if (rooms[i].roomNumber == roomNumber) {
            found = 1;
            printf("Room found! Updating details...\n");

            // Update room type
            do {
                printf("Enter New Room Type (Single, Double, Suite): ");
                scanf("%s", newRoomType);
                if (!validateRoomType(newRoomType)) {
                    printf("Invalid room type. Please enter Single, Double, or Suite.\n");
                }
            } while (!validateRoomType(newRoomType));
            strcpy(rooms[i].roomType, newRoomType);

            // Update price
            printf("Enter New Price: ");
            scanf("%f", &newPrice);
            rooms[i].price = newPrice;

            // Update availability
            printf("Enter New Availability (Yes/No): ");
            scanf("%s", newAvailability);
            strcpy(rooms[i].availability, newAvailability);

            printf("Room details updated successfully!\n");
            break;
        }
    }

    if (!found) {
        printf("Room number %d not found.\n", roomNumber);
        fclose(roomFile);
        return;
    }

    // Rewind the file to the beginning to overwrite the file with updated data
    rewind(roomFile);

    // Write all rooms (including the updated one) back to the file
    for (int i = 0; i < totalRooms; i++) {
        fprintf(roomFile, "Room Number: %d\n", rooms[i].roomNumber);
        fprintf(roomFile, "Room Type: %s\n", rooms[i].roomType);
        fprintf(roomFile, "Price: %.2f\n", rooms[i].price);
        fprintf(roomFile, "Availability: %s\n", rooms[i].availability);
        fprintf(roomFile, "-------------------------\n");
    }

    // Close the file
    fclose(roomFile);
}

// Function to remove a room from the text file
void removeRoom() {
    FILE *roomFile, *tempFile;
    struct room r;
    int roomNumber, found = 0;

    // Open the original file in read mode
    roomFile = fopen("RoomData.txt", "r");
    if (roomFile == NULL) {
        printf("Error opening room data file.\n");
        exit(1);
    }

    // Create a temporary file
    tempFile = fopen("TempRoomData.txt", "w");
    if (tempFile == NULL) {
        printf("Error creating temporary file.\n");
        fclose(roomFile);
        exit(1);
    }

    // Get room number to remove
    printf("Enter Room Number to remove: ");
    scanf("%d", &roomNumber);

    // Read rooms from the file
    while (1) {
        // Read room number
        if (fscanf(roomFile, "Room Number: %d\n", &r.roomNumber) != 1) break;

        // Read other room details
        if (fscanf(roomFile, "Room Type: %s\n", r.roomType) != 1 ||
            fscanf(roomFile, "Price: %f\n", &r.price) != 1 ||
            fscanf(roomFile, "Availability: %s\n", r.availability) != 1 ||
            fscanf(roomFile, "-------------------------\n") != 0) break;

        if (r.roomNumber != roomNumber) {
            // Write room details to the temporary file
            fprintf(tempFile, "Room Number: %d\n", r.roomNumber);
            fprintf(tempFile, "Room Type: %s\n", r.roomType);
            fprintf(tempFile, "Price: %.2f\n", r.price);
            fprintf(tempFile, "Availability: %s\n", r.availability);
            fprintf(tempFile, "-------------------------\n");
        } else {
            found = 1;
        }
    }

    if (!found) {
        printf("Room number %d not found.\n", roomNumber);
    } else {
        printf("Room number %d removed successfully!\n", roomNumber);
    }

    // Close both files
    fclose(roomFile);
    fclose(tempFile);

    // Remove the original file and rename the temporary file
    if (remove("RoomData.txt") != 0) {
        printf("Error removing the original file.\n");
    } else if (rename("TempRoomData.txt", "RoomData.txt") != 0) {
        printf("Error renaming the temporary file.\n");
    }
}


// Function to view available rooms
void viewAvailableRooms() {
    FILE *roomFile;
    struct room r;

    roomFile = fopen("RoomData.txt", "r");
    if (roomFile == NULL) {
        printf("Error opening room data file.\n");
        exit(1);
    }

    printf("\nAvailable Rooms:\n");
    while (fscanf(roomFile, "Room Number: %d\n", &r.roomNumber) != EOF) {
        fscanf(roomFile, "Room Type: %s\n", r.roomType);
        fscanf(roomFile, "Price: %f\n", &r.price);
        fscanf(roomFile, "Availability: %s\n", r.availability);
        fscanf(roomFile, "-------------------------\n");

        if (strcmp(r.availability, "Yes") == 0) {
            printf("Room Number: %d\n", r.roomNumber);
            printf("Room Type: %s\n", r.roomType);
            printf("Price: %.2f\n", r.price);
            printf("Availability: %s\n", r.availability);
            printf("-------------------------\n");
        }
    }

    fclose(roomFile);
}
// Main function
int main() {
    FILE *adminFile;
    int choice, adminChoice;

    // Open admin file
    adminFile = fopen("AdminData", "r");
    if (adminFile == NULL) {
        printf("Error opening admin data file.\n");
        exit(1);
    }

    while (1) {  // Infinite loop for main menu
        // Display menu
        printf("\nChoose user type:\n");
        printf("1. Admin\n");
        printf("2. Customer\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: { // Admin
                if (validateAdminLogin(adminFile)) {
                    while (1) {  // Infinite loop for admin menu
                        // Display admin menu
                        printf("\nAdmin Menu:\n");
                        printf("1. Add Room\n");
                        printf("2. View All Rooms\n");
                        printf("3. Update Room Details\n");
                        printf("4. Remove Room\n");  
                        printf("5. View Bookings\n");  
                        printf("6. Exit Admin Menu\n");
                        printf("Enter your choice: ");
                        scanf("%d", &adminChoice);

                        switch (adminChoice) {
                            case 1:
                                addRoom();
                                break;
                            case 2:
                                viewRooms();
                                break;
                            case 3:
                                updateRoomDetails();
                                break;
                            case 4:
                                removeRoom();
                                break;
                            case 5:
                                // Implement view bookings functionality
                                printf("View Bookings functionality not implemented yet.\n");
                                break;
                            case 6:
                                break;  // Exit admin menu
                            default:
                                printf("Invalid choice. Please try again.\n");
                        }

                        if (adminChoice == 6) {
                            break;  // Exit admin menu loop
                        }
                    }
                } else {
                    printf("Invalid admin credentials.\n");
                }
                break;
            }
            case 2: { // Customer
                while (1) {  // Infinite loop for customer menu
                    // Display customer menu
                    printf("\nCustomer Menu:\n");
                    printf("1. View Rooms\n");
                    printf("2. Book Room\n");
                    printf("3. Exit Customer Menu\n");
                    printf("Enter your choice: ");
                    scanf("%d", &adminChoice);  // Reusing adminChoice for simplicity

                    switch (adminChoice) {
                        case 1:
                            viewAvailableRooms();
                            break;
                        case 2:
                            // Implement book room functionality
                            printf("Book Room functionality not implemented yet.\n");
                            break;
                        case 3:
                            break;  // Exit customer menu
                        default:
                            printf("Invalid choice. Please try again.\n");
                    }

                    if (adminChoice == 3) {
                        break;  // Exit customer menu loop
                    }
                }
                break;
            }
            case 3:
                printf("Exiting program.\n");
                fclose(adminFile);  // Close admin file before exiting
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}

