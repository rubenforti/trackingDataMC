#!/bin/bash

for i in $(seq 1 120) #CHANGE NUMBER TO THE TOTAL NUMBER OF JOBS YOU ARE SUBMITTING (IT HAS TO BE THE SAME AS IN createjob.sh)
do
  condor_submit batch_$i.sub
done
exit
