// Copyright ⓒ 2020 Valentyn Bondarenko. All rights reserved.

#include <StdAfx.hpp>

#include <SystemInfo.hpp>

#include <dxgi.h>
#include <Windows.h>
#include <sysinfoapi.h>
#include <VersionHelpers.h>

namespace be::utils
{
    SystemInfo::SystemInfo()
    {
        determineOSInfo();
        determineCPUInfo();
        determineRAMInfo();
    }

    void SystemInfo::writeToLog()
    {
        utils::log::info("System Capabilities"s);
        utils::log::info("OS: name: {}, x{}, user name {}, pc name: {}"s, os_info.name, os_info.bitness, os_info.user_name, os_info.pc_name);
        utils::log::info("CPU: vendor: {}, brand: {}, cores: {}"s, cpu_info.vendor_name, cpu_info.brand_name, cpu_info.core_count);
        utils::log::info("RAM Memory: total: {} MB, available: {} MB, recerved: {} MB"s, ram_info.total, ram_info.available, ram_info.reserved);
    }

    void SystemInfo::determineOSInfo()
    {
        // determine Windows version
        os_info.name = "Windows "s;

        if (IsWindows10OrGreater())
            os_info.name += "10";
        else if (IsWindows8Point1OrGreater())
            os_info.name += "8.1";
        else if (IsWindows8OrGreater())
            os_info.name += "8";
        else if (IsWindows7SP1OrGreater())
            os_info.name += "7 Service Pack 1";
        else if (IsWindows7OrGreater())
            os_info.name += "7";

        // determine PC name.
        char pc_name[256];
        DWORD pc_name_len = 256 + 1;
        GetUserNameA(pc_name, &pc_name_len);

        os_info.pc_name = pc_name;

        // determine user name.
        char username[256];
        DWORD username_len = 256 + 1;
        GetComputerNameA(username, &username_len);

        os_info.user_name = username;

        // determine bitness
        using IW64PFP = BOOL(WINAPI*)(HANDLE, bool*);
        // typedef BOOL(WINAPI* IW64PFP)(HANDLE, BOOL*);

        auto bitness = 32u;
        // ----
        // https://forum.vingrad.ru/forum/topic-291876.html :
        // When this application is compiled as a 32-bit app,
        // and run on a native 64-bit system, Windows will run
        // this application under WOW64.  WOW64 is the Windows-
        // on-Windows subsystem that lets native 32-bit applications
        // run in 64-bit land.  This calls the kernel32.dll
        // API to see if this process is running under WOW64.
        // If it is running under WOW64, then that clearly means
        // this 32-bit application is running on a 64-bit OS,
        // and IsWow64Process will return true.
        IW64PFP IW64P = (IW64PFP)GetProcAddress(GetModuleHandleA("kernel32"), "IsWow64Process");

        if (IW64P != NULL)
        {
            bool bStatus = false;
            IW64P(GetCurrentProcess(), &bStatus);

            bitness = (bStatus) ? 64 : 32;
        }

    }

    void SystemInfo::determineCPUInfo()
    {
        // Low-level implementation, thanks to Microsoft Docs.
        array<int, 4> cpui;

        // Calling __cpuid with 0x0 as the function_id argument
        // determines the number of the highest valid function ID.
        int nIds_;
        int nExIds_;

        vector<array<int, 4>> data_;
        vector<array<int, 4>> extdata_;
        __cpuid(cpui.data(), 0);
        nIds_ = cpui[0];

        for (int i = 0; i <= nIds_; ++i)
        {
            __cpuidex(cpui.data(), i, 0);
            data_.push_back(cpui);
        }
        //--------------------

        // char vendor [0x20];
        char vendor[0x20];
        memset(vendor, 0, sizeof(vendor));
        *reinterpret_cast<int*>(vendor) = data_[0][1];
        *reinterpret_cast<int*>(vendor + 4) = data_[0][3];
        *reinterpret_cast<int*>(vendor + 8) = data_[0][2];
        cpu_info.vendor_name = vendor;


        auto* amd_vendor = "AuthenticAMD";
        auto* intel_vendor = "GenuineIntel";

        if (vendor == intel_vendor)
            cpu_info.vendor_name = intel_vendor;
        if (vendor == amd_vendor)
            cpu_info.vendor_name = amd_vendor;
        //-------------------

        // Capture brand string

        // Calling __cpuid with 0x80000000 as the function_id argument
        // determines the number of the highest valid extended ID.
        __cpuid(cpui.data(), 0x80000000);
        nExIds_ = cpui[0];

        char brand[0x80];
        memset(brand, 0, sizeof(brand));

        for (int i = 0x80000000; i <= nExIds_; ++i)
        {
            __cpuidex(cpui.data(), i, 0);
            extdata_.push_back(cpui);
        }

        // Interpret CPU brand string if reported
        if (nExIds_ >= 0x80000004)
        {
            memcpy(brand, extdata_[2].data(), sizeof(cpui));
            memcpy(brand + 16, extdata_[3].data(), sizeof(cpui));
            memcpy(brand + 32, extdata_[4].data(), sizeof(cpui));
            cpu_info.brand_name = brand;
        }
        //-------------------

        // CPU Architecture.
        SYSTEM_INFO stInfo;
        GetSystemInfo(&stInfo);
        GetNativeSystemInfo(&stInfo);

        switch (stInfo.wProcessorArchitecture)
        {
            case PROCESSOR_ARCHITECTURE_INTEL:
                cpu_info.architecture = "Intel x32"s;
                break;

            case PROCESSOR_ARCHITECTURE_IA64:
                cpu_info.architecture = "Intel x64"s;

                break;

            case PROCESSOR_ARCHITECTURE_AMD64:
                cpu_info.architecture = "AMD x64"s;
                break;

            default:
                cpu_info.architecture = "Unknown"s;
        }

        cpu_info.core_count = std::thread::hardware_concurrency();
    }

    void SystemInfo::determineRAMInfo()
    {
        MEMORYSTATUSEX mem{ };
        mem.dwLength = sizeof(mem);
        GlobalMemoryStatusEx(&mem);

        ram_info.total = static_cast<uint>(mem.ullTotalPhys / 1024 / 1024);
        ram_info.available = static_cast<uint>(mem.ullAvailPhys / 1024 / 1024);
        ram_info.reserved = static_cast<uint>(ram_info.total - ram_info.available);
    }
}