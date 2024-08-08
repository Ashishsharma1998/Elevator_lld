#pragma once

#include "Direction.hpp"
#include "Request.hpp"
#include "GlobalLock.hpp"
#include<queue>
#include<iostream>
#include<condition_variable>
#include<thread>

class Elevator{
  private:
    int id;
    int capacity;
    int currentFloor;
    Direction currentDirection;
    std::queue<Request*> requests;
    std::condition_variable cv;
    std::thread th;
    bool running;
   public:
    Elevator(int id,int capacity){
       this->id=id;
       this->capacity=capacity;
       this->currentDirection=UP;
       this->currentFloor=0;  
       this->running=false;
    } 

    void addRequest(Request *req){
      if(requests.size()<capacity){
         requests.push(req);
         std::cout<<"Elevator id: "<<id<<" added request :"<<req->getSource()<<"-->"<<req->getDestination()<<std::endl;
         cv.notify_all();
      }  
    }

    void processRequests(){
        while(running){
          std::unique_lock<std::mutex> lock(GlobalLock::mtx);
          cv.wait(lock, [this]() { return !requests.empty() || !running; });
            
            if(!running)break;

            Request * req = requests.front();
            requests.pop();

            processRequest(req);

            lock.unlock();  
            
        }  
    }

    void processRequest(Request *req){
        int startFloor = currentFloor;
        int destinationFloor = req->getDestination();
        if(startFloor < destinationFloor){
          currentDirection = UP;
          for(int i=startFloor;i<=destinationFloor;i++){
            currentFloor=i;
            std::cout<<"Elevator "<<id<<" reached Floor "<<currentFloor<<std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Simulating elevator movement
          }
        }else{
      
          currentDirection = DOWN;
          for(int i=startFloor;i>=destinationFloor;i--){
            currentFloor=i;
            std::cout<<"Elevator "<<id<<" reached Floor "<<currentFloor<<std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Simulating elevator movement
          }
        }
    }

    void start() {
        running = true;
        th = std::thread(Elevator::processRequests, this);
    }
    
    void stop() {
        {
            std::lock_guard<std::mutex> lock(GlobalLock::mtx);
            running = false;
        }
        cv.notify_one();
        th.join();
    }

    int getCurrentFloor(){
      return currentFloor;
    }
};