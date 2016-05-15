#ifndef AVOID_H
#define AVOID_H

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <vector>

#include "Vector2d.h"
#include "behaviour.h"
#include "sensorContact.h"

class BehaviourAvoid: public Behaviour{

 public:

  BehaviourAvoid(float val){
    proxThreshold = val;
    response = new Vector2d(0.,0.);		 
 
    seed = Random();
    //if(Random() > 0.5) seed = -1.*seed;
  };

  ~BehaviourAvoid(){}; 

   // Returns a velocity vector
  Vector2d * GetResponse(){
      
    delete response;
    
    if(activationLevel > 0.){
      //response = new Vector2d( bearing2Obstacle - M_PI );
      //response = new Vector2d( bearing2Obstacle - M_PI - Random() );
      response = new Vector2d( bearing2Obstacle - M_PI - seed);
    }
    else{
      response = new Vector2d(0.,0.);
    }
    //response->Print();
    return response;
  }

  // Takes in a list of vectors giving the local position of neighbours
  void UpdateStimulus( std::vector<SensorContact*> contacts) {
    activationLevel = 0.;
    float closestDist = 999999.;
    int closestID = 0;
    
    // Loop through contacts and find the closest one
    
    for(unsigned int i =0; i < contacts.size(); i++ ){
      
      Vector2d * localPos = contacts[i]->GetVector();
      
      float dist = localPos->GetNorm();
      if(dist < closestDist){
	closestDist = dist;
	closestID = i;
      }
     
    }
    
    // Now that we have the closest one we can determine if activates the behaviour
    if(closestDist <= proxThreshold){
      activationLevel = 1.;
      bearing2Obstacle = contacts[closestID]->GetVector()->GetAngle();
    }
    else{
      activationLevel = 0.;
    }
  
  };
  
  // Display information to the screen.
  void Print(){
    printf("Avoid\t%f\n",activationLevel);
  };


 private:
  
  float Random(){
    return rand()/(float)(RAND_MAX);
  }

  float proxThreshold;
  float bearing2Obstacle;
  float seed;
 
};
#endif
