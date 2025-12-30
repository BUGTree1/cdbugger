#pragma once
#ifndef __RENDERER_H__
#define __RENDERER_H__

#include "utils.h"

namespace CDBugger {

class Renderer;

enum Abstract_Position {
    POSITION_CENTER   ,
    POSITION_TOP      ,
    POSITION_BOTTOM   ,
    POSITION_LEFT     ,
    POSITION_RIGHT    ,
    POSITION_LT_CORNER, // left-top corner
    POSITION_LB_CORNER, // left-bottom corner
    POSITION_RT_CORNER, // right-top corner
    POSITION_RB_CORNER, // right-bottom corner
};

struct UI_Background {
    // if texture is NULL use color
    SDL_Texture* texture;
    Color color;

    bool has_texture();

    UI_Background() = default;
    UI_Background(SDL_Texture* new_texture);
    UI_Background(Color new_color);
    operator SDL_Texture*() {
        return texture;
    }
    operator Color() {
        return color;
    }
};

class UI_Element {
public:

    // Note: all drawing functions use:
    // - normalized coordinates (0,1)
    // - position (x,y) defined in bounds is at the left-top corner of the UI element
    Bounds bounds;

    // init meant to be called after instantiating a child class with new keyword and setting the variables
    virtual void init() {};
    virtual void deinit() {};
    virtual void render(Renderer* renderer) {};
};

class Text : public UI_Element {
public:
    Abstract_Position position;
    std::string       text;

    Color         fg;
    UI_Background bg;

    static Text* init( Bounds new_bounds,
        Abstract_Position     new_position,
        std::string           new_text,
        UI_Background         new_bg,
        Color                 new_fg);
    void init() override;
    void deinit() override;
    void render(Renderer* renderer) override;
};

class Button : public UI_Element {
public:
    Abstract_Position text_position;
    std::string       text;

    UI_Background bg;
    UI_Background hover_bg;
    UI_Background click_bg;
    Color         fg;
    Color         hover_fg;
    Color         click_fg;
    double        gradient_duration;

    void* user_pointer;
    void (*hover_cb)  (Renderer* renderer, void* user_pointer);
    void (*unhover_cb)(Renderer* renderer, void* user_pointer);
    void (*click_cb)  (Renderer* renderer, void* user_pointer);
    void (*up_cb)     (Renderer* renderer, void* user_pointer);

    bool down;
    bool hover;
    double gradient_time;

    struct {
        UI_Background bg;
        Color fg;
    } base;
    struct {
        UI_Background bg;
        Color fg;
    } target;

    static Button* init( Bounds new_bounds,
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
        void (*new_up_cb)       (Renderer* renderer, void* user_pointer));
    void init() override;
    void deinit() override;
    void render(Renderer* renderer) override;
};

class Renderer {
public:
    SDL_Window* window;
    SDL_Renderer* renderer;
    TTF_Font* font;
    TTF_TextEngine* text_engine;
    FcConfig* fc_config;

    int width;
    int height;

    double FPS;
    double delta_time;
    double time;

    glm::vec2 mouse_pos;
    SDL_MouseButtonFlags mouse_flags;

    std::vector<UI_Element*> elements;

    SDL_Texture* load_texture(std::string file);

    void init();
    void deinit();
    void render();
};

};

#endif // __RENDERER_H__
