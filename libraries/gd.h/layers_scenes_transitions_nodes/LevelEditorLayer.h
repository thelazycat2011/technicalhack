#ifndef __LEVELEDITORLAYER_H__
#define __LEVELEDITORLAYER_H__

#include <gd.h>

namespace gd {
	class LevelEditorLayer : public cocos2d::CCLayer {
	public:
		cocos2d::CCLayer* gameLayer() {
			return from<cocos2d::CCLayer*>(this, 0x118);
		}

	};
}
#endif // !__LEVELEDITORLAYER_H__
