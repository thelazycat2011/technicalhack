#ifndef __PLAYLAYER_H__
#define __PLAYLAYER_H__

#include <gd.h>

namespace gd {

	class GJBaseGameLayer;

	class PlayLayer : public GJBaseGameLayer {
		public:
			void resetLevel() {
				return reinterpret_cast<void(__thiscall*)(PlayLayer*)>(0x57C930)(this);
			}
	};
}

#endif