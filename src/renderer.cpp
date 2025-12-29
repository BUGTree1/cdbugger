#include "renderer.h"

namespace CDBugger {

using namespace std;
using namespace glm;

void Renderer::init() {
    width = 800;
    height = 480;

    ASSERT_SDL(SDL_Init(SDL_INIT_VIDEO));
    ASSERT_SDL(TTF_Init());

    fc_config = FcInitLoadConfigAndFonts();
    UTILS_ASSERT(fc_config);

    window = SDL_CreateWindow("CDBugger", width, height, SDL_WINDOW_HIGH_PIXEL_DENSITY | SDL_WINDOW_RESIZABLE);
    ASSERT_SDL(window);

    renderer = SDL_CreateRenderer(window, NULL);
    ASSERT_SDL(renderer);

    ASSERT_SDL(SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND));

    // TODO: Test if this is surely working because mouse state i think is window coordinates
    //SDL_FPoint fmouse;
    //SDL_GetMouseState(&fmouse.x, &fmouse.y);
    //SDL_Point mouse = {(int)fmouse.x, (int)fmouse.y};
    //SDL_DisplayID display = SDL_GetDisplayForPoint(&mouse);
    //SDL_SetWindowPosition(window, SDL_WINDOWPOS_UNDEFINED_DISPLAY(display),SDL_WINDOWPOS_UNDEFINED_DISPLAY(display));

    string font_path;

    FcPattern* name_pattern = FcNameParse((const FcChar8*)"FiraCode-Regular");
    FcConfigSubstitute(fc_config, name_pattern, FcMatchPattern);
    FcDefaultSubstitute(name_pattern);

    FcResult font_result;
    FcPattern* font_pattern = FcFontMatch(fc_config, name_pattern, &font_result);
    if (font_pattern)
    {
        FcChar8* font_ppath = NULL;
        if (FcPatternGetString(font_pattern, FC_FILE, 0, &font_ppath) == FcResultMatch)
        {
            font_path = (char*)font_ppath;
        }
        FcPatternDestroy(font_pattern);
    }

    FcPatternDestroy(name_pattern);

    //cout << font_path << endl;
    font = TTF_OpenFont(font_path.c_str(), 20);
    ASSERT_SDL(font);

    text_engine = TTF_CreateRendererTextEngine(renderer);
    ASSERT_SDL(text_engine);

    //text_bg_texture         = IMG_LoadTexture(renderer, "text_bg.png");
    //textfield_bg_texture    = IMG_LoadTexture(renderer, "textfield_bg.png");
    //button_bg_texture       = IMG_LoadTexture(renderer, "button_bg.png");
    //button_hover_bg_texture = IMG_LoadTexture(renderer, "button_bg_hover.png");
    //button_click_bg_texture = IMG_LoadTexture(renderer, "button_bg_click.png");
    //ASSERT_SDL(text_bg_texture);
    //ASSERT_SDL(textfield_bg_texture);
    //ASSERT_SDL(button_bg_texture);
    //ASSERT_SDL(button_hover_bg_texture);
    //ASSERT_SDL(button_click_bg_texture);
    ////TODO: IDK: should all be ADD not only buttons?
    //SDL_BlendMode texture_blend_mode = SDL_BLENDMODE_ADD;
    //SDL_SetTextureBlendMode(text_bg_texture, texture_blend_mode);
    //SDL_SetTextureBlendMode(textfield_bg_texture, texture_blend_mode);
    //SDL_SetTextureBlendMode(button_bg_texture, texture_blend_mode);
    //SDL_SetTextureBlendMode(button_hover_bg_texture, texture_blend_mode);
    //SDL_SetTextureBlendMode(button_click_bg_texture, texture_blend_mode);
    //SDL_ScaleMode texture_scale_mode = SDL_SCALEMODE_NEAREST;
    //SDL_SetTextureScaleMode(text_bg_texture, texture_scale_mode);
    //SDL_SetTextureScaleMode(textfield_bg_texture, texture_scale_mode);
    //SDL_SetTextureScaleMode(button_bg_texture, texture_scale_mode);
    //SDL_SetTextureScaleMode(button_hover_bg_texture, texture_scale_mode);
    //SDL_SetTextureScaleMode(button_click_bg_texture, texture_scale_mode);

    Button* button = new Button();
    button->bounds = Bounds(100,100,100,100);
    button->text_position = POSITION_CENTER;
    button->text = "Button";
    button->bg = (UI_Background) {NULL, Color(0  ,0  ,0  ,1)};
    button->hover_bg = (UI_Background) {NULL, Color(0.5,0.5,0.5,1)};
    button->click_bg = (UI_Background) {NULL, Color(1  ,1  ,1  ,1)};
    button->fg = Color(1  ,1  ,1  ,1);
    button->hover_fg = Color(0.5,0.5,0.5,1);
    button->click_fg = Color(0  ,0  ,0  ,1);
    button->gradient_duration = 0.1;

    // TODO: MAYBE AUTOMATE THIS?
    button->base   = {button->bg, button->fg};
    button->target = {button->bg, button->fg};

    elements.push_back(button);
}

void Renderer::deinit() {
    //SDL_DestroyTexture(text_bg_texture);
    //SDL_DestroyTexture(textfield_bg_texture);
    //SDL_DestroyTexture(button_bg_texture);
    //SDL_DestroyTexture(button_hover_bg_texture);
    //SDL_DestroyTexture(button_click_bg_texture);

    TTF_DestroyRendererTextEngine(text_engine);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Renderer::render() {
    SDL_FPoint mouse_pos_window = (SDL_FPoint){};
    mouse_flags = SDL_GetMouseState(&mouse_pos_window.x, &mouse_pos_window.y);
    SDL_RenderCoordinatesFromWindow(renderer, mouse_pos_window.x, mouse_pos_window.y, &mouse_pos.x, &mouse_pos.y);

    SDL_SetRenderDrawColorFloat(renderer, 0, 0, 0, 1);
    SDL_RenderClear(renderer);

    for (int i = 0; i < elements.size(); i++) {
        elements[i]->render(this);
    }

    SDL_RenderPresent(renderer);
}

void Text::render(Renderer* renderer) {
    if(bg.texture == NULL) {
        if(bg.color.a != 0){
            ASSERT_SDL(SDL_SetRenderDrawColorFloat(renderer->renderer, bg.color.r, bg.color.g, bg.color.b, bg.color.a));
            ASSERT_SDL(SDL_RenderFillRect(renderer->renderer, (SDL_FRect*)&bounds));
        }
    } else {
        ASSERT_SDL(SDL_RenderTexture(renderer->renderer, bg.texture, NULL, (SDL_FRect*)&bounds));
    }

    TTF_Text* sdl_text = TTF_CreateText(renderer->text_engine, renderer->font, text.c_str(), 0);
    ASSERT_SDL(sdl_text);

    ASSERT_SDL(TTF_SetTextColorFloat(sdl_text, fg.r, fg.g, fg.b, fg.a));
    // TODO: IDK: ASSERT_SDL(TTF_SetTextWrapWidth(text, text_bounds.z));

    vec4 text_bounds = bounds;

    ivec2 text_sizei = ivec2(0,0);
    ASSERT_SDL(TTF_GetTextSize(sdl_text, &text_sizei.x, &text_sizei.y));
    vec2 text_size = text_sizei;

    //TODO: wierd crisp font when position changed

    switch (position) {
        case POSITION_CENTER   :
        text_bounds = vec4(xy((vec4)bounds) + (zw((vec4)bounds) / 2.0f) - (text_size / 2.0f), zw((vec4)bounds));
        break;
        case POSITION_TOP      :

        break;
        case POSITION_BOTTOM   :

        break;
        case POSITION_LEFT     :

        break;
        case POSITION_RIGHT    :

        break;
        case POSITION_LT_CORNER:
        break;
        case POSITION_LB_CORNER:

        break;
        case POSITION_RT_CORNER:

        break;
        case POSITION_RB_CORNER:

        break;
        default:
        UNREACHABLE();
        break;
    }

    ASSERT_SDL(TTF_DrawRendererText(sdl_text, text_bounds.x, text_bounds.y));

    TTF_DestroyText(sdl_text);
}

void Button::render(Renderer* renderer){
    bool curr_hover = SDL_PointInRectFloat((SDL_FPoint*)&renderer->mouse_pos, (SDL_FRect*)&bounds);
    bool curr_down = (SDL_BUTTON_LMASK & renderer->mouse_flags) && hover;

    bool down_now    = !down && curr_down;
    bool up_now      = down && !curr_down;
    bool hover_now   = !hover && curr_hover;
    bool unhover_now = hover && !curr_hover;
    down      = curr_down;
    hover     = curr_hover;

    if (down_now || up_now || hover_now || unhover_now) {
        gradient_time = 0.0;
    }

    if (hover_now) {
        target = {hover_bg, hover_fg};
        base   = {bg, fg};
        if(hover_cb != NULL) hover_cb(renderer, user_pointer);
    }else if (unhover_now){
        target = {bg, fg};
        base   = {hover_bg, hover_fg};
        if(unhover_cb != NULL) unhover_cb(renderer, user_pointer);
    }else if (down_now) {
        target = {click_bg, click_fg};
        base   = {hover_bg, hover_fg};
        if(click_cb != NULL) click_cb(renderer, user_pointer);
    }else if (up_now){
        target = {hover_bg, hover_fg};
        base   = {click_bg, click_fg};
        if(up_cb != NULL) up_cb(renderer, user_pointer);
    }

    Text ui_text = Text();
    ui_text.bounds = bounds;
    ui_text.position = text_position;
    ui_text.text = text;
    ui_text.fg = lerp((vec4)base.fg, (vec4)target.fg, (float)gradient_time);
    if(base.bg.texture != NULL && target.bg.texture != NULL){
        ASSERT_SDL(SDL_SetTextureAlphaModFloat(base.bg.texture, 1 - gradient_time));
        ui_text.bg = {base.bg.texture, Color()};
        ui_text.render(renderer);
        ASSERT_SDL(SDL_SetTextureAlphaModFloat(target.bg.texture, gradient_time));
        ui_text.bg = {target.bg.texture, Color()};
    }else{
        ui_text.bg = {NULL, lerp((vec4)base.bg.color, (vec4)target.bg.color, (float)gradient_time)};
    }
    ui_text.render(renderer);

    if(gradient_time < 1.0){
        gradient_time += renderer->delta_time / gradient_duration;
    }
}

};
