#!/bin/sh
set -o nounset

infilelist="filelist.txt"
let "nfiles = 10" #MODIFY ACCORDING TO THE NUMBER OF FILES PER JOB YOU WANT

let "ifile = 0"
for i in $(seq 1 120) #MODIFY ACCORDING TO THE TOTAL NUMBER OF JOBS YOU WANT (IT SHOULD BE THE INTEGER PART OF (number of lines in filelist.txt)/nfiles +1)
do
  flst=""
  let "n = i * nfiles"
  for f in $(head -$n $infilelist | tail -$nfiles)
  do 
    flst=$flst"$f"
  done
  flist=$(echo $flst | sed s/.$//)
  
  sed -e "s@INDEX@$i@g" \
      -e "s@INPUT_FILES@$flist@" \
      step1_cfg.py.tmpl > step1_cfg_$i.py
  sed -e "s@INDEX@$i@g" batch.sub.tmpl > batch_$i.sub
  sed -e "s@INDEX@$i@g" submit.sh.tmpl > submit_$i.sh
  chmod 775 submit_$i.sh
done
exit 0
