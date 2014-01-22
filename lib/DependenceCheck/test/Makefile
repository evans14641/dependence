# Modify LLVM_PATH to the top-level directory where LLVM is installed
LLVM_PATH = /home/markus/src/install-3.2
# llvm 3.0
#LLVM_PATH = /home/markus/src/install
CC = $(LLVM_PATH)/bin/clang
CXX = $(LLVM_PATH)/bin/clang++
OPT = $(LLVM_PATH)/bin/opt
LLVMDIS = $(LLVM_PATH)/bin/llvm-dis
LLVMAS = $(LLVM_PATH)/bin/llvm-as

all: simple.c simple.bc simple.ll non_local.c non_local.bc non_local.ll

simple.ll: simple.c
	$(CC) -emit-llvm -S simple.c -o simple.ll

simple.bc: simple.ll
	$(LLVMAS) simple.ll

non_local.ll: non_local.c
	$(CC) -emit-llvm -S non_local.c -o non_local.ll

non_local.bc: non_local.ll
	$(LLVMAS) non_local.ll

clean:
	rm -f *.ll *.bc
