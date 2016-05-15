#ifndef BEHAVIOUR_H
#define BEHAVIOUR_H

#include <stdio.h>
#include <math.h>
#include <vector>

#include "Vector2d.h"

#include "sensorContact.h"

class Behaviour{

 public:
  Behaviour(){
    active = false;
  };
  ~Behaviour(){};
 
  // Returns a velocity vector
  virtual Vector2d * GetResponse() = 0;

  // Takes in a list of vectors giving the local position of neighbours
  virtual void UpdateStimulus( std::vector<SensorContact*> contacts) = 0;

  // Display information to the screen.
  virtual void Print() = 0;

  // Return the activation level of the behaviour
  float GetActivationLevel(){
    return activationLevel;
  }

  bool IsActive(){
    return active;
  }

  // Returns value of gain parameter
  float GetGain(){
    return staticGain;
  }
 
  Vector2d* GetLastResponse(){
    return response;
  }    
  
 protected:
  float activationLevel;
  float staticGain;

  bool active;

  Vector2d* response;
};
#endif
