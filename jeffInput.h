#pragma once

#include <string>
#include <map>

namespace jeffNamespace
{
	enum JEFF_KEY
	{
		// Arrow keys
		UP, DOWN, LEFT, RIGHT,
		// ???
		ESCAPE, TAB, WINDOWS, DEL, BACKSPACE, SPACEBAR,
		// Modifiers
		LCTRL, RCTRL, LALT, RALT, LSHIFT, RSHIFT, FN,
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
		// Error type
		UNKNOWN
	};

	class jeffInput
	{
	public:
		JEFF_KEY handleKeyEvent(char keycode);


	};
}
