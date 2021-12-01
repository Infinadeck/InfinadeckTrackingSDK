// Copyright 2021 Infinadeck
#ifndef INFINADECK_SDK_TYPES_H_
#define INFINADECK_SDK_TYPES_H_

#include <string>
#include <stdexcept>

namespace Infinadeck {

    struct TrackingVector3 {
        double x;
        double y;
        double z;
        double& operator[](int i) {
            switch (i) {
            case 0:
                return x;
            case 1:
                return y;
            case 2:
                return z;
            default:
                throw std::invalid_argument("Index must be between 0 and 2");
            }
        }

        TrackingVector3 operator-(TrackingVector3& rhs) {
            TrackingVector3 result;
            result.x = x - rhs.x;
            result.y = y - rhs.y;
            result.z = z - rhs.z;
            return result;
        }
    };

    struct TrackingVector4 {
        double w;
        double x;
        double y;
        double z;
        double& operator[](int i) {
            switch (i) {
            case 0:
                return w;
            case 1:
                return x;
            case 2:
                return y;
            case 3:
                return z;
            default:
                throw std::invalid_argument("Index must be between 0 and 3");
            }
        }
    };

    enum class SmoothLocomotionReference : int {
        HandLeft,
        HandRight,
        Head
    };

    enum class SkeletonJoints: int {
        Pelvis,
        Navel,
        Chest,
        ClavicleLeft,
        ShoulderLeft,
        ElbowLeft,
        WristLeft,
        HandLeft,
        FingersLeft,
        ThumbLeft,
        ClavicleRight,
        ShoulderRight,
        ElbowRight,
        WristRight,
        HandRight,
        FingersRight,
        ThumbRight,
        Neck,
        Head,
        HipLeft,
        KneeLeft,
        AnkleLeft,
        FootLeft,
        HipRight,
        KneeRight,
        AnkleRight,
        FootRight,
        JointCount,
        NoJoint = -1
    };

    static const SkeletonJoints joint_parents[] = {
        SkeletonJoints::NoJoint, // Pelvis,
        SkeletonJoints::Pelvis, // Navel,
        SkeletonJoints::Navel, // Chest,
        SkeletonJoints::Chest, // ClavicleLeft,
        SkeletonJoints::ClavicleLeft, // ShoulderLeft,
        SkeletonJoints::ShoulderLeft, // ElbowLeft,
        SkeletonJoints::ElbowLeft, // WristLeft,
        SkeletonJoints::WristLeft, // HandLeft,
        SkeletonJoints::HandLeft, // FingersLeft,
        SkeletonJoints::HandLeft, // ThumbLeft,
        SkeletonJoints::Chest, // ClavicleRight,
        SkeletonJoints::ClavicleRight, // ShoulderRight,
        SkeletonJoints::ShoulderRight, // ElbowRight,
        SkeletonJoints::ElbowRight, // WristRight,
        SkeletonJoints::WristRight, // HandRight,
        SkeletonJoints::HandRight, // FingersRight,
        SkeletonJoints::HandRight, // ThumbRight,
        SkeletonJoints::Chest, // Neck,
        SkeletonJoints::Neck, // Head,
        SkeletonJoints::Pelvis, // HipLeft,
        SkeletonJoints::HipLeft, // KneeLeft,
        SkeletonJoints::KneeLeft, // AnkleLeft,
        SkeletonJoints::AnkleLeft, // FootLeft,
        SkeletonJoints::Pelvis, // HipRight,
        SkeletonJoints::HipRight, // KneeRight,
        SkeletonJoints::KneeRight, // AnkleRight,
        SkeletonJoints::AnkleRight // FootRight,
    };

    const int SkeletonJointCount = static_cast<int>(SkeletonJoints::JointCount);

    /// <summary>
    /// Gives the integer index of the given joint's parent
    /// </summary>
    /// <param name="child_joint"></param>
    /// <returns>The integer index of the given joint's parent</returns>
    inline int GetParentJointIndex(SkeletonJoints child_joint) {
        return static_cast<int>(joint_parents[static_cast<int>(child_joint)]);
    }

    /// <summary>
    /// Gives the identifier of the given joint's parent
    /// </summary>
    /// <param name="child_joint"></param>
    /// <returns>The identifier of the given joint's parent</returns>
    inline SkeletonJoints GetParentJoint(SkeletonJoints child_joint) {
        return joint_parents[static_cast<int>(child_joint)];
    }

    struct Joint {
        TrackingVector3 position = { 0 };
        TrackingVector3 velocity = { 0 };
        TrackingVector3 acceleration = { 0 };
        TrackingVector4 rotation = { 1,0,0,0 };
        bool is_tracked = false;
        bool tracking_loss = false;
        static const int name_buffer_length = 32;
        char name[name_buffer_length];
    };

    struct Skeleton{
        Joint joints[static_cast<int>(SkeletonJoints::JointCount)];
        inline Joint& GetJoint(SkeletonJoints joint) {
            return joints[static_cast<int>(joint)];
        }
    };
}
#endif