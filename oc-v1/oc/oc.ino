// OpenController V1 - buttons + sticks + LEDs
// Tools -> USB Stack -> Adafruit TinyUSB

#include <Adafruit_TinyUSB.h>
#include <FastLED.h>

// ---------- pins ----------
#define PIN_BUTTON_X      2
#define PIN_BUTTON_Y      3
#define PIN_BUTTON_B      4
#define PIN_BUTTON_A      5
#define PIN_BUTTON_LS     10   // stick click, SW pin, active LOW
#define PIN_BUTTON_RS     11   // stick click, SW pin, active LOW
#define PIN_BUTTON_C      12
#define PIN_BUTTON_Z      13
#define PIN_RGB_LED       15
#define PIN_BUTTON_LB     16
#define PIN_BUTTON_RB     17
#define PIN_BUTTON_SELECT 18
#define PIN_BUTTON_HOME   19
#define PIN_BUTTON_START  20

// mux (CD74HC4067)
#define PIN_MUX_S0  6
#define PIN_MUX_S1  7
#define PIN_MUX_S2  8
#define PIN_MUX_S3  9
#define PIN_MUX_SIG 26

#define CH_LS_X 2
#define CH_LS_Y 3
#define CH_RS_X 0
#define CH_RS_Y 1

// ---------- buttons ----------
// bit 0 = "button 1" on PC. Order matches Xbox: A B X Y LB RB Back Start LS RS
struct Btn { uint8_t pin; uint8_t bit; bool activeLow; };
const Btn buttons[] = {
  { PIN_BUTTON_A,      0,  false },
  { PIN_BUTTON_B,      1,  false },
  { PIN_BUTTON_X,      2,  false },
  { PIN_BUTTON_Y,      3,  false },
  { PIN_BUTTON_LB,     4,  false },
  { PIN_BUTTON_RB,     5,  false },
  { PIN_BUTTON_SELECT, 6,  false },
  { PIN_BUTTON_START,  7,  false },
  { PIN_BUTTON_LS,     8,  true  },
  { PIN_BUTTON_RS,     9,  true  },
  { PIN_BUTTON_HOME,   10, false },
  { PIN_BUTTON_C,      11, false },
  { PIN_BUTTON_Z,      12, false },
};
const int BUTTON_COUNT = sizeof(buttons) / sizeof(buttons[0]);

// ---------- sticks ----------
#define STICK_DEADZONE 6      // out of 127. raise if drift, lower if sluggish
#define INVERT_LS_Y false     // flip if up/down feels wrong
#define INVERT_RS_Y false

int centerLsX, centerLsY, centerRsX, centerRsY;   // read at boot

// ---------- USB HID ----------
uint8_t const desc_hid_report[] = { TUD_HID_REPORT_DESC_GAMEPAD() };
Adafruit_USBD_HID usb_hid(desc_hid_report, sizeof(desc_hid_report),
                          HID_ITF_PROTOCOL_NONE, 1, false);
hid_gamepad_report_t gp;

// ---------- LEDs ----------
#define LED_COUNT 4
CRGB leds[LED_COUNT];

// ---------- helpers ----------
int readMux(int ch) {
  digitalWrite(PIN_MUX_S0, (ch >> 0) & 1);
  digitalWrite(PIN_MUX_S1, (ch >> 1) & 1);
  digitalWrite(PIN_MUX_S2, (ch >> 2) & 1);
  digitalWrite(PIN_MUX_S3, (ch >> 3) & 1);
  delayMicroseconds(10);
  analogRead(PIN_MUX_SIG);
  analogRead(PIN_MUX_SIG);
  return analogRead(PIN_MUX_SIG);   // 0..1023
}

// raw 0..1023 -> -127..127, centered, with deadzone
int8_t toAxis(int raw, int center, bool invert) {
  int v;
  if (raw >= center) v = map(raw, center, 1023, 0, 127);
  else               v = map(raw, 0, center, -127, 0);
  if (abs(v) < STICK_DEADZONE) v = 0;
  if (invert) v = -v;
  return (int8_t)constrain(v, -127, 127);
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, true);

  for (int i = 0; i < BUTTON_COUNT; i++) {
    pinMode(buttons[i].pin, buttons[i].activeLow ? INPUT_PULLUP : INPUT_PULLDOWN);
  }

  pinMode(PIN_MUX_S0, OUTPUT);
  pinMode(PIN_MUX_S1, OUTPUT);
  pinMode(PIN_MUX_S2, OUTPUT);
  pinMode(PIN_MUX_S3, OUTPUT);

  // sticks must be untouched at boot
  centerLsX = readMux(CH_LS_X);
  centerLsY = readMux(CH_LS_Y);
  centerRsX = readMux(CH_RS_X);
  centerRsY = readMux(CH_RS_Y);

  TinyUSBDevice.setManufacturerDescriptor("Liam Swift");
  TinyUSBDevice.setProductDescriptor("OpenController V1");
  TinyUSBDevice.setID(0x7140, 0x4025);

  usb_hid.begin();

  if (TinyUSBDevice.mounted()) {
    TinyUSBDevice.detach();
    delay(10);
    TinyUSBDevice.attach();
  }

  FastLED.addLeds<WS2812B, PIN_RGB_LED, GRB>(leds, LED_COUNT);
  FastLED.setBrightness(50);
  fill_solid(leds, LED_COUNT, CRGB::White);
  FastLED.show();

  Serial.begin(115200);

  delay(500);
  digitalWrite(LED_BUILTIN, false);
}

void loop() {
  if (!usb_hid.ready()) return;

  // buttons
  uint32_t state = 0;
  for (int i = 0; i < BUTTON_COUNT; i++) {
    bool pressed = digitalRead(buttons[i].pin) == (buttons[i].activeLow ? LOW : HIGH);
    if (pressed) state |= (1UL << buttons[i].bit);
  }

  // sticks. left = X/Y, right = Rx/Ry (Xbox layout). Z/Rz free for triggers.
  gp.rx = toAxis(readMux(CH_RS_X), centerRsX, false);
  gp.ry = toAxis(readMux(CH_RS_Y), centerRsY, INVERT_RS_Y);
  gp.x  = toAxis(readMux(CH_LS_X), centerLsX, false);
  gp.y  = toAxis(readMux(CH_LS_Y), centerLsY, INVERT_LS_Y);
  gp.z  = 0;
  gp.rz = 0;
  gp.hat = GAMEPAD_HAT_CENTERED;
  gp.buttons = state;

  usb_hid.sendReport(0, &gp, sizeof(gp));

  // LEDs
  bool anyFace = state & 0b1111;   // A B X Y
  static uint8_t hue = 0;
  if (anyFace) {
    fill_rainbow(leds, LED_COUNT, hue, 255 / LED_COUNT);
    hue++;
  } else {
    leds[0] = CRGB(0, 0, 255);
    leds[1] = CRGB(255, 255, 0);
    leds[2] = CRGB(255, 0, 0);
    leds[3] = CRGB(0, 255, 0);
  }
  FastLED.show();

  delay(1);
}
