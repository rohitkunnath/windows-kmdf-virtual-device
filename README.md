## 💾 3️⃣ Windows KMDF Virtual Device Driver

A simple Windows Kernel-Mode Driver (KMDF) that implements a virtual device.
It demonstrates creating a device object, handling IOCTL read/write operations, and testing via a user-space C program — all without hardware.

# 🧠 What This Project Demonstrates

Windows kernel-mode driver development

KMDF architecture and I/O request handling

IOCTL communication between user and kernel

Testing with a user-space application

Building with Visual Studio + WDK

# 🛠️ Requirements

Windows 10/11

Visual Studio (Community Edition)

Windows Driver Kit (WDK)

Test-signing enabled (bcdedit /set testsigning on)

# 🧱 Folder Structure
```bash
windows-kmdf-virtual-device/
├── Driver/
│   ├── VirtualDevice.c
│   └── Driver.vcxproj
├── UserApp/
│   └── user_ioctl_test.c
└── README.md
```
⚙️ Build Instructions
```bash
Open Visual Studio → Create new KMDF Driver project.

Replace source with VirtualDevice.c.

Build with WDK build environment (Release x64).

Enable test signing:

bcdedit /set testsigning on


Reboot.
```
▶️ Run & Test

Install driver
```bash
pnputil /add-driver VirtualDevice.inf /install


Run user-space test app

gcc -o user_ioctl_test user_ioctl_test.c
user_ioctl_test.exe

```
Expected output
```bash
Read 15 bytes: HELLO_FROM_VIRT
Write success
```

Debug logs (WinDbg)
```bash
VirtDriver: Received USER_SENT_TEST
```

Uninstall driver
```bash
pnputil /delete-driver VirtualDevice.inf /uninstall
```
