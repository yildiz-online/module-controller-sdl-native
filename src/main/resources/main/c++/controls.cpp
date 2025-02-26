#include "controls.h"
#include <unordered_map>

bool init = false;

bool controllerListChanged = false;

std::unordered_map<SDL_JoystickID, int> states;

int* mapping;

void initControls() {
    if(!init) {
        const int linked = SDL_GetVersion();
        SDL_Log("Initializing module controller(%u) - SDL%u.%u.%u...\n", 250226, SDL_VERSIONNUM_MAJOR(linked), SDL_VERSIONNUM_MINOR(linked), SDL_VERSIONNUM_MICRO(linked));
        SDL_Init(SDL_INIT_EVENTS);
        SDL_Init(SDL_INIT_GAMEPAD);
        init = true;
        SDL_Log("Initializing module controller complete.\n");
    }
}

void terminateControls() {
    if(init) {
        SDL_Log("Terminating module controller...\n");
        SDL_Quit();
        init = false;
        SDL_Log("Terminating module controller complete.\n");
    }
}

void update() {
   SDL_Event e;
   while(SDL_PollEvent(&e)) {
       if (e.type == SDL_EVENT_GAMEPAD_BUTTON_DOWN || e.type == SDL_EVENT_GAMEPAD_BUTTON_UP) {
           auto itr = states.find(e.gbutton.which);
           if (itr != states.end()){
               auto state = itr->second;
               state = e.gbutton.down ? state | (1 << e.gbutton.button) : (state & (~(1 << e.gbutton.button)));
               states[e.gbutton.which] = state;
           } else {
               SDL_Log("Unregistered controller %d", e.gbutton.which);
           }
       } else if (e.type == SDL_EVENT_GAMEPAD_AXIS_MOTION) {
           auto controller = SDL_GetGamepadFromID(e.gaxis.which);
           if(e.gaxis.axis == SDL_GAMEPAD_AXIS_LEFT_TRIGGER) {
               auto value = SDL_GetGamepadAxis(controller, SDL_GAMEPAD_AXIS_LEFT_TRIGGER);
               auto itr = states.find(e.gaxis.which);
               if (itr != states.end()) {
                   auto state = itr->second;
                   state = value != 0 ? state | (1 << 24) : state & (~(1 << 24));
                   states[e.gaxis.which] = state;
               }
           } else if (e.gaxis.axis == SDL_GAMEPAD_AXIS_RIGHT_TRIGGER) {
               auto value = SDL_GetGamepadAxis(controller, SDL_GAMEPAD_AXIS_RIGHT_TRIGGER);
               auto itr = states.find(e.gaxis.which);
               if (itr != states.end()){
                   auto state = itr->second;
                   state = value != 0 ? state | (1 << 25) : state & (~(1 << 25));
                   states[e.gaxis.which] = state;
               }
           }
       } else if (e.type == SDL_EVENT_GAMEPAD_ADDED) {
           auto gamepad = SDL_OpenGamepad(e.gdevice.which);
           if (gamepad) {
               states[e.gdevice.which] = 0;
               controllerListChanged = true;
               SDL_Log("Controller added %s", SDL_GetGamepadNameForID(e.gdevice.which));
           } else {
               SDL_Log("Could not open gamepad %d: %s", e.gdevice.which, SDL_GetError());
           }
       } else if (e.type == SDL_EVENT_GAMEPAD_REMOVED) {
           states.erase(e.gdevice.which);
           controllerListChanged = true;
           SDL_Log("Controller removed %s", SDL_GetGamepadNameForID(e.gdevice.which));
           SDL_CloseGamepad(SDL_GetGamepadFromID(e.gdevice.which));
       }
   }
}

const char* getControllerName(SDL_JoystickID controllerId) {
    return SDL_GetGamepadNameForID(controllerId);
}

const char* getControllerGuid(SDL_JoystickID controllerId) {
    SDL_GUID guid = SDL_GetGamepadGUIDForID(controllerId);
    // not thread safe, value shared between all calls to this function, use std::string instead
    static char guid_str[33];
    SDL_GUIDToString(guid, guid_str, sizeof(guid_str));
    return guid_str;
}

int getControllerState(SDL_JoystickID controllerId) {
    auto itr = states.find(controllerId);
    if (itr != states.end()){
        return itr->second;
    } else {
        SDL_Log("Controller %s not found", SDL_GetGamepadNameForID(controllerId));
        return 0;
    }
}

bool isControllerListChanged() {
    if (controllerListChanged) {
        controllerListChanged = false;
        return true;
    }
    return false;
}

int getControllerNumber() {
    return states.size();
}

SDL_JoystickID* getControllers() {
    SDL_JoystickID* list = new SDL_JoystickID[states.size()];
    auto i = 0;
    for (const auto& pair :states) {
        list[i] = pair.first;
        i++;
    }
    return list;
}

void setMapping(int* m) {
    mapping = m;
}
