# Infinadeck Tracking SDK
This repository contains the functions required to develop plugins for feeding tracking data into the Infinadeck Runtime

## Instructions

To connect to the UI, use the following steps:

1. Include all of the files in the "include" folder.
2. Implement each of the required functions. Implement any optional functions as desired.
3. Compile as a .dll.
4. Place the compiled .dll, as well as any necessary dependencies and configuration files into the Runtime's "Plugins" folder.
5. Set the "tracking library" field in the Runtime config file to your plugin .dll path within the Runtime's "Plugins" folder.

## Required Functions

The following functions must be implemented in order for the plugin to work properly. If any of these are not present, the Runtime will refuse to load the plugin.

| Function                                 | Purpose                                                                            |
|------------------------------------------|------------------------------------------------------------------------------------|
|```bool Initialize(void* system)```       | Called when the plugin is loaded. Plugin must be ready to track after this returns. Should return true if init was successful |
|```const char * GetModuleName()```         | Should return the name of the plugin                                              |
|```const char * GetPublisherName()```              | Should return the name of the developer/publisher of the plugin           |
|```Infinadeck::Skeleton GetUserSkeleton()```| Should return a skeleton corresponding to the user's position                    |
|```void Finalize()```                | Called when the plugin is unloaded                                                      |
|```int GetTrackedPointCount()```     | Should return the number of points in the skeleton that are being tracked by the plugin |
|```bool GetTrackingLoss()```          | Should return ```true``` if the plugin determines that the user has lost tracking      |

### Initialize

```c++
INFINADECK_TRACKING_EXPORT bool Initialize(void* system) {
  Infinadeck::SystemInit(system);
  /*...*/
  return true
}
```

This function will be called once the plugin has been loaded and the Runtime has verified that all of the required functions are present. The parameter ```system``` can be passed into the function ```Infinadeck::SystemInit()``` in order to allow the plugin to access the Runtime's logging system and print messages to it.

Do any work here that is required for the tracking system to initialize and interface with the Runtime. Once this function returns, the Runtime will assume that the plugin is ready, and will begin to read tracking data from it.

This function should return true if initialization was successful. If your plugin determines that anything has gone wrong during this process, it should return false instead.
### GetModuleName

```c++
INFINADECK_TRACKING_EXPORT const char * GetModuleName() {
  return "Example Plugin";
}
```

### GetPublisherName

```c++
INFINADECK_TRACKING_EXPORT const char * GetPublisherName() {
  return "Infinadeck";
}
```

### GetUserSkeleton

```c++
INFINADECK_TRACKING_EXPORT Infinadeck::Skeleton GetUserSkeleton() {
  Infinadeck::Skeleton user;
  user.GetJoint(SkeletonJoints::Pelvis) = {1,2,3};
  return user;
}
```

This function will be called whenever the Runtime polls the position of the user. This should be as up-to-date as possible, and should be as fast as possible to reduce latency between user movement and treadmill motion.

All positions and rotations should be given in Infinadeck coordinate system
### Finalize
```c++
INFINADECK_TRACKING_EXPORT void Finalize() {
  /*...*/
}
```

This function will be called when the plugin is unloaded. After this function returns, no more calls will be made to the plugin.

### GetTrackedPointCount
```c++
INFINADECK_TRACKING_EXPORT int GetTrackedPointCount() {
  const int plugin_joint_count = 3;
  return plugin_joint_count;
}
```

This function should return the number of points in the user skeleton that the plugin is actively tracking. This must be no greater than ```Skeleton::JointCount = 27```. 

### GetTrackingLoss
```c++
INFINADECK_TRACKING_EXPORT bool GetTrackingLoss() {
  bool user_has_disappeared = false;
  /* 
  ...
  check if user has disappeared
  */
  return user_has_disappeared;
}
```
This function should tell the Runtime if the user has lost tracking. Each tracked skeleton also keeps track of itself, but the user may still be correctly tracked even if a few of the skeleton joints are missing. Thus, the Runtime will use this function to determine if the tracking system cannot see the user so that action can be taken to keep the user safe.

## Optional Functions

| Function                                 | Purpose                                                                                       |
|------------------------------------------|-----------------------------------------------------------------------------------------------|
|```Infinadeck::SkeletonJoints GetPrimaryJointID()```         | Should return the identifier of the primary tracking joint                             |
|```void CalibrateUser()```              | Should run a routine to calibrate the user's skeleton                                           |
|```void CalibrateCenter()```| Should run a routine to calibrate the center of the user's tracking space                                   |
|```void Refresh()```                | Will be called once per iteration of the Runtime main loop                                          |
|```Infinadeck::SkeletonJoints GetSmoothLocomotionReferenceJoint(Infinadeck::SmoothLocomotionReference reference)``` | Should return the identifier of the joint used for smooth locomotion reference |
|```void SetOffset(Infinadeck::TrackingVector3 offset, Infinadeck::SkeletonJoints joint)``` | Will set the offset of the user's skeleton position      |
|```void GetConfigFilePath(char* path_buffer)``` | Tells the runtime the plugin has config file, and where it can be found |
|```Infinadeck::TrackingVector3 GetCenterPositionInVRSpace()``` | Should return the position of the treadmill center in VR space |
|```Infinadeck::TrackingVector4 GetCenterRotationInVRSpace()``` | Should return the rotation of the treadmill center in VR space |

### GetPrimaryJointID
```c++
INFINADECK_TRACKING_EXPORT Infinadeck::SkeletonJoints GetPrimaryJointID() {
  return Infinadeck::SkeletonJoints::Pelvis;
}
```

By default, the Runtime will take the pelvis as the user's primary joint, and will favor it during tracking calculations. If your plugin instead uses a different joint as the primary joint, use this function to notify the Runtime

### CalibrateUser
```c++
INFINADECK_TRACKING_EXPORT void CalibrateUser() {
    /* 
  ...
  Run user calibration sequence
  */
}
```

Some plugins may require the user to be calibrated. For example, determining which tracker to use as the primary object, or finding the user in a camera-based system. This function should make any necessary calls to perform these operations. The runtime will call this function whenever prompted to by the user.

### CalibrateCenter
```c++
INFINADECK_TRACKING_EXPORT void CalibrateCenter() {
    /* 
  ...
  Run center calibration system
  */
}
```

Some plugins may require the center of the user's tracking space to be calculated. For example, determining the center of the treadmill within the vr tracking space. This function should make any necessary calls to perform these operations. The runtime will call this function whenever prompted to by the user.

### Refresh

```c++
INFINADECK_TRACKING_EXPORT void Refresh() {
  /*...*/
}
```

This function will be called once per iteration of the Runtime's main loop. Call any functions that need to be continually refreshed here. In addition, if making calls to the tracking system are time-consuming, those should be made here instead of ```GetUserSkeleton()```

### GetSmoothLocomotionReferenceJoint
```c++
INFINADECK_TRACKING_EXPORT Infinadeck::SkeletonJoints GetSmoothLocomotionReferenceJoint(Infinadeck::SmoothLocomotionReference reference) {
  switch(reference) {
    case Infinadeck::SmoothLocomotionReference::HandLeft:
      return Infinadeck::SkeletonJoints::HandLeft;
        case Infinadeck::SmoothLocomotionReference::HandRight:
      return Infinadeck::SkeletonJoints::HandRight;
        case Infinadeck::SmoothLocomotionReference::Head:
      return Infinadeck::SkeletonJoints::Head;
  }
}
```

For compatibility with applications that weren't developed for the Infinadeck, the Runtime can tap into "smooth locomotion" control systems to allow the user to walk around. In order to do this, the Runtime must be able to tell which device is being used for reference; this will be either the left hand, right hand, or head-mounted display. This function can be used to tell the runtime which joint corresponds to each of these devices.

### SetOffset
```c++
INFINADECK_TRACKING_EXPORT void SetOffset(Infinadeck::TrackingVector3 offset, Infinadeck::SkeletonJoints joint) {
  /*...*/
}
```

Some systems may require the position of a joint to be offset.

Offset will be given in Infinadeck coordinate system.
### GetConfigFilePath
```c++
INFINADECK_TRACKING_EXPORT void GetConfigFilePath(char* path_buffer) {
  const char* config_path = "path_to_file\\config.json";
  strcpy_s(path_buffer, PATH_BUFFER_MAX_LENGTH, config_path);
}
```

Depending on how complicated your plugin is, you may require the uses of various configuration and calibration parameters, for example center position or enabling certain features. If you wish to use a plugin, use this function to notify the Runtime where to find it. To do this, copy the path including the file location relative to the base directory of the plugin plus the file name into the provided buffer, which is guaranteed to be of length ```PATH_BUFFER_MAX_LENGTH = 256```. The config file must be formatted as an JSON file. For example:

```JSON
{
    "mode": 0,
    "center": {
        "x": -0.5,
        "y": 0.25,
        "z": 0.05
    }
}
```

### GetCenterPositionInVRSpace
```c++
INFINADECK_TRACKING_EXPORT Infinadeck::TrackingVector3 GetCenterPositionInVRSpace() {
  return { 1.0, 2.0, 3.0 };
}
```

Your plugin may invlolve locating the center of the treadmill within the VR tracking space. For example, if you are using SteamVR with HTC Vive Trackers, you will need to find the difference between the center of the tracking space and the center of the treadmill. Use this function to tell the Runtime the position of the center within the VR tracking space.

Position should be given in Infinadeck coordinate system.

### GetCenterRotationInVRSpace
```c++
INFINADECK_TRACKING_EXPORT Infinadeck::TrackingVector4 GetCenterRotationInVRSpace() {
  return { 1.0, 0.0, 0.0, 0.0 };
}
```

Your plugin may invlolve locating the center of the treadmill within the VR tracking space. For example, if you are using SteamVR with HTC Vive Trackers, you will need to know the rotation of the treadmill within the tracking space. Use this function to tell the Runtime the rotation of the center within the VR tracking space.

Rotation should be given in Infinadeck coordinate system.

## Skeleton

Users are tracked via the ```Infinadeck::Skeleton``` object. This consists of ```Skeleton::JointCount = 27``` joints, defined as:
| Joint       | Parent                                                            |
|-------------|-------------------------------------------------------------------|
| ```Infinadeck::SkeletonJoints::Pelvis``` | None |
| ```Infinadeck::SkeletonJoints::Navel``` | ```Infinadeck::SkeletonJoints::Pelvis``` |
| ```Infinadeck::SkeletonJoints::Chest``` | ```Infinadeck::SkeletonJoints::Navel``` |
| ```Infinadeck::SkeletonJoints::ClavicleLeft``` | ```Infinadeck::SkeletonJoints::Chest``` |
| ```Infinadeck::SkeletonJoints::ShoulderLeft``` | ```Infinadeck::SkeletonJoints::ClavicleLeft``` |
| ```Infinadeck::SkeletonJoints::ElbowLeft``` | ```Infinadeck::SkeletonJoints::ShoulderLeft``` |
| ```Infinadeck::SkeletonJoints::WristLeft``` | ```Infinadeck::SkeletonJoints::ElbowLeft``` |
| ```Infinadeck::SkeletonJoints::HandLeft``` | ```Infinadeck::SkeletonJoints::WristLeft``` |
| ```Infinadeck::SkeletonJoints::FingersLeft``` | ```Infinadeck::SkeletonJoints::HandLeft``` |
| ```Infinadeck::SkeletonJoints::ThumbLeft``` | ```Infinadeck::SkeletonJoints::HandLeft``` |
| ```Infinadeck::SkeletonJoints::ClavicleRight``` | ```Infinadeck::SkeletonJoints::Chest``` |
| ```Infinadeck::SkeletonJoints::ShoulderRight``` | ```Infinadeck::SkeletonJoints::ClavicleRight``` |
| ```Infinadeck::SkeletonJoints::ElbowRight``` | ```Infinadeck::SkeletonJoints::ShoulderRight``` |
| ```Infinadeck::SkeletonJoints::WristRight``` | ```Infinadeck::SkeletonJoints::ElbowRight``` |
| ```Infinadeck::SkeletonJoints::HandRight``` | ```Infinadeck::SkeletonJoints::WristRight``` |
| ```Infinadeck::SkeletonJoints::FingersRight``` | ```Infinadeck::SkeletonJoints::HandRight``` |
| ```Infinadeck::SkeletonJoints::ThumbRight``` | ```Infinadeck::SkeletonJoints::HandRight``` |
| ```Infinadeck::SkeletonJoints::Neck``` | ```Infinadeck::SkeletonJoints::Chest``` |
| ```Infinadeck::SkeletonJoints::Head``` | ```Infinadeck::SkeletonJoints::Neck``` |
| ```Infinadeck::SkeletonJoints::HipLeft``` | ```Infinadeck::SkeletonJoints::Pelvis``` |
| ```Infinadeck::SkeletonJoints::KneeLeft``` | ```Infinadeck::SkeletonJoints::HipLeft``` |
| ```Infinadeck::SkeletonJoints::AnkleLeft``` | ```Infinadeck::SkeletonJoints::KneeLeft``` |
| ```Infinadeck::SkeletonJoints::FootLeft``` | ```Infinadeck::SkeletonJoints::AnkleLeft``` |
| ```Infinadeck::SkeletonJoints::HipRight``` | ```Infinadeck::SkeletonJoints::Pelvis``` |
| ```Infinadeck::SkeletonJoints::KneeRight``` | ```Infinadeck::SkeletonJoints::HipRight``` |
| ```Infinadeck::SkeletonJoints::AnkleRight``` | ```Infinadeck::SkeletonJoints::KneeRight``` |
| ```Infinadeck::SkeletonJoints::FootRight``` | ```Infinadeck::SkeletonJoints::AnkleRight``` |

Each joint consists of ```poition```, ```rotation```, ```velocity```, ```acceleration```,  and an optional ```name```. ```position``` and ```rotation``` should always be set for each tracked joint, while ```velociy``` and ```acceleration``` may be optionally set.

Any points that are being tracked should have ```is_tracked``` set to ```true```.

Each joint has a confidence value, by default set to ```1```. If the tracking plugin provides its own confidence values, they can be set using ```SetConfidence()``` to a value between 0 and 1, inclusive.
## Infinadeck Coordinate System

The Infinadeck system defines the X-axis as being parallel to the treadmill X-axis the Y-axis being parallel to the treadmill Y-Axis, and the Z-axis as pointing up from the ground.

<img src="https://raw.githubusercontent.com/Infinadeck/InfinadeckTrackingSDK/main/InfinadeckCoordinateSystem.png">
