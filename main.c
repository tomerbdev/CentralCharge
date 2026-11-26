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

