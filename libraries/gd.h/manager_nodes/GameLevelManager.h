#ifndef __GAMELEVELMANAGER_H__
#define __GAMELEVELMANAGER_H__

#include <gd.h>

namespace gd {

class GJGameLevel;

class GameLevelManager : public cocos2d::CCNode {
    protected:

    public:
        static auto sharedState() {
            return reinterpret_cast<GameLevelManager * (__stdcall*)()>(base + 0x7e830)();
        }

        GJGameLevel* createNewLevel() {
            return reinterpret_cast<GJGameLevel * (__thiscall*)(GameLevelManager*)>(base + 0x7fa40)(this);
        }

        cocos2d::CCArray* getSavedLevels() {
            return reinterpret_cast<cocos2d::CCArray*(__thiscall*)(GameLevelManager*)>( gd::base + 0x57640)(this); //1.91 56f80
        }

        virtual void levelUpdate(GJGameLevel* level) {
            return reinterpret_cast<void(__fastcall*)(GJGameLevel*)>(gd::base + 0x5b530)(level); //1.91 5ae70
        }
};

}

#endif
