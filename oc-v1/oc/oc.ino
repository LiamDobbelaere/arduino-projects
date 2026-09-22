// OpenController V1 - buttons only
// Tools -> USB Stack -> Adafruit TinyUSB

#include <Adafruit_TinyUSB.h>
#include <FastLED.h>

// pins unchanged
#define PIN_BUTTON_X      2
#define PIN_BUTTON_Y      3
#define PIN_BUTTON_B      4
#define PIN_BUTTON_A      5
#define PIN_BUTTON_C      12
#define PIN_BUTTON_Z      13
#define PIN_BUTTON_LB     16
#define PIN_BUTTON_RB     17
#define PIN_BUTTON_SELECT 18
#define PIN_BUTTON_HOME   19
#define PIN_BUTTON_START  20

// gamepad button bit for each pin (bit 0 = "button 1" on PC)
struct Btn { uint8_t pin; uint8_t bit; };
const Btn buttons[] = {
  { PIN_BUTTON_A,      0 },
  { PIN_BUTTON_B,      1 },
  { PIN_BUTTON_X,      2 },
  { PIN_BUTTON_Y,      3 },
  { PIN_BUTTON_LB,     4 },
  { PIN_BUTTON_RB,     5 },
  { PIN_BUTTON_SELECT, 6 },
  { PIN_BUTTON_START,  7 },
  // 8, 9 = LS, RS click later
  { PIN_BUTTON_HOME,   10 },
  { PIN_BUTTON_C,      11 },
  { PIN_BUTTON_Z,      12 },
};
const int BUTTON_COUNT = sizeof(buttons) / sizeof(buttons[0]);

// HID gamepad: 6 axes, hat, 32 buttons
uint8_t const desc_hid_report[] = { TUD_HID_REPORT_DESC_GAMEPAD() };
Adafruit_USBD_HID usb_hid(desc_hid_report, sizeof(desc_hid_report),
                          HID_ITF_PROTOCOL_NONE, 1, false);
hid_gamepad_report_t gp;

#define PIN_RGB_LED 15
#define LED_COUNT 4
CRGB leds[4];

void setup() {
  //set_sys_clock_khz(125000, true);
  pinMode(LED_BUILTIN, OUTPUT);

  for (int i = 0; i < BUTTON_COUNT; i++) {
    pinMode(buttons[i].pin, INPUT_PULLDOWN);   // other leg to 3V3
  }

  TinyUSBDevice.setManufacturerDescriptor("Liam Swift");
  TinyUSBDevice.setProductDescriptor("OpenController V1");
  TinyUSBDevice.setID(0x7140, 0x4020);

  usb_hid.begin();

  if (TinyUSBDevice.mounted()) {
    TinyUSBDevice.detach();
    delay(10);
    TinyUSBDevice.attach();
  }

  FastLED.addLeds<WS2812B, 15, GRB>(leds, 4);
  FastLED.setBrightness(50);
  fill_solid(leds, 4, CRGB::White);
  FastLED.show();

  /*while (!usb_hid.ready()) {
  }

  Serial.begin(115200);
  Serial.println("OpenController says hi");*/
  // TODO: read configuration commands

  digitalWrite(LED_BUILTIN, true);
  delay(1000);
  digitalWrite(LED_BUILTIN, false);
}

void loop() {
  if (!usb_hid.ready()) return;

  uint32_t state = 0;
  for (int i = 0; i < BUTTON_COUNT; i++) {
    if (digitalRead(buttons[i].pin) == HIGH) {
      state |= (1UL << buttons[i].bit);
    }
  }

  gp.x = 0;  gp.y = 0;
  gp.z = 0;  gp.rz = 0;
  gp.rx = 0; gp.ry = 0;
  gp.hat = GAMEPAD_HAT_CENTERED;
  gp.buttons = state;

  usb_hid.sendReport(0, &gp, sizeof(gp));

  bool pressedX = digitalRead(buttons[2].pin) == HIGH;
  bool pressedY = digitalRead(buttons[3].pin) == HIGH;
  bool pressedB = digitalRead(buttons[1].pin) == HIGH;
  bool pressedA = digitalRead(buttons[0].pin) == HIGH;

  static uint8_t hue = 0;
  if (pressedX || pressedY || pressedB || pressedA) {
    fill_rainbow(leds, LED_COUNT, hue, 255 / LED_COUNT);
    hue++;
  } else {
    leds[0] = CRGB(0, 0, 255);
    leds[1] = CRGB(255, 255, 0);
    leds[2] = CRGB(255, 0, 0);
    leds[3] = CRGB(0, 255, 0);
  }
  
  /*leds[0] = !pressedX ? CRGB(0, 0, 255) : CRGB(255,255,255);
  leds[1] = !pressedY ? CRGB(255, 255, 0) : CRGB(255,255,255);
  leds[2] = !pressedB ? CRGB(255, 0, 0) : CRGB(255,255,255);
  leds[3] = !pressedA ? CRGB(0, 255, 0) : CRGB(255,255,255);*/
  FastLED.show();

  delay(1);
}
