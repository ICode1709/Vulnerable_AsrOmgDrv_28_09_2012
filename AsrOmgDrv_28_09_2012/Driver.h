#pragma once

class CDriver
{
public:
	CDriver();
	VIRTUAL ~CDriver();

	VIRTUAL BOOLEAN Load();
	VIRTUAL VOID Unload();

	VIRTUAL PVOID AllocateMemory(ULONG NumberOfBytes, PULONG PhysicalAddress);
	VIRTUAL BOOLEAN FreeMemory(PVOID BaseAddress, ULONG NumberOfBytes);

	VIRTUAL ULONGLONG Rdtsc();
	VIRTUAL ULONGLONG ReadPmc(ULONG Counter);

	VIRTUAL ULONGLONG ReadCr0();
	VIRTUAL ULONGLONG ReadCr2();
	VIRTUAL ULONGLONG ReadCr3();
	VIRTUAL ULONGLONG ReadCr4();
	VIRTUAL ULONGLONG KeGetCurrentIrql();
	VIRTUAL BOOLEAN WriteCr0(ULONGLONG Value);
	VIRTUAL BOOLEAN WriteCr2(ULONGLONG Value);
	VIRTUAL BOOLEAN WriteCr3(ULONGLONG Value);
	VIRTUAL BOOLEAN WriteCr4(ULONGLONG Value);

	VIRTUAL BOOLEAN InByte(USHORT Port, PUCHAR Value);
	VIRTUAL BOOLEAN OutByte(USHORT Port, UCHAR Value);
	VIRTUAL BOOLEAN InWord(USHORT Port, PUSHORT Value);
	VIRTUAL BOOLEAN OutWord(USHORT Port, USHORT Value);
	VIRTUAL BOOLEAN InDword(USHORT Port, PULONG Value);
	VIRTUAL BOOLEAN OutDword(USHORT Port, ULONG Value);

	VIRTUAL BOOLEAN WritePhysicalBytes(ULONG PhysicalAddress, PUCHAR Values, ULONG Count);
	VIRTUAL BOOLEAN WritePhysicalUshorts(ULONG PhysicalAddress, PUSHORT Values, ULONG Count);
	VIRTUAL BOOLEAN WritePhysicalUlongs(ULONG PhysicalAddress, PULONG Values, ULONG Count);
	VIRTUAL BOOLEAN ReadPhysicalBytes(ULONG PhysicalAddress, PUCHAR Values, ULONG Count);
	VIRTUAL BOOLEAN ReadPhysicalUshorts(ULONG PhysicalAddress, PUSHORT Values, ULONG Count);
	VIRTUAL BOOLEAN ReadPhysicalUlongs(ULONG PhysicalAddress, PULONG Values, ULONG Count);

	VIRTUAL BOOLEAN WritePhysicalByte(ULONG PhysicalAddress, UCHAR Value);
	VIRTUAL BOOLEAN WritePhysicalUshort(ULONG PhysicalAddress, USHORT Value);
	VIRTUAL BOOLEAN WritePhysicalUlong(ULONG PhysicalAddress, ULONG Value);

	VIRTUAL BOOLEAN WritePhysical(ULONG PhysicalAddress, PVOID Buffer, ULONG BufferSize);
	VIRTUAL BOOLEAN ReadPhysical(ULONG PhysicalAddress, PVOID Buffer, ULONG BufferSize);
private:
	HANDLE DeviceHandle;
};