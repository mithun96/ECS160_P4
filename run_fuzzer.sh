#!/bin/bash
# Runs the AFL Test with inputs from the testing/in folder
# and outputs to the testing/out folder.

if [ "$EUID" -ne 0 ]
  then echo "Please run as root"
  exit
fi

afl/afl/afl-fuzz -i testing/in -o testing/out -- ./maxTweeter.out @@


