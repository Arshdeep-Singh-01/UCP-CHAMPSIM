
  <h1 align="center"> ChampSim </h1>
 

This directory contains implementation of Cache Partioning in static way i.e once the user sets the partition, it stays the same throughout the execution.

This works for N core where N is {2,4,8}. To run this, first select the partition in file base_replacement.cc. Change the values in cpuways. Don't change the size of array. Keep the entries 0 for the cores which aren't being used and assign atleast 1 way to each core being used. 

# Compile
Run the run.sh file to compile and build the results. You can change the parameters like number of cores, instructions, traces in run.sh file. 
