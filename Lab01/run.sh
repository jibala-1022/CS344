#!/bin/bash
# Run this bash script to execute all programs
# bash run.sh <number>

if [ $# -ne 1 ]; then
    echo "Usage: bash $0 <number>"
    exit 1
fi

n=$1

if [ $n -le 0 ]; then
    echo "Enter a positive number"
    exit 1
fi

rm -f f{a,b,c,d,e,f}.txt

gcc qa.c && ./a.out $n
gcc qb.c && ./a.out $n
gcc qc.c && ./a.out $n
gcc qd.c && ./a.out $n
gcc qe.c && ./a.out $n
gcc qf.c && ./a.out $n

rm a.out