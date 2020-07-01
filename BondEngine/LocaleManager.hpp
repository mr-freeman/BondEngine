// Copyright ⓒ 2020 Valentyn Bondarenko. All rights reserved.

#pragma once

namespace be::utils
{
    using std::locale;

    // handles localization in for a game.
    // changes on sturtup
    class LocaleManager
    {
    public:
        LocaleManager();
       ~LocaleManager() = default;

        LocaleManager(const LocaleManager&) = delete;
        LocaleManager(LocaleManager&&) = delete;

        LocaleManager operator=(const LocaleManager&) = delete;
        LocaleManager operator=(LocaleManager&&) = delete;

    public:
        // load from game configs
        void initialize();
        void finalize();

        const locale& getLocale();
        void setLocale(const locale& loc);

    private:
        locale current_locale;
        std::unordered_map<string, locale> locales; // all supported locales
    };
}