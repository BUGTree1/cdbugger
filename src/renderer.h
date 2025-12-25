#pragma once
#ifndef __RENDERER_H__
#define __RENDERER_H__

#include "utils.h"

struct Text_CStyle {
    glm::vec4 text_color;
    glm::vec4 bg_color;
};

struct Text_Style {
    glm::vec4 text_color;
    SDL_Texture* bg_texture;
};

struct Textfield_CStyle {
    glm::vec4 text_color;
    glm::vec4 bg_color;
    glm::vec4 empty_text_color;
};

struct Textfield_Style {
    glm::vec4 text_color;
    SDL_Texture* bg_texture;
    glm::vec4 empty_text_color;
};

struct Button_CStyle {
    glm::vec4 text_color;
    glm::vec4 bg_color;
    glm::vec4 hover_text_color;
    glm::vec4 hover_bg_color;
    glm::vec4 click_text_color;
    glm::vec4 click_bg_color;
    float gradient_duration;
};

struct Button_Style {
    glm::vec4 text_color;
    SDL_Texture* bg_texture;
    glm::vec4 hover_text_color;
    SDL_Texture* hover_bg_texture;
    glm::vec4 click_text_color;
    SDL_Texture* click_bg_texture;
    float gradient_duration;
};

struct Button_CState {
    bool down; // is currently down/pressed
    bool down_now; // is pressed this frame (wasnt the frame before)
    bool up_now; // is released this frame (was down the frame before)
    bool hover;
    bool hover_now;
    bool unhover_now;

    double gradient_time;
    glm::vec4 base_text_color;
    glm::vec4 target_text_color;
    glm::vec4 base_bg_color;
    glm::vec4 target_bg_color;
};

struct Button_State {
    bool down; // is currently down/pressed
    bool down_now; // is pressed this frame (wasnt the frame before)
    bool up_now; // is released this frame (was down the frame before)
    bool hover;
    bool hover_now;
    bool unhover_now;

    double gradient_time;
    glm::vec4 base_text_color;
    glm::vec4 target_text_color;
    SDL_Texture* base_bg_texture;
    SDL_Texture* target_bg_texture;
};

class Renderer {
public:
    SDL_Window* window;
    SDL_Renderer* renderer;
    int width;
    int height;

    TTF_Font* font;
    TTF_TextEngine* text_engine;

    FcConfig* fc_config;

    double FPS;
    double delta_time;
    double time;

    glm::vec2 mouse_pos;
    SDL_MouseButtonFlags mouse_flags;

    std::unordered_map<glm::vec4, Button_State> button_states;
    std::unordered_map<glm::vec4, Button_CState> button_cstates;

    SDL_Texture* text_bg_texture;
    SDL_Texture* textfield_bg_texture;
    SDL_Texture* button_bg_texture;
    SDL_Texture* button_hover_bg_texture;
    SDL_Texture* button_click_bg_texture;

    void init();
    void deinit();
    void render();

    // Note: all drawing functions use normalized coordinates (-1,1) and bounds is (x,y,width,height) and colors are (r,g,b,a)
    void draw_color_text(std::string str, glm::vec4 bounds, Text_CStyle style);
    void draw_color_textfield(std::string* str, bool* focused, glm::vec4 bounds, Textfield_CStyle style);
    Button_CState draw_color_button(std::string str, glm::vec4 bounds, Button_CStyle style);

    void draw_text(std::string str, glm::vec4 bounds, Text_Style style);
    void draw_textfield(std::string* str, bool* focused, glm::vec4 bounds, Textfield_Style style);
    Button_State draw_button(std::string str, glm::vec4 bounds, Button_Style style);
};

#endif // __RENDERER_H__
