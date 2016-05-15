#!/bin/bash 
# arg1 nRuns
# arg2 algo
# arg3 shape
# arg4 nRobots

clear

# clear out old files and remake them
rm outputData
rm initHeader
rm formDef

touch outputData
cp initFiles/header/"$2"/"h_$2_$4" initHeader
cp initFiles/formation/"$2"/"f_$2_$3_$4" formDef

COUNTER=0
while [  $COUNTER -lt $1 ]; do
    clear
    echo "RUN NUMBER $COUNTER"
    #more initHeader
    #more formDef
    ./diskSim
    let COUNTER=COUNTER+1 
done

cp outputData data/"outputData_$2_$3_$4_$1"
rm *~
