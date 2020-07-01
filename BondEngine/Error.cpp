// Copyright ⓒ 2020 Valentyn Bondarenko. All rights reserved.

#include <StdAfx.hpp>

#include <Error.hpp>

namespace be::core
{
    void FatalError::finalize()
    {
        using namespace utils;

        auto msg = "Fatal Error ocured";
        log::info(msg);
#ifdef WIN32
        OutputDebugStringA(msg);
#endif

        if (type != Type::UNKNOWN)
        {
            log::error("Source File: {}"s, source_file);
            log::error("Line: {}"s, line);
            log::error("Description: {}"s, description);
        }
        else
            log::error("Description: Unknown error. Please, report technical support"s);

        switch (type)
        {
            case Type::UNKNOWN:
            case Type::FILESYSTEM:
            case Type::GRAPHICS:
            case Type::SCRIPT:
                std::exit(error_codes[type]);
        }
    }
}