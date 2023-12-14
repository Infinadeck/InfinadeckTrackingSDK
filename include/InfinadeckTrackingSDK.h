#ifndef INFINADECK_TRACKING_SDK_H_
#define INFINADECK_TRACKING_SDK_H_
#define INFINADECK_TRACKING_EXPORT extern "C" __declspec(dllexport)   

#define PATH_BUFFER_MAX_LENGTH 256
#define INFINADECK_TRACKING_VERSION 2
namespace Infinadeck {
    typedef void (*SYSTEMLOG)(const char*);
    /// <summary>
    /// Write a message to the runtme logger
    /// </summary>
    /// <param name="message">The message to be logged</param>
    inline void (*Log)(const char* message);

    /// <summary>
    /// Should be called in order to allow the plugin to access the runtime logger
    /// </summary>
    /// <param name="system">The parameter provided by the Runtime caller</param>
    inline void SystemInit(void* system) {
        Log = (SYSTEMLOG)(system);
    }

#ifndef INFINADECK_RUNTIME
    INFINADECK_TRACKING_EXPORT int GetTrackingVersion() {
        return INFINADECK_TRACKING_VERSION;
    }
#endif
}
#endif