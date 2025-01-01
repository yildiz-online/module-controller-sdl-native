#include "controls.h"
#include <unordered_map>

bool init = false;

bool controllerListChanged = false;

std::unordered_map<SDL_JoystickID, int> states;

int* mapping;

void initControls() {
    if(!init) {
        SDL_version linked;
        SDL_GetVersion(&linked);
        SDL_Log("Initializing module controller(%u) - SDL%u.%u.%u...\n", 240811, linked.major, linked.minor, linked.patch);
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
        SDL_Quit();
        init = false;
        SDL_Log("Terminating module controller complete.\n");
    }
}

void update() {
   SDL_Event e;
   while(SDL_PollEvent(&e)) {
        if (e.type == SDL_CONTROLLERBUTTONDOWN || e.type == SDL_CONTROLLERBUTTONUP) {
            auto itr = states.find(e.cbutton.which);
            if (itr != states.end()){
                auto state = itr->second;
                state = e.cbutton.state ? state | (1 << e.cbutton.button) : (state & (~(1 << e.cbutton.button)));
                states[e.cbutton.which] = state;
            }
        } else if (e.type == SDL_CONTROLLERAXISMOTION) {
            auto controller = SDL_GameControllerFromInstanceID(e.cbutton.which);
            if(e.cbutton.button == SDL_CONTROLLER_AXIS_TRIGGERLEFT) {
                auto value = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_TRIGGERLEFT);
                auto itr = states.find(e.cbutton.which);
                if (itr != states.end()){
                    auto state = itr->second;
                    state = value != 0 ? state | (1 << 24) : state & (~(1 << 24));
                    states[e.cbutton.which] = state;
                }
            } else if (e.cbutton.button == SDL_CONTROLLER_AXIS_TRIGGERRIGHT) {
                auto value = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_TRIGGERRIGHT);
                auto itr = states.find(e.cbutton.which);
                if (itr != states.end()){
                    auto state = itr->second;
                    state = value != 0 ? state | (1 << 25) : state & (~(1 << 25));
                    states[e.cbutton.which] = state;
                }
            }
        } else if (e.type == SDL_CONTROLLERDEVICEADDED) {
            auto controller = SDL_GameControllerOpen(e.cdevice.which);
            auto id = SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(controller));
            states[id] = 0;
            controllerListChanged = true;
            SDL_Log("Controller added %s", SDL_GameControllerName(controller));
        } else if (e.type == SDL_CONTROLLERDEVICEREMOVED) {
            auto controller = SDL_GameControllerFromInstanceID(e.cbutton.which);
            auto id = SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(controller));
            states.erase(id);
            controllerListChanged = true;
            SDL_Log("Controller removed %s", SDL_GameControllerName(controller));
            SDL_GameControllerClose(controller);
        }
   }
}

const char* getControllerName(SDL_JoystickID controllerId) {
    return SDL_GameControllerName(SDL_GameControllerFromInstanceID(controllerId));
}

const char* getControllerGuid(SDL_JoystickID controllerId) {
    SDL_JoystickGUID guid = SDL_JoystickGetGUID(SDL_JoystickFromInstanceID(controllerId));
    char guid_str[33];
    SDL_JoystickGetGUIDString(guid, guid_str, sizeof(guid_str));
    return guid_str;
}

int getControllerState(SDL_JoystickID controllerId) {
    return states.find(controllerId) -> second;
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

bool setRumbleState(int controller, bool rumble, int strength) {
    return false;
}

void setMapping(int* m) {
    mapping = m;
}

void setFileMapping(const char* file) {
    int mappingsAdded = SDL_GameControllerAddMappingsFromFile(file);
    if (mappingsAdded < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load controller mappings from file: %s", SDL_GetError());
    }
}
