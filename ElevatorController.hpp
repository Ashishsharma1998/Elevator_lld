#pragma once

#include "Elevator.hpp"
#include<vector>
#include<thread>

class ElevatorController{
  private:
    std::vector<Elevator*> elevators;
  public:
   ElevatorController(int numElevator,int capacity){
     for(int i=0;i<numElevator;i++){
        Elevator *elevator = new Elevator(i+1,capacity);
        elevators.push_back(elevator);
        elevator->start();
     }
   }

   ~ElevatorController() {
        for (auto& elevator : elevators) {
            elevator->stop();
        }
    }    

    void requestElevator(int source,int destination){
       std::lock_guard<std::mutex> lock(GlobalLock::mtx);
       Elevator *elevator = findOptimalElevator(source,destination);
       Request *req = new Request(source,destination); 
       elevator->addRequest(req);
    }

    Elevator* findOptimalElevator(int src,int dest){
      int current=1e9;
      Elevator * optimalElevator = nullptr;
       for(auto elevator:elevators){
          int currentFloor = elevator->getCurrentFloor();
          if(abs(currentFloor-src)<current){
              current=abs(currentFloor-src);
              optimalElevator=elevator;
          }
       }
       return optimalElevator;
    }
};