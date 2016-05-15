#ifndef CONTACT_H
#define CONTACT_H

#include <stdio.h>
#include <math.h>
#include <vector>

#include "Vector2d.h"

class SensorContact{

 public:
  SensorContact(int type, Vector2d * v){
    contactType = type;
    contactVector = v;
  };
  
  ~SensorContact(){
    delete contactVector;
  };
 
  // Display information to the screen.
  void Print(){
    printf("%f %f %c\n", contactVector->x, contactVector->y, contactType);
  };

  int GetType(){
    return contactType;
  }    
 
  Vector2d * GetVector(){
    return contactVector;
  }


 protected:

  Vector2d * contactVector;
  int contactType;

};
#endif
