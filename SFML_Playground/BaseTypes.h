#pragma once

constexpr float LERP_SMOOTHNESS = 0.01f;

enum InfoType {
	EMPTY_INFO	= 0,		// No info
	POSITION	= 1 << 0,	// 00001: Position
	SIZE		= 1 << 1,	// 00010: Size
	VELOCITY	= 1 << 2,	// 00100: Velocity
	COLOR		= 1 << 3,	// 01000: Color
	ROTATION	= 1 << 4	// 10000: Rotation
};

// Overload bitwise operators for InfoType if necessary:
inline InfoType operator|(InfoType a, InfoType b) {
	return static_cast<InfoType>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
}
inline InfoType& operator|=(InfoType& a, InfoType b) {
	a = a | b;
	return a;
}

// Enum for handling gameStages in GameInstance
enum GameState {
	QUIT = -1, // Not yet started or game has been interrupted
	MENU_SCREEN = 0, // A Menu with clickable buttons
	UNPAUSED, // From pause to not paused
	GAME_PAUSED, // Freeze gamepay and show PauseMenu
	GAME_OVER, // Game ended somehow, freezes and show GameOverMenu
	GAME_LAUNCHING, // gameLoop should start and execute init functionality
	IN_GAME // gameLoop should start
};