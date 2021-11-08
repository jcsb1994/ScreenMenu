#include <Arduino.h>
#include <unity.h>
#include "DisplayMenu.h"
#include "DisplayWidget.h"

// MENU HEX COLORS
#define HEX_COLOR_BLUE      0x001F
#define HEX_COLOR_BLACK     0x0000 
#define HEX_COLOR_WHITE     0xFFFF
#define HEX_COLOR_GREEN     0x07E0
#define TARGET_TEXT_COLOR       HEX_COLOR_BLUE
#define BACKGROUND_TEXT_COLOR   HEX_COLOR_BLACK      
#define IDLE_TEXT_COLOR         HEX_COLOR_WHITE     
#define EDIT_TEXT_COLOR         HEX_COLOR_GREEN

#define COUNTER_WIDGET_IDX      0
#define RESET_WIDGET_IDX        1
int widgetTestCounter;

DisplayMenu menu;

void resetCounter() {
  widgetTestCounter = 0;
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
    TEST_ASSERT_EQUAL(menu.getPrintColor(), EDIT_TEXT_COLOR);
}

/*! @brief Test that a var associated with a widget is edited. */
void Test_menuVarEdit(void)
{
    TEST_ASSERT_EQUAL(widgetTestCounter, 1);
}

/*! @brief Test that widget activation had its effect. */
void Test_menuWdgActivation(void)
{
    TEST_ASSERT_EQUAL(widgetTestCounter, 0);
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
    RUN_TEST(Test_menuVarEdit);

    menu.interact();
    menu.moveDown();
    menu.interact();
    RUN_TEST(Test_menuWdgActivation);

}

void loop()
{
    UNITY_END(); // stop unit testing
}