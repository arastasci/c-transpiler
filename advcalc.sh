#!/bin/bash

for i in $(seq 1 20) # loop over input files with indices from 1 to 10
do
  ./advcalc2ir inputs/input_${i}.txt # run advcalc2ir and pipe input/output
  lli input_${i}.ll # run lli on the output file
done

