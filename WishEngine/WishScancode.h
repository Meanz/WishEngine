#pragma once

/*
A pure copy of SDL Scancodes
*/
typedef enum
{
	WISH_SCANCODE_UNKNOWN = 0,

	/**
	*  \name Usage page 0x07
	*
	*  These values are from usage page 0x07 (USB keyboard page).
	*/
	/* @{ */

	WISH_SCANCODE_A = 4,
	WISH_SCANCODE_B = 5,
	WISH_SCANCODE_C = 6,
	WISH_SCANCODE_D = 7,
	WISH_SCANCODE_E = 8,
	WISH_SCANCODE_F = 9,
	WISH_SCANCODE_G = 10,
	WISH_SCANCODE_H = 11,
	WISH_SCANCODE_I = 12,
	WISH_SCANCODE_J = 13,
	WISH_SCANCODE_K = 14,
	WISH_SCANCODE_L = 15,
	WISH_SCANCODE_M = 16,
	WISH_SCANCODE_N = 17,
	WISH_SCANCODE_O = 18,
	WISH_SCANCODE_P = 19,
	WISH_SCANCODE_Q = 20,
	WISH_SCANCODE_R = 21,
	WISH_SCANCODE_S = 22,
	WISH_SCANCODE_T = 23,
	WISH_SCANCODE_U = 24,
	WISH_SCANCODE_V = 25,
	WISH_SCANCODE_W = 26,
	WISH_SCANCODE_X = 27,
	WISH_SCANCODE_Y = 28,
	WISH_SCANCODE_Z = 29,

	WISH_SCANCODE_1 = 30,
	WISH_SCANCODE_2 = 31,
	WISH_SCANCODE_3 = 32,
	WISH_SCANCODE_4 = 33,
	WISH_SCANCODE_5 = 34,
	WISH_SCANCODE_6 = 35,
	WISH_SCANCODE_7 = 36,
	WISH_SCANCODE_8 = 37,
	WISH_SCANCODE_9 = 38,
	WISH_SCANCODE_0 = 39,

	WISH_SCANCODE_RETURN = 40,
	WISH_SCANCODE_ESCAPE = 41,
	WISH_SCANCODE_BACKSPACE = 42,
	WISH_SCANCODE_TAB = 43,
	WISH_SCANCODE_SPACE = 44,

	WISH_SCANCODE_MINUS = 45,
	WISH_SCANCODE_EQUALS = 46,
	WISH_SCANCODE_LEFTBRACKET = 47,
	WISH_SCANCODE_RIGHTBRACKET = 48,
	WISH_SCANCODE_BACKSLASH = 49, /**< Located at the lower left of the return
								 *   key on ISO keyboards and at the right end
								 *   of the QWERTY row on ANSI keyboards.
								 *   Produces REVERSE SOLIDUS (backslash) and
								 *   VERTICAL LINE in a US layout, REVERSE
								 *   SOLIDUS and VERTICAL LINE in a UK Mac
								 *   layout, NUMBER SIGN and TILDE in a UK
								 *   Windows layout, DOLLAR SIGN and POUND SIGN
								 *   in a Swiss German layout, NUMBER SIGN and
								 *   APOSTROPHE in a German layout, GRAVE
								 *   ACCENT and POUND SIGN in a French Mac
								 *   layout, and ASTERISK and MICRO SIGN in a
								 *   French Windows layout.
								 */
								 WISH_SCANCODE_NONUSHASH = 50, /**< ISO USB keyboards actually use this code
															  *   instead of 49 for the same key, but all
															  *   OSes I've seen treat the two codes
															  *   identically. So, as an implementor, unless
															  *   your keyboard generates both of those
															  *   codes and your OS treats them differently,
															  *   you should generate WISH_SCANCODE_BACKSLASH
															  *   instead of this code. As a user, you
															  *   should not rely on this code because SDL
															  *   will never generate it with most (all?)
															  *   keyboards.
															  */
															  WISH_SCANCODE_SEMICOLON = 51,
															  WISH_SCANCODE_APOSTROPHE = 52,
															  WISH_SCANCODE_GRAVE = 53, /**< Located in the top left corner (on both ANSI
																					   *   and ISO keyboards). Produces GRAVE ACCENT and
																					   *   TILDE in a US Windows layout and in US and UK
																					   *   Mac layouts on ANSI keyboards, GRAVE ACCENT
																					   *   and NOT SIGN in a UK Windows layout, SECTION
																					   *   SIGN and PLUS-MINUS SIGN in US and UK Mac
																					   *   layouts on ISO keyboards, SECTION SIGN and
																					   *   DEGREE SIGN in a Swiss German layout (Mac:
																					   *   only on ISO keyboards), CIRCUMFLEX ACCENT and
																					   *   DEGREE SIGN in a German layout (Mac: only on
																					   *   ISO keyboards), SUPERSCRIPT TWO and TILDE in a
																					   *   French Windows layout, COMMERCIAL AT and
																					   *   NUMBER SIGN in a French Mac layout on ISO
																					   *   keyboards, and LESS-THAN SIGN and GREATER-THAN
																					   *   SIGN in a Swiss German, German, or French Mac
																					   *   layout on ANSI keyboards.
																					   */
																					   WISH_SCANCODE_COMMA = 54,
																					   WISH_SCANCODE_PERIOD = 55,
																					   WISH_SCANCODE_SLASH = 56,

																					   WISH_SCANCODE_CAPSLOCK = 57,

																					   WISH_SCANCODE_F1 = 58,
																					   WISH_SCANCODE_F2 = 59,
																					   WISH_SCANCODE_F3 = 60,
																					   WISH_SCANCODE_F4 = 61,
																					   WISH_SCANCODE_F5 = 62,
																					   WISH_SCANCODE_F6 = 63,
																					   WISH_SCANCODE_F7 = 64,
																					   WISH_SCANCODE_F8 = 65,
																					   WISH_SCANCODE_F9 = 66,
																					   WISH_SCANCODE_F10 = 67,
																					   WISH_SCANCODE_F11 = 68,
																					   WISH_SCANCODE_F12 = 69,

																					   WISH_SCANCODE_PRINTSCREEN = 70,
																					   WISH_SCANCODE_SCROLLLOCK = 71,
																					   WISH_SCANCODE_PAUSE = 72,
																					   WISH_SCANCODE_INSERT = 73, /**< insert on PC, help on some Mac keyboards (but
																												 does send code 73, not 117) */
																												 WISH_SCANCODE_HOME = 74,
																												 WISH_SCANCODE_PAGEUP = 75,
																												 WISH_SCANCODE_DELETE = 76,
																												 WISH_SCANCODE_END = 77,
																												 WISH_SCANCODE_PAGEDOWN = 78,
																												 WISH_SCANCODE_RIGHT = 79,
																												 WISH_SCANCODE_LEFT = 80,
																												 WISH_SCANCODE_DOWN = 81,
																												 WISH_SCANCODE_UP = 82,

																												 WISH_SCANCODE_NUMLOCKCLEAR = 83, /**< num lock on PC, clear on Mac keyboards
																																				 */
																																				 WISH_SCANCODE_KP_DIVIDE = 84,
																																				 WISH_SCANCODE_KP_MULTIPLY = 85,
																																				 WISH_SCANCODE_KP_MINUS = 86,
																																				 WISH_SCANCODE_KP_PLUS = 87,
																																				 WISH_SCANCODE_KP_ENTER = 88,
																																				 WISH_SCANCODE_KP_1 = 89,
																																				 WISH_SCANCODE_KP_2 = 90,
																																				 WISH_SCANCODE_KP_3 = 91,
																																				 WISH_SCANCODE_KP_4 = 92,
																																				 WISH_SCANCODE_KP_5 = 93,
																																				 WISH_SCANCODE_KP_6 = 94,
																																				 WISH_SCANCODE_KP_7 = 95,
																																				 WISH_SCANCODE_KP_8 = 96,
																																				 WISH_SCANCODE_KP_9 = 97,
																																				 WISH_SCANCODE_KP_0 = 98,
																																				 WISH_SCANCODE_KP_PERIOD = 99,

																																				 WISH_SCANCODE_NONUSBACKSLASH = 100, /**< This is the additional key that ISO
																																													*   keyboards have over ANSI ones,
																																													*   located between left shift and Y.
																																													*   Produces GRAVE ACCENT and TILDE in a
																																													*   US or UK Mac layout, REVERSE SOLIDUS
																																													*   (backslash) and VERTICAL LINE in a
																																													*   US or UK Windows layout, and
																																													*   LESS-THAN SIGN and GREATER-THAN SIGN
																																													*   in a Swiss German, German, or French
																																													*   layout. */
																																													WISH_SCANCODE_APPLICATION = 101, /**< windows contextual menu, compose */
																																													WISH_SCANCODE_POWER = 102, /**< The USB document says this is a status flag,
																																																			  *   not a physical key - but some Mac keyboards
																																																			  *   do have a power key. */
																																																			  WISH_SCANCODE_KP_EQUALS = 103,
																																																			  WISH_SCANCODE_F13 = 104,
																																																			  WISH_SCANCODE_F14 = 105,
																																																			  WISH_SCANCODE_F15 = 106,
																																																			  WISH_SCANCODE_F16 = 107,
																																																			  WISH_SCANCODE_F17 = 108,
																																																			  WISH_SCANCODE_F18 = 109,
																																																			  WISH_SCANCODE_F19 = 110,
																																																			  WISH_SCANCODE_F20 = 111,
																																																			  WISH_SCANCODE_F21 = 112,
																																																			  WISH_SCANCODE_F22 = 113,
																																																			  WISH_SCANCODE_F23 = 114,
																																																			  WISH_SCANCODE_F24 = 115,
																																																			  WISH_SCANCODE_EXECUTE = 116,
																																																			  WISH_SCANCODE_HELP = 117,
																																																			  WISH_SCANCODE_MENU = 118,
																																																			  WISH_SCANCODE_SELECT = 119,
																																																			  WISH_SCANCODE_STOP = 120,
																																																			  WISH_SCANCODE_AGAIN = 121,   /**< redo */
																																																			  WISH_SCANCODE_UNDO = 122,
																																																			  WISH_SCANCODE_CUT = 123,
																																																			  WISH_SCANCODE_COPY = 124,
																																																			  WISH_SCANCODE_PASTE = 125,
																																																			  WISH_SCANCODE_FIND = 126,
																																																			  WISH_SCANCODE_MUTE = 127,
																																																			  WISH_SCANCODE_VOLUMEUP = 128,
																																																			  WISH_SCANCODE_VOLUMEDOWN = 129,
																																																			  /* not sure whether there's a reason to enable these */
																																																			  /*     WISH_SCANCODE_LOCKINGCAPSLOCK = 130,  */
																																																			  /*     WISH_SCANCODE_LOCKINGNUMLOCK = 131, */
																																																			  /*     WISH_SCANCODE_LOCKINGSCROLLLOCK = 132, */
																																																			  WISH_SCANCODE_KP_COMMA = 133,
																																																			  WISH_SCANCODE_KP_EQUALSAS400 = 134,

																																																			  WISH_SCANCODE_INTERNATIONAL1 = 135, /**< used on Asian keyboards, see
																																																												 footnotes in USB doc */
																																																												 WISH_SCANCODE_INTERNATIONAL2 = 136,
																																																												 WISH_SCANCODE_INTERNATIONAL3 = 137, /**< Yen */
																																																												 WISH_SCANCODE_INTERNATIONAL4 = 138,
																																																												 WISH_SCANCODE_INTERNATIONAL5 = 139,
																																																												 WISH_SCANCODE_INTERNATIONAL6 = 140,
																																																												 WISH_SCANCODE_INTERNATIONAL7 = 141,
																																																												 WISH_SCANCODE_INTERNATIONAL8 = 142,
																																																												 WISH_SCANCODE_INTERNATIONAL9 = 143,
																																																												 WISH_SCANCODE_LANG1 = 144, /**< Hangul/English toggle */
																																																												 WISH_SCANCODE_LANG2 = 145, /**< Hanja conversion */
																																																												 WISH_SCANCODE_LANG3 = 146, /**< Katakana */
																																																												 WISH_SCANCODE_LANG4 = 147, /**< Hiragana */
																																																												 WISH_SCANCODE_LANG5 = 148, /**< Zenkaku/Hankaku */
																																																												 WISH_SCANCODE_LANG6 = 149, /**< reserved */
																																																												 WISH_SCANCODE_LANG7 = 150, /**< reserved */
																																																												 WISH_SCANCODE_LANG8 = 151, /**< reserved */
																																																												 WISH_SCANCODE_LANG9 = 152, /**< reserved */

																																																												 WISH_SCANCODE_ALTERASE = 153, /**< Erase-Eaze */
																																																												 WISH_SCANCODE_SYSREQ = 154,
																																																												 WISH_SCANCODE_CANCEL = 155,
																																																												 WISH_SCANCODE_CLEAR = 156,
																																																												 WISH_SCANCODE_PRIOR = 157,
																																																												 WISH_SCANCODE_RETURN2 = 158,
																																																												 WISH_SCANCODE_SEPARATOR = 159,
																																																												 WISH_SCANCODE_OUT = 160,
																																																												 WISH_SCANCODE_OPER = 161,
																																																												 WISH_SCANCODE_CLEARAGAIN = 162,
																																																												 WISH_SCANCODE_CRSEL = 163,
																																																												 WISH_SCANCODE_EXSEL = 164,

																																																												 WISH_SCANCODE_KP_00 = 176,
																																																												 WISH_SCANCODE_KP_000 = 177,
																																																												 WISH_SCANCODE_THOUSANDSSEPARATOR = 178,
																																																												 WISH_SCANCODE_DECIMALSEPARATOR = 179,
																																																												 WISH_SCANCODE_CURRENCYUNIT = 180,
																																																												 WISH_SCANCODE_CURRENCYSUBUNIT = 181,
																																																												 WISH_SCANCODE_KP_LEFTPAREN = 182,
																																																												 WISH_SCANCODE_KP_RIGHTPAREN = 183,
																																																												 WISH_SCANCODE_KP_LEFTBRACE = 184,
																																																												 WISH_SCANCODE_KP_RIGHTBRACE = 185,
																																																												 WISH_SCANCODE_KP_TAB = 186,
																																																												 WISH_SCANCODE_KP_BACKSPACE = 187,
																																																												 WISH_SCANCODE_KP_A = 188,
																																																												 WISH_SCANCODE_KP_B = 189,
																																																												 WISH_SCANCODE_KP_C = 190,
																																																												 WISH_SCANCODE_KP_D = 191,
																																																												 WISH_SCANCODE_KP_E = 192,
																																																												 WISH_SCANCODE_KP_F = 193,
																																																												 WISH_SCANCODE_KP_XOR = 194,
																																																												 WISH_SCANCODE_KP_POWER = 195,
																																																												 WISH_SCANCODE_KP_PERCENT = 196,
																																																												 WISH_SCANCODE_KP_LESS = 197,
																																																												 WISH_SCANCODE_KP_GREATER = 198,
																																																												 WISH_SCANCODE_KP_AMPERSAND = 199,
																																																												 WISH_SCANCODE_KP_DBLAMPERSAND = 200,
																																																												 WISH_SCANCODE_KP_VERTICALBAR = 201,
																																																												 WISH_SCANCODE_KP_DBLVERTICALBAR = 202,
																																																												 WISH_SCANCODE_KP_COLON = 203,
																																																												 WISH_SCANCODE_KP_HASH = 204,
																																																												 WISH_SCANCODE_KP_SPACE = 205,
																																																												 WISH_SCANCODE_KP_AT = 206,
																																																												 WISH_SCANCODE_KP_EXCLAM = 207,
																																																												 WISH_SCANCODE_KP_MEMSTORE = 208,
																																																												 WISH_SCANCODE_KP_MEMRECALL = 209,
																																																												 WISH_SCANCODE_KP_MEMCLEAR = 210,
																																																												 WISH_SCANCODE_KP_MEMADD = 211,
																																																												 WISH_SCANCODE_KP_MEMSUBTRACT = 212,
																																																												 WISH_SCANCODE_KP_MEMMULTIPLY = 213,
																																																												 WISH_SCANCODE_KP_MEMDIVIDE = 214,
																																																												 WISH_SCANCODE_KP_PLUSMINUS = 215,
																																																												 WISH_SCANCODE_KP_CLEAR = 216,
																																																												 WISH_SCANCODE_KP_CLEARENTRY = 217,
																																																												 WISH_SCANCODE_KP_BINARY = 218,
																																																												 WISH_SCANCODE_KP_OCTAL = 219,
																																																												 WISH_SCANCODE_KP_DECIMAL = 220,
																																																												 WISH_SCANCODE_KP_HEXADECIMAL = 221,

																																																												 WISH_SCANCODE_LCTRL = 224,
																																																												 WISH_SCANCODE_LSHIFT = 225,
																																																												 WISH_SCANCODE_LALT = 226, /**< alt, option */
																																																												 WISH_SCANCODE_LGUI = 227, /**< windows, command (apple), meta */
																																																												 WISH_SCANCODE_RCTRL = 228,
																																																												 WISH_SCANCODE_RSHIFT = 229,
																																																												 WISH_SCANCODE_RALT = 230, /**< alt gr, option */
																																																												 WISH_SCANCODE_RGUI = 231, /**< windows, command (apple), meta */

																																																												 WISH_SCANCODE_MODE = 257,    /**< I'm not sure if this is really not covered
																																																																			 *   by any of the above, but since there's a
																																																																			 *   special KMOD_MODE for it I'm adding it here
																																																																			 */

																																																																			 /* @} *//* Usage page 0x07 */

																																																																			 /**
																																																																			 *  \name Usage page 0x0C
																																																																			 *
																																																																			 *  These values are mapped from usage page 0x0C (USB consumer page).
																																																																			 */
																																																																			 /* @{ */

																																																																			 WISH_SCANCODE_AUDIONEXT = 258,
																																																																			 WISH_SCANCODE_AUDIOPREV = 259,
																																																																			 WISH_SCANCODE_AUDIOSTOP = 260,
																																																																			 WISH_SCANCODE_AUDIOPLAY = 261,
																																																																			 WISH_SCANCODE_AUDIOMUTE = 262,
																																																																			 WISH_SCANCODE_MEDIASELECT = 263,
																																																																			 WISH_SCANCODE_WWW = 264,
																																																																			 WISH_SCANCODE_MAIL = 265,
																																																																			 WISH_SCANCODE_CALCULATOR = 266,
																																																																			 WISH_SCANCODE_COMPUTER = 267,
																																																																			 WISH_SCANCODE_AC_SEARCH = 268,
																																																																			 WISH_SCANCODE_AC_HOME = 269,
																																																																			 WISH_SCANCODE_AC_BACK = 270,
																																																																			 WISH_SCANCODE_AC_FORWARD = 271,
																																																																			 WISH_SCANCODE_AC_STOP = 272,
																																																																			 WISH_SCANCODE_AC_REFRESH = 273,
																																																																			 WISH_SCANCODE_AC_BOOKMARKS = 274,

																																																																			 /* @} *//* Usage page 0x0C */

																																																																			 /**
																																																																			 *  \name Walther keys
																																																																			 *
																																																																			 *  These are values that Christian Walther added (for mac keyboard?).
																																																																			 */
																																																																			 /* @{ */

																																																																			 WISH_SCANCODE_BRIGHTNESSDOWN = 275,
																																																																			 WISH_SCANCODE_BRIGHTNESSUP = 276,
																																																																			 WISH_SCANCODE_DISPLAYSWITCH = 277, /**< display mirroring/dual display
																																																																											   switch, video mode switch */
																																																																											   WISH_SCANCODE_KBDILLUMTOGGLE = 278,
																																																																											   WISH_SCANCODE_KBDILLUMDOWN = 279,
																																																																											   WISH_SCANCODE_KBDILLUMUP = 280,
																																																																											   WISH_SCANCODE_EJECT = 281,
																																																																											   WISH_SCANCODE_SLEEP = 282,

																																																																											   WISH_SCANCODE_APP1 = 283,
																																																																											   WISH_SCANCODE_APP2 = 284,

																																																																											   /* @} *//* Walther keys */

																																																																											   /* Add any other keys here. */

																																																																											   WISH_NUM_SCANCODES = 512 /**< not a key, just marks the number of scancodes
																																																																																	   for array bounds */
} wish_scancode;