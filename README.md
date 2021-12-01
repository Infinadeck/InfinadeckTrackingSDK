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
|```void Initialize(void* system)```       | Called when the plugin is loaded. Plugin must be ready to track after this returns |
|```const char * GetModuleName()```         | Should return the name of the plugin                                              |
|```const char * GetPublisherName()```              | Should return the name of the developer/publisher of the plugin           |
|```Infinadeck::Skeleton GetUserSkeleton()```| Should return a skeleton corresponding to the user's position                    |
|```void Finalize()```                | Called when the plugin is unloaded                                                      |
|```int GetTrackedPointCount()```     | Should return the number of points in the skeleton that are being tracked by the plugin |
|```bool GetTrackingLoss()```          | Should return ```true``` if the plugin determines that the user has lost tracking      |

### Initialize

```c++
void Initialize(void* system) {
  Infinadeck::SystemInit(system);
  /*...*/
}
```

This function will be called once the plugin has been loaded and the Runtime has verified that all of the required functions are present. The parameter ```system``` can be passed into the function ```Infinadeck::SystemInit()``` in order to allow the plugin to access the Runtime's logging system and print messages to it.

Do any work here that is required for the tracking system to initialize and interface with the Runtime. Once this function returns, the Runtime will assume that the plugin is ready, and will begin to read tracking data from it.

### GetModuleName

```c++
const char * GetModuleName() {
  return "Example Plugin";
}
```

### GetPublisherName

```c++
const char * GetPublisherName() {
  return "Infinadeck";
}
```

### GetUserSkeleton

```c++
Infinadeck::Skeleton GetUserSkeleton() {
  Infinadeck::Skeleton user;
  user.GetJoint(SkeletonJoints::Pelvis) = {1,2,3};
  return user;
}
```

This function will be called whenever the Runtime polls the position of the user. This should be as up-to-date as possible, and should be as fast as possible to reduce latency between user movement and treadmill motion.

### Finalize
```c++
void Finalize() {
  /*...*/
}
```

This function will be called when the plugin is unloaded. After this function returns, no more calls will be made to the plugin.

### GetTrackedPointCount
```c++
int GetTrackedPointCount() {
  const int plugin_joint_count = 3;
  return plugin_joint_count;
}
```

This function should return the number of points in the user skeleton that the plugin is actively tracking. This must be no greater than ```Skeleton::JointCount = 27```. 

### GetTrackingLoss
```c++
bool GetTrackingLoss() {
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
|```SkeletonJoints GetPrimaryJointID()```         | Should return the identifier of the primary tracking joint                             |
|```void CalibrateUser()```              | Should run a routine to calibrate the user's skeleton                                           |
|```void CalibrateCenter()```| Should run a routine to calibrate the center of the user's tracking space                                   |
|```void Refresh()```                | Will be called once per iteration of the Runtime main loop                                          |
|```SkeletonJoints GetSmoothLocomotionReferenceJoint(SmoothLocomotionReference reference)``` | Should return the identifier of the joint used for smooth locomotion reference |
|```void SetOffset(Infinadeck::TrackingVector3 offset, SkeletonJoints joint)``` | Will set the offset of the user's skeleton position      |

### GetPrimaryJointID
```c++
SkeletonJoints GetPrimaryJointID() {
  return SkeletonJoints::Pelvis;
}
```

By default, the Runtime will take the pelvis as the user's primary joint, and will favor it during tracking calculations. If your plugin instead uses a different joint as the primary joint, use this function to notify the Runtime

### CalibrateUser
```c++
void CalibrateUser() {
    /* 
  ...
  Run user calibration sequence
  */
}
```

Some plugins may require the user to be calibrated. For example, determining which tracker to use as the primary object, or finding the user in a camera-based system. This function should make any necessary calls to perform these operations. The runtime will call this function whenever prompted to by the user.

### CalibrateCenter
```c++
void CalibrateCenter() {
    /* 
  ...
  Run center calibration system
  */
}
```

Some plugins may require the center of the user's tracking space to be calculated. For example, determining the center of the treadmill within the vr tracking space. This function should make any necessary calls to perform these operations. The runtime will call this function whenever prompted to by the user.

### Refresh

```c++
void Refresh() {
  /*...*/
}
```

This function will be called once per iteration of the Runtime's main loop. Call any functions that need to be continually refreshed here. In addition, if making calls to the tracking system are time-consuming, those should be made here instead of ```GetUserSkeleton()```

### GetSmoothLocomotionReferenceJoint
```c++
SkeletonJoints GetSmoothLocomotionReferenceJoint(SmoothLocomotionReference reference) {
  switch(reference) {
    case SmoothLocomotionReference::LeftHand:
      return SkeletonJoints::LeftHand;
        case SmoothLocomotionReference::RightHand:
      return SkeletonJoints::RightHand;
        case SmoothLocomotionReference::Head:
      return SkeletonJoints::Head;
  }
}
```

For compatibility with applications that weren't developed for the Infinadeck, the Runtime can tap into "smooth locomotion" control systems to allow the user to walk around. In order to do this, the Runtime must be able to tell which device is being used for reference; this will be either the left hand, right hand, or head-mounted display. This function can be used to tell the runtime which joint corresponds to each of these devices.

### SetOffset
```c++
void SetOffset(Infinadeck::TrackingVector3 offset, SkeletonJoints joint) {
  /*...*/
}
```

Some systems may require the position of a joint to be offset.
