#include <Arduino.h>
#include <unity.h>
#include "DisplayMenu.h"
#include "DisplayWidget.h"

// void setUp(void) {
// // set stuff up here
// }

// void tearDown(void) {
// // clean stuff up here
// }

#define BUZZER_PIN      7
#define STEP_PERIOD_MS  100

#define HEX_COLOR_BLUE      0x001F
#define HEX_COLOR_BLACK     0x0000 
#define HEX_COLOR_WHITE     0xFFFF
#define HEX_COLOR_GREEN     0x07E0

#define TARGET_TEXT_COLOR       HEX_COLOR_BLUE
#define BACKGROUND_TEXT_COLOR   HEX_COLOR_BLACK      
#define IDLE_TEXT_COLOR         HEX_COLOR_WHITE     
#define EDIT_TEXT_COLOR         HEX_COLOR_GREEN     

int widgetTestCounter;

DisplayMenu menu;

void resetCounter() {
  widgetTestCounter
}

DisplayWidget widgets[DANCE_MENU_WIDGET_NB] = {
  DisplayWidget(&widgetTestCounter, 1, 20), //check to replace pointer with a void pointer, but needs to understand if is a fct ptr
  DisplayWidget(widgetTestCounter)
};

void Test_menuBackgroundColor(void)
{
    TEST_ASSERT_EQUAL(menu.getBackgroundColor(), HEX_COLOR_BLACK);
}

void Test_menuEditingWidget(void)
{
    TEST_ASSERT_EQUAL(menu.isEditingTarget(), true);
    TEST_ASSERT_EQUAL(menu.getWidgetColor(), EDIT_TEXT_COLOR);
}

void test_led_state_low(void)
{
    digitalWrite(LED_BUILTIN, LOW);
    TEST_ASSERT_EQUAL(LOW, digitalRead(LED_BUILTIN));
}

void setup()
{
    // NOTE!!! Wait for >2 secs
    // if board doesn't support software reset via Serial.DTR/RTS
    delay(2000);
    
    UNITY_BEGIN(); // IMPORTANT LINE!
    menu.setColors(IDLE_TEXT_COLOR, TARGET_TEXT_COLOR, EDIT_TEXT_COLOR, BACKGROUND_TEXT_COLOR);
    RUN_TEST(Test_menuBackgroundColor);

    menu.interact();
    RUN_TEST(Test_menuEditingWidget);

    menu.moveUp();
    RUN_TEST(Test_menuWidgetColor);

    menu.interact();
    menu.moveDown();
    menu.interact();
}

uint8_t i = 0;
uint8_t max_blinks = 5;

void loop()
{
    if (i < max_blinks)
    {
        RUN_TEST(test_led_state_high);
        delay(500);
        RUN_TEST(test_led_state_low);
        delay(500);
        i++;
    }
    else if (i == max_blinks)
    {
        UNITY_END(); // stop unit testing
    }
}