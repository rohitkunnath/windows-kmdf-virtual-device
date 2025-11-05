# windows-kmdf-virtual-device

Install Visual Studio Community + Windows Driver Kit (WDK).

Create a KMDF Driver project, paste VirtualDevice.c, set a proper INF, driver signing (test-signed), and build.

Deploy to a test machine or use a VM with test-signing enabled: bcdedit /set testsigning on.

Install driver using pnputil or Device Manager (INF) or DevCon.

Run UserApp to send IOCTLs. Use WinDbg for kernel log
