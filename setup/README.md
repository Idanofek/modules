## Debugging the Kernel

### Debugging Symbol Installation

https://xakcop.com/post/vmw-kernel-debugging/
https://wiki.ubuntu.com/Debug%20Symbol%20Packages#

In addition to the documents in the repo, use:

```
#!/bin/bash
#
# https://askubuntu.com/questions/197016/how-to-install-a-package-that-contains-ubuntu-kernel-debug-symbols
# https://wiki.ubuntu.com/DebuggingProgramCrash#Debug_Symbol_Packages

echo "deb http://ddebs.ubuntu.com $(lsb_release -cs) main restricted universe multiverse" | sudo tee -a /etc/apt/sources.list.d/ddebs.list
echo "deb http://ddebs.ubuntu.com $(lsb_release -cs)-updates main restricted universe multiverse" | sudo tee -a /etc/apt/sources.list.d/ddebs.list
echo "deb http://ddebs.ubuntu.com $(lsb_release -cs)-proposed main restricted universe multiverse" | sudo tee -a /etc/apt/sources.list.d/ddebs.list
sudo apt-key adv --keyserver keyserver.ubuntu.com --recv-keys ECDCAD72428D7C01
sudo apt-get update

# "This is rather huge (>680MB), so prepare for a wait"
sudo apt-get install -y linux-image-`uname -r`-dbgsym

# "automatically build debug symbol ddeb packages"
#   for any subsequent Kernel builds / installs
sudo apt-get install -y pkg-create-dbgsym

sudo apt-get install gdb
```

### Configuring VM to Open a Remote Debugging Port

Need to add the following lines to the VM's `.vmx` file:
```
debugStub.listen.guest32 = "TRUE"
debugStub.listen.guest32.remote = "TRUE"
debugStub.listen.guest64 = "TRUE"
debugStub.listen.guest64.remote = "TRUE"
```

Default 32bit port is `8832`, 64bit port is `8864`.


## GDB Init Script

https://github.com/cyrus-and/gdb-dashboard
