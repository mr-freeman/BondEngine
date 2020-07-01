// Copyright ⓒ 2020 Valentyn Bondarenko. All rights reserved.

#pragma once

#include <Log.hpp>

namespace be::core
{
    namespace
    {
        // Error handling without C++ native exeptions
        // structured in order to keep code clear.
        // This is presumably the engine core component.
        struct Error
        {
        public:
            Error() :
                type{ Type::UNKNOWN },
                line{ 0u },
                description{ "" },
                source_file{ "" },
                error_codes{ { Type::GRAPHICS,   -14 },
                             { Type::SCRIPT,     -15 },
                             { Type::FILESYSTEM, -16 },
                             { Type::UNKNOWN,    -20 } }
            { }

        public:
            // High-level error type enum, has nothing to do with error codes.
            // Besides error codes are used, see next
            enum class Type
            {
                // INPUT is not needed, because GaInput manages device (mouse, keyboard etc) aqcuiring dynamically.
                // SOUND is not needed, because there's nothing exceptional in sound devices.
                GRAPHICS, // renderer-related issues
                SCRIPT, // always run-time error, invalid args, lua errors, also sol2 uses error
                FILESYSTEM, // can't find a vital/default file mostly, engine's/game's directory is not found

                UNKNOWN
            };

        public:
            std::map<Type, uint> error_codes;
            Type type;

            // C++20 is about to add <source_version> header which provides the next info
            // this will set us free from using __LINE__ / __FILE__ macros
            unsigned int line;
            const char* description;
            const char* source_file;
        };
    }

    class FatalError : public Error
    {
    public:
        explicit FatalError(const char* source_file, const unsigned int& line, Error::Type type, const char* description)
        {
            this->source_file = source_file;
            this->line = line;
            this->type = type;
            this->description = description;
        }

       ~FatalError() = default;

        FatalError(const FatalError&) = delete;
        FatalError(FatalError&&) = delete;

        FatalError operator=(const FatalError&) = delete;
        FatalError operator=(FatalError&&) = delete;

    public:
        [[noreturn]]
        void finalize();
    };
}