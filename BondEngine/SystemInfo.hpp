// Copyright ⓒ 2020 Valentyn Bondarenko. All rights reserved.

#pragma once

#include <Log.hpp>

namespace be::utils
{
    // Windows only.
    // GPU Info is determined via Diligent Graphics Engine API.
    class SystemInfo
    {
    public:
        SystemInfo();
       ~SystemInfo() = default;

        SystemInfo(const SystemInfo&) = delete;
        SystemInfo(SystemInfo&&) = delete;

        SystemInfo operator=(const SystemInfo&) = delete;
        SystemInfo operator=(SystemInfo&&) = delete;

    public:
        struct CPUInfo
        {
            string vendor_name;
            string brand_name;

            uint core_count;

            string architecture;
        };

        struct RAM
        {
            uint total;
            uint available;
            uint reserved;
        };

        struct OSInfo
        {
            string name;
            string version;
            string bitness;
            string user_name;
            string pc_name;
        };

    public:
        const OSInfo& getOSInfo() const { return os_info; }
        const CPUInfo& getCPUInfo() const { return cpu_info; }
        const RAM& getRAMInfo() const { return ram_info; }

    public:
        void determineOSInfo();
        void determineCPUInfo();
        void determineRAMInfo();

    public:
        void writeToLog();

    private:
        OSInfo os_info;
        CPUInfo cpu_info;
        RAM ram_info;
    };
}