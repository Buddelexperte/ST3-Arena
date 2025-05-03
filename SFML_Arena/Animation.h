#pragma once

class IWidgetAnimation
{
protected:
	enum EAnimation
	{
		NO_ANIMATION = -1,
		OPEN_ANIM,
		OPEN_SUB_ANIM,
		CLOSE_ANIM,
		IDLE_ANIM
	};

	virtual void tick_anim(const float& deltaTime)
	{
		if (!IWidgetAnimation::isAnimPlaying())
			return;

		switch (IWidgetAnimation::getPlayingAnim())
		{
		case OPEN_ANIM:
			tick_openAnim(deltaTime);
			break;
		case OPEN_SUB_ANIM:
			tick_openSubAnim(deltaTime);
			break;
		case CLOSE_ANIM:
			tick_closeAnim(deltaTime);
			break;
		case IDLE_ANIM:
			tick_idleAnim(deltaTime);
			break;
		case NO_ANIMATION:
		default:
			IWidgetAnimation::stopAnim();
			break;
		}
	}

private:
	bool bIsPlaying = false;
	EAnimation animationPlaying = NO_ANIMATION;

	// Different Widget Animations
	virtual void start_openAnim() { stopAnim(); };
	virtual void start_openSubAnim() {};
	virtual void start_closeAnim() {};
	virtual void start_idleAnim() {};

	virtual void tick_openAnim(const float&) {};
	virtual void tick_openSubAnim(const float&) {};
	virtual void tick_closeAnim(const float&) {};
	virtual void tick_idleAnim(const float&) {};

public:
	void playAnim(const EAnimation& anim)
	{
		bIsPlaying = true;
		animationPlaying = anim;

		switch (IWidgetAnimation::getPlayingAnim())
		{
		case OPEN_ANIM:
			start_openAnim();
			break;
		case OPEN_SUB_ANIM:
			start_openSubAnim();
			break;
		case CLOSE_ANIM:
			start_closeAnim();
			break;
		case IDLE_ANIM:
			start_idleAnim();
			break;
		case NO_ANIMATION:
		default:
			IWidgetAnimation::stopAnim();
			break;
		}
	}
	void stopAnim()
	{
		bIsPlaying = false;
	}
	void clearAnimation()
	{
		stopAnim();
		animationPlaying = NO_ANIMATION;
	}
	virtual bool isAnimPlaying() const
	{
		return bIsPlaying;
	}
	EAnimation getPlayingAnim() const
	{
		return animationPlaying;
	}
};