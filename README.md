# lris
lris (referred to as Iris) is a Unix-like operating system that is designed to run on Intel x86 CPU.

### Preparations

You should install these for install and run this operating system.
```
build-essential
nasm
xorriso
qemu-system-i386
```

In ubuntu, you can install these as follows:
```
sudo apt-get install build-essential nasm xorriso qemu-system-i386
```

### Install/Run
```
git clone git@github.com:hyahong/lris.git
cd lris
make
```

### Implementation
It is currently about 1/10 complete.

|function|?|
|------|---|
|Boot|O|
|Interrupt|Partially|
|Memory Management| |
|- Segmentation|O|
|- Paging|O|
|- Allocator|O|
|Driver| |
|- VGA|O|
|- Keyboard|O|
|Debuging|Partially|
|...||

### Demo

![image](https://github.com/hyahong/lris/assets/12230655/b8c8e113-b2bb-40cd-9d6c-5284d2f30526)

![2023](https://github.com/hyahong/lris/assets/12230655/b87272bd-09a2-43ce-8efb-bcf9f8fbd3de)


