/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package frostfe;

import java.util.Vector;
import org.lwjgl.input.Keyboard;

public class Input
{
	public static Vector<KeyboardKey> MonitoredKeys = new Vector();
	
	public static void Initialize()
	{
		MonitoredKeys.add(new KeyboardKey(Keyboard.KEY_BACK, (char)0, (char)0)); // Backspace
		MonitoredKeys.add(new KeyboardKey(Keyboard.KEY_TAB, (char)0, (char)0)); // Tab
		MonitoredKeys.add(new KeyboardKey(Keyboard.KEY_RETURN, (char)0, (char)0)); // Enter
		MonitoredKeys.add(new KeyboardKey(Keyboard.KEY_LSHIFT, (char)0, (char)0)); // Shift
		MonitoredKeys.add(new KeyboardKey(Keyboard.KEY_LCONTROL, (char)0, (char)0)); // Control
		MonitoredKeys.add(new KeyboardKey(Keyboard.KEY_LMENU, (char)0, (char)0)); // Alt
		MonitoredKeys.add(new KeyboardKey(Keyboard.KEY_CAPITAL, (char)0, (char)0)); // Caps Lock
		MonitoredKeys.add(new KeyboardKey(Keyboard.KEY_ESCAPE, (char)0, (char)0)); // Esc
		MonitoredKeys.add(new KeyboardKey(Keyboard.KEY_SPACE, ' ', (char)0)); // Spacebar
		MonitoredKeys.add(new KeyboardKey(Keyboard.KEY_LEFT, (char)0, (char)0)); // Left arrow
		MonitoredKeys.add(new KeyboardKey(Keyboard.KEY_UP, (char)0, (char)0)); // Up arrow
		MonitoredKeys.add(new KeyboardKey(Keyboard.KEY_RIGHT, (char)0, (char)0)); // Right arrow
		MonitoredKeys.add(new KeyboardKey(Keyboard.KEY_DOWN, (char)0, (char)0)); // Down arrow
		MonitoredKeys.add(new KeyboardKey(Keyboard.KEY_DELETE, (char)0, (char)0)); // Delete
		MonitoredKeys.add(new KeyboardKey(Keyboard.KEY_0, '0', ')')); // 0
		MonitoredKeys.add(new KeyboardKey(Keyboard.KEY_1, '1', '!')); // 1
		MonitoredKeys.add(new KeyboardKey(Keyboard.KEY_2, '2', '@')); // 2
		MonitoredKeys.add(new KeyboardKey(Keyboard.KEY_3, '3', '#')); // 3
		MonitoredKeys.add(new KeyboardKey(Keyboard.KEY_4, '4', '$')); // 4
		MonitoredKeys.add(new KeyboardKey(Keyboard.KEY_5, '5', '%')); // 5
		MonitoredKeys.add(new KeyboardKey(Keyboard.KEY_6, '6', '^')); // 6
		MonitoredKeys.add(new KeyboardKey(Keyboard.KEY_7, '7', '&')); // 7
		MonitoredKeys.add(new KeyboardKey(Keyboard.KEY_8, '8', '*')); // 8
		MonitoredKeys.add(new KeyboardKey(Keyboard.KEY_9, '9', '(')); // 9
		MonitoredKeys.add(new KeyboardKey(Keyboard.KEY_A, 'a', 'A')); // A
		MonitoredKeys.add(new KeyboardKey(Keyboard.KEY_B, 'b', 'B')); // B
		MonitoredKeys.add(new KeyboardKey(Keyboard.KEY_C, 'c', 'C')); // C
		MonitoredKeys.add(new KeyboardKey(Keyboard.KEY_D, 'd', 'D')); // D
		MonitoredKeys.add(new KeyboardKey(Keyboard.KEY_E, 'e', 'E')); // E
		MonitoredKeys.add(new KeyboardKey(Keyboard.KEY_F, 'f', 'F')); // F
		MonitoredKeys.add(new KeyboardKey(Keyboard.KEY_G, 'g', 'G')); // G
		MonitoredKeys.add(new KeyboardKey(Keyboard.KEY_H, 'h', 'H')); // H
		MonitoredKeys.add(new KeyboardKey(Keyboard.KEY_I, 'i', 'I')); // I
		MonitoredKeys.add(new KeyboardKey(Keyboard.KEY_J, 'j', 'J')); // J
		MonitoredKeys.add(new KeyboardKey(Keyboard.KEY_K, 'k', 'K')); // K
		MonitoredKeys.add(new KeyboardKey(Keyboard.KEY_L, 'l', 'L')); // L
		MonitoredKeys.add(new KeyboardKey(Keyboard.KEY_M, 'm', 'M')); // M
		MonitoredKeys.add(new KeyboardKey(Keyboard.KEY_N, 'n', 'N')); // N
		MonitoredKeys.add(new KeyboardKey(Keyboard.KEY_O, 'o', 'O')); // O
		MonitoredKeys.add(new KeyboardKey(Keyboard.KEY_P, 'p', 'P')); // P
		MonitoredKeys.add(new KeyboardKey(Keyboard.KEY_Q, 'q', 'Q')); // Q
		MonitoredKeys.add(new KeyboardKey(Keyboard.KEY_R, 'r', 'R')); // R
		MonitoredKeys.add(new KeyboardKey(Keyboard.KEY_S, 's', 'S')); // S
		MonitoredKeys.add(new KeyboardKey(Keyboard.KEY_T, 't', 'T')); // T
		MonitoredKeys.add(new KeyboardKey(Keyboard.KEY_U, 'u', 'U')); // U
		MonitoredKeys.add(new KeyboardKey(Keyboard.KEY_V, 'v', 'V')); // V
		MonitoredKeys.add(new KeyboardKey(Keyboard.KEY_W, 'w', 'W')); // W
		MonitoredKeys.add(new KeyboardKey(Keyboard.KEY_X, 'x', 'X')); // X
		MonitoredKeys.add(new KeyboardKey(Keyboard.KEY_Y, 'y', 'Y')); // Y
		MonitoredKeys.add(new KeyboardKey(Keyboard.KEY_Z, 'z', 'Z')); // Z
		MonitoredKeys.add(new KeyboardKey(Keyboard.KEY_F1, (char)0, (char)0)); // F1
		MonitoredKeys.add(new KeyboardKey(Keyboard.KEY_F2, (char)0, (char)0)); // F2
		MonitoredKeys.add(new KeyboardKey(Keyboard.KEY_F3, (char)0, (char)0)); // F3
		MonitoredKeys.add(new KeyboardKey(Keyboard.KEY_F4, (char)0, (char)0)); // F4
		MonitoredKeys.add(new KeyboardKey(Keyboard.KEY_F5, (char)0, (char)0)); // F5
		MonitoredKeys.add(new KeyboardKey(Keyboard.KEY_F6, (char)0, (char)0)); // F6
		MonitoredKeys.add(new KeyboardKey(Keyboard.KEY_F7, (char)0, (char)0)); // F7
		MonitoredKeys.add(new KeyboardKey(Keyboard.KEY_F8, (char)0, (char)0)); // F8
		MonitoredKeys.add(new KeyboardKey(Keyboard.KEY_F9, (char)0, (char)0)); // F9
		MonitoredKeys.add(new KeyboardKey(Keyboard.KEY_F10, (char)0, (char)0)); // F10
		MonitoredKeys.add(new KeyboardKey(Keyboard.KEY_F11, (char)0, (char)0)); // F11
		MonitoredKeys.add(new KeyboardKey(Keyboard.KEY_F12, (char)0, (char)0)); // F12
		MonitoredKeys.add(new KeyboardKey(Keyboard.KEY_SEMICOLON, ';', ':')); // ;:
		MonitoredKeys.add(new KeyboardKey(Keyboard.KEY_EQUALS, '=', '+')); // +=
		MonitoredKeys.add(new KeyboardKey(Keyboard.KEY_COMMA, ',', '<')); // ,<
		MonitoredKeys.add(new KeyboardKey(Keyboard.KEY_MINUS, '-', '_')); // -_
		MonitoredKeys.add(new KeyboardKey(Keyboard.KEY_PERIOD, '.', '>')); // .>
		MonitoredKeys.add(new KeyboardKey(Keyboard.KEY_SLASH, '/', '?')); // /?
		MonitoredKeys.add(new KeyboardKey(Keyboard.KEY_CIRCUMFLEX, '`', '~')); // `~
		MonitoredKeys.add(new KeyboardKey(Keyboard.KEY_LBRACKET, '[', '{')); // [{
		MonitoredKeys.add(new KeyboardKey(Keyboard.KEY_BACKSLASH, '\\', '|')); // \|
		MonitoredKeys.add(new KeyboardKey(Keyboard.KEY_RBRACKET, ']', '}')); // ]}
		MonitoredKeys.add(new KeyboardKey(Keyboard.KEY_APOSTROPHE, '\'', '\"')); // '"
	}
	
	public static boolean RawIsKeyPressed(int KeyCode)
	{
		return Keyboard.isKeyDown(KeyCode);
	}
	
	public static void Update()
	{
		// Update the state of all keyboard keys.
		for (int i = 0; i < MonitoredKeys.size(); i++)
		{
			MonitoredKeys.get(i).Update();
		}
	}
	
	public static KeyboardKey GetKeyByKeyCode(int KeyCode)
	{
		for (int i = 0; i < MonitoredKeys.size(); i++)
		{
			if (MonitoredKeys.get(i).KeyCode == KeyCode)
			{
				return MonitoredKeys.get(i);
			}
		}
		return null;
	}
}