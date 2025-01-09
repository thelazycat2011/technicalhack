#pragma once
#ifndef GJGAMELEVEL_H
#define GJGAMELEVEL_H

#include <gd.h>

namespace gd {

enum class GJLevelType {
    Local = 1,
    Editor = 2,
    Saved = 3
};
// This class is missing a lot of member function. If you happen to reverse-engineer any of it (for 2.0), make a pull request and (if you want to) contact me through Discord
// Discord : Thereallazycat
class GJGameLevel : public cocos2d::CCNode {
public:
    PAD(0xEF);
    gd::string m_levelName;
    gd::string m_levelDesc;
    gd::string m_levelString;
    int m_levelID; //0xEC
    int m_songID;
    int m_audioTrack;
    int m_Jumpcount; //0x1D8
    int m_attempts; //0x1D4

    auto levelID() {
        return from<int>(this, 0xEC);
    }

    std::string getAudioFileName() {
        std::string ret;
        reinterpret_cast<void(__thiscall*)(
            GJGameLevel*, std::string*
        )>(
            base + 0x116ad0
        )(
            this, &ret
        );
        return ret;
    }
    void setLevelData(const char* data) {
        auto len = strlen(data);
        auto addedLvlStr = reinterpret_cast<uintptr_t>(this) + 0x12c;
        *reinterpret_cast<size_t*>(addedLvlStr + 16) = len;   // length
        *reinterpret_cast<size_t*>(addedLvlStr + 20) = max(len, 15); // capacity
        if (len <= 15)
            memcpy(reinterpret_cast<char*>(addedLvlStr), data, len + 1);
        else {
            void* newb = malloc(len + 1);
            memcpy(newb, data, len + 1);
            *reinterpret_cast<void**>(addedLvlStr) = newb;
        }
    }
    void setLevelData(std::string const& data) {
        this->setLevelData(data.c_str());
    }
    
};


}

#endif
