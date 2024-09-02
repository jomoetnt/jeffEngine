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
	case 0x5B: translatedKey = WINDOWS; break;
	case 0x2E: translatedKey = DEL; break;
	case 0x08: translatedKey = BACKSPACE; break;
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
	}

	return translatedKey;
}