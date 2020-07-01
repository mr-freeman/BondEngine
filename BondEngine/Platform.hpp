// Copyright ⓒ 2020 Valentyn Bondarenko. All rights reserved.

#pragma once

#ifndef _MSC_VER
#   error Bond Engine does not support this compiler. 
#endif

#ifndef _WIN32
#   error Bond Engine does not support this platform.
#endif

#ifdef _DEBUG
#   define BE_DEBUG
#endif

#ifdef BE_DEBUG
#   define BE_TEST_CODE
#endif