// =================================================================== //
// Copyright (C) 2014-2015 Kimura Ryo                                  //
//                                                                     //
// This Source Code Form is subject to the terms of the Mozilla Public //
// License, v. 2.0. If a copy of the MPL was not distributed with this //
// file, You can obtain one at http://mozilla.org/MPL/2.0/.            //
// =================================================================== //

/*!
 * \file    Utility.h
 * \brief   Utility functions for libbsdf.
 */

#ifndef LIBBSDF_UTILITY_H
#define LIBBSDF_UTILITY_H

#include <libbsdf/Common/Vector.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace lb {

const float PI_F = 3.14159265358979323846f;
const float PI_2_F = 1.57079632679489661923f;
const float EPSILON_F = std::numeric_limits<float>::epsilon();

/*! \brief Clamps a value between a minimum and maximum value. */
template <typename T>
T clamp(T value, T minValue, T maxValue);

/*! \brief Copies an array. */
template <typename SrcT, typename DestT>
void copy(const SrcT& srcArray, DestT& destArray, int size);

/*! \brief Copies an array. */
template <typename SrcT, typename DestT>
void copy(const SrcT& srcArray, DestT& destArray);

/*! \brief Returns true if two values are nearly equal. */
template <typename T>
bool isEqual(T lhs, T rhs);

/*! \brief Computes linearly-interpolated values. */
template <typename T>
T lerp(const T& lhs, const T& rhs, float weight);

/*! \brief Computes a specular direction. */
template <typename Vec3T>
Vec3T reflect(const Vec3T& inDir, const Vec3T& normalDir);

/*! \brief Converts a value from radian to degree. */
template <typename T>
T toDegree(T radian);

/*! \brief Converts a value from degree to radian. */
template <typename T>
T toRadian(T degree);

/*! \brief Converts an array from degree to radian. */
template <typename T>
T toRadians(const T& degrees);

/*! \brief Returns true if the elements of an array are equally-spaced intervals. */
template <typename T>
bool isEqualInterval(const T& array);

/*! \brief Fixes a direction if the Z-component is negative. */
template <typename Vec3T>
void fixDownwardDir(Vec3T& dir);

/* Implementation */

template <typename T>
inline T clamp(T value, T minValue, T maxValue)
{
    using std::min;
    using std::max;
    return max(minValue, min(maxValue, value));
}

template <typename SrcT, typename DestT>
inline void copy(const SrcT& srcArray, DestT& destArray, int size)
{
    for (int i = 0; i < size; ++i) {
        destArray[i] = srcArray[i];
    }
}

template <typename SrcT, typename DestT>
inline void copy(const SrcT& srcArray, DestT& destArray)
{
    int i = 0;
    for (auto it = srcArray.begin(); it != srcArray.end(); ++it, ++i) {
        destArray[i] = *it;
    }
}

template <typename T>
inline bool isEqual(T lhs, T rhs)
{
    using std::abs;
    return (abs(lhs - rhs) <= std::numeric_limits<T>::epsilon() * abs(lhs + rhs));
}

template <typename T>
inline T lerp(const T& lhs, const T& rhs, float weight)
{
    return lhs + weight * (rhs - lhs);
}

template <typename Vec3T>
inline Vec3T reflect(const Vec3T& inDir, const Vec3T& normalDir)
{
    typedef typename Vec3T::Scalar ScalarType;
    return static_cast<ScalarType>(2.0) * normalDir.dot(inDir) * normalDir - inDir;
}

template <typename T>
inline T toDegree(T radian)
{
    return radian / static_cast<T>(PI_F)* static_cast<T>(180.0);
}

template <typename T>
inline T toRadian(T degree)
{
    return degree / static_cast<T>(180.0) * static_cast<T>(PI_F);
}

template <typename T>
inline T toRadians(const T& degrees)
{
    typedef typename T::Scalar ScalarType;
    return degrees / static_cast<ScalarType>(180.0) * static_cast<ScalarType>(PI_F);
}

template <typename T>
inline bool isEqualInterval(const T& array)
{
    if (array.size() <= 1) return false;

    float interval = array[array.size() - 1] / (array.size() - 1);
    for (int i = 0; i < array.size(); ++i) {
        if (!isEqual(array[i], interval * i)) {
            return false;
        }
    }

    return true;
}

template <typename Vec3T>
inline void fixDownwardDir(Vec3T& dir)
{
    if (dir[2] < 0.0) {
        dir[2] = 0.0;
        if (dir[0] == 0.0 && dir[1] == 0.0) {
            dir[0] = 1.0;
        }
        else {
            dir.normalize();
        }
    }
}

}  // namespace lb

#endif // LIBBSDF_UTILITY_H