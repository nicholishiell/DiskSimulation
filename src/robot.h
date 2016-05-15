#ifndef ROBOT
#define ROBOT

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <sstream>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "xHandle.h"
#include "displayable.h"
#include "sensorContact.h"
#include "Vector2d.h"
#include "controller.h"

using namespace std;

class Robot: public Displayable{
 public:
  Robot(Vector2d *pos, Vector2d *vel, int c = -1, Controller * aContr = NULL){

    positionVector = new Vector2d(pos->GetX(), pos->GetY());
    positionVectorPrev = new Vector2d(pos->GetX(), pos->GetY());

    velocityVector = vel;
    velocityCmdVector = vel;
    
    colour = c;
 
    radius = 5.;
    mass = 1.;

    aController = aContr;

    totalDistance = 0.;
    travelLog.push_back(pos->GetCopy());  
  };

  ~Robot(){
   
  }
   // This lets the xHandle draw the robot.
  virtual void paint(xHandle* xHandler){
   
    //Window win = xHandler->win;
    GC gc = xHandler->gc;
  
    float x = positionVector->x;
    float y = positionVector->y;
    //float bearing = velocityCmdVector->GetAngle();
    float bearing = velocityVector->GetAngle();

    // Draw the body of the robot.
    //XDrawArc(xHandler->display, win, gc, x-radius, y-radius, 2.*radius, 2.*radius, 0, 360*64);
    XDrawArc(xHandler->display, xHandler->backBuffer, gc, x-radius, y-radius, 2.*radius, 2.*radius, 0, 360*64);
 
    // Calculate the end point of the bearing indicator
    int velX = x + radius*cos(bearing);
    int velY = y + radius*sin(bearing);

    XDrawLine(xHandler->display, xHandler->backBuffer, gc, x, y, velX, velY);
   
    ostringstream id;
    if(colour < 10) id <<"0"<<colour;
    else id <<colour;
    XDrawString(xHandler->display, xHandler->backBuffer, gc, x, y, id.str().c_str(), 2);
    // Loop over sensor contacts and draw them too!
    /*for(unsigned int i = 0; i < sensorContacts.size();i++){
      int contX = x+sensorContacts[i]->GetVector()->GetX();
      int contY = y+sensorContacts[i]->GetVector()->GetY();
      
      if(aController != NULL)
      XDrawLine(xHandler->display, xHandler->backBuffer, gc, x, y, contX, contY);
      }*/

    //XDrawPoint(disp, win, gc, x, y);
  }
  

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // These are the methods called by the main simulation loop
  // in the referee class.
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  void UpdateSensors(SensorContact* aContact){
    sensorContacts.push_back(aContact);
  }
 
  void ResetSensorContacts(){
    //sensorContacts.clear();
    for(unsigned int i = 0; i < sensorContacts.size(); i++){
      delete sensorContacts[i];
    }
    sensorContacts.clear();
  }

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  void UpdateVelocityCommand(){
    
    if( aController != NULL){
      delete velocityVector;
      
      aController->UpdateSensors(sensorContacts);

      velocityVector = aController->GetVelocityCommand();
      velocityCmdVector = 
	new Vector2d(velocityVector->GetX(), velocityVector->GetY());
    }
    
  }
 
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  
  Vector2d * GetPositionVector(){
    return positionVector;
  }
  
  Vector2d * GetVelocityVector(){
    return velocityVector;
  } 

  Vector2d * GetVelocityCmdVector(){
    return velocityCmdVector;
  } 

  int GetColour(){return colour;} 
  float GetMass(){return mass;}
  float GetRadius(){return radius;}

  float GetTotalDistance(){
    return totalDistance;
  }
  vector<Vector2d*> GetTravelLog(){
    return travelLog;
  }

  Vector2d* GetMomentum(){
    return ScalarMultiplyVector(mass, velocityVector);
  }
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  void UpdateVelocity(Vector2d* aVec){
    delete velocityVector;
    velocityVector = aVec;
  }

  void UpdatePosition( Vector2d * newPosition){
    // Log travel!
    travelLog.push_back(newPosition->GetCopy());
    
    Vector2d* disp = SubtractVectors(positionVector, newPosition);
    totalDistance += disp->GetNorm();    

    // Now update position!
    delete positionVector;
    positionVector = newPosition; 

    delete disp;
  }
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  
  void Display(){
    float px =  positionVector->GetX();
    float py =  positionVector->GetY();
    float vx =  velocityVector->GetX();
    float vy =  velocityVector->GetY();

    printf ("%f\t%f\t%f\t%f\n",px, py, vx, vy);
    
    //for(unsigned int i = 0; i < sensorContacts.size(); i++){
    // sensorContacts[i]->Print();
    //}
  }
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 
 private:
  
  float GetRandom(){
    return (float)rand()/(float)RAND_MAX;
  }

  Vector2d * positionVector;
  Vector2d * positionVectorPrev;
  Vector2d * velocityVector;
  Vector2d * velocityCmdVector;

  Controller *  aController;

  float mass;
  float radius;

  int colour; // 1 = red, 2 = green, 3 = blue, 4 = yellow

  vector<SensorContact*> sensorContacts;

  vector<Vector2d*> travelLog;
  float totalDistance;
};

#endif
