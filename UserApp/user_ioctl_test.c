#include <windows.h>
#include <stdio.h>

#define IOCTL_VIRT_READ CTL_CODE(FILE_DEVICE_UNKNOWN, 0x800, METHOD_BUFFERED, FILE_READ_DATA | FILE_WRITE_DATA)
#define IOCTL_VIRT_WRITE CTL_CODE(FILE_DEVICE_UNKNOWN, 0x801, METHOD_BUFFERED, FILE_READ_DATA | FILE_WRITE_DATA)

int main() {
    HANDLE h = CreateFile("\\\\.\\VirtDevice", GENERIC_READ|GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h == INVALID_HANDLE_VALUE) { printf("Open failed %lu\n", GetLastError()); return 1; }

    char out[128];
    DWORD bytes;
    if (DeviceIoControl(h, IOCTL_VIRT_READ, NULL, 0, out, sizeof(out), &bytes, NULL)) {
        printf("Read %d bytes: %s\n", bytes, out);
    } else {
        printf("DeviceIoControl read failed %lu\n", GetLastError());
    }

    const char msg[] = "USER_SENT_TEST";
    if (DeviceIoControl(h, IOCTL_VIRT_WRITE, (LPVOID)msg, sizeof(msg), NULL, 0, &bytes, NULL)) {
        printf("Write success\n");
    } else {
        printf("Write failed %lu\n", GetLastError());
    }

    CloseHandle(h);
    return 0;
}
