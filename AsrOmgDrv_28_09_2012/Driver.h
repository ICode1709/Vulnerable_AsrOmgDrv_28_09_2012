#pragma once

class CDriver
{
public:
	CDriver();
	VIRTUAL ~CDriver();

	VIRTUAL BOOLEAN Load();
	VIRTUAL VOID Unload();

	VIRTUAL ULONGLONG AllocateMemory(ULONG NumberOfBytes, PULONG PhysicalAddress);
	VIRTUAL BOOLEAN FreeMemory(ULONGLONG BaseAddress, ULONG NumberOfBytes);

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

	VIRTUAL BOOLEAN WritePhysicalBytes(ULONGLONG PhysicalAddress, PUCHAR Values, ULONG Count);
	VIRTUAL BOOLEAN WritePhysicalUshorts(ULONGLONG PhysicalAddress, PUSHORT Values, ULONG Count);
	VIRTUAL BOOLEAN WritePhysicalUlongs(ULONGLONG PhysicalAddress, PULONG Values, ULONG Count);
	VIRTUAL BOOLEAN ReadPhysicalBytes(ULONGLONG PhysicalAddress, PUCHAR Values, ULONG Count);
	VIRTUAL BOOLEAN ReadPhysicalUshorts(ULONGLONG PhysicalAddress, PUSHORT Values, ULONG Count);
	VIRTUAL BOOLEAN ReadPhysicalUlongs(ULONGLONG PhysicalAddress, PULONG Values, ULONG Count);

	VIRTUAL BOOLEAN WritePhysicalByte(ULONGLONG PhysicalAddress, UCHAR Value);
	VIRTUAL BOOLEAN WritePhysicalUshort(ULONGLONG PhysicalAddress, USHORT Value);
	VIRTUAL BOOLEAN WritePhysicalUlong(ULONGLONG PhysicalAddress, ULONG Value);

	VIRTUAL BOOLEAN WritePhysical(ULONGLONG PhysicalAddress, PVOID Buffer, ULONG BufferSize);
	VIRTUAL BOOLEAN ReadPhysical(ULONGLONG PhysicalAddress, PVOID Buffer, ULONG BufferSize);
private:
	HANDLE DeviceHandle;
};
