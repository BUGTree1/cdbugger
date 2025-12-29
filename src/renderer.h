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
};

class UI_Element {
public:

    // Note: all drawing functions use:
    // - normalized coordinates (0,1)
    // - position (x,y) defined in bounds is at the left-top corner of the UI element
    Bounds bounds;

    virtual void render(Renderer* renderer) {};
};

class Text : public UI_Element {
public:
    Abstract_Position position;
    std::string       text;

    Color         fg;
    UI_Background bg;

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

    void init();
    void deinit();
    void render();
};

};

#endif // __RENDERER_H__
