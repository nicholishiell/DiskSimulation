#ifndef FOLLOW_HEADING_H
#define FOLLOW_HEADING_H

#include <stdio.h>
#include <math.h>
#include <vector>

#include "Vector2d.h"
#include "sensorContact.h"
#include "behaviour.h"


class BehaviourFollowHeading: public Behaviour{

 public:
  BehaviourFollowHeading(Vector2d* a){
    heading = a->GetAngle();

    activationLevel = a->GetNorm();
   
  };
  ~BehaviourFollowHeading(){};
 
  // Returns a velocity vector
  Vector2d * GetResponse(){
    response = new Vector2d(heading);
    response = ScalarMultiplyVector(activationLevel, response);
    return response;
  }

  // Takes in a list of vectors giving the local position of neighbours
  void UpdateStimulus( std::vector<SensorContact*> contacts){
    
  }
  
  // Display information to the screen.
  void Print(){ 
     printf("Follow\t%f\n",activationLevel);
  };


 private:
  float heading;

  Vector2d* response;

};
#endif
