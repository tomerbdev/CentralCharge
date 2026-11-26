//forgive input
#define _CRT_SECURE_NO_WARNINGS

//libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//definitions
#define MAX_LICENSE_STR 9
#define INPUT_BUF_SIZE 100

//initializing array size
#define INITIAL_CAR_CAPACITY 10
#define INITIAL_PORT_CAPACITY 10

//charging mode
#define FAST 1
#define MID 2
#define SLOW 3

//car connection status to port
#define CONNECTED 1
#define UNCONNECTED 0
#define DEFAULT_PORT_ID -1 //default port id if not connected
#define PORT_NOT_FOUND -1

//port status
#define FREE 1
#define TAKEN 2
#define BROKEN 3
#define DEFAULT_OCCUPIED ""

//connect car to port
#define C_CAR_NOT_FOUND -1
#define C_PORT_NOT_FOUND 0
#define C_CONNECTION_COMPLETE 1
#define C_ALREADY_CONNECTED 2

//disconnect car from port
#define D_CAR_NOT_FOUND -1
#define D_DISCONNECTION_COMPLETE 1
#define D_CAR_NOT_CONNECTED 0

//structs: Date, Car and Port
typedef struct {
    int year; //1900-2025
    int month; //1-12
    int day; //1-31
    int hour; //0-23
    int min; //0-59
}Date;

typedef struct {
    char licensePlate[MAX_LICENSE_STR]; //inculde '\0'
    int portType; //FAST=1, MID=2, SLOW=3
    int totalChargedMinutes;
    int isConnected; //0=UNCONNECTED, 1=CONNECTED
    int connectedPortId; //-1=default_port
}Car;

typedef struct {
    int portID; //positive integer
    int portType; //FAST=1, MID=2, SLOW=3
    int status; //1=FREE, 2=TAKEN, 3=BROKEN
    char occupiedByCarLicense[MAX_LICENSE_STR + 1]; //
    Date startTime; //system time
}Port;

//function declarations

//memory AND initialization
void initArrays(Car** carsArray, int* numCars, int* carsCapacity, Port** portsArray, int* numPorts, int* portsCapacity);
void resizeCarsArray(Car** carsArray, int* carsCapacity);
void resizePortsArray(Port** portsArray, int* portsCapacity);
void freeAllMemory(Car* carsArray, Port* portsArray);

//utility
int findPortIndex(Port* portsArray, int numPorts, int portID);
int findCarIndex(Car* carsArray, int numCars, const char* licensePlate);
int calculateTotalMinutes(const Date* start, const Date* end);
int handleChargingTime(Car* car, Port* port, int applyToCar);
void getCurrentTime(Date* date);
void clearInputBuffer(void);
void getValidPlate(char* plateDest);
int getValidPortID(void);
int getValidStatus(void);
int getValidPortType(void);

//car management
void addCar(Car** carsArray, int* numCars, int* carsCapacity);
void printCarDetails(Car* carToDisplay, Port* portsArray, int numPorts);
void displayCar(Car* carsArray, int numCars, Port* portsArray, int numPorts, char* licensePlate);
void displayAllCars(Car* carsArray, int numCars, Port* portsArray, int numPorts);
void deleteCar(Car** carsArray, int* carsCapacity, int* numCars, const char* license, Port* portsArray, int numPorts);

//port management
void addPort(Port** portsArray, int* numPorts, int* portsCapacity);
void updatePortStatus(Port* portsArray, int numPorts, Car* carsArray, int numCars);
void printPortDetails(Port port);
void displayPort(Port* portsArray, int numPorts, int portID);
void displayAllPorts(Port* portsArray, int numPorts);
void deletePort(Port** portsArray, int* portsCapacity, int* numPorts, int portId, Car* carsArray, int numCars);

//connection management
int connectCarToPort(Car* carsArray, int numCars, Port* portsArray, int numPorts);
void portConnectionResult(int resultCode);
int disconnectCarFromPort(Car* carsArray, int numCars, Port* portsArray, int numPorts);
void portDisconnectResult(int resultCode);
void checkCarChargingStatus(Car* carsArray, int numCars, Port* portsArray, int numPorts);

void printMenu(void);
void preloadTestData(Car** carsArray, int* numCars, int* carsCapacity, Port** portsArray, int* numPorts, int* portsCapacity);

int main() {
    Car* carsArray = NULL;
    Port* portsArray = NULL;
    int numCars = 0, numPorts = 0, portsCapacity = 0, carsCapacity = 0, userOption = 0;
    char licensePlate[MAX_LICENSE_STR + 1] = "";

    initArrays(&carsArray, &numCars, &carsCapacity, &portsArray, &numPorts, &portsCapacity);
    preloadTestData(&carsArray, &numCars, &carsCapacity, &portsArray, &numPorts, &portsCapacity);

    while (1) {
        printMenu();

        if (scanf("%d", &userOption) != 1) {
            printf("Invalid option, please try again.\n");
            clearInputBuffer();
            continue;
        }
        clearInputBuffer();

        switch (userOption) {
        case 1:
            displayAllCars(carsArray, numCars, portsArray, numPorts);
            break;
        case 2:
            addCar(&carsArray, &numCars, &carsCapacity);
            break;
        case 3:
            getValidPlate(licensePlate);
            displayCar(carsArray, numCars, portsArray, numPorts, licensePlate);
            break;
        case 4:
            displayAllPorts(portsArray, numPorts);
            break;
        case 5:
            addPort(&portsArray, &numPorts, &portsCapacity);
            break;
        case 6:
            displayPort(portsArray, numPorts, getValidPortID());
            break;
        case 7:
            updatePortStatus(portsArray, numPorts, carsArray, numCars);
            break;
        case 8:
            portConnectionResult(connectCarToPort(carsArray, numCars, portsArray, numPorts));
            break;
        case 9:
            checkCarChargingStatus(carsArray, numCars, portsArray, numPorts);
            break;
        case 10:
            portDisconnectResult(disconnectCarFromPort(carsArray, numCars, portsArray, numPorts));
            break;
        case 11:
            getValidPlate(licensePlate);
            deleteCar(&carsArray, &carsCapacity, &numCars, licensePlate, portsArray, numPorts);
            break;
        case 12:
            deletePort(&portsArray, &portsCapacity, &numPorts, getValidPortID(), carsArray, numCars);
            break;
        case 13:
            freeAllMemory(carsArray, portsArray);
            exit(0);
        default:
            printf("\nInvalid option. Try again.\n");
            continue;
        }
    }

    return 0;//unreachable unless exit() removed
}


//functions implements

void initArrays(Car** carsArray, int* numCars, int* carsCapacity, Port** portsArray, int* numPorts, int* portsCapacity) {
    *carsCapacity = INITIAL_CAR_CAPACITY;
    *portsCapacity = INITIAL_PORT_CAPACITY;
    *numCars = 0, *numPorts = 0;

    *carsArray = malloc(*carsCapacity * sizeof(Car));
    if (!*carsArray) {
        printf("Failed to allocate memory for carsArray\n");
        exit(1);
    }

    *portsArray = malloc(*portsCapacity * sizeof(Port));
    if (!*portsArray) {
        printf("Failed to allocate memory for portsArray\n");
        free(*carsArray);
        exit(1);
    }
}

void resizeCarsArray(Car** carsArray, int* carsCapacity){
    *carsCapacity *= 2;
    Car* temp = realloc(*carsArray, *carsCapacity * sizeof(Car));
    if (!temp) {
        printf("Failed to allocate memory for carsArray\n");
        exit(1);
    }
    *carsArray = temp;
}

void resizePortsArray(Port** portsArray, int* portsCapacity){
    *portsCapacity *= 2;
    Port* temp = realloc(*portsArray, *portsCapacity * sizeof(Port));
    if (!temp) {
        printf("Failed to allocate memory for portsArray\n");
        exit(1);
    }
    *portsArray = temp;
}

void freeAllMemory(Car* carsArray, Port* portsArray){
    free(carsArray);
    free(portsArray);
}

int findPortIndex(Port* portsArray, int numPorts, int portID){
    for (int index = 0; index < numPorts; index++) {
        if (portsArray[index].portID == portID)
            return index;
    }
    return PORT_NOT_FOUND;
}

void getCurrentTime(Date* date) {
    time_t now = time(NULL);
    struct tm* t = localtime(&now);

    date->year = t->tm_year + 1900;
    date->month = t->tm_mon + 1;
    date->day = t->tm_mday;
    date->hour = t->tm_hour;
    date->min = t->tm_min;
}

void clearInputBuffer(void) {
    int input = 0;
    while ((input = getchar()) != '\n' && input != EOF);
}

void getValidPlate(char* plateDest) {
    //buffer to store user input including newline and null terminator
    char inputBuffer[INPUT_BUF_SIZE];

    while (1) {
        //prompt user for input with max allowed characters reminder
        printf("Enter car license plate (1-%d chars): ", MAX_LICENSE_STR);

        //read a line of input from stdin, including the newline if fits
        if (fgets(inputBuffer, sizeof(inputBuffer), stdin) == NULL) {
            //fgets failed, likely EOF or input error
            printf("Input error. Try again.\n");
            continue;  //retry
        }

        //search for newline character in inputBuffer
        char* newline = strchr(inputBuffer, '\n');
        if (newline) {
            //if newline found, replace it with null terminator to remove it
            *newline = '\0';
        }
        else {
            //newline not found means input was longer than buffer - flush remaining chars
            clearInputBuffer();
            printf("Input too long. Max %d chars allowed. Try again.\n", MAX_LICENSE_STR);
            continue;  //retry
        }

        //get length of the cleaned input (without newline)
        size_t len = strlen(inputBuffer);

        //check for empty input (user just pressed Enter)
        if (len == 0) {
            printf("Input cannot be empty. Try again.\n");
            continue;  //retry
        }

        //check if input exceeds maximum allowed license plate length
        if (len > MAX_LICENSE_STR) {
            printf("Input too long. Max %d chars allowed. Try again.\n", MAX_LICENSE_STR);
            continue;  //retry
        }

        //input is valid, copy it to destination buffer (plateDest)
        strcpy(plateDest, inputBuffer);
        break;  //exit loop
    }
}


int getValidPortID(void) {
    int portID = 0;

    while (1) {
        printf("Please enter port ID: ");
        if (scanf("%d", &portID) != 1) {
            printf("Invalid port ID. Must be positive integer.\n");
            clearInputBuffer();
            continue;
        }
        clearInputBuffer();

        //check if ID is positive
        if (portID <= 0)
            printf("Invalid port ID. Must be positive integer.\n");
        else
            break;
    }
    return portID;
}

int getValidStatus(void) {
    char inputBuffer[100];
    int status = 0;
    char* newline;

    while (1) {
        printf("Please enter port status (1=FREE OR 3=BROKEN): ");

        if (fgets(inputBuffer, sizeof(inputBuffer), stdin) == NULL) {
            printf("Input error. Try again.\n");
            continue;
        }

        //remove trailing newline
        newline = strchr(inputBuffer, '\n');
        if (newline) *newline = '\0';

        //check for empty input
        if (strlen(inputBuffer) == 0) {
            printf("Input cannot be empty. Try again.\n");
            continue;
        }

        //parse integer from input string
        if (sscanf(inputBuffer, "%d", &status) != 1) {
            printf("Invalid input. Only 1 (FREE) or 3 (BROKEN) can be set manually.\n");
