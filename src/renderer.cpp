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

    SDL_FPoint fmouse;
    SDL_GetMouseState(&fmouse.x, &fmouse.y);
    SDL_Point mouse = {(int)fmouse.x, (int)fmouse.y};
    SDL_DisplayID display = SDL_GetDisplayForPoint(&mouse);
    SDL_SetWindowPosition(window, SDL_WINDOWPOS_UNDEFINED_DISPLAY(display),SDL_WINDOWPOS_UNDEFINED_DISPLAY(display));

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
    ASSERT_SDL(text_bg_texture);
    textfield_bg_texture    = IMG_LoadTexture(renderer, "textfield_bg.png");
    ASSERT_SDL(textfield_bg_texture);
    button_bg_texture       = IMG_LoadTexture(renderer, "button_bg.png");
    ASSERT_SDL(button_bg_texture);
    button_hover_bg_texture = IMG_LoadTexture(renderer, "button_bg_hover.png");
    ASSERT_SDL(button_hover_bg_texture);
    button_click_bg_texture = IMG_LoadTexture(renderer, "button_bg_click.png");
    ASSERT_SDL(button_click_bg_texture);
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
                        draw_color_text(     "Text", vec4(0,0,  300,100), (Text_CStyle)     { vec4(0,0,0,1), vec4(1,1,1,1)});
                        draw_color_textfield(&s, &f, vec4(0,200,300,100), (Textfield_CStyle){ vec4(0,0,0,1), vec4(1,1,1,1), vec4(0.5,0.5,0.5,1)});
    Button_CState bcs = draw_color_button( "Button", vec4(0,400,300,100), (Button_CStyle)   { vec4(0,0,0,1), vec4(1,1,1,1), vec4(0,0,0,1), vec4(0.5f,0.5f,0.5f,1),vec4(1,1,1,1), vec4(0,0,0,1), 0.1f});
    
                      draw_text(     "Text", vec4(400,0,  300,100), (Text_Style)     { vec4(0,0,0,1), text_bg_texture});
                      draw_textfield(&s, &f, vec4(400,200,300,100), (Textfield_Style){ vec4(0,0,0,1), textfield_bg_texture, vec4(0.5,0.5,0.5,1)});
    Button_State bs = draw_button( "Button", vec4(400,400,300,100), (Button_Style)   { vec4(0,0,0,1), button_bg_texture, vec4(1,0,0,1), button_hover_bg_texture, vec4(0,1,0,1), button_click_bg_texture, 0.1f});
    
    SDL_RenderPresent(renderer);
}

void Renderer::draw_color_text(std::string str, glm::vec4 bounds, Text_CStyle style){
    
    if(style.bg_color.w != 0){
        ASSERT_SDL(SDL_SetRenderDrawColorFloat(renderer, style.bg_color.x, style.bg_color.y, style.bg_color.z, style.bg_color.w));
        SDL_FRect bounds_rect = (SDL_FRect){bounds.x, bounds.y, bounds.z, bounds.w};
        ASSERT_SDL(SDL_RenderFillRect(renderer, &bounds_rect));
    }

    TTF_Text* text = TTF_CreateText(text_engine, font, str.c_str(), 0);
    ASSERT_SDL(text);

    ASSERT_SDL(TTF_SetTextColorFloat(text, style.text_color.x, style.text_color.y, style.text_color.z, style.text_color.w));
    ASSERT_SDL(TTF_SetTextWrapWidth(text, bounds.z));
    ASSERT_SDL(TTF_DrawRendererText(text, bounds.x, bounds.y));

    TTF_DestroyText(text);
}

void Renderer::draw_color_textfield(std::string* str, bool* focused, glm::vec4 bounds, Textfield_CStyle style){

    draw_color_text(*str, bounds, (Text_CStyle){style.text_color, style.bg_color});
}

Button_CState Renderer::draw_color_button(std::string str, glm::vec4 bounds, Button_CStyle style){

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
        lerp(state->base_text_color, state->target_text_color, (float)state->gradient_time),
        lerp(state->base_bg_color, state->target_bg_color, (float)state->gradient_time)
    });

    if(state->gradient_time < 1.0){
        state->gradient_time += delta_time / style.gradient_duration;
    }

    return *state;
}



void Renderer::draw_text(std::string str, glm::vec4 bounds, Text_Style style){
    SDL_FRect dst_rect = (SDL_FRect){bounds.x,bounds.y,bounds.z,bounds.w};
    ASSERT_SDL(SDL_RenderTexture(renderer, style.bg_texture, NULL, &dst_rect));
    draw_color_text(str, bounds, (Text_CStyle){style.text_color, vec4(0,0,0,0)});
}

void Renderer::draw_textfield(std::string* str, bool* focused, glm::vec4 bounds, Textfield_Style style){
    draw_text(*str, bounds, (Text_Style){style.text_color, style.bg_texture});
}

Button_State Renderer::draw_button(std::string str, glm::vec4 bounds, Button_Style style){
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

    // figure out blending ( base texture wierd? )

    SDL_FRect dst_rect = (SDL_FRect){bounds.x,bounds.y,bounds.z,bounds.w};
    //ASSERT_SDL(SDL_SetTextureAlphaMod(state->base_bg_texture, (Uint8)((1 - state->gradient_time) * 255)));
    ASSERT_SDL(SDL_RenderTexture(renderer, state->base_bg_texture, NULL, &dst_rect));

    //ASSERT_SDL(SDL_SetTextureAlphaMod(state->target_bg_texture, (Uint8)(state->gradient_time * 255)));
    //ASSERT_SDL(SDL_RenderTexture(renderer, state->target_bg_texture, NULL, &dst_rect));

    draw_color_text(str, bounds, (Text_CStyle){
        lerp(state->base_text_color, state->target_text_color, (float)state->gradient_time),
        vec4(0,0,0,0)
    });

    if(state->gradient_time < 1.0){
        state->gradient_time += delta_time / style.gradient_duration;
    }

    return *state;
}
