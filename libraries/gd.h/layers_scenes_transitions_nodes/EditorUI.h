#ifndef __EDITORUI_H__
#define __EDITORUI_H__

#include <gd.h>

class EditButtonBar;

namespace gd {
	class EditorUI : public cocos2d::CCLayer {
	public:
		EditButtonBar* m_pEditButtonBar;
		EditButtonBar* m_pEditButtonBar2;

		void updateZoom(float amt) {
			reinterpret_cast<void(__vectorcall*)(float, float, EditorUI*)>(base + 0x46b4e0)(0.f, amt, this);
		}
	};
}

#endif // !__EDITORUI_H__
