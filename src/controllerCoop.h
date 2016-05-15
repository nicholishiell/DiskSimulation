#ifndef CONTROLLER_COOP_H
#define CONTROLLER_COOP_H

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <vector>

#include "Vector2d.h"
#include "sensorContact.h"
#include "controller.h"
#include "behaviour.h"

class ControllerCoop: public Controller {

 public:
  ControllerCoop(){}

  ~ControllerCoop(){}

  void AddBehaviour(Behaviour * newBehaviour){
    behaviourSet.push_back(newBehaviour);
  }

  
  Vector2d * GetVelocityCommand(){

    Vector2d * velocityCommand = new Vector2d(0., 0.);

    // Sum all behaviour responses scaled by gain
    for(unsigned int i = 0; i < behaviourSet.size(); i++){
      
      velocityCommand = 
	AddVectors(velocityCommand, behaviourSet[i]->GetResponse() );
      
    }  
    
    return ScalarMultiplyVector(10.,velocityCommand);
  }


  void UpdateSensors(std::vector<SensorContact*> contacts){ 
      
    for(unsigned int i = 0; i < behaviourSet.size(); i++){ 
      behaviourSet[i]->UpdateStimulus(contacts);
    }
  }

  void Print(){
  }

 private:
 float Random(){
    return rand()/(float)(RAND_MAX);
  }
  
};


#endif
