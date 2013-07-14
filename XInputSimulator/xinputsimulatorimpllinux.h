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

#ifndef XINPUTSIMULATORIMPLLINUX_H
#define XINPUTSIMULATORIMPLLINUX_H

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "xinputsimulatorimpl.h"

class XInputSimulatorImplLinux : public XInputSimulatorImpl
{
private:
    Display *display;
    Window root;
    XEvent event;

    void initMouseEvent(int button);

public:
    XInputSimulatorImplLinux();
    ~XInputSimulatorImplLinux(){ delete display; }

    virtual void mouseMoveTo(int x, int y) override;
    virtual void mouseMoveRelative(int x, int y) override;
    virtual void mouseDown(int button) override;
    virtual void mouseUp(int button) override;
    virtual void mouseClick(int button) override;
    virtual void mouseScrollX(int length) override;
    virtual void mouseScrollY(int length) override;

    virtual void keyDown(int key) override;
    virtual void keyUp(int key) override;

};

#endif // XINPUTSIMULATORIMPLLINUX_H
