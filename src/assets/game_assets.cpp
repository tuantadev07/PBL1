// D:/code/project/PBL1-NIM-Game/src/assets/game_assets.cpp
#include "assets/game_assets.h"

namespace {

    constexpr const char* musicBgmPath = "assets/music/beanfeast.mp3";
    constexpr const char* soundClickPath = "assets/music/Click1B.mp3";
    constexpr const char* soundEndGamePath = "assets/music/Retro_Success.wav";
    
    constexpr const char* bgMainMenuPath = "assets/images/bg_main_menu.png";
    constexpr const char* bgMatchSetupPath = "assets/images/bg_match_setup.png";
    constexpr const char* bgPlayPath = "assets/images/bg_play.png";

    constexpr const char* btnNormalPath = "assets/ui/button/btn_normal.png";
    constexpr const char* btnHoverPath = "assets/ui/button/btn_hover.png";
    constexpr const char* btnPressedPath = "assets/ui/button/btn_pressed.png";
    constexpr const char* btnSelectedPath = "assets/ui/button/btn_selected.png";

    constexpr const char* playSetupDecreaseBtnNormalPath = "assets/ui/button/btn_minus.png";
    constexpr const char* playSetupDecreaseBtnHoverPath = "assets/ui/button/btn_minus.png";
    constexpr const char* playSetupDecreaseBtnPressedPath = "assets/ui/button/btn_minus.png";
    constexpr const char* playSetupDecreaseBtnSelectedPath = "assets/ui/button/btn_minus.png";

    constexpr const char* playSetupIncreaseBtnNormalPath = "assets/ui/button/btn_plus.png";
    constexpr const char* playSetupIncreaseBtnHoverPath = "assets/ui/button/btn_plus.png";
    constexpr const char* playSetupIncreaseBtnPressedPath = "assets/ui/button/btn_plus.png";
    constexpr const char* playSetupIncreaseBtnSelectedPath = "assets/ui/button/btn_plus.png";

    constexpr const char* playLimitSwitchOffPath = "assets/ui/button/btn_switch_off.png";
    constexpr const char* playLimitSwitchOnPath = "assets/ui/button/btn_switch_on.png";
    
    constexpr const char* fontTitlePath = "assets/fonts/pixelFont-7-8x14-sproutLands.ttf";
    constexpr const char* fontCommonPath = "assets/fonts/SVN-New-Athletic-M54.ttf";

    constexpr const char* cloudNormalPath = "assets/ui/cloud_single/cloud4.png";
    constexpr const char* cloudHoverPath = "assets/ui/cloud_single/cloud5.png";
    constexpr const char* cloudPressedPath = "assets/ui/cloud_single/cloud5.png";
    constexpr const char* cloudSelectedPath = "assets/ui/cloud_single/cloud6.png";

    
    bool _is_texture_valid(Texture2D t) {
        return t.id != 0;
    }

    bool _is_font_valid(Font f) {
        return f.texture.id != 0;
    }
    
    bool _load_texture_if_exists(Texture2D& out, const char* path) {
        out = {};

        if (path == nullptr || !FileExists(path)) {
            return false;
        }

        out = LoadTexture(path);
        if (!_is_texture_valid(out)) {
            return false;
        }

        SetTextureFilter(out, TEXTURE_FILTER_POINT);
        return true;
    }

    void _unload_if_loaded(Texture2D& tex) {
        if (_is_texture_valid(tex)) {
            UnloadTexture(tex);
        }
        tex = {};
    }

    void _unload_if_loaded(UISwitchTextures& tex) {
        _unload_if_loaded(tex.off);
        _unload_if_loaded(tex.on);
    }
    void _unload_if_loaded(Font& font, bool& owned) {
        if (owned && _is_font_valid(font)) {
            UnloadFont(font);
        }

        font = {};
        owned = false;
    }
    
    
    bool _game_assets_load_button_textures_from_single_png(UIButtonStateTextures& out, const char* path) {
        out = {};
        
        bool okNormal = _load_texture_if_exists(out.normal, path);
        bool okHover = _load_texture_if_exists(out.hover, path);
        bool okPressed = _load_texture_if_exists(out.pressed, path);
        bool okSelected = _load_texture_if_exists(out.selected, path);
        
        if (!okNormal || !okHover || !okPressed || !okSelected) {
            _unload_if_loaded(out.normal);
            _unload_if_loaded(out.hover);
            _unload_if_loaded(out.pressed);
            _unload_if_loaded(out.selected);
            return false;
        }
        
        return true;
    }

    bool _game_assets_load_button_textures_from_multiple_pngs(
        UIButtonStateTextures& out,
        const char* normalPath,
        const char* hoverPath,
        const char* pressedPath,
        const char* selectedPath
    ) {
        out = {};
        
        if (!_load_texture_if_exists(out.normal, normalPath)) {
            return false;
        }
        
        if (!_load_texture_if_exists(out.hover, hoverPath)) {
            _load_texture_if_exists(out.hover, normalPath);
        }
        
        if (!_load_texture_if_exists(out.pressed, pressedPath)) {
            _load_texture_if_exists(out.pressed, normalPath);
        }
        
        if (!_load_texture_if_exists(out.selected, selectedPath)) {
            _load_texture_if_exists(out.selected, normalPath);
        }
        
        if (!_is_texture_valid(out.hover) || !_is_texture_valid(out.pressed) || !_is_texture_valid(out.selected)) {
            _unload_if_loaded(out.normal);
            _unload_if_loaded(out.hover);
            _unload_if_loaded(out.pressed);
            _unload_if_loaded(out.selected);
            return false;
        }
        
        return true;
    }

    bool _load_font_if_exists(Font& out, bool& owned, const char* path) {
        out = {};
        owned = false;
        
        if (path == nullptr || !FileExists(path)) {
            return false;
        }

        const char* allChars = u8" abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.,?!:;+-=*/()<>[]{}@#$%^&*_\"'\\àáạảãâầấậẩẫăằắặẳẵèéẹẻẽêềếệểễìíịỉĩòóọỏõôồốộổỗơờớợởỡùúụủũưừứựửữỳýỵỷỹđÀÁẠẢÃÂẦẤẬẨẪĂẰẮẶẲẴÈÉẸẺẼÊỀẾỆỂỄÌÍỊỈĨÒÓỌỎÕÔỒỐỘỔỖƠỜỚỢỞỠÙÚỤỦŨƯỪỨỰỬỮỲÝỴỶỸĐ";

        int codepointsCount = 0;
        int* codepoints = LoadCodepoints(allChars, &codepointsCount);

        out = LoadFontEx(path, 128, codepoints, codepointsCount);

        if (codepoints != nullptr) {
            UnloadCodepoints(codepoints);
        }

        if (!_is_font_valid(out)) {
            return false;
        }

        SetTextureFilter(out.texture, TEXTURE_FILTER_POINT);
        owned = true;
        return true;
    }

     bool _load_switch_if_exists(UISwitchTextures& out, const char* offPath, const char* onPath) {
        out = {};

        if (!_load_texture_if_exists(out.off, offPath)) {
            _unload_if_loaded(out.off);
            return false;
        }

        if (!_load_texture_if_exists(out.on, onPath)) {
            _unload_if_loaded(out.on);
            return false;
        }

        return true;
    }


} // namépace khong ten

bool game_assets_load() {
    if (g_assets.loaded) {
        return true;
    }

    bool bgMainMenuOk = _load_texture_if_exists(g_assets.images.bg_main_menu, bgMainMenuPath);
    bool bgMatchSetupOk = _load_texture_if_exists(g_assets.images.bg_match_setup, bgMatchSetupPath);
    bool bgPlayOk = _load_texture_if_exists(g_assets.images.bg_play, bgPlayPath);

    bool btnCommonOk = _game_assets_load_button_textures_from_multiple_pngs(
        g_assets.buttons.common,
        btnNormalPath,
        btnHoverPath,
        btnPressedPath,
        btnSelectedPath
    );
    bool btnCloudOk = _game_assets_load_button_textures_from_multiple_pngs(
        g_assets.buttons.cloud,
        cloudNormalPath,
        cloudHoverPath,
        cloudPressedPath,
        cloudSelectedPath
    );
    bool btnPlaySetupDecreaseOk = _game_assets_load_button_textures_from_multiple_pngs(
        g_assets.buttons.play_setup_decrease,
        playSetupDecreaseBtnNormalPath,
        playSetupDecreaseBtnHoverPath,
        playSetupDecreaseBtnPressedPath,
        playSetupDecreaseBtnSelectedPath
    );

    bool btnPlaySetupIncreaseOk = _game_assets_load_button_textures_from_multiple_pngs(
        g_assets.buttons.play_setup_increase,
        playSetupIncreaseBtnNormalPath,
        playSetupIncreaseBtnHoverPath,
        playSetupIncreaseBtnPressedPath,
        playSetupIncreaseBtnSelectedPath
    );

    bool btnPlaySwitchOk = _load_switch_if_exists(
        g_assets.switches.play_limit,
        playLimitSwitchOffPath,
        playLimitSwitchOnPath
    );

    bool commonFontOk = _load_font_if_exists(
        g_assets.fonts.common,
        g_assets.fonts.ownsCommon,
        fontCommonPath
    );
    bool titleFontOk = _load_font_if_exists(
        g_assets.fonts.title,
        g_assets.fonts.ownsTitle,
        fontTitlePath
    );

    if (!commonFontOk) {
        g_assets.fonts.common = GetFontDefault();
        g_assets.fonts.ownsCommon = false;
    }
    if (!titleFontOk) {
        g_assets.fonts.title = GetFontDefault();
        g_assets.fonts.ownsTitle = false;
    }

    g_assets.sounds.click = LoadSound(soundClickPath);
    g_assets.sounds.end = LoadSound(soundEndGamePath);

    
    const bool allAssetsLoaded =
    bgMainMenuOk &&
    bgMatchSetupOk &&
    bgPlayOk &&
    btnCommonOk &&
    btnCloudOk &&
    btnPlaySetupDecreaseOk &&
    btnPlaySetupIncreaseOk &&
    btnPlaySwitchOk &&
    commonFontOk &&
    titleFontOk;
    
    g_assets.loaded = allAssetsLoaded;
    return g_assets.loaded;
}

void game_assets_unload() {
    _unload_if_loaded(g_assets.images.bg_main_menu);
    _unload_if_loaded(g_assets.images.bg_match_setup);
    _unload_if_loaded(g_assets.images.bg_play);

    _unload_if_loaded(g_assets.buttons.common.normal);
    _unload_if_loaded(g_assets.buttons.common.hover);
    _unload_if_loaded(g_assets.buttons.common.pressed);
    _unload_if_loaded(g_assets.buttons.common.selected);

    _unload_if_loaded(g_assets.buttons.cloud.normal);
    _unload_if_loaded(g_assets.buttons.cloud.hover);
    _unload_if_loaded(g_assets.buttons.cloud.pressed);
    _unload_if_loaded(g_assets.buttons.cloud.selected);

    _unload_if_loaded(g_assets.buttons.play_setup_decrease.normal);
    _unload_if_loaded(g_assets.buttons.play_setup_decrease.hover);
    _unload_if_loaded(g_assets.buttons.play_setup_decrease.pressed);
    _unload_if_loaded(g_assets.buttons.play_setup_decrease.selected);

    _unload_if_loaded(g_assets.buttons.play_setup_increase.normal);
    _unload_if_loaded(g_assets.buttons.play_setup_increase.hover);
    _unload_if_loaded(g_assets.buttons.play_setup_increase.pressed);
    _unload_if_loaded(g_assets.buttons.play_setup_increase.selected);

    _unload_if_loaded(g_assets.switches.play_limit);

    _unload_if_loaded(g_assets.fonts.common, g_assets.fonts.ownsCommon);
    _unload_if_loaded(g_assets.fonts.title, g_assets.fonts.ownsTitle);

    g_assets = {};
}
