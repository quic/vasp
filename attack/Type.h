/*
 * MIT License
 *
 * Copyright (c) 2022 Qualcomm Innovation Center, Inc., SPDX-License-Identifier: MIT
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is furnished to do
 * so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * Project: V2X Application Spoofing Platform (VASP)
 * Author: Raashid Ansari
 * Email: quic_ransari@quicinc.com
 */

#pragma once

namespace vasp {
namespace attack {
enum Type {
    _kAttackMinValue = -1,
    // No attacks
    kAttackNo,

    // Position attacks (self telemetry based)
    kAttackRandomPosition,
    kAttackRandomPositionOffset,
    kAttackConstantPositionOffset,
    kAttackPlaygroundConstantPosition,
    kAttackSuddenDisappearance,

    // Position attacks (ghost vehicle based)
    kAttackSuddenAppearance,
    kAttackTargetedConstantPosition,

    // Mobility attacks (ghost vehicle based)
    kAttackCommRangeBraking,

    // Channel attacks
    kAttackDenialOfService,

    // EEBL-specific attacks (ghost vehicle based)
    kAttackFakeEEBLJustAttack,
    kAttackFakeEEBLStopPositionUpdateAfterAttack,

    // IMA-specific attacks
    kAttackIMAPosOffset,
    kAttackIMAJunctionPos,
    kAttackIMAHighSpeed,
    kAttackIMALowSpeed,
    kAttackIMAHighAcceleration,
    kAttackIMALowAcceleration,

    // Dimension attacks
    kAttackHighDimension,
    kAttackLowDimension,
    kAttackRandomDimension,
    kAttackRandomDimensionOffset,
    kAttackConstantDimensionOffset,
    kAttackBadRatioDimension,

    // Length attacks
    kAttackHighLength,
    kAttackLowLength,
    kAttackRandomLength,
    kAttackRandomLengthOffset,
    kAttackConstantLengthOffset,
    kAttackBadRatioLength,

    // Width attacks
    kAttackHighWidth,
    kAttackLowWidth,
    kAttackRandomWidth,
    kAttackRandomWidthOffset,
    kAttackConstantWidthOffset,
    kAttackBadRatioWidth,

    // Heading attacks
    kAttackOppositeHeading,
    kAttackPerpendicularHeading,
    kAttackRotatingHeading,
    kAttackConstantHeading,
    kAttackRandomHeading,
    kAttackRandomHeadingOffset,
    kAttackConstantHeadingOffset,

    // Yaw-rate attacks
    kAttackHighYawRate,
    kAttackLowYawRate,
    kAttackConstantYawRate,
    kAttackRandomYawRate,
    kAttackRandomYawRateOffset,
    kAttackConstantYawRateOffset,

    // Heading and Yaw-rate matching attacks
    kAttackHighHeadingYawRate,
    kAttackLowHeadingYawRate,
    kAttackConstantHeadingYawRate,
    kAttackRandomHeadingYawRate,
    kAttackRandomHeadingYawRateOffset,
    kAttackConstantHeadingYawRateOffset,

    // Acceleration attacks
    kAttackHighAcceleration,
    kAttackLowAcceleration,
    kAttackConstantAcceleration,
    kAttackRandomAcceleration,
    kAttackRandomAccelerationOffset,
    kAttackConstantAccelerationOffset,

    // Speed attacks
    kAttackHighSpeed,
    kAttackLowSpeed,
    kAttackConstantSpeed,
    kAttackRandomSpeed,
    kAttackRandomSpeedOffset,
    kAttackConstantSpeedOffset,

    _kAttackMaxValue
};
} // namespace attack
} // namespace vasp