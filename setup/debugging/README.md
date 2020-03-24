# Kernel Debugging

## Prolog

This readme file is used asa summary for my tries to debug the kernel, each way with its issues.
Basically, I've tried two ways of debugging the Linux's kernel:
1. By installing a VM (VMware) from scratch using an ubuntu distribution.
2. By using Buildroot and QEMU, compiling my own kernel (and fs) and debugging using QEMU.

</br>

## Way 1 - Using VMware

In order to debug the kernel using VMware, I did the following:
1. Created (from scratch) a VMware's VM running the desired distro and kernel version.
2. Installed all dependencies on the VM (it is recommended to connect the VM to the internet).
3. Opened the VM's `vmx` file and added:
 ```
 debugStub.listen.guest32 = "TRUE"
 debugStub.listen.guest32.remote = "TRUE" --> Listens for gdb-connections on port 8832
 debugStub.listen.guest64 = "TRUE"
 debugStub.listen.guest64.remote = "TRUE" --> Listens gor gdb-connections on port 8864
 ```
4. Entering the VM and compiling the desired kernel module.
5. Connecting with `gdb` to the VM by running `gdb -ex target remote :8832`.

### What I didn't do?
1. Figured how to make `vmlinux-gdb.py` (the kernel script that provides all the lx* commands) to work.
2. Figured how to debug the kernel from the very-beginning (`start_kernel` function).

</br>

## Way 2 - Using QEMU

### Prerequisites 

Build a kernel using Buildroot (guide can be found in the `kernel-debugging` directory).

### Running QEMU

The interesting Buildroot's output is the two following file:
1. __`bzImage`__ contains the compiled kernel's image.
2. __`rootfs.ext2`__ contains the created file-system (with all its dependencies).

Once you have the above file, run QEMU:
```
qemu-system-x86_64 -kernel <path_to_bzImage> -hda <path_to_rootfs.ext2> -nographic --append "console=ttyS0 root=/dev/sda1 nokaslr" -m 512 -s -S
```

Where:
* `kernel` flag specifies the location of the compiled kernel (bzImage file).
* `hda` points to a the fs file.
* `nographic` and `console=ttyS0` used to output the stdout to the terminal.
* `root` is passed to the kernel and indicates to QEMU where to mound the disk.
* `nokaslr` disables the Kernel's ASLR.
* `m 512` indicates the block size of the disk (?).
* `s` indicates to listen on a gdb server on port 1234
* `S` halts the kernel startup until resumed from inside gdb (allows to start debugging from the very-beginning).  

### Running GDB

Once QEMU is running, we need to run gdb:
```
gdb -ex "add-auto-load-safe-path $(pwd)" -ex "file vmlinux" -ex 'set arch i386:x86-64:intel' -ex 'target remote localhost:1234' -ex 'break start_kernel' -ex 'continue' -ex 'disconnect' -ex 'set arch i386:x86-64' -ex 'target remote localhost:1234'
```

___Note:___ the reason why we reconnect GDB right after the first breakpoint is reached, is due to QEMU's bug ("Reply 'g' packet is too long").

### What I didn't do?
1. Managed to connect to QEMU with ssh from the host (should be easy, although didn't work).
2. Managed to build a kernel module (gcc's binary was missing, need to figure how to configure Buildroot to include it).
