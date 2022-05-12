<p align="center">
  <h1 align="center"> ChampSim </h1>
  <p> ChampSim is a trace-based simulator for a microarchitecture study. You can sign up to the public mailing list by sending an empty mail to champsim+subscribe@googlegroups.com. Traces for the 3rd Data Prefetching Championship (DPC-3) can be found from here (https://dpc3.compas.cs.stonybrook.edu/?SW_IS). A set of traces used for the 2nd Cache Replacement Championship (CRC-2) can be found from this link. (http://bit.ly/2t2nkUj) <p>
</p>

This directory contains implementation of Cache Partioning in static way i.e once the user sets the partition, it stays the same throughout the execution.

This works for N core where N is {2,4,8}. To run this, first select the partition in file base_replacement.cc. Change the values in cpuways. Don't change the size of array. Keep the entries 0 for the cores which aren't being used and assign atleast 1 way to each core being used. 

# Compile
Run the run.sh file to compile and build the results. You can change the parameters like number of cores, instructions, traces in run.sh file. 
