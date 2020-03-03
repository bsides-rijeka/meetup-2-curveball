# Curveball

## Preparation

The CurveBall Windows 10 update needs to be uninstalled. To locate it:

1. On the window system, go to your Start -> About Your PC
2. Locate the Windows specifications -> Version field
3. Go to https://portal.msrc.microsoft.com/en-US/security-guidance/advisory/CVE-2020-0601
4. On the update table locate your version. The Article column is the KB number you are looking for (for instance KB4534273)
5. Go to Start -> View Update History
6. Click on Uninstall updates
7. Uninstall the cumulative update under the KB number you found in step 4.

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

## What to look for

Login account: /_/signin/sl/challenge?hl=hr&_reqid=250726&rt=j
