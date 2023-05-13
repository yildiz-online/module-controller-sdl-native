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

typedef void (PXLCALL *controllerEventCallback)(int controller, int button, bool state);

PXLEXPORT void initControls();

PXLEXPORT void setFileMapping(const char* file);

PXLEXPORT void terminateControls();

PXLEXPORT const char* getControllerName(int c);

///
/// Function to invoke when the state is to be polled instead of event driven.
/// This can be helpful in multi threaded environment where the event callback can suffer from crashes.
/// The function will invoke update and return the updated state.
///
PXLEXPORT int update();

PXLEXPORT int* getMapping();

PXLEXPORT int getController2();

PXLEXPORT int getController3();

PXLEXPORT int getController4();

PXLEXPORT bool isC1Plugged();

PXLEXPORT bool isC2Plugged();

PXLEXPORT bool isC3Plugged();

PXLEXPORT bool isC4Plugged();

PXLEXPORT void runController();

PXLEXPORT void registerCallback(controllerEventCallback cb);

PXLEXPORT void setMapping(int* m);

PXLEXPORT bool setRumbleState(int controller, bool rumble, int strength);

#ifdef __cplusplus
}
#endif
#endif