# iOS-pie-remover

This is a tool which flips the MH_PIE bit in an iOS application, meaning Address Space Layout Randomization (ASLR) is being disabled (not in the kernel of course). Both 32- and 64-bit executables are supported.

## Usage

```
$ ./iOS-pie-remover <binary>
```

### To do

- add automatic codesign
- log further information (image base, entry point etc.)