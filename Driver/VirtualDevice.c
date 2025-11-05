#include <ntddk.h>
#include <wdf.h>
#include <initguid.h>

#define IOCTL_VIRT_READ CTL_CODE(FILE_DEVICE_UNKNOWN, 0x800, METHOD_BUFFERED, FILE_READ_DATA | FILE_WRITE_DATA)
#define IOCTL_VIRT_WRITE CTL_CODE(FILE_DEVICE_UNKNOWN, 0x801, METHOD_BUFFERED, FILE_READ_DATA | FILE_WRITE_DATA)

DRIVER_INITIALIZE DriverEntry;
EVT_WDF_DRIVER_DEVICE_ADD VirtEvtDeviceAdd;
EVT_WDF_IO_QUEUE_IO_DEVICE_CONTROL VirtEvtIoDeviceControl;

NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING RegistryPath) {
    WDF_DRIVER_CONFIG config;
    NTSTATUS status;

    WDF_DRIVER_CONFIG_INIT(&config, VirtEvtDeviceAdd);
    status = WdfDriverCreate(DriverObject, RegistryPath, WDF_NO_OBJECT_ATTRIBUTES, &config, WDF_NO_HANDLE);
    return status;
}

NTSTATUS VirtEvtDeviceAdd(IN WDFDRIVER Driver, IN PWDFDEVICE_INIT DeviceInit) {
    WDFDEVICE device;
    WDF_IO_QUEUE_CONFIG ioQueueConfig;
    NTSTATUS status;

    UNREFERENCED_PARAMETER(Driver);
    WdfDeviceInitSetDeviceType(DeviceInit, FILE_DEVICE_UNKNOWN);
    status = WdfDeviceCreate(&DeviceInit, WDF_NO_OBJECT_ATTRIBUTES, &device);
    if (!NT_SUCCESS(status)) return status;

    WDF_IO_QUEUE_CONFIG_INIT_DEFAULT_QUEUE(&ioQueueConfig, WdfIoQueueDispatchSequential);
    ioQueueConfig.EvtIoDeviceControl = VirtEvtIoDeviceControl;
    status = WdfIoQueueCreate(device, &ioQueueConfig, WDF_NO_OBJECT_ATTRIBUTES, NULL);
    return status;
}

VOID VirtEvtIoDeviceControl(IN WDFQUEUE Queue, IN WDFREQUEST Request, IN size_t OutputBufferLength, IN size_t InputBufferLength, IN ULONG IoControlCode) {
    UNREFERENCED_PARAMETER(Queue);
    NTSTATUS status = STATUS_SUCCESS;
    size_t bytesReturned = 0;

    switch (IoControlCode) {
    case IOCTL_VIRT_READ:
        // return a sample string
        {
            char *outBuf;
            status = WdfRequestRetrieveOutputBuffer(Request, 1, (PVOID*)&outBuf, NULL);
            if (NT_SUCCESS(status)) {
                const char msg[] = "HELLO_FROM_VIRT";
                RtlCopyMemory(outBuf, msg, sizeof(msg));
                bytesReturned = sizeof(msg);
            }
        }
        break;
    case IOCTL_VIRT_WRITE:
        {
            char *inBuf;
            status = WdfRequestRetrieveInputBuffer(Request, 1, (PVOID*)&inBuf, NULL);
            if (NT_SUCCESS(status)) {
                // process input (just log)
                KdPrint(("VirtDriver: Received %s\n", inBuf));
            }
        }
        break;
    default:
        status = STATUS_INVALID_DEVICE_REQUEST;
        break;
    }

    WdfRequestCompleteWithInformation(Request, status, bytesReturned);
}
