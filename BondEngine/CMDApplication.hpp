// Copyright ⓒ 2020 Valentyn Bondarenko. All rights reserved.

#pragma once

namespace be::core::cmd
{
    // Console CMD in the game / Settings CMD in the game files / Executable CMD in the BondEngine.exe args (-nosound)
    // Apparently, all engine commands must have description.
    class CMDApplication
    {
    public:
        CMDApplication();
       ~CMDApplication() = default;

        CMDApplication(const CMDApplication&) = delete;
        CMDApplication(CMDApplication&&) = delete;

        CMDApplication operator=(const CMDApplication&) = delete;
        CMDApplication operator=(CMDApplication&&) = delete;

    public:
        using Command = std::pair<string, bool>;

    public:
        bool isNoSound() const { return nosound.second; }
        bool isNoSplash() const { return nosplash.second; }

    private:
        string cmd_args;

        Command nosound; // disable/enable sound system
        Command nosplash; // disable/enable splash screen
    };
}

