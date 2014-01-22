#!/bin/sh

# Modify LLVM_PATH to the top-level directory where LLVM is installed
# llvm 3.3
LLVM_PATH=/home/markus/src/install-3.3
# llvm 3.2
#LLVM_PATH=/home/markus/src/install-3.2
# llvm 3.0
#LLVM_PATH=/home/markus/src/install
CC=${LLVM_PATH}/bin/clang
CXX=${LLVM_PATH}/bin/clang++
OPT=${LLVM_PATH}/bin/opt
LLVMDIS=${LLVM_PATH}/bin/llvm-dis
LLVMAS=${LLVM_PATH}/bin/llvm-as

# DependenceCheck library location
DEPCHECK=/home/markus/src/dependence/install/lib/DependenceCheck.so

# tests for simple.c
#$OPT -analyze -print-memdeps <simple.bc >/dev/null
#$OPT -basicaa -print-memdeps <simple.bc >/dev/null
#$OPT -analyze -basicaa -memdep -print-memdeps <simple.bc
#opt -analyze -basicaa -print-memdeps <simple.bc >simple.results
#opt -analyze -basicaa -print-memdeps <non_local.bc >non_local.results
#$OPT --version

echo "Running: $OPT -analyze -basicaa -load $DEPCHECK -depcheck <simple.bc"
$OPT -basicaa -memdep -analyze -load $DEPCHECK -depcheck <simple.bc >simple.results

#echo  "Running : $OPT -basicaa -memdep -analyze -load $DEPCHECK -depcheck <non_local.bc >non_local.results"
#$OPT -basicaa -memdep -analyze -load $DEPCHECK -depcheck <non_local.bc >non_local.results
