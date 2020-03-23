# Kernel Debugging

## Setup 

In order the debug the kernel, you need the following:
* Kernel's source.
* vmlinux file (can be downloaded from the interent / using the `ddeb` soultion in the internet).
  * Although downloading from the internet is a better solution.
* QEMU installed  

## Running QEMU

In order to run QEMU, run the following command:
```
qemu-system-x86_64 -kernel arch/x86_64/boot/bzImage -initrd /boot/initrd.img-4.10.0-38-generic -hda ~/Downloads/debian_squeeze_amd64_desktop.qcow2 -nographic --append "console=ttyS0 root=/dev/sda1 nokaslr" -m 512 -s -S
```

Where:
* `kernel` flag specifies the location of the compiled kernel (bzImage file).
* `initrd` points to the initrd file of the desired kernel.
* `hda` points to a `qcow2` file, representing the file system 
  * (can be downloaded from https://people.debian.org/~aurel32/qemu/amd64/).
* `nographic` and `console=ttyS0` used tooutput the stdout to the terminal.
* `root` is passed to the kernel and indicates to QEMU where to mound the disk.
* `nokaslr` disables the Kernel's ASLR.
* `m 512` indicates the block size of the disk (?).
* `s` indicates to listen on a gdb server on port 1234
* `S` halts the kernel startup until resumed from inside gdb (allows to start debugging from the very-beginning).  

## Running GDB

Once QEMU is running, we need to run gdb:
```
gdb -ex "add-auto-load-safe-path $(pwd)" -ex "file vmlinux" -ex 'set arch i386:x86-64:intel' -ex 'target remote localhost:1234' -ex 'break start_kernel' -ex 'continue' -ex 'disconnect' -ex 'set arch i386:x86-64' -ex 'target remote localhost:1234'
```

___Note:___ the reason why we reconnect GDB right after the first breakpoint is reached, is due to QEMU's bug ("Reply 'g' packet is too long").
