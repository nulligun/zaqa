#include "kaleidoscope/Runtime.h"
#include "kaleidoscope/plugin/LEDMode.h"

namespace kaleidoscope {
namespace plugin {

class LEDMode;

class LEDAfterControl : public kaleidoscope::Plugin {
 public:
  LEDAfterControl(void) {};
  EventHandlerResult onKeyswitchEvent(Key &mapped_key, KeyAddr key_addr, uint8_t keyState);

  static cRGB afterColors[64];
};

class FocusLEDAfterCommand : public Plugin {
 public:
  FocusLEDAfterCommand() {}

  EventHandlerResult onFocusEvent(const char *command);
};

}
}

extern kaleidoscope::plugin::LEDAfterControl LEDAfterControl;
extern kaleidoscope::plugin::FocusLEDAfterCommand FocusLEDAfterCommand;
