# iOS-pie-remover

This is a tool which flips the MH_PIE bit in an iOS application, meaning Address Space Layout Randomization (ASLR) is being disabled (not in the kernel of course).

## Usage

```
$ ./iOS-pie-remover binary
loading header...
looks ok
mach-o header: cffaedfe0c00000100000000020000004c000000d822000085808100

backing up application binary...
done

original flags: 8580a100
disabling ASLR...
new flags:      85808100

ASLR has been disabled for binary!
```

### To do

- add automatic codesign
- log further information (image base, entry point etc.)