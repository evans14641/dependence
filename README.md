Author: Markus Kusano

## Dependence Check
Pass for querying dependency information of LLVM instructions

## Configure
run `./configure` with atleast the following options:


`--with-llvmsrc=<directory>`

    Tell your project where the LLVM source tree is located.

`--with-llvmobj=<directory>`

    Tell your project where the LLVM object tree is located.

for example:

  ./configure --with-llvmsrc=/home/markus/src/llvm-3.3.src --with-llvmobj=/home/markus/install-3.3 --prefix=`pwd`/install

see `./configure --help` for more options
