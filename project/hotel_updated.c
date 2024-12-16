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

// Define customer structure
struct customer {
    int customerID;
    char customerName[50];
    int roomNumber;
    char bookingDate[20];  // New field for booking date (format: DD/MM/YYYY)
};

// Function prototypes
int validateAdminLogin(FILE *ptr);
void addRoom();
void viewRooms();
void updateRoomDetails();
void removeRoom();
void bookRoom();
void cancelBooking();
void viewBookings();
void viewMyBookings();
int validateRoomType(const char *roomType);
int isRoomAlreadyBooked(int roomNumber);

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

// Function to add a room
void addRoom() {
    struct room r;
    char roomType[25];

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

    // Print room details to the console
    printf("Room Number: %d\n", r.roomNumber);
    printf("Room Type: %s\n", r.roomType);
    printf("Price: %.2f\n", r.price);
    printf("Availability: %s\n", r.availability);
    printf("-------------------------\n");
}

// Function to view rooms
void viewRooms() {
    struct room r;
    int continueInput = 1;

    while (continueInput) {
        // Collect room details (simulating reading from a file)
        printf("Enter Room Number: ");
        scanf("%d", &r.roomNumber);
        printf("Enter Room Type: ");
        scanf("%s", r.roomType);
        printf("Enter Price: ");
        scanf("%f", &r.price);
        printf("Enter Availability: ");
        scanf("%s", r.availability);

        // Print the room details to the console
        printf("\nRoom Number: %d\n", r.roomNumber);
        printf("Room Type: %s\n", r.roomType);
        printf("Price: %.2f\n", r.price);
        printf("Availability: %s\n", r.availability);
        printf("-------------------------\n");

        // Ask if the user wants to continue
        printf("Do you want to continue? (1 for Yes, 0 for No): ");
        scanf("%d", &continueInput);
    }
}

// Function to update room details
void updateRoomDetails() {
    struct room r;
    int roomNumber;
    char newRoomType[25], newAvailability[10];
    float newPrice;

    // Get room number to update
    printf("Enter Room Number to update: ");
    scanf("%d", &roomNumber);

    // Collect and validate room type
    do {
        printf("Enter New Room Type (Single, Double, Suite): ");
        scanf("%s", newRoomType);
        if (!validateRoomType(newRoomType)) {
            printf("Invalid room type. Please enter Single, Double, or Suite.\n");
        }
    } while (!validateRoomType(newRoomType));

    // Collect new price
    printf("Enter New Price: ");
    scanf("%f", &newPrice);

    // Collect new availability status
    printf("Enter New Availability (Yes/No): ");
    scanf("%s", newAvailability);

    // Print updated room details to the console
    printf("Updated Room Details:\n");
    printf("Room Number: %d\n", roomNumber);
    printf("Room Type: %s\n", newRoomType);
    printf("Price: %.2f\n", newPrice);
    printf("Availability: %s\n", newAvailability);
    printf("-------------------------\n");
}

// Function to remove a room
void removeRoom() {
    int roomNumber;

    // Get room number to remove
    printf("Enter Room Number to remove: ");
    scanf("%d", &roomNumber);

    // Print confirmation message to the console
    printf("Room number %d removed successfully!\n", roomNumber);
}

// Function to check if a room is already booked
int isRoomAlreadyBooked(int roomNumber) {
    int bookedRoomNumber;

    // Simulate checking bookings (hardcoded for this example)
    printf("Enter Booked Room Number to check: ");
    scanf("%d", &bookedRoomNumber);

    if (bookedRoomNumber == roomNumber) {
        return 1;  // Room is already booked
    }

    return 0;  // Room is not booked
}

// Function to book a room
void bookRoom() {
    struct customer c;
    static int customerID = 1;  // Auto-incrementing customer ID
    int roomNumber;

    // Get room number to book
    printf("Enter Room Number to book: ");
    scanf("%d", &roomNumber);

    // Check if the room is already booked
    if (isRoomAlreadyBooked(roomNumber)) {
        printf("Room number %d is already booked.\n", roomNumber);
        return;
    }

    // Collect customer details
    printf("Enter Customer Name: ");
    scanf("%s", c.customerName);
    printf("Enter Booking Date (DD/MM/YYYY): ");
    scanf("%s", c.bookingDate);

    // Assign auto-incremented customer ID
    c.customerID = customerID++;

    // Print booking confirmation to the console
    printf("Room %d has been successfully booked by %s (Customer ID: %d) for %s.\n", roomNumber, c.customerName, c.customerID, c.bookingDate);
}

// Function to cancel a booking
void cancelBooking() {
    int customerID, roomNumber;

    // Get customer ID and room number to cancel
    printf("Enter Your Customer ID: ");
    scanf("%d", &customerID);
    printf("Enter Room Number to cancel: ");
    scanf("%d", &roomNumber);

    // Print cancellation message to the console
    printf("Booking for Room %d under Customer ID %d has been successfully cancelled.\n", roomNumber, customerID);
}

// Function to view all bookings
void viewBookings() {
    struct customer c;
    int continueInput = 1;

    while (continueInput) {
        // Collect booking details (simulating reading from a file)
        printf("Enter Customer ID: ");
        scanf("%d", &c.customerID);
        printf("Enter Customer Name: ");
        scanf("%s", c.customerName);
        printf("Enter Room Number: ");
        scanf("%d", &c.roomNumber);
        printf("Enter Booking Date (DD/MM/YYYY): ");
        scanf("%s", c.bookingDate);

        // Print booking details to the console
        printf("\nCustomer ID: %d\n", c.customerID);
        printf("Customer Name: %s\n", c.customerName);
        printf("Room Number: %d\n", c.roomNumber);
        printf("Booking Date: %s\n", c.bookingDate);
        printf("-------------------------\n");

        // Ask if the user wants to continue
        printf("Do you want to continue viewing bookings? (1 for Yes, 0 for No): ");
        scanf("%d", &continueInput);
    }
}

// Function to view bookings by a specific customer
void viewMyBookings() {
    struct customer c;
    int customerID;

    // Get customer ID to view their bookings
    printf("Enter Your Customer ID: ");
    scanf("%d", &customerID);

    // Collect booking details (simulating reading from a file)
    printf("Enter Customer Name: ");
    scanf("%s", c.customerName);
    printf("Enter Room Number: ");
    scanf("%d", &c.roomNumber);
    printf("Enter Booking Date (DD/MM/YYYY): ");
    scanf("%s", c.bookingDate);

    // Print the customer's booking details
    printf("\nCustomer ID: %d\n", customerID);
    printf("Customer Name: %s\n", c.customerName);
    printf("Room Number: %d\n", c.roomNumber);
    printf("Booking Date: %s\n", c.bookingDate);
    printf("-------------------------\n");
}

// Main function with admin menu
int main() {
    int choice;
    FILE *adminFile = fopen("adminData.txt", "r");

    if (adminFile == NULL) {
        printf("Error: Unable to open admin file.\n");
        return 1;
    }

    // Validate admin login
    if (!validateAdminLogin(adminFile)) {
        fclose(adminFile);
        return 1;
    }

    fclose(adminFile);

    do {
        // Display the admin menu
        printf("\n--- Admin Menu ---\n");
        printf("1. Add Room\n");
        printf("2. View Rooms\n");
        printf("3. Update Room Details\n");
        printf("4. Remove Room\n");
        printf("5. Book Room\n");
        printf("6. Cancel Booking\n");
        printf("7. View All Bookings\n");
        printf("8. View My Bookings\n");
        printf("9. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        // Perform action based on user's choice
        switch (choice) {
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
                bookRoom();
                break;
            case 6:
                cancelBooking();
                break;
            case 7:
                viewBookings();
                break;
            case 8:
                viewMyBookings();
                break;
            case 9:
                printf("Exiting program...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }
    } while (choice != 9);

    return 0;
}

