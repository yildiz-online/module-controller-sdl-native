#include "controls.h"
#include <SDL.h>
#include <iostream>

#define MAX_PLAYERS 4

SDL_GameController* controllers[MAX_PLAYERS];

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
    }
}

void initControls() {
    SDL_GameControllerAddMappingsFromFile("sdl_gamecontrollerdb.txt");
    SDL_Init(SDL_INIT_GAMECONTROLLER);
    for(int i = 0; i < SDL_NumJoysticks(); i++) {
        initJoystick(i);
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

void setRumbleState(int controller, bool rumble, int strength) {
	
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
