extern "C" {
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
}
#include "init.h"
#include <kandinsky.h>

#include <keys.h>
#include <libndls.h>

#undef None // TODO: Remove me

static KDFrameBuffer *sFrameBuffer = nullptr;
static KDColor *pixels = nullptr;

Ion::Events::Event currentEvent = Ion::Events::None;

static const t_key kShortcutForKey[Ion::Keyboard::NumberOfKeys] = {
  KEY_NSPIRE_LEFT, KEY_NSPIRE_UP, KEY_NSPIRE_DOWN, KEY_NSPIRE_RIGHT, KEY_NSPIRE_CLICK, KEY_NSPIRE_ESC,
  KEY_NSPIRE_HOME, KEY_NSPIRE_LTHAN, KEY_NSPIRE_LTHAN, KEY_NSPIRE_LTHAN, KEY_NSPIRE_LTHAN, KEY_NSPIRE_LTHAN,
  KEY_NSPIRE_SHIFT, KEY_NSPIRE_CTRL, KEY_NSPIRE_X, KEY_NSPIRE_VAR, KEY_NSPIRE_MENU, KEY_NSPIRE_DEL,
  KEY_NSPIRE_LTHAN, KEY_NSPIRE_LTHAN, KEY_NSPIRE_LTHAN, KEY_NSPIRE_I, KEY_NSPIRE_COMMA, KEY_NSPIRE_X,
  KEY_NSPIRE_LTHAN, KEY_NSPIRE_LTHAN, KEY_NSPIRE_LTHAN, KEY_NSPIRE_LTHAN, KEY_NSPIRE_LTHAN, KEY_NSPIRE_LTHAN,
  KEY_NSPIRE_7, KEY_NSPIRE_8, KEY_NSPIRE_9, KEY_NSPIRE_LP, KEY_NSPIRE_RP, KEY_NSPIRE_LTHAN,
  KEY_NSPIRE_4, KEY_NSPIRE_5, KEY_NSPIRE_6, KEY_NSPIRE_MULTIPLY, KEY_NSPIRE_DIVIDE, KEY_NSPIRE_LTHAN,
  KEY_NSPIRE_1, KEY_NSPIRE_2, KEY_NSPIRE_3, KEY_NSPIRE_PLUS, KEY_NSPIRE_MINUS, KEY_NSPIRE_LTHAN,
  KEY_NSPIRE_0, KEY_NSPIRE_PERIOD, KEY_NSPIRE_LTHAN, KEY_NSPIRE_NEGATIVE, KEY_NSPIRE_ENTER, KEY_NSPIRE_LTHAN
};

void terminateHandler() {
  currentEvent = Ion::Events::Termination;
}

static void end()
{
	lcd_init(SCR_TYPE_INVALID);
}

void init_platform() {
  lcd_init(SCR_320x240_565);

  pixels = (KDColor *)malloc(Ion::Display::Width*Ion::Display::Height*sizeof(KDColor));
  sFrameBuffer = new KDFrameBuffer(pixels, KDSize(Ion::Display::Width, Ion::Display::Height));

  atexit(end);
}

void Ion::Display::pushRect(KDRect r, const KDColor * pixels) {
  sFrameBuffer->pushRect(r, pixels);
}

void Ion::Display::pushRectUniform(KDRect r, KDColor c) {
  sFrameBuffer->pushRectUniform(r, c);
}

void Ion::Display::pullRect(KDRect r, KDColor * pixels) {
  sFrameBuffer->pullRect(r, pixels);
}

void Ion::Display::waitForVBlank() {
}

static bool key_down(Ion::Keyboard::Key key)
{
	return isKeyPressed(kShortcutForKey[(int)key]);
}

Ion::Keyboard::State Ion::Keyboard::scan() {
  Ion::Keyboard::State result = 0;
  for (int i=0; i<Ion::Keyboard::NumberOfKeys; i++) {
    result = result << 1 | key_down((Ion::Keyboard::Key)(Ion::Keyboard::NumberOfKeys-1-i));
  }

  return result;
}

Ion::Events::Event Ion::Events::getEvent(int * timeout) {
  do {
	lcd_blit(pixels, SCR_320x240_565);

	static uint64_t last_state = 0;
	uint64_t state = any_key_pressed() ? (uint64_t)Ion::Keyboard::scan() : 0;
	uint64_t pressed = state & ~last_state;
	if(currentEvent == None && pressed != 0)
	{
		int key;
		for(key = 0; (pressed & 1) == 0; pressed >>= 1, ++key);
		currentEvent = Ion::Events::Event((Ion::Keyboard::Key)key,
									Ion::Events::isShiftActive(),
									Ion::Events::isAlphaActive());
		updateModifiersFromEvent(currentEvent);
	}
	last_state = state;

	if(*timeout)
	{
		int tosleep = *timeout >= 16 ? 16 : *timeout;
		msleep(tosleep);
		*timeout -= tosleep;
	}
  } while (*timeout && currentEvent == None);
  Event event = currentEvent;
  currentEvent = None;
  return event;
}

void Ion::msleep(long ms) {
	::msleep(ms);
}
