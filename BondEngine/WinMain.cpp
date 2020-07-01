// Copyright ⓒ 2020 Valentyn Bondarenko. All rights reserved.

#include <StdAfx.hpp>

#include <IEngine.hpp>
#include <BondEngine.hpp>

int __stdcall WinMain(HINSTANCE, HINSTANCE, char*, int)
{
    auto bond_engine = std::make_unique<be::BondEngine>();

    bond_engine->initialize();
    bond_engine->run();
    bond_engine->finalize();

    return 0;
}