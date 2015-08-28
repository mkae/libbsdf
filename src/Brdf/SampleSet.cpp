// =================================================================== //
// Copyright (C) 2014-2015 Kimura Ryo                                  //
//                                                                     //
// This Source Code Form is subject to the terms of the Mozilla Public //
// License, v. 2.0. If a copy of the MPL was not distributed with this //
// file, You can obtain one at http://mozilla.org/MPL/2.0/.            //
// =================================================================== //

#include <libbsdf/Brdf/SampleSet.h>

#include <algorithm>
#include <iostream>

#include <libbsdf/Common/Utility.h>
#include <libbsdf/Common/SpectrumUtility.h>

using namespace lb;

SampleSet::SampleSet(int        numAngles0,
                     int        numAngles1,
                     int        numAngles2,
                     int        numAngles3,
                     ColorModel colorModel,
                     int        numWavelengths)
                     : equalIntervalAngles0_(false),
                       equalIntervalAngles1_(false),
                       equalIntervalAngles2_(false),
                       equalIntervalAngles3_(false),
                       oneSide_(false)
{
    assert(numAngles0 > 0 && numAngles1 > 0 && numAngles2 > 0 && numAngles3 > 0);

    resizeAngles(numAngles0, numAngles1, numAngles2, numAngles3);

    colorModel_ = colorModel;

    if (colorModel == SPECTRAL_MODEL) {
        resizeWavelengths(numWavelengths);
    }
    else if (colorModel == MONOCHROMATIC_MODEL) {
        resizeWavelengths(1);
        wavelengths_ = Arrayf::Zero(1);
    }
    else {
        resizeWavelengths(3);
        wavelengths_ = Arrayf::Zero(3);
    }
}

void SampleSet::updateAngleAttributes()
{
    updateEqualIntervalAngles();
    updateOneSide();
}

void SampleSet::resizeAngles(int numAngles0,
                             int numAngles1,
                             int numAngles2,
                             int numAngles3)
{
    assert(numAngles0 > 0 && numAngles1 > 0 && numAngles2 > 0 && numAngles3 > 0);

    numAngles0_ = numAngles0;
    numAngles1_ = numAngles1;
    numAngles2_ = numAngles2;
    numAngles3_ = numAngles3;

    int numSamples = numAngles0 * numAngles1 * numAngles2 * numAngles3;
    spectra_.resize(numSamples);

    angles0_.resize(numAngles0);
    angles1_.resize(numAngles1);
    angles2_.resize(numAngles2);
    angles3_.resize(numAngles3);
}

void SampleSet::resizeWavelengths(int numWavelengths)
{
    assert(numWavelengths > 0);

    int numSamples = numAngles0_ * numAngles1_ * numAngles2_ * numAngles3_;

    for (int i = 0; i < numSamples; ++i) {
        Spectrum sp;
        sp.resize(numWavelengths);
        spectra_.at(i) = sp;
    }

    wavelengths_.resize(numWavelengths);
}

void SampleSet::updateEqualIntervalAngles()
{
    equalIntervalAngles0_ = isEqualInterval(angles0_);
    equalIntervalAngles1_ = isEqualInterval(angles1_);
    equalIntervalAngles2_ = isEqualInterval(angles2_);
    equalIntervalAngles3_ = isEqualInterval(angles3_);

    std::cout << "[SampleSet::updateEqualIntervalAngles] Angle0: " << equalIntervalAngles0_ << std::endl;
    std::cout << "[SampleSet::updateEqualIntervalAngles] Angle1: " << equalIntervalAngles1_ << std::endl;
    std::cout << "[SampleSet::updateEqualIntervalAngles] Angle2: " << equalIntervalAngles2_ << std::endl;
    std::cout << "[SampleSet::updateEqualIntervalAngles] Angle3: " << equalIntervalAngles3_ << std::endl;
}

void SampleSet::updateOneSide()
{
    bool contain_0_PI = false;
    bool contain_PI_2PI = false;

    for (int i = 0; i < angles3_.size(); ++i) {
        float angle = angles3_[i];

        if (angle > 0.0f && angle < PI_F) {
            contain_0_PI = true;
        }

        if (angle > PI_F && angle < 2.0f * PI_F) {
            contain_PI_2PI = true;
        }
    }

    oneSide_ = (!contain_0_PI || !contain_PI_2PI);

    std::cout << "[SampleSet::updateOneSide] " << oneSide_ << std::endl;
}
