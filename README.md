# ScreenMenu
This repository provides a software library loosely based on the Arduino framework (could easily be branched and integrated for different hardware) to help develop embedded software that manages screen display menu, selectable items on the screen and basic UI navigation for bare metal or RTOS applications.

![ScreenShot](https://raw.githubusercontent.com/jcsb1994/ScreenMenu/master/documentation/displayNumberEdit.gif)

## Setup
Clone the repository in your desired libraries folder.

### PlatformIO
To tell PlatformIO where to look for this library, specify the include path with the the `lib_extra_dirs` argument in the platformio.ini file of your project. 
For example:

`lib_extra_dirs = C:\Users\<yourUserName>\Documents\Libraries`

## Software
The software library contains multiple files that offer simple UI design tools.

### DisplayWidget
The DisplayWidget file provides widget objects, that represent a selectable item that will be shown on screen. Each widget will be printed in a specific color whether it is selected or not. The library will manage which widget is selected automatically from user inputs.

A widget can be used to modify values, or to call a function when pressed. This allows for user settings as well as changing pages, triggering specific effects, etc.

### DisplayMenu
The DisplayMenu file offers the principal functionality of this library. The menu object follows the user's navigation and triggers widget effects.

#### Workflow
- DisplayMenu object creation
- DisplayWidget objects array creation (for each different display page)
- Set the desired colors that the menu should use to print widgets when selected or not
- Set one widget array as the current displayed widgets
- Tell the menu when the user has pressed a key, will determine which widget is now selected automatically
- Print the page on screen using the menu widget color map to color the selected widget accordingly

