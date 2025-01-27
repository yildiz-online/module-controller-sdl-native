#ifndef PXL_CONTROLS_H
#define PXL_CONTROLS_H

#ifdef _WIN32
#define PXLEXPORT __declspec(dllexport)
#define PXLCALL __stdcall
#elif __linux__
#define PXLEXPORT __attribute__((visibility("default")))
#define PXLCALL
#endif

#ifdef __cplusplus
extern "C" {
#endif

#include <SDL.h>

typedef void (PXLCALL *controllerEventCallback)(int controller, int button, bool state);

PXLEXPORT void registerCallback(controllerEventCallback cb);


PXLEXPORT void initControls();

PXLEXPORT void setFileMapping(const char* file);

PXLEXPORT void terminateControls();

PXLEXPORT const char* getControllerName(SDL_JoystickID controllerId);

PXLEXPORT const char* getControllerGuid(SDL_JoystickID controllerId);

///
/// Function to invoke when the state is to be polled instead of event driven.
/// This can be helpful in multi threaded environment where the event callback can suffer from crashes.
///
PXLEXPORT void update();

/**
 * Check if the controller list has changed since last call.
 * The value will be reset to false after the call.
 *
 * @return true if the controller list has changed since last call.
 */
PXLEXPORT bool isControllerListChanged();

PXLEXPORT int getControllerNumber();

PXLEXPORT SDL_JoystickID* getControllers();

PXLEXPORT int getControllerState(SDL_JoystickID controllerId);

#ifdef __cplusplus
}
#endif
#endif