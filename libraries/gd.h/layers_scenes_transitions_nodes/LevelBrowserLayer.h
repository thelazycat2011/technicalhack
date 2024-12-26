#ifndef __LEVELBROWSERLAYER_H_
#define __LEVELBROWSERLAYER_H__

#include <gd.h>

namespace gd {

	enum class SearchType {
		Search = 0,
		Downloaded = 1,
		MostLiked = 2,
		Trending = 3,
		Recent = 4,
		UsersLevels = 5,
		Featured = 6,
		Magic = 7,
		Sends = 8,
		MapPack = 9,
		MapPackOnClick = 10,
		Awarded = 11,
		Followed = 12,
		Friends = 13,
		Users = 14,
		LikedGDW = 15,
		HallOfFame = 16,
		FeaturedGDW = 17,
		Similar = 18,
		MyLevels = 98,
		SavedLevels = 99,
		FavouriteLevels = 100
	};


	class GJSearchObject : public cocos2d::CCNode {
	public:
		SearchType m_type;
	};

    class LevelBrowserLayer : public cocos2d::CCLayer {
    public:

    };
}
#endif // !__LEVELBROWSERLAYER_H_
