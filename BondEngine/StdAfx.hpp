// Copyright ⓒ 2020 Valentyn Bondarenko. All rights reserved.

#pragma once

// std
#include <vector>
#include <array>
#include <stack>
#include <queue>
#include <deque>
#include <list>

#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>

#include <fstream>
#include <sstream>

#include <utility>
#include <numbers>
#include <random>
#include <chrono>
#include <memory>

#include <optional>
#include <bitset>
#include <locale>
#include <string>
#include <tuple>
#include <regex>
#include <any>

#include <thread>
#include <mutex>

#include <algorithm>
#include <functional>
#include <filesystem>

// Windows
#define WIN32_LEAN_AND_MEAN
#ifndef NOMINMAX
#    define NOMINMAX
#endif

#include <Windows.h>
//---------------

// Diligent
#ifndef PLATFORM_WIN32
#    define PLATFORM_WIN32 1
#endif

#ifndef ENGINE_DLL
#    define ENGINE_DLL 1
#endif

#ifndef D3D11_SUPPORTED
#    define D3D11_SUPPORTED 1
#endif

#ifdef _DEBUG
#   define DILIGENT_DEBUG 1
#endif

#include "Graphics/GraphicsEngineD3D11/interface/EngineFactoryD3D11.h"

#include "Graphics/GraphicsEngine/interface/ShaderResourceBinding.h"
#include "Graphics/GraphicsEngine/interface/DeviceContext.h"
#include "Graphics/GraphicsEngine/interface/RenderDevice.h"
#include "Graphics/GraphicsEngine/interface/SwapChain.h"

#include <..\DiligentCore\Common\interface\BasicMath.hpp>
#include <..\DiligentCore\Common\interface\AdvancedMath.hpp>

#include <..\DiligentTools\TextureLoader\interface\TextureUtilities.h>
#include <..\DiligentCore\Graphics\GraphicsTools\interface\MapHelper.hpp>
#include <..\DiligentCore\Graphics\GraphicsTools\interface\GraphicsUtilities.h>
#include <..\DiligentCore\Graphics\GraphicsAccessories\interface\GraphicsAccessories.hpp>


#include <Common/interface/RefCntAutoPtr.hpp>
//---------------

// ImGUI
#include <ImGuiImplWin32.hpp>

#include <..\DiligentTools\ThirdParty\imgui\imgui.h>
//---------------

// SPDLog
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
//---------------

// SOL2
#include <..\lua53\include\lua.hpp>

#include <sol/sol.hpp>
//---------------

// GaInput
#pragma warning(push)
#   pragma warning(disable : 4267)
#   include <gainput/gainput.h>
#pragma warning(pop)
//---------------

// Emyl
#include <../third-party/Emyl/emyl.h>
//---------------

#include <Types.hpp>
#include <Platform.hpp>


// draft namespace tree
namespace be
{
    namespace render
    {
        namespace utils
        {
            // Diligent Engine provides a unified means to manage renderers and resources.
        }

        using namespace Diligent;
    }

    namespace sound
    {

    }

    namespace input
    {

    }

    namespace script
    {

    }

    namespace game
    {
        namespace ui
        {

        }
    
        namespace ai
        {
            
        }

        using namespace render::utils;
    }
    
    namespace cmd
    {
        
    }

    namespace core
    {
        // class FileSystem
        // class FatalError
        // reserved.
    }

    namespace utils
    {

    }

    namespace math = Diligent::Math;
    namespace fs = std::filesystem;

    using namespace std::string_literals;
}