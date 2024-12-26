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
	};
}

#endif // !__EDITLEVELLAYER_H_
