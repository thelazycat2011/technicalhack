#ifndef __EDITLEVELLAYER_H__
#define __EDITLEVELLAYER_H__

#include <gd.h>

namespace gd {
	class GJGameLevel;

	class EditLevelLayer : public cocos2d::CCLayer {
	public:
		GJGameLevel* level() {
			return from<GJGameLevel*>(this, 0x12c);
		}
		static cocos2d::CCScene* scene(GJGameLevel* level) {
			return reinterpret_cast<cocos2d::CCScene*(__fastcall*)(GJGameLevel*)>(base + 0x56EC0)(level);
		}
	};
}

#endif // !__EDITLEVELLAYER_H_
