#include "jeffInput.h"

using namespace jeffNamespace;

JEFF_KEY jeffInput::handleKeyEvent(char keycode)
{
	JEFF_KEY translatedKey = UNKNOWN;

	switch (keycode)
	{
		// Arrow keys
	case 0x26: translatedKey = UP; break;
	case 0x28: translatedKey = DOWN; break;
	case 0x25: translatedKey = LEFT; break;
	case 0x27: translatedKey = RIGHT; break;
		// ???
	case 0x20: translatedKey = SPACEBAR; break;
	case 0x1B: translatedKey = ESCAPE; break;
	case 0x09: translatedKey = TAB; break;
	case 0x5B: translatedKey = WINDOWS; break; case 0x5C: translatedKey = WINDOWS; break;
	case 0x2E: translatedKey = DEL; break;
	case 0x08: translatedKey = BACKSPACE; break;
		// Modifiers
	case 0x10: translatedKey = LSHIFT; break;
	case 0x11: translatedKey = LCTRL; break;
	case 0x12: translatedKey = LALT; break;
		// Island
	case 0x21: translatedKey = PGUP; break; case 0x22: translatedKey = PGDOWN; break;
	case 0x23: translatedKey = END; break; case 0x24: translatedKey = HOME; break;
	case 0x2D: translatedKey = INSERT; break;
		// Symbols
	case 0xC0: translatedKey = TILDE; break; case 0xBD: translatedKey = MINUS; break; case 0xBB: translatedKey = EQUALS; break;
	case 0xDC: translatedKey = BSLASH; break; case 0xBF: translatedKey = FSLASH; break;
	case 0xDB: translatedKey = LBRACKET; break; case 0xDD: translatedKey = RBRACKET; break;
	case 0xBA: translatedKey = SEMICOLON; break; case 0xDE: translatedKey = APOSTROPHE; break;
	case 0xBC: translatedKey = COMMA; break; case 0xBE: translatedKey = PERIOD; break;
		// Digits
	case '0': translatedKey = ZERO; break; case '1': translatedKey = ONE; break; case '2': translatedKey = TWO; break;
	case '3': translatedKey = THREE; break; case '4': translatedKey = FOUR; break; case '5': translatedKey = FIVE; break;
	case '6': translatedKey = SIX; break; case '7': translatedKey = SEVEN; break; case '8': translatedKey = EIGHT; break;
	case '9': translatedKey = NINE; break;
		// Function keys
	case 0x70: translatedKey = F1; break; case 0x71: translatedKey = F2; break; case 0x72: translatedKey = F3; break; case 0x73: translatedKey = F4; break;
	case 0x74: translatedKey = F5; break; case 0x75: translatedKey = F6; break; case 0x76: translatedKey = F7; break; case 0x77: translatedKey = F8; break;
	case 0x78: translatedKey = F9; break; case 0x79: translatedKey = F10; break; case 0x7A: translatedKey = F11; break; case 0x7B: translatedKey = F12; break;
		// Letters
	case 'A': translatedKey = A; break; case 'B': translatedKey = B; break; case 'C': translatedKey = C; break; case 'D': translatedKey = D; break;
	case 'E': translatedKey = E; break; case 'F': translatedKey = F; break; case 'G': translatedKey = G; break; case 'H': translatedKey = H; break; 
	case 'I': translatedKey = I; break; case 'J': translatedKey = J; break; case 'K': translatedKey = K; break; case 'L': translatedKey = L; break; 
	case 'M': translatedKey = M; break; case 'N': translatedKey = N; break; case 'O': translatedKey = O; break; case 'P': translatedKey = P; break;
	case 'Q': translatedKey = Q; break; case 'R': translatedKey = R; break; case 'S': translatedKey = S; break; case 'T': translatedKey = T; break; 
	case 'U': translatedKey = U; break; case 'V': translatedKey = V; break; case 'W': translatedKey = W; break; case 'X': translatedKey = X; break; 
	case 'Y': translatedKey = Y; break; case 'Z': translatedKey = Z; break;
		// Mouse buttons
	case 0x01: translatedKey = LEFTCLICK; break; case 0x02: translatedKey = RIGHTCLICK; break; case 0x04: translatedKey = MIDDLECLICK; break;
	}

	return translatedKey;
}