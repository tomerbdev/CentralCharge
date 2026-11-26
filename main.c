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
            continue;
        }

        if (status != FREE && status != BROKEN) {
            printf("Invalid status. Only 1 (FREE) or 3 (BROKEN) can be set manually.\n");
            continue;
        }

        break;
    }

    return status;
}

int getValidPortType(void) {
    char inputBuffer[100];
    int portType = 0;
    char* newline;

    while (1) {
        printf("Please enter port type (1=FAST, 2=MID, 3=SLOW): ");

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
        if (sscanf(inputBuffer, "%d", &portType) != 1) {
            printf("Invalid input. Please enter a number (1=FAST, 2=MID, 3=SLOW).\n");
            continue;
        }

        if (portType != FAST && portType != MID && portType != SLOW) {
            printf("Invalid port type. Must be 1=FAST, 2=MID, or 3=SLOW. Try again.\n");
            continue;
        }

        break;
    }

    return portType;
}

//car management functions
void addCar(Car** carsArray, int* numCars, int* carsCapacity){
    char licensePlate[MAX_LICENSE_STR + 1];

    printf("Please enter new car details to system according to instructions: \n");

    getValidPlate(licensePlate);

    //check if already exists in system
    for (int index = 0; index < *numCars; index++) {
        if (strcmp((*carsArray)[index].licensePlate, licensePlate) == 0) {
            printf("License plate already exists. Returning to Menu.\n");
            return;
        }
    }

    int portType = getValidPortType();

    //creating new car based on input
    Car newCar = { 0 };
    strcpy(newCar.licensePlate, licensePlate);
    newCar.portType = portType;
    newCar.totalChargedMinutes = 0;
    newCar.isConnected = UNCONNECTED;
    newCar.connectedPortId = DEFAULT_PORT_ID;

    //car insertion
    if (*numCars >= *carsCapacity) //memory check
        resizeCarsArray(carsArray, carsCapacity);

    int insertion = *numCars;
    while (insertion > 0 && strcmp((*carsArray)[insertion - 1].licensePlate, newCar.licensePlate) > 0) {
        (*carsArray)[insertion] = (*carsArray)[insertion - 1];
        insertion--;
    }

    (*carsArray)[insertion] = newCar;
    (*numCars)++;

    printf("New car added to system.\n");
}

//car details printer helper for both displayCar and displayAllCars
void printCarDetails(Car* carToDisplay, Port* portsArray, int numPorts) {
    int elapsed = 0;
    if (!carToDisplay) return;

    //if car is connected, update charged minutes based on port's startTime
    if (carToDisplay->isConnected) {
        //find the connected port
        int connectedPortIndex = -1;
        for (int i = 0; i < numPorts; i++) {
            if (portsArray[i].portID == carToDisplay->connectedPortId) {
                connectedPortIndex = i;
                break;
            }
        }

        if (connectedPortIndex != -1) {
            //update charged minutes based on current time and port startTime
            elapsed = handleChargingTime(carToDisplay, &portsArray[connectedPortIndex], 0);
            printf("Current session charging minutes: %d\n", elapsed);
        }
    }

    printf("License plate: %s\n", carToDisplay->licensePlate);
    printf("Port-type: ");
    switch (carToDisplay->portType) {
    case FAST: printf("FAST"); break;
    case MID:  printf("MID"); break;
    case SLOW: printf("SLOW"); break;
    default:   printf("UNKNOWN"); break;
    }
    printf("\n");

    printf("Total charged minutes: %d\n", carToDisplay->totalChargedMinutes + elapsed);
    printf("Connection status: %s\n", carToDisplay->isConnected == CONNECTED ? "Connected" : "Not Connected");

    if (carToDisplay->isConnected)
        printf("Connected port ID: %d\n", carToDisplay->connectedPortId);

    printf("\n");
}

void displayCar(Car* carsArray, int numCars, Port* portsArray, int numPorts, char* licensePlate) {
    for (int index = 0; index < numCars; index++) {
        if (strcmp(carsArray[index].licensePlate, licensePlate) == 0) {
            printf("Car #%d:\n", index);
            printCarDetails(&carsArray[index], portsArray, numPorts);  //use index here, not carIndex
            return;
        }
    }
    printf("Car not found. Returning to Menu.\n");
}

void displayAllCars(Car* carsArray, int numCars, Port* portsArray, int numPorts){
    if (numCars == 0) {
        printf("No cars found. Returning to menu.\n");
        return;
    }

    printf("\nDisplaying all cars in system:\n");
    for (int index = 0; index < numCars; index++) {
        printf("Car #%d:\n", index);
        printCarDetails(&carsArray[index], portsArray, numPorts);
    }
}

//port management functions
void addPort(Port** portsArray, int* numPorts, int* portsCapacity){
    int portID = getValidPortID();

    //check if ID exists
    for (int index = 0; index < *numPorts; index++) {
        if ((*portsArray)[index].portID == portID) {
            printf("Port ID already exists in the system. Returning to Menu.\n");
            return;
        }
    }

    int portType = getValidPortType();
    int status = getValidStatus();

    //creating new port based on input
    Port newPort = { 0 };
    newPort.portID = portID;
    newPort.portType = portType;
    newPort.status = status;
    strcpy(newPort.occupiedByCarLicense, DEFAULT_OCCUPIED);
    memset(&newPort.startTime, 0, sizeof(Date));

    //new port insertion
    if (*numPorts >= *portsCapacity) //memory check
        resizePortsArray(portsArray, portsCapacity);

    int insertion = *numPorts;
    while (insertion > 0 && (*portsArray)[insertion - 1].portID > newPort.portID) {
        (*portsArray)[insertion] = (*portsArray)[insertion - 1];
        insertion--;
    }
    (*portsArray)[insertion] = newPort;
    (*numPorts)++;

    printf("New port added to system.\n");
}

void updatePortStatus(Port* portsArray, int numPorts, Car* carsArray, int numCars){
    int portIndex = -1, newStatus = 0;
    int portID = getValidPortID();

    //search port in system
    for (int index = 0; index < numPorts; index++) {
        if (portsArray[index].portID == portID) {
            portIndex = index;
            break;
        }
    }
    if (portIndex == PORT_NOT_FOUND) {
        printf("Port not found in system.\n");
        return;
    }

    //update new status to port
    while (1) {
        printf("Enter new status (1=FREE, 3=BROKEN): ");
        if (scanf("%d", &newStatus) != 1) {
            printf("Invalid status. Only FREE or BROKEN can be set manually.\n");
            clearInputBuffer();
            continue;
        }
        clearInputBuffer();

        if (newStatus != FREE && newStatus != BROKEN) {
            printf("Invalid status. Only FREE or BROKEN can be set manually.\n");
            continue;
        }
        break;
    }

    //if changing from TAKEN to BROKEN -> disconnect car
    if (portsArray[portIndex].status != newStatus) {
        if (newStatus == BROKEN && portsArray[portIndex].status == TAKEN) {
            //find car occupying port
            char* licensePlate = portsArray[portIndex].occupiedByCarLicense;

            if (licensePlate[0] != '\0') {
                //find car index
                for (int index = 0; index < numCars; index++) {
                    if (strcmp(carsArray[index].licensePlate, licensePlate) == 0) {
                        //add charged minutes before disconnecting
                        handleChargingTime(&carsArray[index], &portsArray[portIndex], 1);

                        //disconnect car
                        carsArray[index].isConnected = UNCONNECTED;
                        carsArray[index].connectedPortId = DEFAULT_PORT_ID;

                        //clear port
                        strcpy(portsArray[portIndex].occupiedByCarLicense, DEFAULT_OCCUPIED);
                        portsArray[portIndex].status = BROKEN;
                        memset(&portsArray[portIndex].startTime, 0, sizeof(Date));

                        printf("Car disconnected due to port status change.\n");
                        break;
                    }
                }
            }
        }

        //update port status
        portsArray[portIndex].status = newStatus;
        printf("Port %d status updated to ", portID);
        switch (newStatus) {
        case FREE: printf("FREE\n"); break;
        case TAKEN: printf("TAKEN\n"); break;
        case BROKEN: printf("BROKEN\n"); break;
        }
    }
    else
        printf("New status is like old status.\n");
}

void printPortDetails(Port port){
    printf("Port ID: %d\n", port.portID);

    printf("Port-type: ");
    switch (port.portType) {
    case FAST: printf("FAST\n"); break;
    case MID: printf("MID\n"); break;
    case SLOW: printf("SLOW\n"); break;
    default: printf("UNKNOWN\n"); break;
    }

    printf("Status: ");
    switch (port.status) {
    case FREE: printf("FREE\n"); break;
    case TAKEN: printf("TAKEN\n"); break;
    case BROKEN: printf("BROKEN\n"); break;
    default: printf("UNKNOWN\n"); break;
    }

    if (port.status == TAKEN)
        printf("Port is occupied by car: %s\n", port.occupiedByCarLicense);
    else
        printf("Port is not occupied.\n");
}

void displayPort(Port* portsArray, int numPorts, int portID){
    for (int index = 0; index < numPorts; index++) {
        if (portsArray[index].portID == portID) {
            printPortDetails(portsArray[index]);
            return;
        }
    }
    printf("Port %d not found.\n", portID);
}

void displayAllPorts(Port* portsArray, int numPorts){
    if (numPorts == 0) {
        printf("No ports in the system.\n");
        return;
    }

    printf("All charging ports:\n");
    for (int index = 0; index < numPorts; index++) {
        printf("\n");
        printPortDetails(portsArray[index]);
    }
}

int findCarIndex(Car* carsArray, int numCars, const char* licensePlate) {
    for (int index = 0; index < numCars; index++) {
        if (strcmp(carsArray[index].licensePlate, licensePlate) == 0)
            return index;
    }
    return C_CAR_NOT_FOUND;
}

int connectCarToPort(Car* carsArray, int numCars, Port* portsArray, int numPorts) {
    char licensePlate[MAX_LICENSE_STR + 1];
    getValidPlate(licensePlate);

    //search for car
    int carIndex = findCarIndex(carsArray, numCars, licensePlate);

    if (carIndex == -1) {
        return C_CAR_NOT_FOUND; //car not found
    }

    if (carsArray[carIndex].isConnected == CONNECTED) {
        return C_ALREADY_CONNECTED; //already connected
    }

    //search for free fitting port
    for (int index = 0; index < numPorts; index++) {
        if (portsArray[index].status == FREE &&
            portsArray[index].portType == carsArray[carIndex].portType) {

            //connect car to port
            carsArray[carIndex].isConnected = CONNECTED;
            carsArray[carIndex].connectedPortId = portsArray[index].portID;

            //update port info
            portsArray[index].status = TAKEN;
            strcpy(portsArray[index].occupiedByCarLicense, licensePlate);
            getCurrentTime(&portsArray[index].startTime);
            return C_CONNECTION_COMPLETE; //connection complete
        }
    }
    return C_PORT_NOT_FOUND; //no suitable port found
}


void portConnectionResult(int resultCode) {
    switch (resultCode) {
    case C_CAR_NOT_FOUND:
        printf("Car not found in the system.\n");
        break;
    case C_ALREADY_CONNECTED:
        printf("Car is already connected to a port.\n");
        break;
    case C_CONNECTION_COMPLETE:
        printf("Car successfully connected to a port.\n");
        break;
    case C_PORT_NOT_FOUND:
        printf("No suitable charging port found for the car.\n");
        break;
    default:
        printf("Unknown result code.\n");
        break;
    }
}


int calculateTotalMinutes(const Date* start, const Date* end) {
    if (!start || !end) return 0;

    struct tm start_tm = { 0 };
    struct tm end_tm = { 0 };

    start_tm.tm_year = start->year - 1900;
    start_tm.tm_mon = start->month - 1;
    start_tm.tm_mday = start->day;
    start_tm.tm_hour = start->hour;
    start_tm.tm_min = start->min;
    start_tm.tm_isdst = -1;

    end_tm.tm_year = end->year - 1900;
    end_tm.tm_mon = end->month - 1;
    end_tm.tm_mday = end->day;
    end_tm.tm_hour = end->hour;
    end_tm.tm_min = end->min;
    end_tm.tm_isdst = -1;

    time_t start_time = mktime(&start_tm);
    time_t end_time = mktime(&end_tm);

    if (start_time == (time_t)-1 || end_time == (time_t)-1 || end_time < start_time) {
        return 0;
    }

    return (int)((end_time - start_time) / 60);
}


int handleChargingTime(Car* car, Port* port, int applyToCar) {
    if (!car || !port) return 0;

    Date now;
    getCurrentTime(&now);

    int minutes = calculateTotalMinutes(&port->startTime, &now);

    if (applyToCar && minutes > 0) {
        car->totalChargedMinutes += minutes;
    }

    return minutes;
}

int disconnectCarFromPort(Car* carsArray, int numCars, Port* portsArray, int numPorts) {
    char licensePlate[MAX_LICENSE_STR + 1];
    getValidPlate(licensePlate);
    int chargedMinutes = 0;

    //search for the car
    int carIndex = findCarIndex(carsArray, numCars, licensePlate);
    if (carIndex == -1) {
        return D_CAR_NOT_FOUND;
    }

    if (carsArray[carIndex].isConnected == UNCONNECTED) {
        return D_CAR_NOT_CONNECTED;
    }

    //find connected port
    int portIndex = findPortIndex(portsArray, numPorts, carsArray[carIndex].connectedPortId);
    if (portIndex == PORT_NOT_FOUND) {
        return D_CAR_NOT_CONNECTED;
    }

    //apply charging time to car before resetting anything
    chargedMinutes = handleChargingTime(&carsArray[carIndex], &portsArray[portIndex], 1);
    printf("Car was charging for %d minutes.\n", chargedMinutes);

    //disconnect car from port
    carsArray[carIndex].isConnected = UNCONNECTED;
    carsArray[carIndex].connectedPortId = DEFAULT_PORT_ID;

    //reset port status, clear occupant info and startTime
    portsArray[portIndex].status = FREE;
    strcpy(portsArray[portIndex].occupiedByCarLicense, DEFAULT_OCCUPIED);
    memset(&portsArray[portIndex].startTime, 0, sizeof(Date));

    printf("Total charged minutes now: %d\n", carsArray[carIndex].totalChargedMinutes);

    return D_DISCONNECTION_COMPLETE;
}

void portDisconnectResult(int resultCode) {
    switch (resultCode) {
    case D_CAR_NOT_FOUND:
        printf("Car not found in system.\n");
        break;
    case D_CAR_NOT_CONNECTED:
        printf("Car is not connected to any port.\n");
        break;
    case D_DISCONNECTION_COMPLETE:
        printf("Car disconnected successfully.\n");
        break;
    default:
        printf("Unknown result.\n");
        break;
    }
}

void checkCarChargingStatus(Car* carsArray, int numCars, Port* portsArray, int numPorts) {
    char licensePlate[MAX_LICENSE_STR + 1];
    getValidPlate(licensePlate);

    int carIndex = findCarIndex(carsArray, numCars, licensePlate);
    if (carIndex == -1) {
        printf("Car not found charging, returning to menu.\n");
        return;
    }

    if (carsArray[carIndex].isConnected == UNCONNECTED) {
        printf("Car is not currently connected to any port.\n");
        return;
    }

    int portIndex = findPortIndex(portsArray, numPorts, carsArray[carIndex].connectedPortId);
    if (portIndex == PORT_NOT_FOUND) {
        printf("Port connected to car not found.\n");
        return;
    }

    int minutesCharging = calculateTotalMinutes(&portsArray[portIndex].startTime, &(Date){0});
    Date now;
    getCurrentTime(&now);
    minutesCharging = calculateTotalMinutes(&portsArray[portIndex].startTime, &now);

    printf("Car %s has been charging for %d minutes.\n", licensePlate, minutesCharging);
}

void deleteCar(Car** carsArray, int* carsCapacity, int* numCars, const char* license, Port* portsArray, int numPorts) {
    int index = -1;

    // Find car by license
    for (int i = 0; i < *numCars; i++) {
        if (strcmp((*carsArray)[i].licensePlate, license) == 0) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("Can not delete. Car with license %s not found. Return to menu.\n", license);
        return;
    }

    //if connected, disconnect first
    if ((*carsArray)[index].isConnected == CONNECTED) {
        int portIndex = -1;
        for (int j = 0; j < numPorts; j++) {
            if (portsArray[j].portID == (*carsArray)[index].connectedPortId) {
                portIndex = j;
                break;
            }
        }

        if (portIndex != -1) {
            //update charged minutes and disconnect
            int chargedMinutes = handleChargingTime(&(*carsArray)[index], &portsArray[portIndex], 0);
            printf("Car was charging for %d minutes before deletion.\n", chargedMinutes);

            //disconnect car
            (*carsArray)[index].isConnected = UNCONNECTED;
            (*carsArray)[index].connectedPortId = DEFAULT_PORT_ID;

            //free the port
            portsArray[portIndex].status = FREE;
            strcpy(portsArray[portIndex].occupiedByCarLicense, DEFAULT_OCCUPIED);
            memset(&portsArray[portIndex].startTime, 0, sizeof(Date));
            printf("Car disconnected from port before deletion.\n");
        }
        else {
            printf("Warning: connected port not found, cannot disconnect car properly.\n");
        }
    }

    //shift cars left to remove the car
    for (int i = index; i < (*numCars) - 1; i++) {
        (*carsArray)[i] = (*carsArray)[i + 1];
    }

    (*numCars)--;

    printf("Car %s deleted successfully.\n", license);

    //resize array if too sparse
    if (*numCars < *carsCapacity / 4 && *carsCapacity > 1) {
        int newCapacity = *carsCapacity / 2;
        Car* temp = realloc(*carsArray, newCapacity * sizeof(Car));
        if (temp != NULL) {
            *carsArray = temp;
            *carsCapacity = newCapacity;
            printf("Cars array resized to capacity %d.\n", newCapacity);
        }
        else {
            printf("Warning: resizing cars array failed.\n");
        }
    }
}

void deletePort(Port** portsArray, int* portsCapacity, int* numPorts, int portId, Car* carsArray, int numCars) {
    int index = -1;
    int chargedMinutes = 0;

    //find port by id
    for (int i = 0; i < *numPorts; i++) {
        if ((*portsArray)[i].portID == portId) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("Port %d not found.\n", portId);
        return;
    }

    //disconnect any car connected to this port
    for (int i = 0; i < numCars; i++) {
        if (carsArray[i].isConnected == CONNECTED && carsArray[i].connectedPortId == portId) {
            // Update charged minutes and disconnect car
            chargedMinutes = handleChargingTime(&carsArray[i], &(*portsArray)[index], 1);
            printf("Car %s was charging for %d minutes before port deletion.\n", carsArray[i].licensePlate, chargedMinutes);

            carsArray[i].isConnected = UNCONNECTED;
            carsArray[i].connectedPortId = DEFAULT_PORT_ID;

            printf("Car %s disconnected from port %d before port deletion.\n", carsArray[i].licensePlate, portId);
        }
    }

    //shift ports left to remove the port
    for (int i = index; i < (*numPorts) - 1; i++) {
        (*portsArray)[i] = (*portsArray)[i + 1];
    }

    (*numPorts)--;

    printf("Port %d deleted successfully.\n", portId);

    //resize array if too sparse
    if (*numPorts < *portsCapacity / 4 && *portsCapacity > 1) {
        int newCapacity = *portsCapacity / 2;
        Port* temp = realloc(*portsArray, newCapacity * sizeof(Port));
        if (temp != NULL) {
            *portsArray = temp;
            *portsCapacity = newCapacity;
            printf("Ports array resized to capacity %d.\n", newCapacity);
        }
        else {
            printf("Warning: resizing ports array failed.\n");
        }
    }
}

void printMenu(void) {
    printf("\nCentral Charge Menu, please choose one of the options (1-13):\n");
    printf("1. Display all cars\n");
    printf("2. Add a new car\n");
    printf("3. Display car details\n");
    printf("4. Display all charging ports\n");
    printf("5. Add a new charging port\n");
    printf("6. Display port details\n");
    printf("7. Update port status\n");
    printf("8. Connect a car to port\n");
    printf("9. Check car charging status\n");
    printf("10. Disconnect a car from port\n");
    printf("11. Delete car\n");
    printf("12. Delete charging port\n");
    printf("13. Exit system\n");
    printf("\nEnter your choice: ");
}

//helper to subtract minutes from current time into Date struct
void getPastTime(Date* date, int minutesAgo) {
    time_t now = time(NULL);
    time_t past = now - (minutesAgo * 60);
    struct tm* tm_info = localtime(&past);

    date->year = tm_info->tm_year + 1900;
    date->month = tm_info->tm_mon + 1;
    date->day = tm_info->tm_mday;
    date->hour = tm_info->tm_hour;
    date->min = tm_info->tm_min;
}

//format Date to string for printing
void formatDate(const Date* date, char* buffer, int bufsize) {
    snprintf(buffer, bufsize, "%04d-%02d-%02d %02d:%02d",
        date->year, date->month, date->day, date->hour, date->min);
}

void preloadTestData(Car** carsArray, int* numCars, int* carsCapacity,
    Port** portsArray, int* numPorts, int* portsCapacity) {
    while (*carsCapacity < 15)
        resizeCarsArray(carsArray, carsCapacity);
    while (*portsCapacity < 15)
        resizePortsArray(portsArray, portsCapacity);

    const char* plates[15] = {
        "11111", "12345", "22222", "23344", "334455",
        "44444", "556677", "66666", "77777", "778899",
        "88888", "98765", "998877", "99999", "112233"
    };

    int carTypes[15] = {
        2, 3, 2, 1, 2,
        3, 3, 1, 3, 3,
        1, 2, 1, 2, 1
    };

    int portIDs[15] = {
        1001, 1100, 1110, 2222, 3003,
        3333, 4200, 5555, 6100, 7000,
        7500, 8005, 8888, 9001, 9900
    };

    int portTypes[15] = {
        2, 3, 1, 3, 1,
        2, 3, 1, 3, 1,
        2, 3, 2, 2, 1
    };

    //initialize ports first
    for (int i = 0; i < 15; i++) {
        (*portsArray)[i].portID = portIDs[i];
        (*portsArray)[i].portType = portTypes[i];
        (*portsArray)[i].status = FREE;
        strcpy((*portsArray)[i].occupiedByCarLicense, "");
        memset(&(*portsArray)[i].startTime, 0, sizeof(Date));
    }
    *numPorts = 15;

    //initialize cars and connect to ports with staggered past times
    for (int i = 0; i < 15; i++) {
        strcpy((*carsArray)[i].licensePlate, plates[i]);
        (*carsArray)[i].portType = carTypes[i];
        (*carsArray)[i].totalChargedMinutes = 0;
        (*carsArray)[i].isConnected = CONNECTED;
        (*carsArray)[i].connectedPortId = portIDs[i]; //connect car to matching port by index

        //update port status and occupied car
        (*portsArray)[i].status = TAKEN;
        strcpy((*portsArray)[i].occupiedByCarLicense, plates[i]);

        //asign startTime as different past time:
        int minutesAgo = (2 * 24 * 60) + (i * 3 * 60); //2 days + i*3 hours in minutes
        getPastTime(&(*portsArray)[i].startTime, minutesAgo);

        //calculate minutes charging so far and update car
        int chargedMins = handleChargingTime(&(*carsArray)[i], &(*portsArray)[i], 0);

        char startTimeStr[20];
        formatDate(&(*portsArray)[i].startTime, startTimeStr, sizeof(startTimeStr));

        printf("Car %s connected to port %d at %s, charged minutes so far: %d\n",
            plates[i], portIDs[i], startTimeStr, chargedMins);
    }
    *numCars = 15;

    printf("\nATTENTION! FOR TESTING: Preloaded 15 cars and 15 ports with staggered past connection times.\n");
}
