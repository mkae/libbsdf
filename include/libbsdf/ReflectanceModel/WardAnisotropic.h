// =================================================================== //
// Copyright (C) 2015-2016 Kimura Ryo                                  //
//                                                                     //
// This Source Code Form is subject to the terms of the Mozilla Public //
// License, v. 2.0. If a copy of the MPL was not distributed with this //
// file, You can obtain one at http://mozilla.org/MPL/2.0/.            //
// =================================================================== //

#ifndef LIBBSDF_WARD_ANISOTROPIC_H
#define LIBBSDF_WARD_ANISOTROPIC_H

#include <libbsdf/Common/Global.h>
#include <libbsdf/ReflectanceModel/ReflectanceModel.h>

namespace lb {

/*! Ward anisotropic reflectance model. */
class WardAnisotropic : public ReflectanceModel
{
public:
    WardAnisotropic(float roughnessX,
                    float roughnessY)
                    : roughnessX_(roughnessX),
                      roughnessY_(roughnessY)
    {
        parameters_["Roughness X"] = &roughnessX_;
        parameters_["Roughness Y"] = &roughnessY_;
    }

    static float compute(const Vec3&    L,
                         const Vec3&    V,
                         const Vec3&    N,
                         const Vec3&    T,
                         const Vec3&    B,
                         float          roughnessX,
                         float          roughnessY);

    float getValue(const Vec3& inDir, const Vec3& outDir) const
    {
        const Vec3 N = Vec3(0.0, 0.0, 1.0);
        const Vec3 T = Vec3(1.0, 0.0, 0.0);
        const Vec3 B = Vec3(0.0, -1.0, 0.0);

        return compute(inDir, outDir, N, T, B, roughnessX_, roughnessY_);
    }

    bool isIsotropic() const { return false; }

    std::string getName() const { return "Ward anisotropic"; }

    std::string getDescription() const
    {
        std::string reference("Gregory J. Ward, \"Measuring and modeling anisotropic reflection,\" Computer Graphics (SIGGRAPH '92 Proceedings), pp. 265-272, July 1992.");
        return reference;
    }

private:
    float roughnessX_;
    float roughnessY_;
};

/*
 * Implementation
 */

inline float WardAnisotropic::compute(const Vec3&   L,
                                      const Vec3&   V,
                                      const Vec3&   N,
                                      const Vec3&   T,
                                      const Vec3&   B,
                                      float         roughnessX,
                                      float         roughnessY)
{
    using std::acos;
    using std::exp;
    using std::sqrt;
    using std::tan;

    float dotLN = L.dot(N);
    float dotVN = V.dot(N);

    Vec3 H = (L + V).normalized();
    float dotHN = H.dot(N);
    float dotHT = H.dot(T);
    float dotHB = H.dot(B);

    float sqDotHT = (dotHT / roughnessX) * (dotHT / roughnessX);
    float sqDotHB = (dotHB / roughnessY) * (dotHB / roughnessY);

    float brdf = 1.0f / sqrt(dotLN * dotVN)
               * exp(-2.0f * (sqDotHT + sqDotHB) / (1.0f + dotHN))
               / (4.0f * PI_F * roughnessX * roughnessY);
    return brdf;
}

} // namespace lb

#endif // LIBBSDF_WARD_ANISOTROPIC_H
