#include "LEDAfterControl.h"
#include <Kaleidoscope.h>
#include <Kaleidoscope-FocusSerial.h>
#include "Kaleidoscope-LEDControl.h"
#include "string.h"
#include "kaleidoscope/keyswitch_state.h"

using namespace kaleidoscope::internal; // NOLINT(build/namespaces)

namespace kaleidoscope {
namespace plugin {

cRGB LEDAfterControl::afterColors[64];

EventHandlerResult LEDAfterControl::onKeyswitchEvent(Key &mapped_key, KeyAddr key_addr, uint8_t keyState) {
  if (keyToggledOn(keyState)) {
      int8_t idx = Runtime.device().getLedIndex(key_addr);//KeyboardHardware.getLedIndex(key_addr, key_addr);

      ::LEDControl.setCrgbAt(idx, LEDAfterControl::afterColors[idx]);

      char snum[5];
      itoa(idx, snum, 10);
      char eg[20];
      sprintf(eg, "EGRESS_ack %s\r\n", snum);
      Focus.send(eg);
  }

  return EventHandlerResult::EVENT_CONSUMED;
}

EventHandlerResult FocusLEDAfterCommand::onFocusEvent(const char *command) {
  enum {
    AFTER,
  } subCommand;

  if (!Runtime.has_leds)
    return EventHandlerResult::OK;

  if (strncmp_P(command, PSTR("led."), 4) != 0)
    return EventHandlerResult::OK;
  if (strcmp_P(command + 4, PSTR("after")) == 0)
    subCommand = AFTER;
  else
    return EventHandlerResult::OK;

  switch (subCommand) {
  case AFTER: {
    uint8_t idx;

    ::Focus.read(idx);

    if (!::Focus.isEOL()) {
      cRGB c;
      ::Focus.read(c);
      ::LEDControl.setCrgbAt(idx, c);
      if (!::Focus.isEOL()) {
        cRGB c_after;
        ::Focus.read(c_after);
        LEDAfterControl::afterColors[idx] = c_after;
      }
    }
    break;
  }
  }

  return EventHandlerResult::EVENT_CONSUMED;
}

}
}

kaleidoscope::plugin::LEDAfterControl LEDAfterControl;
kaleidoscope::plugin::FocusLEDAfterCommand FocusLEDAfterCommand;