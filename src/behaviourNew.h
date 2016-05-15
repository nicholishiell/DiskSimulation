#ifndef NEW_H
#define NEW_H

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "behaviour.h"
#include "Vector2d.h"
#include "sensorContact.h"

class BehaviourNew: public Behaviour{

 public:

 BehaviourNew(vector<float> edgeBearings){ 

   for(unsigned int i = 0; i < edgeBearings.size(); i++){
     edgeNormals.push_back( new Vector2d(edgeBearings[i]) );
   }

   zoneCounts = new int[edgeNormals.size()];

   formationNormal = edgeNormals[0];
   UpdateConstraints();
            
   if(Random() > 0.5) leftHanded = true;
   else leftHanded = false;
 }
 //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ~BehaviourNew(){};
 //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 void UpdateConstraints(){
   float bearingConstraint = formationNormal->GetAngle() + M_PI/2.; 
   f = new Vector2d(bearingConstraint);
 } 

 //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 // Returns a velocity vector
 Vector2d * GetResponse(){

   Vector2d * r = new Vector2d(targetBearing);
   
   float mag = DotProduct(r, formationNormal);   
   
   if(robotsBehind and !robotsAhead){ 
     response = ScalarMultiplyVector(mag, formationNormal);
   }
   else if(robotsDeadAhead){
     if(leftHanded) response = f;
     else response = ScalarMultiplyVector(-1., f);
   }
   else{ 
     response = ScalarMultiplyVector(mag, formationNormal); 
   }
   
   return response;
   //return new Vector2d(0.,0.);
 }

 //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 void PickEdge( std::vector<SensorContact*> contacts){

   // Loop over each edge
   for(unsigned int i = 0; i < edgeNormals.size(); i++){
     // Zero the count of robots in ith zone
     zoneCounts[i] = 0;

     // Loop over sensor contacts 
     for(unsigned int j = 0; j < contacts.size(); j++){
       Vector2d * v = contacts[j]->GetVector();
       
       // If vector is in the sail semicircle as the normal count it.
       // Its okay that contacts are double counted.
       if(DotProduct(v,edgeNormals[i]) >= 0.) zoneCounts[i]++;
       
     }
   }  
   //printf("~~~~~~~~~~~~~~~~~\n");
   // Now choose the edge with the smallest count
   int lowestVal = 999999;
   int lowestID = 0;
   for(unsigned int i = 0; i < edgeNormals.size(); i++){
     //printf("%f\t%d\n",edgeNormals[i]->GetAngle(), zoneCounts[i]);
     if(zoneCounts[i] < lowestVal){
       lowestVal = zoneCounts[i];
       lowestID = i;
     }
   }

   // Now set the new formation normal and update the constraints
   formationNormal = edgeNormals[lowestID];
   UpdateConstraints();
 }
   
 // Takes in a list of vectors giving the local position of neighbours
 void UpdateStimulus( std::vector<SensorContact*> contacts){
   // Reset Stimuli
   robotsBehind = false;
   robotsAhead = false;
   robotsDeadAhead = false;
   targetBearing = 0.;
   
   float largest = -2.;
   int targetID  = -1;
   
   // Pick edge based on the number of robots in each zone.
   PickEdge(contacts);
   
   // Now evaluate the contacts
   for(unsigned int i = 0; i < contacts.size(); i++){
     Vector2d * v = contacts[i]->GetVector();
     
     
     // Get the current bearing vector
     v->Normalize();
     
     // Project contact bearing on to formation normal
     float dot = DotProduct(formationNormal, v);   
     
     if(dot < 0.) robotsBehind = true;
     else if(dot < 1.9) robotsDeadAhead = true;
     else robotsAhead = true;
     
     // Find the bearing which is closest to formation Normal
     if( dot > largest ){
       largest = dot;
       targetID = i;
     }
     
   } // End of contact loop.  
   
   // Set the targetBearing
   //printf("Target ID = %i\n", targetID);
   //printf("agentBehind = %i\n", agentBehind);
   //printf("agentAhead = %i\n", agentAhead);
   //printf("agentDeadAhead = %i\n", agentDeadAhead);
   if(targetID != -1){
     targetBearing = contacts[targetID]->GetVector()->GetAngle();
   }
 }
 
 // Display information to the screen.
 void Print(){
   printf("DynamicFC\t%f\n",activationLevel);
 }
 
 private:
 
 // Stimuli variables
 bool robotsBehind;
 bool robotsAhead;
 bool robotsDeadAhead;
 float targetBearing;
 
 // Variables set during initialization
 vector<Vector2d*> edgeNormals;
 int* zoneCounts;

 Vector2d * f; 
 Vector2d * formationNormal;
 
 bool leftHanded;
 
 // Random number
 float Random(){
   return rand()/(float)(RAND_MAX);
 }

};
#endif
