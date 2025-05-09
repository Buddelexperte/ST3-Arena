#pragma once

#include "Functions.h"
#include <unordered_map>
#include <vector>

class IWidgetAnimation
{
protected:
	enum EAnimation
	{
		NO_ANIMATION = -1,
		OPEN_ANIM,
		OPEN_SUB_ANIM,
		CLOSE_ANIM,
		IDLE_ANIM,
		ON_HOVER,
		ON_UNHOVER,
		ON_CLICK
	};

	virtual void tick_anim(const float& deltaTime)
	{
		for (const auto& [anim, playing] : animationMap)
		{
			if (!playing) continue;

			switch (anim)
			{
			case OPEN_ANIM:			tick_openAnim(deltaTime); break;
			case OPEN_SUB_ANIM:		tick_openSubAnim(deltaTime); break;
			case CLOSE_ANIM:		tick_closeAnim(deltaTime); break;
			case IDLE_ANIM:			tick_idleAnim(deltaTime); break;
			case ON_HOVER:			tick_onHoverAnim(deltaTime); break;
			case ON_UNHOVER:		tick_onUnhoverAnim(deltaTime); break;
			case ON_CLICK:			tick_onClickAnim(deltaTime); break;
			default:				break;
			}
		}
	}

private:
	bool bImportantAnim = false;
	std::unordered_map<EAnimation, bool> animationMap = { };

	// START methods
	virtual void start_openAnim() { stopAnim(OPEN_ANIM); }
	virtual void start_openSubAnim() {}
	virtual void start_closeAnim() {}
	virtual void start_idleAnim() {}
	virtual void start_onHoverAnim() {}
	virtual void start_onUnhoverAnim() {}
	virtual void start_onClickAnim() {}

	// TICK methods
	virtual void tick_openAnim(const float&) {}
	virtual void tick_openSubAnim(const float&) {}
	virtual void tick_closeAnim(const float&) {}
	virtual void tick_idleAnim(const float&) {}
	virtual void tick_onHoverAnim(const float&) {}
	virtual void tick_onUnhoverAnim(const float&) {}
	virtual void tick_onClickAnim(const float&) {}

public:
	virtual void playAnim(const EAnimation& anim)
	{
		bImportantAnim = false;

		animationMap[anim] = true;

		switch (anim)
		{
		case OPEN_ANIM:
			bImportantAnim = true;
			start_openAnim();
			break;
		case OPEN_SUB_ANIM:
			start_openSubAnim();
			break;
		case CLOSE_ANIM:
			bImportantAnim = true;
			start_closeAnim();
			break;
		case IDLE_ANIM:
			start_idleAnim();
			break;
		case ON_HOVER:
			start_onHoverAnim();
			break;
		case ON_UNHOVER:
			start_onUnhoverAnim();
			break;
		case ON_CLICK:
			start_onClickAnim();
			break;
		case NO_ANIMATION:
		default:
			clearAnims();
			break;
		}
	}

	void stopAnim(const EAnimation& anim)
	{
		animationMap[anim] = false;
		// Reset the blocking of input if important animation was ended
		if (anim == OPEN_ANIM || anim == CLOSE_ANIM)
			bImportantAnim = false;
	}

	void clearAnims()
	{
		bImportantAnim = false;
		for (auto& [anim, playing] : animationMap)
		{
			playing = false;
		}
	}

	virtual bool isAnimPlaying() const
	{
		for (const auto& [anim, playing] : animationMap)
		{
			if (playing) return true;
		}

		return false;
	}

	virtual bool isAnimPlaying(const EAnimation anim) const
	{
		auto it = animationMap.find(anim);
		return it != animationMap.end() && it->second;
	}

	virtual bool isAnimBlockingInput() const
	{
		return bImportantAnim && isAnimPlaying();
	}

	std::vector<EAnimation> getPlayingAnims() const
	{
		std::vector<EAnimation> result;
		for (const auto& [anim, playing] : animationMap)
		{
			if (playing) result.push_back(anim);
		}
		return result;
	}
};
