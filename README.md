# Memory Simulation
Written June 2024, Benjamin Cash
## Compiling process
```
gcc -fpic -c memory.c
gcc -shared -o libmemory.so memory.o
gcc -c simulator.c
gcc simulator.o -L. -lmemory -lm -o sim
```
## Configure Environment Variable
```
LD_LIBRARY_PATH=$(pwd)
export LD_LIBRARY_PATH
```
## Usage
```
./sim mem_file1
```