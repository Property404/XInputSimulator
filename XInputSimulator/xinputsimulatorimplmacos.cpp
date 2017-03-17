//    Copyright 2013 Dustin Bensing

//    This file is part of XInputSimulator.

//    XInputSimulator is free software: you can redistribute it and/or modify
//    it under the terms of the GNU Lesser Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    any later version.

//    XInputSimulator is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU Lesser Public License for more details.

//    You should have received a copy of the GNU Lesser Public License
//    along with XInputSimulator.  If not, see <http://www.gnu.org/licenses/>.


#ifdef __APPLE__

#include <iostream>

//sleep
#include <unistd.h>

#include "xinputsimulatorimplmacos.h"
#include "notimplementedexception.h"

//#include <QDebug>
#include <map>

XInputSimulatorImplMacOs::XInputSimulatorImplMacOs()
{
    //TODO
    //this->initCurrentMousePosition();
    this->currentX = 500;
    this->currentY = 500;

    CGDirectDisplayID displayID = CGMainDisplayID();

    this->displayX = CGDisplayPixelsWide(displayID);
    this->displayY = CGDisplayPixelsHigh(displayID);
}

//void XInputSimulatorImplMacOs::initMouseEvent(int button)
//{

//}

void XInputSimulatorImplMacOs::initCurrentMousePosition()
{
    throw NotImplementedException();
}

void XInputSimulatorImplMacOs::mouseMoveTo(int x, int y)
{

    //TODO screen check see moveRelative

    CGEventRef mouseEv = CGEventCreateMouseEvent(
                NULL, kCGEventMouseMoved,
                CGPointMake(x, y),
                kCGMouseButtonLeft);


    CGEventPost(kCGHIDEventTap, mouseEv);

    CFRelease(mouseEv);

    this->currentX = x;
    this->currentY = y;
}

void XInputSimulatorImplMacOs::mouseMoveRelative(int x, int y)
{
    int newX = this->currentX + x;
    int newY = this->currentY + y;

    if(newX < 0 || newX > this->displayX || newY < 0 || newY > this->displayY )
    {
        std::cout << "mouse moved beyound screensize." << std::endl;
        return;
    }


    CGEventRef mouseEv = CGEventCreateMouseEvent(
                NULL, kCGEventMouseMoved,
                CGPointMake(newX, newY),
                kCGMouseButtonLeft);

    CGEventPost(kCGHIDEventTap, mouseEv);

    CFRelease(mouseEv);

    this->currentX = newX;
    this->currentY = newY;
}

//TODO use the button from parameter list
void XInputSimulatorImplMacOs::mouseDown(int button)
{
    CGEventRef mouseEv = CGEventCreateMouseEvent(
                NULL, kCGEventLeftMouseDown,
                CGPointMake(this->currentX, this->currentY),
                kCGMouseButtonLeft); // use int buttn from parameter

    CGEventPost(kCGHIDEventTap, mouseEv);

    CFRelease(mouseEv);

}

//TODO use the button from parameter list
void XInputSimulatorImplMacOs::mouseUp(int button)
{
    CGEventRef mouseEv = CGEventCreateMouseEvent(
                NULL, kCGEventLeftMouseUp,
                CGPointMake(this->currentX, this->currentY),
                kCGMouseButtonLeft); // use int buttn from parameter

    CGEventPost(kCGHIDEventTap, mouseEv);

    CFRelease(mouseEv);
}

void XInputSimulatorImplMacOs::mouseClick(int button)
{
    //throw NotImplementedException();
    this->mouseDown(button);
    this->mouseUp(button);
}

void XInputSimulatorImplMacOs::mouseScrollX(int length)
{
    int scrollDirection = -1; // 1 left -1 right

    if(length < 0){
        length *= -1;
        scrollDirection *= -1;
    }

    //length *= 100;

    for(int cnt = 0; cnt < length; cnt++){


        CGEventRef scrollEv = CGEventCreateScrollWheelEvent (
                    NULL, kCGScrollEventUnitLine,  // kCGScrollEventUnitLine  //kCGScrollEventUnitPixel
                    2, //CGWheelCount 1 = y 2 = xy 3 = xyz
                    0,
                    scrollDirection); // length of scroll from -10 to 10 higher values lead to undef behaviour

        CGEventPost(kCGHIDEventTap, scrollEv);

        CFRelease(scrollEv);
        //sleep(1);
    }
}

void XInputSimulatorImplMacOs::mouseScrollY(int length)
{
    int scrollDirection = -1; // 1 down -1 up

    if(length < 0){
        length *= -1;
        scrollDirection *= -1;
    }

    //length *= 100;

    for(int cnt = 0; cnt < length; cnt++){


        CGEventRef scrollEv = CGEventCreateScrollWheelEvent (
                    NULL, kCGScrollEventUnitLine,  // kCGScrollEventUnitLine  //kCGScrollEventUnitPixel
                    1, //CGWheelCount 1 = y 2 = xy 3 = xyz
                    scrollDirection); // length of scroll from -10 to 10 higher values lead to undef behaviour

        CGEventPost(kCGHIDEventTap, scrollEv);

        CFRelease(scrollEv);
        //sleep(1);
    }
}

void XInputSimulatorImplMacOs::keyDown(int key)
{
    CGKeyCode keycode = key;    
    CGEventRef commandDown = CGEventCreateKeyboardEvent(NULL, keycode, true);
    CGEventPost(kCGAnnotatedSessionEventTap, commandDown);
    CFRelease(commandDown);
}

void XInputSimulatorImplMacOs::keyUp(int key)
{
    CGKeyCode keycode = key;
    CGEventRef commandUp = CGEventCreateKeyboardEvent(NULL, keycode, false);
    CGEventPost(kCGAnnotatedSessionEventTap, commandUp);
    CFRelease(commandUp);
}


void XInputSimulatorImplMacOs::keyClick(int key)
{
    
    this->keyDown(key);
    this->keyUp(key);
}
void XInputSimulatorImplMacOs::keyClickShift(int key)
{
	CGEventRef event1;
	event1 = CGEventCreateKeyboardEvent(NULL, (CGKeyCode)key, true);
	CGEventSetFlags(event1, kCGEventFlagMaskShift);
	CGEventPost(kCGAnnotatedSessionEventTap, event1);
	CFRelease(event1);
	this->keyUp(key);
}

CFStringRef XInputSimulatorImplMacOs::createStringForKey(CGKeyCode keyCode)
{
    TISInputSourceRef currentKeyboard = TISCopyCurrentKeyboardInputSource();
    //CFDataRef layoutData = TISGetInputSourceProperty(currentKeyboard, kTISPropertyUnicodeKeyLayoutData);
    CFDataRef layoutData = (CFDataRef)TISGetInputSourceProperty(currentKeyboard, kTISPropertyUnicodeKeyLayoutData);
    
    const UCKeyboardLayout *keyboardLayout = (const UCKeyboardLayout *)CFDataGetBytePtr(layoutData);
    
    UInt32 keysDown = 0;
    UniChar chars[4];
    UniCharCount realLength;
    
    UCKeyTranslate(keyboardLayout,
                   keyCode,
                   kUCKeyActionDisplay,
                   0,
                   LMGetKbdType(),
                   kUCKeyTranslateNoDeadKeysBit,
                   &keysDown,
                   sizeof(chars) / sizeof(chars[0]),
                   &realLength,
                   chars);
    
    CFRelease(currentKeyboard);
    
    return CFStringCreateWithCharacters(kCFAllocatorDefault, chars, 1);
    
    return NULL;
}
const std::map<char,int> sequence_map =
 {{'a',0x00},{'s',0x01},{'d',0x02},{'f',0x03},{'h',0x04},{'g',0x05},{'z',0x06},{'x',0x07},{'c',0x08},
{'v',0x09},{'b',0x0b},{'q',0x0c},{'w',0x0d},{'e',0x0e},{'r',0x0f},{'y',0x10},{'t',0x11},{'1',0x12},{'2',0x13},
{'3',0x14},{'4',0x15},{'6',0x16},{'5',0x17},{'=',0x18},{'9',0x19},{'7',0x1a},{'-',0x1b},{'8',0x1c},{'0',0x1d},
{']',0x1e},{'o',0x1f},{'u',0x20},{'[',0x21},{'i',0x22},{'p',0x23},{'l',0x25},{'j',0x26},{'\'',0x27},{'k',0x28},
{';',0x29},{'\\',0x2a},{',',0x2b},{'/',0x2c},{'n',0x2d},{'m',0x2e},{'.',0x2f},{'`',0x32}, {' ',0x31}, {'\n',0x24}};
int XInputSimulatorImplMacOs::charToKeyCode(char key_char)
{
    return sequence_map.count(key_char)?sequence_map.at(key_char):-1;
}
void XInputSimulatorImplMacOs::keySequence(const std::string &sequence)
{
    const char* lower_keys = "1234567890abcdefghijklmnopqrstuvwxyz`-=[]\\;',./";
    const std::string upper_keys = "!@#$%^&*()ABCDEFGHIJKLMNOPQRSTUVWXYZ~_+{}|:\"<>?";
   
    for(char c : sequence) {
	bool shift = false;
	if(upper_keys.find(c)!=-1){
		c = lower_keys[upper_keys.find(c)];
		shift = true;
	}

        int keyCode = this->charToKeyCode(c);
        shift?this->keyClickShift(keyCode):keyClick(keyCode);

    }

    //throw NotImplementedException();
}

#endif //apple
