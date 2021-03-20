#ifndef BUILD_INFORMATION
#define BUILD_INFORMATION "locally built"
#endif

// The Kaleidoscope core
#include "Kaleidoscope.h"

// Support for storing the keymap in EEPROM
#include "Kaleidoscope-EEPROM-Settings.h"

// Support for communicating with the host via a simple Serial protocol
#include "Kaleidoscope-FocusSerial.h"

// Support for macros
#include "Kaleidoscope-Macros.h"

// Support for controlling the keyboard's LEDs
#include "Kaleidoscope-LEDControl.h"

// Support for magic combos (key chords that trigger an action)
#include "Kaleidoscope-MagicCombo.h"

// Support for USB quirks, like changing the key state report protocol
//#include "Kaleidoscope-USB-Quirks.h"


// Single key macro to clear pressed key
enum { MACRO_KP };

// Combo macro to clear all
enum { COMBO_CLEAR_ALL };

/* This comment temporarily turns off astyle's indent enforcement
 *   so we can make the keymaps actually resemble the physical key layout better
 */
// *INDENT-OFF*

KEYMAPS(

  [0] = KEYMAP_STACKED
  (M(MACRO_KP), M(MACRO_KP), M(MACRO_KP), M(MACRO_KP), M(MACRO_KP), M(MACRO_KP), M(MACRO_KP),
   M(MACRO_KP), M(MACRO_KP), M(MACRO_KP), M(MACRO_KP), M(MACRO_KP), M(MACRO_KP), M(MACRO_KP),
   M(MACRO_KP), M(MACRO_KP), M(MACRO_KP), M(MACRO_KP), M(MACRO_KP), M(MACRO_KP),
   M(MACRO_KP), M(MACRO_KP), M(MACRO_KP), M(MACRO_KP), M(MACRO_KP), M(MACRO_KP), M(MACRO_KP),
   M(MACRO_KP), M(MACRO_KP), M(MACRO_KP), M(MACRO_KP),
   M(MACRO_KP),

   M(MACRO_KP),  M(MACRO_KP), M(MACRO_KP), M(MACRO_KP), M(MACRO_KP), M(MACRO_KP), M(MACRO_KP),
   M(MACRO_KP),  M(MACRO_KP), M(MACRO_KP), M(MACRO_KP), M(MACRO_KP), M(MACRO_KP), M(MACRO_KP),
                  M(MACRO_KP), M(MACRO_KP), M(MACRO_KP), M(MACRO_KP), M(MACRO_KP), M(MACRO_KP),
   M(MACRO_KP),  M(MACRO_KP), M(MACRO_KP), M(MACRO_KP), M(MACRO_KP),    M(MACRO_KP),     M(MACRO_KP),
   M(MACRO_KP), M(MACRO_KP), M(MACRO_KP), M(MACRO_KP),
   M(MACRO_KP))


  
) // KEYMAPS(

/* Re-enable astyle's indent enforcement */
// *INDENT-ON*

/* static void keypressMacro(uint8_t keyState) {
  if (keyToggledOn(keyState)) {
    LEDControl.setCrgbAt(Macros.row, Macros.col, CRGB(0, 0, 0));
    int8_t k = KeyboardHardware.getLedIndex(Macros.row, Macros.col);
    char snum[5];
    itoa(k, snum, 10);
    char eg[20];
    sprintf(eg, "EGRESS_ack %s\r\n", snum);
    Focus.send(eg);
  }
}

const macro_t *macroAction(uint8_t macroIndex, uint8_t keyState) {
  switch (macroIndex) {

  case MACRO_KP:
    keypressMacro(keyState);
    break;
  }
  return MACRO_NONE;
} */

static void clearAll(uint8_t combo_index) {
  LEDControl.set_all_leds_to({0, 0, 0});
  Focus.send("EGRESS_ackall\r\n");
}

/** Magic combo list, a list of key combo and action pairs the firmware should
 * recognise.
 */
USE_MAGIC_COMBOS({.action = clearAll,
                  .keys = { R3C6, R3C9 }
                 });

// First, tell Kaleidoscope which plugins you want to use.
// The order can be important. For example, LED effects are
// added in the order they're listed here.
KALEIDOSCOPE_INIT_PLUGINS(
  // The EEPROMSettings & EEPROMKeymap plugins make it possible to have an
  // editable keymap in EEPROM.
  EEPROMSettings,

  // Focus allows bi-directional communication with the host, and is the
  // interface through which the keymap in EEPROM can be edited.
  Focus,

  // FocusSettingsCommand adds a few Focus commands, intended to aid in
  // changing some settings of the keyboard, such as the default layer (via the
  // `settings.defaultLayer` command)
  FocusSettingsCommand,
  FocusLEDCommand,

  // LEDControl provides support for other LED modes
  LEDControl,

  // We start with the LED effect that turns off all the LEDs.
  LEDOff,

  // The macros plugin adds support for macros
  Macros,

  // The MagicCombo plugin lets you use key combinations to trigger custom
  // actions - a bit like Macros, but triggered by pressing multiple keys at the
  // same time.
  MagicCombo

  // The USBQuirks plugin lets you do some things with USB that we aren't
  // comfortable - or able - to do automatically, but can be useful
  // nevertheless. Such as toggling the key report protocol between Boot (used
  // by BIOSes) and Report (NKRO).
  //USBQuirks
);

/** The 'setup' function is one of the two standard Arduino sketch functions.
 * It's called when your keyboard first powers up. This is where you set up
 * Kaleidoscope and any plugins.
 */
void setup() {
  // First, call Kaleidoscope's internal setup function
  Kaleidoscope.setup();

  // We want to make sure that the firmware starts with LED effects off
  // This avoids over-taxing devices that don't have a lot of power to share
  // with USB devices
  LEDOff.activate();
}

/** loop is the second of the standard Arduino sketch functions.
  * As you might expect, it runs in a loop, never exiting.
  *
  * For Kaleidoscope-based keyboard firmware, you usually just want to
  * call Kaleidoscope.loop(); and not do anything custom here.
  */

void loop() {
  Kaleidoscope.loop();
}
