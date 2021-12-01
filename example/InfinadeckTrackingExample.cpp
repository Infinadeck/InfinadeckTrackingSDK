#include "InfinadeckTrackingSDK.h"
#include "InfinadeckSDKTypes.h"

#define MODULE_NAME "ExamplePlugin"
#define MODULE_PUBLISHER "ExamplePublisher"
#define X_POSITION 1.1
#define Y_POSITION 1.2
#define Z_POSITION 1.3
#define W_ROTATION 1
#define X_ROTATION 0
#define Y_ROTATION 0
#define Z_ROTATION 0

static Infinadeck::Skeleton body;
INFINADECK_TRACKING_EXPORT void Initialize(void* system) {
	Infinadeck::SystemInit(system);
	for (int i = 0; i < Infinadeck::SkeletonJointCount; i++) {
		body.joints[i].position = { X_POSITION, Y_POSITION, Z_POSITION };
		body.joints[i].rotation = { W_ROTATION, X_ROTATION, Y_ROTATION, Z_ROTATION };
	}
	Infinadeck::Log("Example plugin created");
}

INFINADECK_TRACKING_EXPORT const char* GetModuleName() {
	return MODULE_NAME;
}

INFINADECK_TRACKING_EXPORT const char* GetPublisherName() {
	return MODULE_PUBLISHER;
}

INFINADECK_TRACKING_EXPORT Infinadeck::Skeleton GetUserSkeleton() {
	return body;
}

INFINADECK_TRACKING_EXPORT void Finalize() {
	for (int i = 0; i < Infinadeck::SkeletonJointCount; i++) {
		body.joints[i].position = { 0 };
		body.joints[i].rotation = { 0 };
	}
}

INFINADECK_TRACKING_EXPORT int GetTrackedPointCount() {
	return Infinadeck::SkeletonJointCount;
}

INFINADECK_TRACKING_EXPORT bool GetTrackingLoss() {
	return false;
}

INFINADECK_TRACKING_EXPORT Infinadeck::SkeletonJoints GetPrimaryJointID() {
	return Infinadeck::SkeletonJoints::Pelvis;
}

INFINADECK_TRACKING_EXPORT void CalibrateUser() {
	/* 
	Run user calibration 
	*/
}

INFINADECK_TRACKING_EXPORT void CalibrateCenter() {
	/* 
	Run center calibration 
	*/
}

INFINADECK_TRACKING_EXPORT void Refresh() {
	// Update user position and rotation
	for (int i = 0; i < Infinadeck::SkeletonJointCount; i++) {
		body.joints[i].position = { X_POSITION, Y_POSITION, Z_POSITION };
		body.joints[i].rotation = { W_ROTATION, X_ROTATION, Y_ROTATION, Z_ROTATION };
	}
}

INFINADECK_TRACKING_EXPORT Infinadeck::SkeletonJoints GetSmoothLocomotionReferenceJoint(Infinadeck::SmoothLocomotionReference reference) {
	switch (reference) {
	case Infinadeck::SmoothLocomotionReference::HandLeft:
		return Infinadeck::SkeletonJoints::HandLeft;
	case Infinadeck::SmoothLocomotionReference::HandRight:
		return Infinadeck::SkeletonJoints::HandRight;
	case Infinadeck::SmoothLocomotionReference::Head:
		return Infinadeck::SkeletonJoints::Head;
	default:
		return Infinadeck::SkeletonJoints::HandLeft;
	}
}

INFINADECK_TRACKING_EXPORT void SetOffset(Infinadeck::TrackingVector3 offset, Infinadeck::SkeletonJoints joint) {
	for (int i = 0; i < 3; i++) {
		body.GetJoint(joint).position[i] = offset[i];
	}
}