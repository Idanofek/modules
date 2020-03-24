## Building Your Own Kernel Using Buildroot

When talking about Linux, Buildroot provides two main features:
  * Building a custom kernel.
  * Creating a file-system file, contains all the needed dependencies and tools.
  
### Running Buildroot

1. Download buildroot source files from https://buildroot.org/download.html.
2. `cd` to buildroot directory, and enter the configuration menu by running `make menuconfig`.
3. The menu allows you to configure a variety of options, where:
  * __`Target Options`__ contains definitions about the target's architecture.
  * __`Build Options`__ contains definitions about Buildroot's build (number of cores, paths, etc.).
  * __`Toolchain`__ contains definitions about the toolchain with which the kernel will be built (gcc's version, C++ support, etc.).
  * __`System Configuration`__ contains some system-related options (hostname, user's password, etc.).
  * __`Kernel`__ contains definitions about the desired kernel (version, patches, configuration file, etc.).
  * __`Target Packages`__ allows you the choose which packages will be installed on the image file-system.
  * __`Filesystem Images`__ allows you to configure the format and size of the file-system that Buildroot generates.
  * __`Bootloades`__ allows you choose which bootloader to use.
  * __`Host Utilities`__ allows you to choose which utilities will be installed on the host machine.
  * __`Legacy Config Options`__ contain some of Buildroot's legacy options (usually not-interesting).
4. Once you finish configuring Buildroot's, run `make`.
5. Both the Linux image and the file-system file will be under `<BUILDROOT_DIR>/output/images`.

### Rebuilding 

Buildroot is pretty smart when it comes to rebuilding the entire tree.
Usually, rebuild is not needed when adding new packages / changing configuration related to Buildroot's itself.
However, to save a pitfall - always rread the documentation found in https://buildroot.org/downloads/manual/manual.html for further information.

If (after reading the manual) a rebuild is still needed, it can be achieved by running `make clean all`.

### Common Pitfalls

#### GCC's fails with error "canno't compute suffix for files"

I didn't completley understand ___why___ it can happen, and the internet doesn't really helps with it.
Try running `make clean all`, solved my problem.

#### Build fails with "kernel source not found"

Buildroot's takes the choosen version of the kernel, and downloads the matching `tar.gz` file from https://cdn.kernel.org/pub/linux/kernel/v4.x/.
If the inserted version doesn't exist, or the above link doesn't contain the desired tar file - it'll fail.
