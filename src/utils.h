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

#include "subprocess.h"

#define ASSERT_SDL(expr) if(!(expr)){ error(string(__FILE__) + string(":") + to_string(__LINE__) + string(": ") + string(SDL_GetError())); }
#define UTILS_ASSERT(expr) if(!(expr)){ error(string(__FILE__) + string(":") + to_string(__LINE__)); }

void error(std::string msg, int code = 1);
void warning(std::string msg);
void todo(std::string msg);

#endif // __UTILS_H__
