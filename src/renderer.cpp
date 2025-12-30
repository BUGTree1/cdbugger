#include "renderer.h"

namespace CDBugger {

using namespace std;
using namespace glm;

bool UI_Background::has_texture(){
    return texture != NULL;
}
UI_Background::UI_Background(SDL_Texture* new_texture){
    texture = new_texture;
    color = Color();
}
UI_Background::UI_Background(Color new_color){
    texture = NULL;
    color = new_color;
}

Text* Text::init( Bounds new_bounds,
    Abstract_Position   new_position,
    std::string         new_text,
    UI_Background       new_bg,
    Color               new_fg
){
    Text* text_obj     = new Text();
    text_obj->bounds   = new_bounds;
    text_obj->position = new_position;
    text_obj->text     = new_text;
    text_obj->bg       = new_bg;
    text_obj->fg       = new_fg;
    text_obj->init();
    return text_obj;
};
void Text::init() {};
void Text::deinit() {
    if(bg.texture != NULL) {
        SDL_DestroyTexture(bg.texture);
    }
}

Button* Button::init(Bounds new_bounds,
    Abstract_Position       new_text_position,
    std::string             new_text,
    UI_Background           new_bg,
    UI_Background           new_hover_bg,
    UI_Background           new_click_bg,
    Color                   new_fg,
    Color                   new_hover_fg,
    Color                   new_click_fg,
    double                  new_gradient_duration,
    void*                   new_user_pointer,
    void (*new_hover_cb)    (Renderer* renderer, void* user_pointer),
    void (*new_unhover_cb)  (Renderer* renderer, void* user_pointer),
    void (*new_click_cb)    (Renderer* renderer, void* user_pointer),
    void (*new_up_cb)       (Renderer* renderer, void* user_pointer)
){
    Button* button            = new Button();
    button->bounds            = new_bounds;
    button->text_position     = new_text_position;
    button->text              = new_text;
    button->bg                = new_bg;
    button->hover_bg          = new_hover_bg;
    button->click_bg          = new_click_bg;
    button->fg                = new_fg;
    button->hover_fg          = new_hover_fg;
    button->click_fg          = new_click_fg;
    button->gradient_duration = new_gradient_duration;
    button->user_pointer      = new_user_pointer;
    button->hover_cb          = new_hover_cb;
    button->unhover_cb        = new_unhover_cb;
    button->click_cb          = new_click_cb;
    button->up_cb             = new_up_cb;
    button->init();
    return button;
}

void Button::init() {
    //set start gradient
    base   = {bg, fg};
    target = {bg, fg};
}

void Button::deinit() {
    if(bg.texture != NULL){
        SDL_DestroyTexture(bg.texture);
    }
    if(hover_bg.texture != NULL){
        SDL_DestroyTexture(hover_bg.texture);
    }
    if(click_bg.texture != NULL){
        SDL_DestroyTexture(click_bg.texture);
    }
    if(base.bg.texture != NULL){
        SDL_DestroyTexture(base.bg.texture);
    }
    if(target.bg.texture != NULL){
        SDL_DestroyTexture(target.bg.texture);
    }
}

SDL_Texture* Renderer::load_texture(string file) {
    const SDL_BlendMode texture_blend_mode = SDL_BLENDMODE_ADD;
    const SDL_ScaleMode texture_scale_mode = SDL_SCALEMODE_NEAREST;

    SDL_Texture* texture = IMG_LoadTexture(renderer, file.c_str());
    ASSERT_SDL(texture);
    SDL_SetTextureBlendMode(texture, texture_blend_mode);
    SDL_SetTextureScaleMode(texture, texture_scale_mode);
    return texture;
}

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

    elements.push_back(Button::init(
        Bounds(100,100,100,100),
        POSITION_CENTER,
        "Button",
        load_texture("button_bg.png"),
        load_texture("button_bg_hover.png"),
        load_texture("button_bg_click.png"),
        Color(1  ,1  ,1  ,1),
        Color(0.5,0.5,0.5,1),
        Color(0  ,0  ,0  ,1),
        0.1,
        NULL,
        [](Renderer*, void*){cout << "hover" << endl;},
        [](Renderer*, void*){cout << "unhover" << endl;},
        [](Renderer*, void*){cout << "click" << endl;},
        [](Renderer*, void*){cout << "up" << endl;}
    ));

    elements.push_back(Text::init(
        Bounds(300,100,100,100),
        POSITION_LT_CORNER,
        "Text",
        load_texture("text_bg.png"),
        Color(1  ,1  ,1  ,1)
    ));

    elements.push_back(Text::init(
        Bounds(500,100,100,100),
        POSITION_TOP,
        "Text",
        load_texture("text_bg.png"),
        Color(1  ,1  ,1  ,1)
    ));

    elements.push_back(Text::init(
        Bounds(700,100,100,100),
        POSITION_RT_CORNER,
        "Text",
        load_texture("text_bg.png"),
        Color(1  ,1  ,1  ,1)
    ));

    elements.push_back(Text::init(
        Bounds(300,300,100,100),
        POSITION_LEFT,
        "Text",
        load_texture("text_bg.png"),
        Color(1  ,1  ,1  ,1)
    ));

    elements.push_back(Text::init(
        Bounds(500,300,100,100),
        POSITION_CENTER,
        "Text",
        load_texture("text_bg.png"),
        Color(1  ,1  ,1  ,1)
    ));

    elements.push_back(Text::init(
        Bounds(700,300,100,100),
        POSITION_RIGHT,
        "Text",
        load_texture("text_bg.png"),
        Color(1  ,1  ,1  ,1)
    ));

    elements.push_back(Text::init(
        Bounds(300,500,100,100),
        POSITION_LB_CORNER,
        "Text",
        load_texture("text_bg.png"),
        Color(1  ,1  ,1  ,1)
    ));

    elements.push_back(Text::init(
        Bounds(500,500,100,100),
        POSITION_BOTTOM,
        "Text",
        load_texture("text_bg.png"),
        Color(1  ,1  ,1  ,1)
    ));

    elements.push_back(Text::init(
        Bounds(700,500,100,100),
        POSITION_RB_CORNER,
        "Text",
        load_texture("text_bg.png"),
        Color(1  ,1  ,1  ,1)
    ));
}

void Renderer::deinit() {
    for (int i = 0; i < elements.size(); i++) {
        elements[i]->deinit();
    }

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
    // TODO: - Should we ASSERT_SDL(TTF_SetTextWrapWidth(text, text_bounds.z)?

    ivec2 render_sizei = ivec2(0,0);
    ASSERT_SDL(TTF_GetTextSize(sdl_text, &render_sizei.x, &render_sizei.y));
    vec2 render_size = render_sizei;
    vec2 text_position = xy((vec4)bounds);
    vec2 text_size = zw((vec4)bounds);

    switch (position) {
        case POSITION_CENTER   :
        text_position += (text_size / 2.0f) - (render_size / 2.0f);
        break;
        case POSITION_TOP      :
        text_position += vec2((text_size.x / 2.0f) - (render_size.x / 2.0f), 0.0f);
        break;
        case POSITION_BOTTOM   :
        text_position += vec2((text_size.x / 2.0f) - (render_size.x / 2.0f), text_size.y - render_size.y);
        break;
        case POSITION_LEFT     :
        text_position += vec2(0.0f, (text_size.y / 2.0f) - (render_size.y / 2.0f));
        break;
        case POSITION_RIGHT    :
        text_position += vec2(text_size.x - render_size.x, (text_size.y / 2.0f) - (render_size.y / 2.0f));
        break;
        case POSITION_LT_CORNER: break;
        case POSITION_LB_CORNER:
        text_position += vec2(0.0f, text_size.y - render_size.y);
        break;
        case POSITION_RT_CORNER:
        text_position += vec2(text_size.x - render_size.x, 0.0f);
        break;
        case POSITION_RB_CORNER:
        text_position += vec2(text_size.x - render_size.x, text_size.y - render_size.y);
        break;
        default:
        UNREACHABLE();
        break;
    }

    Bounds text_bounds = Bounds(vec4(text_position, render_size));

    ASSERT_SDL(TTF_DrawRendererText(sdl_text, text_bounds.x, text_bounds.y));

    TTF_DestroyText(sdl_text);
}

void Button::render(Renderer* renderer){
    bool curr_hover = SDL_PointInRectFloat((SDL_FPoint*)&renderer->mouse_pos, (SDL_FRect*)&bounds);
    bool curr_down = (SDL_BUTTON_LMASK & renderer->mouse_flags) && curr_hover;

    bool hover_now   = !hover &&  curr_hover;
    bool unhover_now =  hover && !curr_hover;
    bool down_now    = !down  &&  curr_down;
    bool up_now      =  down  && !curr_down;

    down             = curr_down;
    hover            = curr_hover;

    if (down_now || up_now || hover_now || unhover_now) {
        gradient_time = 0.0;
    }

    if (up_now){
        target = {hover_bg, hover_fg};
        base   = {click_bg, click_fg};
        if(up_cb != NULL) up_cb(renderer, user_pointer);
    }
    if (hover_now) {
        target = {hover_bg, hover_fg};
        base   = {bg, fg};
        if(hover_cb != NULL) hover_cb(renderer, user_pointer);
    }
    if (down_now) {
        target = {click_bg, click_fg};
        base   = {hover_bg, hover_fg};
        if(click_cb != NULL) click_cb(renderer, user_pointer);
    }
    if (unhover_now){
        target = {bg, fg};
        base   = {hover_bg, hover_fg};
        if(unhover_cb != NULL) unhover_cb(renderer, user_pointer);
    }

    Text ui_text = Text();
    ui_text.bounds = bounds;
    ui_text.position = text_position;
    ui_text.text = text;
    ui_text.fg = lerp((vec4)base.fg, (vec4)target.fg, (float)gradient_time);
    if(base.bg.texture != NULL && target.bg.texture != NULL){
        ASSERT_SDL(SDL_SetTextureAlphaModFloat(base.bg.texture, 1 - gradient_time));
        ui_text.bg = UI_Background(base.bg.texture);
        ui_text.render(renderer);
        ASSERT_SDL(SDL_SetTextureAlphaModFloat(target.bg.texture, gradient_time));
        ui_text.bg = UI_Background(target.bg.texture);
    }else{
        ui_text.bg = UI_Background(lerp((vec4)base.bg.color, (vec4)target.bg.color, (float)gradient_time));
    }
    ui_text.render(renderer);

    if(gradient_time < 1.0){
        gradient_time += renderer->delta_time / gradient_duration;
    }
}

};
