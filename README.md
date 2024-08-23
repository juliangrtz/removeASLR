# removeASLR

This is a tool which flips the MH_PIE bit in an iOS application, meaning Address Space Layout Randomization (ASLR) is being disabled. Reverse-engineering without ASLR is much easier.

## Building

```bash
$ git clone https://github.com/juliangrtz/removeASLR && cd removeASLR
# maybe adjust the Makefile
$ make
gcc -g -c -Wall removeASLR.c
gcc -g removeASLR.o xnu-definitions.o -o removeASLR 
```

## Usage

Copy the compiled iOS executable to your Linux/macOS machine and pass the path to the executable as the first argument:

```
$ ./removeASLR ExampleBinary
loading header...
looks ok
mach-o header: cffaedfe0c00000100000000020000004c000000d822000085808100

backing up application binary...
done

original flags: 8580a100
disabling ASLR...
new flags:      85808100

ASLR has been disabled for ExampleBinary!
```

### To do

- notify the user when ASLR is already disabled
- add codesign to Makefile
- move some funcs to own header file
