#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <stdio.h>
#include <math.h>
#include <vector>

#include "Vector2d.h"
#include "behaviour.h"
#include "sensorContact.h"

class Controller{

 public:

  Controller(){};
  ~Controller(){};
  
  virtual Vector2d * GetVelocityCommand() = 0;

  virtual void UpdateSensors(std::vector<SensorContact*> contacts) = 0;
 
  int GetNumberOfBehaviours(){
    return behaviourSet.size();
  }

  std::vector<Vector2d*> GetBehaviourResponseVectors(){
    std::vector<Vector2d*> responseVectors;
    
    for(unsigned int i = 0; i < behaviourSet.size(); i++){
      Vector2d* aVec = behaviourSet[i]->GetLastResponse();
      responseVectors.push_back(aVec);
    }
 
  return responseVectors;
  }
 
 protected:
  
  std::vector<Behaviour*> behaviourSet;


 private:
  
};
#endif
