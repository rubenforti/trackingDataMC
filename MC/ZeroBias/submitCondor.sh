
BASEDIR=$PWD
BASE_JOBS_FOLDER=/afs/cern.ch/user/r/rforti/run_condor
JOBTYPE=ZeroBias

rm -rf $BASE_JOBS_FOLDER/$JOBTYPE

mkdir $BASE_JOBS_FOLDER/$JOBTYPE/
mkdir $BASE_JOBS_FOLDER/$JOBTYPE/output/
mkdir $BASE_JOBS_FOLDER/$JOBTYPE/error/
mkdir $BASE_JOBS_FOLDER/$JOBTYPE/log/

./createjob.sh 

cp submitscript.sh $BASE_JOBS_FOLDER/$JOBTYPE/

mv batch_*.sub $BASE_JOBS_FOLDER/$JOBTYPE/
mv submit_*.sh $BASE_JOBS_FOLDER/$JOBTYPE/

cd $BASE_JOBS_FOLDER/$JOBTYPE

./submitscript.sh

cd $BASEDIR
