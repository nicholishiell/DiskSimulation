#ifndef REF
#define REF

#include <stdio.h>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <string>

#include "robot.h"
#include "displayable.h"
#include "PhysicsEngine.h"
#include "Vector2d.h"
#include "sensorContact.h"

using namespace std;

class Referee{

public:
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
  // Constructors.
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
  Referee(PhysicsEngine* pe, vector<Robot *> rl){
    physEng = pe;
    
    for(unsigned int i = 0; i < rl.size(); i++)
      robotVector.push_back(rl[i]);
  };
   
  Referee(PhysicsEngine* pe){
    physEng = pe;
  };

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
  // Used to add robots to the simulation during initialization
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
  void AddRobot(Robot * aRobot){
    robotVector.push_back(aRobot);
  }
  
  vector<Robot *> GetRobots(){return robotVector;}
  
  vector<Displayable *> GetDisplayableRobots(){
    vector<Displayable *> displayList;

     for(unsigned int i = 0; i < robotVector.size(); i++){
       displayList.push_back(robotVector[i]);
     }
     
     return displayList;
  }

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
  // This is the main simulation loop.
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
  void Update(){
    
    //printf("Update Sensors.\n");
    UpdateRobotSensors();

    //printf("Update Veclocities.\n");    
    UpdateRobotVelocityCommands();  
    
    //printf("Update Positions.\n");
    UpdateRobotPositions();
  }

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
  // These functions are the three main steps of the simulations loop
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

  // Send bearings to all robots.
  void UpdateRobotSensors(){
    // Loop over all robots and reset sensor contacts
    for(unsigned i = 0; i < robotVector.size(); i++){
      robotVector[i]->ResetSensorContacts();
    }

    // Loop over all the robots and calculate bearings to other robots
    for(unsigned i = 0; i < robotVector.size(); i++){
      Robot *r1 = robotVector[i];
      
      // Loop over remaing robots.
      for(unsigned j = i+1; j < robotVector.size(); j++){
	Robot *r2 = robotVector[j];

	Vector2d * d_12 = SubtractVectors(r2->GetPositionVector(), 
					  r1->GetPositionVector());	

	//if(d_12->GetNorm() > 100.) continue;
	Vector2d * d_12_neg =  ScalarMultiplyVector(-1., d_12);
	
	// Add bearing contact
	int id1, id2;
	id1 = r1->GetColour();
	id2 = r2->GetColour();
	// Make sure sensor contacts have opposite ids!
	r1->UpdateSensors( new SensorContact(id2, d_12) );
	r2->UpdateSensors( new SensorContact(id1, d_12_neg ) );

	
      }      
    }
      
    // Now loop over all obstacles to see if there are any proximity contacts
    // !!!THIS STILL NEEDS TO BE ADDED!!!    
  };

  // Give the latest sensor data robots are asked to update their vel cmd
  void UpdateRobotVelocityCommands(){
    
    for(unsigned i = 0; i < robotVector.size(); i++){
      Robot * aRobot = robotVector[i];
      
      aRobot->UpdateVelocityCommand();

     
    }
  
  };  

  void UpdateRobotPositions(){  
    physEng->Step(robotVector);
  }  

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
  // This function cycles through robotVector and displays 
  // information regarding there state to the stdout
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

  void Display(){
     for(unsigned i = 0; i < robotVector.size(); i++){
       Robot * aRobot = robotVector[i];
      aRobot->Display();

     
    }
    printf("===============================\n");
   
    
  }
  
private:

 vector<Robot *> robotVector; 
 PhysicsEngine* physEng;

 float timeStep;
};
#endif
