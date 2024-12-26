#ifndef __BUTTONSPRITE_H__
#define __BUTTONSPRITE_H__

#include <gd.h>

namespace gd {
	#pragma runtime_checks("s", off)
	class ButtonSprite : public cocos2d::CCSprite {
	public:
		/*PARAMS:
		* caption - the button's caption.
		* width - the width of the button. only used if absolute is TRUE.
		* absolute - whether or not to use provided width. if FALSE, game will auto-adjust.
		* font - font file for caption.
		* texture - texture file for button background.
		* height - height of button. put 0 for auto.
		* scale - scale of the caption.
		*/
		static auto create(const char* label, int idk, int width, float scale, bool absolute, const char* font, const char* sprite, float height) {
			auto ret = reinterpret_cast<ButtonSprite * (__vectorcall*)(
				float, float, float, float, float, float,
				const char*, int,
				int, bool, const char*, const char*, float
				)>(base + 0xffa0)(0.f, 0.f, 0.f, scale, 0.f, 0.f, label, idk, width, absolute, font, sprite, height);
			__asm add esp, 20;
			return ret;
		}
	};
	#pragma runtime_checks("s", restore)
}

#endif