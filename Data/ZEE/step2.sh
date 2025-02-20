#!/bin/bash
#set -o nounset

APPDIR=/eos/home-r/rforti/CMSSW_14_1_0_pre2/src 
JOBDIR=$APPDIR/DQM/TrackingMonitorSource/test/trackingDataMC/Data/ZEE
cd $APPDIR
eval $(scramv1 runtime -sh)
cd $JOBDIR
cmsRun step2_cfg_multirun.py
exit $?
