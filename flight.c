#include <stdio.h>    
#include <stdlib.h>   
#include <string.h>   
#include <stdbool.h>  


#define MAX_FLIGHTS 100     
#define MAX_BOOKINGS 100    
#define MAX_STR_LEN 50      
#define FILENAME_FLIGHTS "flights.txt" 
#define FILENAME_BOOKINGS "bookings.txt" 
typedef struct {
    int flightNumber;
    char destination[MAX_STR_LEN];
    int availableSeats;
} Flight;


typedef struct {
    int flightNumber;
} Booking;


Flight flights[MAX_FLIGHTS];
int numFlights = 0; 

Booking bookings[MAX_BOOKINGS];
int numBookings = 0; 
void loadFlights();
void saveFlights();
void loadBookings();
void saveBookings();


void displayMenu();
int getValidatedIntegerInput(); 


void viewAvailableFlights();
void bookFlight();
void cancelReservation();
void viewCurrentBookings();


int findFlightIndex(int flightNum); 

int main() {
    
    loadFlights();
    loadBookings();

    int choice; 
    do {
        displayMenu(); 
        printf("Enter your choice: ");
        choice = getValidatedIntegerInput(); 

        switch (choice) {
            case 1:
                viewAvailableFlights();
                break;
            case 2:
                bookFlight();
                break;
            case 3:
                cancelReservation();
                break;
            case 4:
                viewCurrentBookings();
                break;
            case 5:
                printf("\nExiting Flight Reservation System. Goodbye!\n");
                break;
            default:
                printf("\nInvalid choice. Please enter a number between 1 and 5.\n");
        }
        printf("\n"); 
    } while (choice != 5); 

    return 0; 
}


void loadFlights() {
    FILE *file = fopen(FILENAME_FLIGHTS, "r"); 
    if (file == NULL) {
        printf("Info: '%s' not found. A new file will be created upon saving.\n", FILENAME_FLIGHTS);
        return; 
    }

    numFlights = 0; 
    int i; 
    while (fscanf(file, "%d %s %d", &flights[numFlights].flightNumber,
                  flights[numFlights].destination,
                  &flights[numFlights].availableSeats) == 3) {
        numFlights++;
        if (numFlights >= MAX_FLIGHTS) {
            printf("Warning: Maximum number of flights (%d) reached. Some flights may not be loaded.\n", MAX_FLIGHTS);
            break;
        }
    }
    fclose(file); 
    printf("Flights loaded successfully from '%s'.\n", FILENAME_FLIGHTS);
}


void saveFlights() {
    FILE *file = fopen(FILENAME_FLIGHTS, "w"); 
    if (file == NULL) {
        printf("Error: Could not save flight data to '%s'.\n", FILENAME_FLIGHTS);
        return;
    }

    int i; 
    for (i = 0; i < numFlights; i++) {
        fprintf(file, "%d %s %d\n", flights[i].flightNumber,
                flights[i].destination, flights[i].availableSeats);
    }
    fclose(file);
}


void loadBookings() {
    FILE *file = fopen(FILENAME_BOOKINGS, "r"); 
    if (file == NULL) {
        printf("Info: '%s' not found. A new file will be created upon saving.\n", FILENAME_BOOKINGS);
        return;
    }

    numBookings = 0; 
    int i; 
    while (fscanf(file, "%d", &bookings[numBookings].flightNumber) == 1) { 
        numBookings++;
        if (numBookings >= MAX_BOOKINGS) {
            printf("Warning: Maximum number of bookings (%d) reached. Some bookings may not be loaded.\n", MAX_BOOKINGS);
            break;
        }
    }
    fclose(file); 
    printf("Bookings loaded successfully from '%s'.\n", FILENAME_BOOKINGS);
}


void saveBookings() {
    FILE *file = fopen(FILENAME_BOOKINGS, "w"); 
    if (file == NULL) {
        printf("Error: Could not save booking data to '%s'.\n", FILENAME_BOOKINGS);
        return;
    }

    int i; 
    for (i = 0; i < numBookings; i++) {
        fprintf(file, "%d\n", bookings[i].flightNumber);
    }
    fclose(file); 
}


void displayMenu() {
    printf("--- Flight Reservation System ---\n");
    printf("1. View Available Flights\n");
    printf("2. Book a Flight\n");
    printf("3. Cancel Reservation\n");
    printf("4. View Current Bookings\n");
    printf("5. Exit\n");
    printf("---------------------------------\n");
}


int getValidatedIntegerInput() {
    int input;
    char buffer[256]; 
    char *endptr;     

    while (true) { 
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("Error reading input. Please try again.\n");
            continue; 
        }

    
        buffer[strcspn(buffer, "\n")] = '\0';

        
        if (strlen(buffer) == 0) {
            printf("Invalid input. Input cannot be empty. Please enter a number: ");
            continue;
        }

        
        input = (int)strtol(buffer, &endptr, 10);

        
        if (endptr == buffer || *endptr != '\0') {
            printf("Invalid input. Please enter a valid number: ");
        } else {
            return input; 
        }
    }
}


void viewAvailableFlights() {
    printf("\n--- Available Flights ---\n");
    if (numFlights == 0) {
        printf("No flights available at this time.\n");
        return;
    }
    
    printf("%-15s %-20s %-15s\n", "Flight Number", "Destination", "Available Seats");
    printf("--------------------------------------------------\n");
    
    int i; 
    for (i = 0; i < numFlights; i++) {
        printf("%-15d %-20s %-15d\n", flights[i].flightNumber,
               flights[i].destination, flights[i].availableSeats);
    }
    printf("--------------------------------------------------\n");
}


void bookFlight() {
    printf("\n--- Book a Flight ---\n");
    int flightNum;
    printf("Enter the flight number you wish to book: ");
    flightNum = getValidatedIntegerInput(); 
    int flightIndex = findFlightIndex(flightNum);
    if (flightIndex == -1) {
        printf("Error: Flight %d not found. Please check the flight number.\n", flightNum);
        return;
    }

    
    if (flights[flightIndex].availableSeats <= 0) {
        printf("Sorry, Flight %d to %s has no available seats.\n",
               flightNum, flights[flightIndex].destination);
        return;
    }

    
    if (numBookings >= MAX_BOOKINGS) {
        printf("Error: Maximum number of bookings (%d) reached. Cannot book more flights.\n", MAX_BOOKINGS);
        return;
    }


    flights[flightIndex].availableSeats--; 
    bookings[numBookings].flightNumber = flightNum; 
    numBookings++; 
    saveFlights();
    saveBookings();
    printf("Successfully booked a seat on Flight %d to %s. Enjoy your trip!\n",
           flightNum, flights[flightIndex].destination);
}

void cancelReservation() {
    printf("\n--- Cancel Reservation ---\n");
    int flightNum;
    printf("Enter the flight number of the reservation you wish to cancel: ");
    flightNum = getValidatedIntegerInput(); 

    bool foundBooking = false;
    int i, j; 
    for (i = 0; i < numBookings; i++) {
        if (bookings[i].flightNumber == flightNum) {
            for (j = i; j < numBookings - 1; j++) {
                bookings[j] = bookings[j + 1];
            }
            numBookings--; 
            foundBooking = true; 
            int flightIndex = findFlightIndex(flightNum);
            if (flightIndex != -1) {
                flights[flightIndex].availableSeats++;
                saveFlights(); 
            } else {
                printf("Warning: Flight details for %d not found, but booking was removed.\n", flightNum);
            }

            saveBookings(); 
            printf("Reservation for Flight %d cancelled successfully.\n", flightNum);
            return; 
        }
    }

    
    if (!foundBooking) {
        printf("Error: No reservation found for Flight %d.\n", flightNum);
    }
}


void viewCurrentBookings() {
    printf("\n--- Your Current Bookings ---\n");
    if (numBookings == 0) {
        printf("You have no current bookings.\n");
        return;
    }

    
    printf("%-15s %-20s\n", "Flight Number", "Destination");
    printf("-------------------------------------\n");
    
    int i; 
    for (i = 0; i < numBookings; i++) {
        int flightIndex = findFlightIndex(bookings[i].flightNumber);
        if (flightIndex != -1) {
            
            printf("%-15d %-20s\n", bookings[i].flightNumber,
                   flights[flightIndex].destination);
        } else {
        
            printf("%-15d %-20s (Details unavailable)\n", bookings[i].flightNumber, "N/A");
        }
    }
    printf("-------------------------------------\n");
}


int findFlightIndex(int flightNum) {
    int i; 
    for (i = 0; i < numFlights; i++) {
        if (flights[i].flightNumber == flightNum) {
            return i; 
        }
    }
    return -1; 
}
