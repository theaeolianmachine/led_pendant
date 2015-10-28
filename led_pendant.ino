#include <FastLED.h>

#define BRIGHTNESS 64
#define DATA_PIN 0
#define NUM_COLORS 8
#define NUM_SHADES 4
#define NUM_LEDS 16
#define BASE_HUE 0
#define BASE_VALUE 255

const CHSVPalette16 PENDANT_COLORS(
    // Red
    CHSV(0, 255, 255),
    CHSV(0, 156, 255),

    // Orange/Yellow
    CHSV(24, 255, 255),
    CHSV(24, 156, 255),

    // Yellow
    CHSV(35, 255, 255),
    CHSV(35, 156, 255),

    // Yellow-Green
    CHSV(50, 255, 246),
    CHSV(50, 155, 248),

    // Cyan
    CHSV(128, 255, 153),
    CHSV(128, 155, 178),

    // Light Blue
    CHSV(146, 238, 164),
    CHSV(146, 146, 187),

    // Blue/Purple
    CHSV(180, 226, 175),
    CHSV(180, 137, 195),

    // Violet
    CHSV(231, 255, 205),
    CHSV(231, 155, 217)
);

CRGB leds[NUM_LEDS];

void setup() {
    FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
    FastLED.setBrightness(BRIGHTNESS);
}

void colorFlow() {
    uint16_t dot, index, colorIndex;
    for (index = 0; index < 256; ++index) {
        colorIndex = index;
        for (dot = 0; dot < NUM_LEDS; ++dot) {
            leds[dot] = ColorFromPalette(
                PENDANT_COLORS, colorIndex, BASE_VALUE, LINEARBLEND);
            colorIndex += 2;
        }
        FastLED.show();
        delay(50);
    }
}

void lightFirstFourLEDS(uint16_t color, uint8_t offset) {
    uint8_t dot;
    uint16_t colorIndex;
    for (dot = 0; dot < NUM_LEDS; ++dot) {
        if (((dot + offset) & 7) < 4) {
            colorIndex = ((dot & 1) == 0) ? color : color + 16;
            leds[dot] = ColorFromPalette(
                PENDANT_COLORS, colorIndex, BASE_VALUE, LINEARBLEND);
        } else {
            leds[dot] = CRGB::Black;
        }
    }
    FastLED.show();
    delay(75);
}

void forwardAndBackwards() {
    uint8_t dot;
    int8_t offset;
    uint16_t color;
    for (color = 0; color < 256; color += 32) {
        for (offset = 0; offset < NUM_LEDS; ++offset) {
            lightFirstFourLEDS(color, offset);
        }
    }
    for (color = 0; color < 256; color += 32) {
        for (offset = NUM_LEDS - 1; offset >= 0; --offset) {
            lightFirstFourLEDS(color, offset);
        }
    }
}

void twelveToSix() {
    uint8_t dot;
    uint16_t color;
    int8_t offset;
    for (color = 0; color < 256; color += 4) {
        for (offset = 0; offset < 8; ++offset) {
            for (dot = 0; dot < (NUM_LEDS / 2); ++dot) {
                if (((dot + offset) & 7) < 4) {
                    leds[dot] = ColorFromPalette(
                        PENDANT_COLORS, color, BASE_VALUE, LINEARBLEND);
                    leds[NUM_LEDS - dot - 1] = ColorFromPalette(
                        PENDANT_COLORS, color, BASE_VALUE, LINEARBLEND);
                } else {
                    leds[dot] = CRGB::Black;
                    leds[NUM_LEDS - dot - 1] = CRGB::Black;
                }
            }
            FastLED.show();
            delay(75);
        }
    }
}

void loop() {
    colorFlow();
    forwardAndBackwards();
    twelveToSix();
}
