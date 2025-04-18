#include<bits/stdc++.h>
using namespace std;

enum Direction {
    UP,
    DOWN,
    IDLE
};

enum Status {
    MOVING,
    STOPPED,
    MAINTENANCE
};

enum RequestType {
    EXTERNAL,
    INTERNAL
};

class Request {
public:
    int floor;
    Direction direction;
    RequestType type;

    Request(int f, Direction d) : floor(f), direction(d), type(EXTERNAL) {}
    Request(int f) : floor(f), direction(IDLE), type(INTERNAL) {}
};

class Elevator {
private:
    int id;
    int currentFloor;
    Status status;
    Direction direction;
    bool inMaintenance;
    set<int> externalRequests;
    set<int> internalRequests;

public:
    Elevator(int id, int startFloor = 0)
        : id(id), currentFloor(startFloor), status(STOPPED),
          direction(IDLE), inMaintenance(false) {}

    int getId() const { return id; }
    int getCurrentFloor() const { return currentFloor; }
    Status getStatus() const { return inMaintenance ? MAINTENANCE : status; }
    Direction getDirection() const { return direction; }

    void pressButtonInside(int floor) {
        if (!inMaintenance) {
            internalRequests.insert(floor);
            updateDirection();
        }
    }

    void addExternalRequest(int floor) {
        if (!inMaintenance) {
            externalRequests.insert(floor);
            updateDirection();
        }
    }

    void step() {
        if (inMaintenance) {
            cout << "Elevator " << id << " is under maintenance.\n";
            return;
        }

        // Merge all requests
        for (int f : internalRequests) externalRequests.insert(f);
        internalRequests.clear();

        if (externalRequests.empty()) {
            direction = IDLE;
            status = STOPPED;
            return;
        }

        status = MOVING;
        if (direction == UP) currentFloor++;
        else if (direction == DOWN) currentFloor--;

        if (externalRequests.count(currentFloor)) {
            cout << "Elevator " << id << " stopped at floor " << currentFloor << "\n";
            externalRequests.erase(currentFloor);
            status = STOPPED;
        }

        updateDirection();
    }

    void updateDirection() {
        if (externalRequests.empty()) {
            direction = IDLE;
            return;
        }

        int maxDest = *externalRequests.rbegin();
        int minDest = *externalRequests.begin();

        if (currentFloor < minDest)
            direction = UP;
        else if (currentFloor > maxDest)
            direction = DOWN;
        else {
            // Pick the closest in the direction
            for (int f : externalRequests) {
                if (f > currentFloor) {
                    direction = UP;
                    return;
                } else if (f < currentFloor) {
                    direction = DOWN;
                    return;
                }
            }
            direction = IDLE;
        }
    }

    void setMaintenance(bool value) {
        inMaintenance = value;
        if (value) {
            externalRequests.clear();
            internalRequests.clear();
            status = MAINTENANCE;
        } else {
            status = STOPPED;
        }
    }

    void printStatus() const {
        string dirStr = (direction == UP ? "UP" : direction == DOWN ? "DOWN" : "IDLE");
        string statusStr = (getStatus() == MOVING ? "MOVING" :
                                 getStatus() == STOPPED ? "STOPPED" : "MAINTENANCE");

        cout << "[Elevator " << id << "] Floor: " << currentFloor
                  << " | Direction: " << dirStr
                  << " | Status: " << statusStr << "\n";
    }
};

class ElevatorController {
private:
    vector<Elevator> elevators;

public:
    ElevatorController(int count) {
        for (int i = 0; i < count; ++i)
            elevators.emplace_back(i);
    }

    void handleExternalRequest(int requestedFloor, Direction dir) {
        int bestIdx = -1;
        int minDistance = INT_MAX;

        for (int i = 0; i < elevators.size(); ++i) {
            Elevator& e = elevators[i];
            if (e.getStatus() == MAINTENANCE) continue;

            int currFloor = e.getCurrentFloor();
            Direction eDir = e.getDirection();

            bool sameDir = (eDir == dir);
            bool willPass = (dir == UP && currFloor <= requestedFloor) ||
                            (dir == DOWN && currFloor >= requestedFloor);

            int dist = abs(currFloor - requestedFloor);

            if (eDir == IDLE || (sameDir && willPass)) {
                if (dist < minDistance) {
                    minDistance = dist;
                    bestIdx = i;
                }
            }
        }

        if (bestIdx != -1) {
            elevators[bestIdx].addExternalRequest(requestedFloor);
        } else {
            cout << "No suitable elevator found. Try again later.\n";
        }
    }

    void handleInternalRequest(int elevatorId, int floor) {
        if (elevatorId >= 0 && elevatorId < elevators.size())
            elevators[elevatorId].pressButtonInside(floor);
    }

    void setElevatorMaintenance(int elevatorId, bool value) {
        if (elevatorId >= 0 && elevatorId < elevators.size())
            elevators[elevatorId].setMaintenance(value);
    }

    void stepAll() {
        for (auto& elevator : elevators) {
            elevator.step();
            elevator.printStatus();
        }
        cout << "----------------------------\n";
    }
};

class ElevatorSystem {
private:
    ElevatorController controller;

public:
    ElevatorSystem(int count) : controller(count) {}

    void requestFromOutside(int floor, Direction dir) {
        controller.handleExternalRequest(floor, dir);
    }

    void requestFromInside(int elevatorId, int floor) {
        controller.handleInternalRequest(elevatorId, floor);
    }

    void putElevatorInMaintenance(int elevatorId) {
        controller.setElevatorMaintenance(elevatorId, true);
    }

    void removeElevatorFromMaintenance(int elevatorId) {
        controller.setElevatorMaintenance(elevatorId, false);
    }

    void runSteps(int steps) {
        for (int i = 0; i < steps; ++i)
            controller.stepAll();
    }
};

int main() {
    ElevatorSystem system(3); 

    // External requests
    system.requestFromOutside(3, UP);
    system.requestFromOutside(6, DOWN);
    system.requestFromOutside(9, DOWN);

    // Simulate 2 steps
    system.runSteps(2);

    // Internal request in elevator 0
    system.requestFromInside(0, 7);
    system.requestFromInside(0, 1);

    // Put elevator 2 in maintenance
    system.putElevatorInMaintenance(2);

    // Run 6 more steps
    system.runSteps(6);

    // Bring elevator 2 back
    system.removeElevatorFromMaintenance(2);
    system.requestFromOutside(4, UP);
    system.runSteps(4);

    return 0;
}
