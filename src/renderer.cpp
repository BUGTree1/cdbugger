#include "renderer.h"

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

    text_bg_texture         = IMG_LoadTexture(renderer, "text_bg.png");
    textfield_bg_texture    = IMG_LoadTexture(renderer, "textfield_bg.png");
    button_bg_texture       = IMG_LoadTexture(renderer, "button_bg.png");
    button_hover_bg_texture = IMG_LoadTexture(renderer, "button_bg_hover.png");
    button_click_bg_texture = IMG_LoadTexture(renderer, "button_bg_click.png");
    ASSERT_SDL(text_bg_texture);
    ASSERT_SDL(textfield_bg_texture);
    ASSERT_SDL(button_bg_texture);
    ASSERT_SDL(button_hover_bg_texture);
    ASSERT_SDL(button_click_bg_texture);
    //TODO: IDK: should all be ADD not only buttons?
    SDL_BlendMode texture_blend_mode = SDL_BLENDMODE_ADD;
    SDL_SetTextureBlendMode(text_bg_texture, texture_blend_mode);
    SDL_SetTextureBlendMode(textfield_bg_texture, texture_blend_mode);
    SDL_SetTextureBlendMode(button_bg_texture, texture_blend_mode);
    SDL_SetTextureBlendMode(button_hover_bg_texture, texture_blend_mode);
    SDL_SetTextureBlendMode(button_click_bg_texture, texture_blend_mode);
    SDL_ScaleMode texture_scale_mode = SDL_SCALEMODE_NEAREST;
    SDL_SetTextureScaleMode(text_bg_texture, texture_scale_mode);
    SDL_SetTextureScaleMode(textfield_bg_texture, texture_scale_mode);
    SDL_SetTextureScaleMode(button_bg_texture, texture_scale_mode);
    SDL_SetTextureScaleMode(button_hover_bg_texture, texture_scale_mode);
    SDL_SetTextureScaleMode(button_click_bg_texture, texture_scale_mode);
}

void Renderer::deinit() {
    SDL_DestroyTexture(text_bg_texture);
    SDL_DestroyTexture(textfield_bg_texture);
    SDL_DestroyTexture(button_bg_texture);
    SDL_DestroyTexture(button_hover_bg_texture);
    SDL_DestroyTexture(button_click_bg_texture);

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

    string s = "Text Field";
    bool f = true;
    /*
                        draw_color_text("Text"     , vec4(0  ,0,  300,100), (Text_CStyle)     { POSITION_CENTER, vec4(0,0,0,1), vec4(1,1,1,1)});
                        draw_color_textfield(&s, &f, vec4(0  ,200,300,100), (Textfield_CStyle){ POSITION_CENTER, vec4(0,0,0,1), vec4(1,1,1,1), vec4(0.5,0.5,0.5,1)});
    Button_CState bcs = draw_color_button("Button" , vec4(0  ,400,300,100), (Button_CStyle)   { POSITION_CENTER, vec4(0,0,0,1), vec4(1,1,1,1), vec4(0,0,0,1), vec4(0.5f,0.5f,0.5f,1),vec4(1,1,1,1), vec4(0,0,0,1), 0.1f});

                        draw_text("Text"           , vec4(400,0  ,300,100), (Text_Style)     { POSITION_CENTER, vec4(1,1,0,1), text_bg_texture});
                        draw_textfield(&s, &f      , vec4(400,200,300,100), (Textfield_Style){ POSITION_CENTER, vec4(1,1,0,1), textfield_bg_texture, vec4(0.5,0.5,0.5,1)});
    Button_State bs   = draw_button("Button"       , vec4(400,400,300,100), (Button_Style)   { POSITION_CENTER, vec4(1,1,0,1), button_bg_texture   , vec4(1,1,0,1), button_hover_bg_texture, vec4(1,1,0,1), button_click_bg_texture, 0.1f});
    */

    draw_color_textfield(&s, &f, vec4(0  ,0  ,100,100), (Textfield_CStyle){ POSITION_LT_CORNER, vec4(0,0,0,1), vec4(1,1,1,1), vec4(0.5,0.5,0.5,1)});
    draw_color_textfield(&s, &f, vec4(200,0  ,100,100), (Textfield_CStyle){ POSITION_LEFT     , vec4(0,0,0,1), vec4(1,1,1,1), vec4(0.5,0.5,0.5,1)});
    draw_color_textfield(&s, &f, vec4(400,0  ,100,100), (Textfield_CStyle){ POSITION_LB_CORNER, vec4(0,0,0,1), vec4(1,1,1,1), vec4(0.5,0.5,0.5,1)});
    draw_color_textfield(&s, &f, vec4(0  ,200,100,100), (Textfield_CStyle){ POSITION_TOP      , vec4(0,0,0,1), vec4(1,1,1,1), vec4(0.5,0.5,0.5,1)});
    draw_color_textfield(&s, &f, vec4(200,200,100,100), (Textfield_CStyle){ POSITION_CENTER   , vec4(0,0,0,1), vec4(1,1,1,1), vec4(0.5,0.5,0.5,1)});
    draw_color_textfield(&s, &f, vec4(400,200,100,100), (Textfield_CStyle){ POSITION_BOTTOM   , vec4(0,0,0,1), vec4(1,1,1,1), vec4(0.5,0.5,0.5,1)});
    draw_color_textfield(&s, &f, vec4(0  ,400,100,100), (Textfield_CStyle){ POSITION_RT_CORNER, vec4(0,0,0,1), vec4(1,1,1,1), vec4(0.5,0.5,0.5,1)});
    draw_color_textfield(&s, &f, vec4(200,400,100,100), (Textfield_CStyle){ POSITION_RIGHT    , vec4(0,0,0,1), vec4(1,1,1,1), vec4(0.5,0.5,0.5,1)});
    draw_color_textfield(&s, &f, vec4(400,400,100,100), (Textfield_CStyle){ POSITION_RB_CORNER, vec4(0,0,0,1), vec4(1,1,1,1), vec4(0.5,0.5,0.5,1)});

    SDL_RenderPresent(renderer);
}

void Renderer::draw_color_text(string str, vec4 bounds, Text_CStyle style){

    if(style.bg_color.a != 0){
        ASSERT_SDL(SDL_SetRenderDrawColorFloat(renderer, style.bg_color.r, style.bg_color.g, style.bg_color.b, style.bg_color.a));
        SDL_FRect bounds_rect = (SDL_FRect){bounds.x, bounds.y, bounds.z, bounds.w};
        ASSERT_SDL(SDL_RenderFillRect(renderer, &bounds_rect));
    }

    TTF_Text* text = TTF_CreateText(text_engine, font, str.c_str(), 0);
    ASSERT_SDL(text);

    ASSERT_SDL(TTF_SetTextColorFloat(text, style.text_color.r, style.text_color.g, style.text_color.b, style.text_color.a));
    // TODO: IDK: ASSERT_SDL(TTF_SetTextWrapWidth(text, text_bounds.z));

    vec4 text_bounds = bounds;

    ivec2 text_sizei = ivec2(0,0);
    ASSERT_SDL(TTF_GetTextSize(text, &text_sizei.x, &text_sizei.y));
    vec2 text_size = text_sizei;

    //TODO: wierd crisp font when position changed

    switch (style.position) {
        case POSITION_CENTER   :
        text_bounds = vec4(xy(bounds) + (zw(bounds) / 2.0f) - (text_size / 2.0f), zw(bounds));
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

    ASSERT_SDL(TTF_DrawRendererText(text, text_bounds.x, text_bounds.y));

    TTF_DestroyText(text);
}

void Renderer::draw_color_textfield(string* str, bool* focused, vec4 bounds, Textfield_CStyle style){

    draw_color_text(*str, bounds, (Text_CStyle){style.position, style.text_color, style.bg_color});
}

Button_CState Renderer::draw_color_button(string str, vec4 bounds, Button_CStyle style){

    SDL_FRect bounds_rect = {bounds.x,bounds.y,bounds.z,bounds.w};
    bool hover = SDL_PointInRectFloat((SDL_FPoint*)&mouse_pos, &bounds_rect);
    bool down = (SDL_BUTTON_LMASK & mouse_flags) && hover;

    unordered_map<vec4,Button_CState>::iterator iter = button_cstates.find(bounds);
    Button_CState* state;
    if(iter != button_cstates.end()){
        state = &(iter->second);
    }else{
        state = &button_cstates[bounds];
        state->base_bg_color = style.bg_color;
        state->target_bg_color = style.bg_color;
        state->base_text_color = style.text_color;
        state->target_text_color = style.text_color;
    }

    state->down_now = !state->down && down;
    state->up_now = state->down && !down;
    state->down = down;

    state->hover_now = !state->hover && hover;
    state->unhover_now = state->hover && !hover;
    state->hover = hover;

    if (state->hover_now) {
        state->gradient_time     = 0.0;
        state->target_bg_color   = style.hover_bg_color;
        state->target_text_color = style.hover_text_color;
        state->base_bg_color     = style.bg_color;
        state->base_text_color   = style.text_color;
    }else if (state->unhover_now){
        state->gradient_time     = 0.0;
        state->target_bg_color   = style.bg_color;
        state->target_text_color = style.text_color;
        state->base_bg_color     = style.hover_bg_color;
        state->base_text_color   = style.hover_text_color;
    }else if (state->down_now) {
        state->gradient_time     = 0.0;
        state->target_bg_color   = style.click_bg_color;
        state->target_text_color = style.click_text_color;
        state->base_bg_color     = style.hover_bg_color;
        state->base_text_color   = style.hover_text_color;
    }else if (state->up_now){
        state->gradient_time     = 0.0;
        state->target_bg_color   = style.hover_bg_color;
        state->target_text_color = style.hover_text_color;
        state->base_bg_color     = style.click_bg_color;
        state->base_text_color   = style.click_text_color;
    }

    draw_color_text(str, bounds, (Text_CStyle){
        style.position,
        lerp(state->base_text_color, state->target_text_color, (float)state->gradient_time),
        lerp(state->base_bg_color, state->target_bg_color, (float)state->gradient_time)
    });

    if(state->gradient_time < 1.0){
        state->gradient_time += delta_time / style.gradient_duration;
    }

    return *state;
}



void Renderer::draw_text(string str, vec4 bounds, Text_Style style){
    SDL_FRect dst_rect = (SDL_FRect){bounds.x,bounds.y,bounds.z,bounds.w};
    ASSERT_SDL(SDL_RenderTexture(renderer, style.bg_texture, NULL, &dst_rect));
    draw_color_text(str, bounds, (Text_CStyle){style.position, style.text_color, vec4(0,0,0,0)});
}

void Renderer::draw_textfield(string* str, bool* focused, vec4 bounds, Textfield_Style style){
    draw_text(*str, bounds, (Text_Style){style.position, style.text_color, style.bg_texture});
}

Button_State Renderer::draw_button(string str, vec4 bounds, Button_Style style){
    SDL_FRect bounds_rect = {bounds.x,bounds.y,bounds.z,bounds.w};
    bool hover = SDL_PointInRectFloat((SDL_FPoint*)&mouse_pos, &bounds_rect);
    bool down = (SDL_BUTTON_LMASK & mouse_flags) && hover;

    unordered_map<vec4,Button_State>::iterator iter = button_states.find(bounds);
    Button_State* state;
    if(iter != button_states.end()){
        state = &(iter->second);
    }else{
        state = &button_states[bounds];
        state->base_bg_texture = style.bg_texture;
        state->target_bg_texture = style.bg_texture;
        state->base_text_color = style.text_color;
        state->target_text_color = style.text_color;
    }

    state->down_now = !state->down && down;
    state->up_now = state->down && !down;
    state->down = down;

    state->hover_now = !state->hover && hover;
    state->unhover_now = state->hover && !hover;
    state->hover = hover;

    if (state->hover_now) {
        state->gradient_time     = 0.0;
        state->target_bg_texture = style.hover_bg_texture;
        state->target_text_color = style.hover_text_color;
        state->base_bg_texture   = style.bg_texture;
        state->base_text_color   = style.text_color;
    }else if (state->unhover_now){
        state->gradient_time     = 0.0;
        state->target_bg_texture = style.bg_texture;
        state->target_text_color = style.text_color;
        state->base_bg_texture   = style.hover_bg_texture;
        state->base_text_color   = style.hover_text_color;
    }else if (state->down_now) {
        state->gradient_time     = 0.0;
        state->target_bg_texture = style.click_bg_texture;
        state->target_text_color = style.click_text_color;
        state->base_bg_texture   = style.hover_bg_texture;
        state->base_text_color   = style.hover_text_color;
    }else if (state->up_now){
        state->gradient_time     = 0.0;
        state->target_bg_texture = style.hover_bg_texture;
        state->target_text_color = style.hover_text_color;
        state->base_bg_texture   = style.click_bg_texture;
        state->base_text_color   = style.click_text_color;
    }

    SDL_FRect dst_rect = (SDL_FRect){bounds.x,bounds.y,bounds.z,bounds.w};
    ASSERT_SDL(SDL_SetTextureAlphaModFloat(state->base_bg_texture, 1 - state->gradient_time));
    ASSERT_SDL(SDL_RenderTexture(renderer, state->base_bg_texture, NULL, &dst_rect));

    ASSERT_SDL(SDL_SetTextureAlphaModFloat(state->target_bg_texture, state->gradient_time));
    ASSERT_SDL(SDL_RenderTexture(renderer, state->target_bg_texture, NULL, &dst_rect));

    draw_color_text(str, bounds, (Text_CStyle){
        style.position,
        lerp(state->base_text_color, state->target_text_color, (float)state->gradient_time),
        vec4(0,0,0,0)
    });

    if(state->gradient_time < 1.0){
        state->gradient_time += delta_time / style.gradient_duration;
    }

    return *state;
}
