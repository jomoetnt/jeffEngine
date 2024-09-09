#pragma once

#include <string>

namespace jeffNamespace
{
	enum JEFF_KEY
	{
		// Arrow keys
		UP, DOWN, LEFT, RIGHT,
		// ???
		ESCAPE, TAB, WINDOWS, DEL, BACKSPACE, SPACEBAR, ENTER, CAPSLOCK,
		// Modifiers
		LCTRL, RCTRL, LALT, RALT, LSHIFT, RSHIFT,
		// Island
		INSERT, HOME, PGUP, PGDOWN, END,
		// Symbols
		TILDE, MINUS, EQUALS, BSLASH, FSLASH, LBRACKET, RBRACKET, SEMICOLON, APOSTROPHE, COMMA, PERIOD,
		// Digits
		ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, ZERO,
		// Function keys
		F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
		// Letters
		Q, W, E, R, T, Y, U, I, O, P, A, S, D, F, G, H, J, K, L, Z, X, C, V, B, N, M,
		// Mouse buttons
		LEFTCLICK, RIGHTCLICK, MIDDLECLICK,
		// Error type
		UNKNOWN
	};

	struct jeffVec { float x, y; };

	class jeffInput
	{
	public:
		JEFF_KEY handleKeyEvent(char keycode);

		static void makeInstance() { instance = new jeffInput(); }
		static jeffInput* getInstance() { return instance; }
		static void destroyInstance() { delete instance; }

	private:
		jeffInput() = default;
		~jeffInput() = default;
		static inline jeffInput* instance = nullptr;

	};
}
