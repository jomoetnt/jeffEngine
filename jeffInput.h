#pragma once

#include <string>
#include <map>

namespace jeffNamespace
{
	class jeffInput
	{
	public:
		enum JEFF_KEY
		{
			// Arrow keys
			UP, DOWN, LEFT, RIGHT,
			// ???
			ESCAPE, TAB, WINDOWS, DEL, BACKSPACE,
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

		std::string handleKeyEvent(char keycode)
		{
			jeffInput::JEFF_KEY translatedKey = UNKNOWN;

			switch (keycode)
			{
			case 'a': translatedKey = jeffInput::A; break; case 'b': translatedKey = jeffInput::B; break; case 'c': translatedKey = jeffInput::C; break;
			case 'd': translatedKey = jeffInput::D; break; case 'e': translatedKey = jeffInput::E; break; case 'f': translatedKey = jeffInput::F; break;
			case 'g': translatedKey = jeffInput::G; break; case 'h': translatedKey = jeffInput::H; break; case 'i': translatedKey = jeffInput::I; break;
			case 'j': translatedKey = jeffInput::J; break; case 'k': translatedKey = jeffInput::K; break; case 'l': translatedKey = jeffInput::L; break;
			case 'm': translatedKey = jeffInput::M; break; case 'n': translatedKey = jeffInput::N; break; case 'o': translatedKey = jeffInput::O; break;
			case 'p': translatedKey = jeffInput::P; break; case 'q': translatedKey = jeffInput::Q; break; case 'r': translatedKey = jeffInput::R; break;
			case 's': translatedKey = jeffInput::S; break; case 't': translatedKey = jeffInput::T; break; case 'u': translatedKey = jeffInput::U; break;
			case 'v': translatedKey = jeffInput::V; break; case 'w': translatedKey = jeffInput::W; break; case 'x': translatedKey = jeffInput::X; break;
			case 'y': translatedKey = jeffInput::Y; break; case 'z': translatedKey = jeffInput::Z; break; case 'A': translatedKey = jeffInput::A; break;
			case 'B': translatedKey = jeffInput::B; break; case 'C': translatedKey = jeffInput::C; break; case 'D': translatedKey = jeffInput::D; break;
			case 'E': translatedKey = jeffInput::E; break; case 'F': translatedKey = jeffInput::F; break; case 'G': translatedKey = jeffInput::G; break;
			case 'H': translatedKey = jeffInput::H; break; case 'I': translatedKey = jeffInput::I; break; case 'J': translatedKey = jeffInput::J; break;
			case 'K': translatedKey = jeffInput::K; break; case 'L': translatedKey = jeffInput::L; break; case 'M': translatedKey = jeffInput::M; break;
			case 'N': translatedKey = jeffInput::N; break; case 'O': translatedKey = jeffInput::O; break; case 'P': translatedKey = jeffInput::P; break;
			case 'Q': translatedKey = jeffInput::Q; break; case 'R': translatedKey = jeffInput::R; break; case 'S': translatedKey = jeffInput::S; break;
			case 'T': translatedKey = jeffInput::T; break; case 'U': translatedKey = jeffInput::U; break; case 'V': translatedKey = jeffInput::V; break;
			case 'W': translatedKey = jeffInput::W; break; case 'X': translatedKey = jeffInput::X; break; case 'Y': translatedKey = jeffInput::Y; break;
			case 'Z': translatedKey = jeffInput::Z; break;
			}

			return handleKey(translatedKey);
		}

		std::string handleKey(JEFF_KEY key)
		{
			switch (key)
			{
			case W: return std::string("moveCube(true)");
			case S: return std::string("moveCube(false)");
			}
			return std::string("unknown");
		}
	};
}
