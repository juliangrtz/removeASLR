# removePIE

This is a tool which flips the MH_PIE bit in an iOS application, meaning Address Space Layout Randomization (ASLR) is being disabled. Reverse-engineering without ASLR is much easier.

## Building

```bash
$ git clone https://github.com/juliangrtz/removePIE && cd removePIE
# maybe adjust the Makefile
$ make
gcc -g -c -Wall removePIE.c
gcc -g removePIE.o xnu-definitions.o -o removePIE 
```

## Usage

Copy the compiled executable to your Linux/macOS PC and use this tool:

```bash
$ ./removePIE <application binary>

backing up application binary...
binary backed up to ExampleBinary.bak
mach_header: cffaedfe0c00000100000000020000004c000000d822000085808100
original flags: 85808100
disabling ASLR/PIE ...
new flags:
85808100
ASLR has been disabled for ExampleBinary
```