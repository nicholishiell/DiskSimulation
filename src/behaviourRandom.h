#ifndef RANDOM_WALK_H
#define RANDOM_WALK_H

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <vector>

#include "Vector2d.h"
#include "behaviour.h"
#include "sensorContact.h"

class BehaviourRandom: public Behaviour{

 public:
  BehaviourRandom(float val){
    activationLevel = val;

    staticGain = 1.;

  };

  ~BehaviourRandom(){};
 
  // Returns a velocity vector
  Vector2d * GetResponse(){
    response = new Vector2d( 2.*M_PI*Random() ); 
    return ScalarMultiplyVector(activationLevel, response);
  }

  // Takes in a list of vectors giving the local position of neighbours
  void UpdateStimulus( std::vector<SensorContact*> contacts){
  };
  
  // Display information to the screen.
  void Print(){
    printf("Random\t%f\n",activationLevel);
  };

  
 private:
  
  float Random(){
    return rand()/(float)(RAND_MAX);
  }
};
#endif
