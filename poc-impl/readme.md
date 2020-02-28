# Curveball

## Build POC

```
gcc -o curveball curveball.c -lcrypto
```

Make sure you put `-lcrypto` at the end.

## Usage

```
./curveball > ./c_spoofed_ca.key
```

Then compare `c_spoofed_ca.key` with `spoofed_ca.key` from olli's repo:
https://github.com/ollypwn/CurveBall

## TODO

* Implement use cases:
    * Captive portal
    * Code signing
    * MITM
    * Driver signing
* Implement whole process in c, so that we don't have to use `openssl` commands at all.
* Reverse the fix to see how it was fixed