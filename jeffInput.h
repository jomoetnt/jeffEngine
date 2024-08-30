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
		JEFF_KEY handleKeyEvent(char keycode)
		{
			JEFF_KEY translatedKey = UNKNOWN;

   			switch (keycode)
			{
			// Letters
			case 'a': translatedKey = A; break; case 'b': translatedKey = B; break; case 'c': translatedKey = C; break;
			case 'd': translatedKey = D; break; case 'e': translatedKey = E; break; case 'f': translatedKey = F; break;
			case 'g': translatedKey = G; break; case 'h': translatedKey = H; break; case 'i': translatedKey = I; break;
			case 'j': translatedKey = J; break; case 'k': translatedKey = K; break; case 'l': translatedKey = L; break;
			case 'm': translatedKey = M; break; case 'n': translatedKey = N; break; case 'o': translatedKey = O; break;
			case 'p': translatedKey = P; break; case 'q': translatedKey = Q; break; case 'r': translatedKey = R; break;
			case 's': translatedKey = S; break; case 't': translatedKey = T; break; case 'u': translatedKey = U; break;
			case 'v': translatedKey = V; break; case 'w': translatedKey = W; break; case 'x': translatedKey = X; break;
			case 'y': translatedKey = Y; break; case 'z': translatedKey = Z; break; case 'A': translatedKey = A; break;
			case 'B': translatedKey = B; break; case 'C': translatedKey = C; break; case 'D': translatedKey = D; break;
			case 'E': translatedKey = E; break; case 'F': translatedKey = F; break; case 'G': translatedKey = G; break;
			case 'H': translatedKey = H; break; case 'I': translatedKey = I; break; case 'J': translatedKey = J; break;
			case 'K': translatedKey = K; break; case 'L': translatedKey = L; break; case 'M': translatedKey = M; break;
			case 'N': translatedKey = N; break; case 'O': translatedKey = O; break; case 'P': translatedKey = P; break;
			case 'Q': translatedKey = Q; break; case 'R': translatedKey = R; break; case 'S': translatedKey = S; break;
			case 'T': translatedKey = T; break; case 'U': translatedKey = U; break; case 'V': translatedKey = V; break;
			case 'W': translatedKey = W; break; case 'X': translatedKey = X; break; case 'Y': translatedKey = Y; break;
			case 'Z': translatedKey = Z; break; 
			// ???
			//case VK_SPACE: translatedKey = SPACEBAR; break; case VK_ESCAPE: translatedKey = ESCAPE; break;

			}

			return translatedKey;
		}
	};
}
