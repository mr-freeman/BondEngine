// Copyright ⓒ 2020 Valentyn Bondarenko. All rights reserved.

#pragma once

#include <spdlog/spdlog.h>

#include <FileSystem.hpp>
#include <Error.hpp>
#include <Log.hpp>

#include <filesystem>

namespace be
{
    struct File
    {
        File() = default;
        virtual ~File() = default;

        std::string name;
        std::filesystem::path directory;
        std::string extension;

        std::filesystem::path full_name;

    public:
        // param 1: apply default file or fatal error if file is not found
        // throws exception if a file is not found and there's no default one provided.
        void check(bool def)
        {
            using namespace std::string_literals;

            full_name = FileSystem::getGameDataDirectory(directory.string()) / (name + extension);

            if (!std::filesystem::exists(full_name))
            {
                if (def)
                {
                    utils::log::warn("Can't open file {}. Used default file instead"s, full_name.string().c_str());
                    full_name = FileSystem::getGameDataDirectory(directory.string()) / ("default"s + extension);
                }
                else
                {
                    core::FatalError fe("File.hpp",
                                  45u,
                                  core::Error::Type::FILESYSTEM,
                                  ("Can't open file "s + full_name.string()).c_str()
                                  );

                    fe.finalize();
                }
            }
            else
                utils::log::info("Open file {}"s, full_name.string().c_str());
        }
    };
}