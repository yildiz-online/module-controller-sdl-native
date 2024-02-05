#include "controls.h"
#include <SDL.h>
#include <iostream>

/*
SDL_FIRSTEVENT: 0
SDL_QUIT: 256
SDL_APP_TERMINATING: 257
SDL_APP_LOWMEMORY: 258
SDL_APP_WILLENTERBACKGROUND: 259
SDL_APP_DIDENTERBACKGROUND: 260
SDL_APP_WILLENTERFOREGROUND: 261
SDL_APP_DIDENTERFOREGROUND: 262
SDL_WINDOWEVENT: 512
SDL_SYSWMEVENT: 513
SDL_KEYDOWN: 768
SDL_KEYUP: 769
SDL_TEXTEDITING: 770
SDL_TEXTINPUT: 771
SDL_KEYMAPCHANGED: 772
SDL_MOUSEMOTION: 1024
SDL_MOUSEBUTTONDOWN: 1025
SDL_MOUSEBUTTONUP: 1026
SDL_MOUSEWHEEL: 1027
SDL_JOYAXISMOTION: 1536
SDL_JOYBALLMOTION: 1537
SDL_JOYHATMOTION: 1538
SDL_JOYBUTTONDOWN: 1539
SDL_JOYBUTTONUP: 1540
SDL_JOYDEVICEADDED: 1541
SDL_JOYDEVICEREMOVED: 1542
SDL_CONTROLLERAXISMOTION: 1616
SDL_CONTROLLERBUTTONDOWN: 1617
SDL_CONTROLLERBUTTONUP: 1618
SDL_CONTROLLERDEVICEADDED: 1619
SDL_CONTROLLERDEVICEREMOVED: 1620
SDL_CONTROLLERDEVICEREMAPPED: 1621
SDL_FINGERDOWN: 1792
SDL_FINGERUP: 1793
SDL_FINGERMOTION: 1794
SDL_DOLLARGESTURE: 2048
SDL_DOLLARRECORD: 2049
SDL_MULTIGESTURE: 2050
SDL_CLIPBOARDUPDATE: 2304
SDL_DROPFILE: 4096
SDL_DROPTEXT: 4097
SDL_DROPBEGIN: 4098
SDL_DROPCOMPLETE: 4099
SDL_AUDIODEVICEADDED: 4352
SDL_AUDIODEVICEREMOVED: 4353
SDL_SENSORUPDATE: 5120
SDL_RENDER_TARGETS_RESET: 8192
SDL_RENDER_DEVICE_RESET: 8193
SDL_USEREVENT: 32768
SDL_LASTEVENT: 65535
*/

#define MAX_PLAYERS 4

bool init = false;

SDL_GameController* controllers[MAX_PLAYERS] = {};

SDL_JoystickID ids[MAX_PLAYERS];

bool c1Plugged = false;
bool c2Plugged = false;
bool c3Plugged = false;
bool c4Plugged = false;

//Contains the state of the 4 first controllers buttons.
int c1 = 0;
int c2 = 0;
int c3 = 0;
int c4 = 0;

int* mapping;

void setMapping(int* m) {
    mapping = m;
}

const char* getControllerName(int player) {
    return SDL_JoystickName(SDL_GameControllerGetJoystick(controllers[player]));
}

void initJoystick(int player) {
    if(player < MAX_PLAYERS -1) {
        controllers[player] = SDL_GameControllerOpen(player);
        ids[player] = SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(controllers[player]));
        SDL_Log("Controller added %s", getControllerName(player));
    }
}

void setFileMapping(const char* file) {
    int mappingsAdded = SDL_GameControllerAddMappingsFromFile(file);
    if (mappingsAdded < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load controller mappings from file: %s", SDL_GetError());
    }
}

void initControls() {
    if(!init) {
        SDL_version linked;
        SDL_GetVersion(&linked);
        SDL_Log("Initializing module controller(%u) - SDL%u.%u.%u...\n", 230509, linked.major, linked.minor, linked.patch);
        SDL_Init(SDL_INIT_VIDEO);
        SDL_Init(SDL_INIT_EVENTS);
        SDL_Init(SDL_INIT_GAMECONTROLLER);
        init = true;
        SDL_Log("Initializing module controller complete.\n");
    }
}

void terminateControls() {
    if(init) {
        SDL_Log("Terminating module controller...\n");
        c1Plugged = false;
        c2Plugged = false;
        c3Plugged = false;
        c4Plugged = false;
        c1 = 0;
        c2 = 0;
        c3 = 0;
        c4 = 0;
        SDL_Quit();
        init = false;
        SDL_Log("Terminating module controller complete.\n");
    }
}

int getControllerIndex(SDL_JoystickID id) {
    for(int i = 0; i < MAX_PLAYERS; i++) {
        if(id == ids[i]) {
           return i;
        }
    }
    return -1;
}

int update() {
   SDL_Event e;
   while(SDL_PollEvent(&e)) {
        if (e.type == SDL_CONTROLLERBUTTONDOWN || e.type == SDL_CONTROLLERBUTTONUP) {
            switch(getControllerIndex(e.cbutton.which)) {
                case 0: c1 = e.cbutton.state ? c1 | (1 << e.cbutton.button) : c1 & (~(1 << e.cbutton.button)); break;
                case 1: c2 = e.cbutton.state ? c2 | (1 << e.cbutton.button) : c2 & (~(1 << e.cbutton.button)); break;
                case 2: c3 = e.cbutton.state ? c3 | (1 << e.cbutton.button) : c3 & (~(1 << e.cbutton.button)); break;
                case 3: c4 = e.cbutton.state ? c4 | (1 << e.cbutton.button) : c4 & (~(1 << e.cbutton.button)); break;
                case -1: SDL_Log("Button pressed for unregistered controller.\n");
            }
        } else if (e.type == SDL_CONTROLLERAXISMOTION) {
            auto index = getControllerIndex(e.cbutton.which);
            if(e.cbutton.button == SDL_CONTROLLER_AXIS_TRIGGERLEFT) {
                auto value = SDL_GameControllerGetAxis(controllers[index], SDL_CONTROLLER_AXIS_TRIGGERLEFT);
                switch(getControllerIndex(e.cbutton.which)) {
                    case 0: c1 = value != 0 ? c1 | (1 << 24) : c1 & (~(1 << 24)); break;
                    case 1: c2 = value != 0 ? c2 | (1 << 24) : c2 & (~(1 << 24)); break;
                    case 2: c3 = value != 0 ? c3 | (1 << 24) : c3 & (~(1 << 24)); break;
                    case 3: c4 = value != 0 ? c4 | (1 << 24) : c4 & (~(1 << 24)); break;
                    case -1: SDL_Log("Button pressed for unregistered controller.\n");
                }
            } else if (e.cbutton.button == SDL_CONTROLLER_AXIS_TRIGGERRIGHT) {
                auto value = SDL_GameControllerGetAxis(controllers[index], SDL_CONTROLLER_AXIS_TRIGGERRIGHT);
                switch(getControllerIndex(e.cbutton.which)) {
                    case 0: c1 = value != 0 ? c1 | (1 << 25) : c1 & (~(1 << 25)); break;
                    case 1: c2 = value != 0 ? c2 | (1 << 25) : c2 & (~(1 << 25)); break;
                    case 2: c3 = value != 0 ? c3 | (1 << 25) : c3 & (~(1 << 25)); break;
                    case 3: c4 = value != 0 ? c4 | (1 << 25) : c4 & (~(1 << 25)); break;
                    case -1: SDL_Log("Button pressed for unregistered controller.\n");
                }
            }
        } else if (e.type == SDL_KEYUP) {
            switch(e.key.keysym.sym) {
                case SDLK_RETURN: c1 = c1 & (~(1 << 6)); break;
                case SDLK_UP: c1 = c1 & (~(1 << 11)); break;
                case SDLK_DOWN: c1 = c1 & (~(1 << 12));  break;
                case SDLK_RIGHT: c1 = c1 & (~(1 << 14)); break;
                case SDLK_LEFT: c1 = c1 & (~(1 << 13)); break;
                case SDLK_ESCAPE:
                    c1 = c1 & (~(1 << 4));
                    c1 = c1 & (~(1 << 9));
                    c1 = c1 & (~(1 << 10));
                    break;
            }
        } else if (e.type == SDL_KEYDOWN) {
            switch(e.key.keysym.sym) {
                case SDLK_RETURN: c1 = c1 | (1 << 6); break;
                case SDLK_UP: c1 = c1 | (1 << 11); break;
                case SDLK_DOWN: c1 = c1 | (1 << 12); break;
                case SDLK_RIGHT: c1 = c1 | (1 << 14); break;
                case SDLK_LEFT: c1 = c1 | (1 << 13); break;
                case SDLK_ESCAPE:
                    c1 = c1 | (1 << 4);
                    c1 = c1 | (1 << 9);
                    c1 = c1 | (1 << 10);
                    break;
            }
        } else if (e.type == SDL_CONTROLLERDEVICEADDED) {
            switch(getControllerIndex(e.cbutton.which)) {
                case 0: c1Plugged = true; break;
                case 1: c2Plugged = true; break;
                case 2: c3Plugged = true; break;
                case 3: c4Plugged = true; break;
                case -1: SDL_Log("Controller added for unregistered controller.\n");
            }
        } else if (e.type == SDL_CONTROLLERDEVICEREMOVED) {
            switch(getControllerIndex(e.cbutton.which)) {
                case 0: c1 = 0; c1Plugged = false; break;
                case 1: c2 = 0; c2Plugged = false; break;
                case 2: c3 = 0; c3Plugged = false; break;
                case 3: c4 = 0; c4Plugged = false; break;
            }
        } else if (e.type == SDL_JOYDEVICEADDED) {
            initJoystick(e.jbutton.which);
            switch(getControllerIndex(e.jbutton.which)) {
                case 0: c1Plugged = true; break;
                case 1: c2Plugged = true; break;
                case 2: c3Plugged = true; break;
                case 3: c4Plugged = true; break;
                case -1: SDL_Log("Joystick added for unregistered controller.\n");
            }
        } else if (e.type == SDL_JOYDEVICEREMOVED) {
            switch(getControllerIndex(e.jbutton.which)) {
                case 0: c1 = 0; c1Plugged = false; break;
                case 1: c2 = 0; c2Plugged = false; break;
                case 2: c3 = 0; c3Plugged = false; break;
                case 3: c4 = 0; c4Plugged = false; break;
            }
        }
   }
   return c1;
}

bool setRumbleState(int controller, bool rumble, int strength) {
    return false;
}

int* getMapping() {
    return mapping;
}

int getController2() {
    return c2;
}

int getController3() {
    return c3;
}

int getController4() {
    return c4;
}

bool isC1Plugged() {
    return c1Plugged;
}

bool isC2Plugged() {
    return c2Plugged;
}

bool isC3Plugged() {
    return c3Plugged;
}

bool isC4Plugged() {
    return c4Plugged;
}
