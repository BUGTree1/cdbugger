#pragma once
#ifndef __UTILS_H__
#define __UTILS_H__

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <iomanip>

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>
#include <fontconfig/fontconfig.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/vec1.hpp>
#include <glm/gtx/compatibility.hpp>
#include <glm/gtx/hash.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/vec_swizzle.hpp>

#include "subprocess.h"

#define ASSERT_SDL(expr) if(!(expr)){ error(string(__FILE__) + string(":") + to_string(__LINE__) + string(": ") + string(SDL_GetError())); }
#define UTILS_ASSERT(expr) if(!(expr)){ error(string(__FILE__) + string(":") + to_string(__LINE__)); }
#define UNREACHABLE() error(string("UNREACHABLE: ") + string(__FILE__) + string(":") + to_string(__LINE__));

namespace CDBugger {

void error(std::string msg, int code = 1);
void warning(std::string msg);
void todo(std::string msg);

struct Color {
    float r, g, b, a;

    Color() = default;
    Color(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {};
    Color(glm::vec4 v) {
        r = v.x;
        g = v.y;
        b = v.z;
        a = v.w;
    };
    Color(SDL_FColor c) {
        r = c.r;
        g = c.g;
        b = c.b;
        a = c.a;
    };

    operator glm::vec4() const {
        return glm::vec4(r, g, b, a);
    }
    operator SDL_FColor() const {
        return (SDL_FColor){r, g, b, a};
    }
};
struct Bounds {
    float x, y, w, h;

    Bounds() = default;
    Bounds(float x, float y, float w, float h) : x(x), y(y), w(w), h(h) {};
    Bounds(glm::vec4 v) {
        x = v.x;
        y = v.y;
        w = v.z;
        h = v.w;
    };
    Bounds(SDL_FRect r) {
        x = r.x;
        y = r.y;
        w = r.w;
        h = r.h;
    };

    operator glm::vec4() const {
        return glm::vec4(x, y, w, h);
    }
    operator SDL_FRect() const {
        return (SDL_FRect){x, y, w, h};
    }
};

};

#endif // __UTILS_H__
