// Copyright ⓒ 2020 Valentyn Bondarenko. All rights reserved.

#pragma once

#include <StdAfx.hpp>

#include <LocaleManager.hpp>

namespace be::utils
{
    LocaleManager::LocaleManager() :
        locales
    {
        { "English"s, locale("en_EN.UTF8") },
        { "Russian"s, locale("ru_RU.UTF8") },
        { "German"s, locale("de_DE.UTF8") },
        { "French"s, locale("fr_FR.UTF8") }
    }
    { }

    void LocaleManager::initialize()
    {
        this->setLocale(locales["English"]);
    }

    const locale& LocaleManager::getLocale()
    {
        return current_locale;
    }

    void LocaleManager::setLocale(const locale& loc)
    {
        current_locale = locale(loc);
        locale::global(current_locale);
    }

}