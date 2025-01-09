#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <cocos2d.h>
#include <MinHook.h>
#include "imgui-hook.hpp"
#include "speedhack.h"
#include <gd.h>

using std::uintptr_t;
#define Hook(orig, new, trampoline) MH_CreateHook(reinterpret_cast<void*>(orig), reinterpret_cast<void*>(&new), reinterpret_cast<void**>(&trampoline))
// pretty everything in cocos2d starts with CC, so there is a low chance of collisions
// it also makes using cocos a lot nicer imo
using namespace cocos2d;
