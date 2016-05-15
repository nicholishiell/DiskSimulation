#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <stdio.h>
#include "controller.h"
#include "Vector2d.h"

class Obstacle{
 public:
   
  Obstacle(Vector2d * pVector, float r){
    px0 = pVector->x;
    py0 = pVector->y;
  
    printf("Obstacle Created at (%f, %f).\n", px0, py0);

    radius = r;
  }
  
  ~Obstacle(){
    printf("Agent Deleted.\n");
  };

  Vector2d* GetPosition(){
    return new Vector2d(px0,py0);
  }

  float GetRadius(){
    return radius;
  }

 private:
  
  float px0, py0;

  float radius;
};
#endif
