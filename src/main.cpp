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

#include <iostream>
#include "xinputsimulator.h"

#ifdef __unix__
//sleep
#include <unistd.h>
#elif __APPLE__
//sleep
#include <unistd.h>
#elif _WIN32
//sleep
#include <windows.h>
#endif

using namespace std;

// Effectively `sleep(1)`
void waitABit();
// Do a little dance - Show off what XInputSimulator can do
void showOff();
// Show commands and such
void displayHelp(std::string command="");


// Arg Utility (pure gold, lol)
struct AU {
	string arg;
	// setArgc and setArgv must be called before other methods can be used
	void setArgc(int c){
		this->argc = c;
	}
	void setArgv(char** argv){
		this->argv = argv;
	}

	// Set arg to next argument in argv
	int forwardArgument(){
		static int pos = 1;
		if (pos<this->argc){
			arg = this->argv[pos];
			pos++;
			return 1;
		}
		return 0;
	}
	// Call if and only if the next argument is necessary
	void forwardNeededArgument(std::string command=""){
		forwardArgument()?0:(displayHelp(command), exit(1),1);
	}
private:
	int argc;
	char** argv;
};

// Let's do the do
int main(int argc, char** argv){
	
	// Argument flags
	XInputSimulator &sim = XInputSimulator::getInstance();

	// Set up argument utility
	AU au = AU();
	au.setArgc(argc);
	au.setArgv(argv);
	au.forwardNeededArgument();

	// Here we parse the arguments - seems rather self explanatory
	if(au.arg=="mouse"){
		au.forwardNeededArgument("mouse");
		if(au.arg=="moveto"){
			au.forwardNeededArgument("mouse");
			int x = atoi(au.arg.c_str());
			au.forwardNeededArgument("mouse");
			int y = atoi(au.arg.c_str());
			sim.mouseMoveTo(x,y);
			exit(0);
		}else if(au.arg=="moverelative"){
			au.forwardNeededArgument("mouse");
			int x = atoi(au.arg.c_str());
			au.forwardNeededArgument("mouse");
			int y = atoi(au.arg.c_str());
			sim.mouseMoveRelative(x,y);
			exit(0);
		}else if(au.arg=="down"){
			sim.mouseDown((au.forwardNeededArgument("mouse down"), atoi(
			au.arg.c_str())));		
		}else if(au.arg=="up"){
			sim.mouseUp((au.forwardNeededArgument("mouse up"), atoi(
			au.arg.c_str())));
		}else{
			displayHelp("mouse");
			exit(1);
		}
	}else if(au.arg=="key"){
		au.forwardNeededArgument("key");
		if(au.arg=="click"){
			au.forwardNeededArgument("key");
			sim.keyClick(sim.charToKeyCode(au.arg[0]));
		}else if(au.arg=="sequence"){
			au.forwardNeededArgument("key");
			sim.keySequence(au.arg);
		}else{
			displayHelp("key");
			exit(1);
		}
	}else if(au.arg=="showoff"){
		showOff();
		exit(0);
	}else{
		displayHelp();
		exit(1);
	}
	return 0;
}
void displayHelp(std::string command){
	cout << "This is a help message on "<<command<<"(it's very helpful)\n";
}
void showOff()
{
    cout << "What XIS can do for you:" << endl;

    XInputSimulator &sim = XInputSimulator::getInstance();

    waitABit();
    sim.mouseMoveTo(500,400);
    waitABit();
    sim.mouseMoveRelative(400, -100);
    waitABit();
    sim.mouseDown(XIS::LEFT_MOUSE_BUTTON);
    waitABit();
    sim.mouseMoveRelative(0, 300);
    waitABit();
    sim.mouseUp(XIS::LEFT_MOUSE_BUTTON);
    waitABit();
    //scroll down and up
    sim.mouseScrollY(10);
    waitABit();
    sim.mouseScrollY(-10);
    //scroll left and right
    waitABit();
    sim.mouseScrollX(10);
    waitABit();
    sim.mouseScrollX(-10);

#if defined(__unix__) || defined(__APPLE__)
	char anA = 'a';
    cout << "a: " << (int)anA << " " << sim.charToKeyCode(anA) << endl;
    std::cout << std::endl;
    waitABit();
    sim.keyClick(sim.charToKeyCode(anA));
    std::cout << std::endl;
    waitABit();
    sim.keySequence(" Simple sentence Here 123 ");
#elif _WIN32
	//not implemented
#endif


    waitABit();
}

void waitABit()
{
#if defined(__unix__) || defined(__APPLE__)
	sleep(1);
#elif _WIN32
	Sleep(1000);
#endif
    
}
