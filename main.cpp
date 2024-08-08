#include "ElevatorController.hpp"
#include "Request.hpp"

int main(){
  
  ElevatorController *elevator = new ElevatorController(3,1);
  elevator->requestElevator(0,8);

  elevator->requestElevator(2,6);
 
  elevator->requestElevator(4,9);

  std::this_thread::sleep_for(std::chrono::seconds(1000));
  return 0;
}