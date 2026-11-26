# CentralCharge — EV Charging Station Management System

A command-line Electric Vehicle (EV) charging station management system written in C11. It manages vehicles, charging ports, active charging sessions, and elapsed billing/charging durations using dynamic memory allocation.

## Features
- **Vehicle Management:** Register, view, search by license plate, and delete vehicles stored in sorted dynamic arrays.
- **Charging Port Management:** Add, inspect, delete, and toggle port status (`FREE`, `TAKEN`, `BROKEN`) across `FAST`, `MID`, and `SLOW` charging tiers.
- **Session & Time Tracking:** Automatically matches cars to compatible free ports, calculates elapsed charging minutes from system timestamps, and updates totals on disconnect or port fault.
- **Dynamic Memory Scaling:** Automatically expands (`2x`) and shrinks (`0.5x`) heap-allocated arrays as entries are added or removed.
- **Preloaded Test Environment:** Initializes 15 vehicles and 15 ports with staggered past connection times for immediate testing.

## Build & Run
```bash
mkdir -p build && cd build
cmake ..
cmake --build .
./CentralCharge
```

## Menu Options
1. Display all cars
2. Add a new car
3. Display car details
4. Display all charging ports
5. Add a new charging port
6. Display port details
7. Update port status
8. Connect a car to port
9. Check car charging status
10. Disconnect a car from port
11. Delete car
12. Delete charging port
13. Exit system
