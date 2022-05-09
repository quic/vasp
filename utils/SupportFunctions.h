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

#include <cmath>
#include <veins/base/utils/Coord.h>
#include <veins/base/utils/Heading.h>
#include <vasp/messages/BasicSafetyMessage_m.h>
#include <vasp/utils/SupportFunctions.h>

namespace vasp {
namespace utils {

inline double toPositiveAngle(double angle)
{
    angle = std::fmod(angle, 360);
    while (angle < 0)
        angle += 360.0;
    return angle;
}

inline bool isBehind(veins::Coord const& p0, veins::Coord const& p2, veins::Heading const& direction)
{
    double const angle{toPositiveAngle(direction.getRad() * 180 / M_PI)};

    //======================================================================================================================
    //  Given a directed line from point p0(x0, y0) to p1(x1, y1),
    //  the following condition tells whether a point p2(x2, y2) is
    //  on the left of the line, on the right, or on the same line:
    //
    //  value = (x1 - x0)*(y2 - y0) - (y1 - y0)*(x2 - x0)
    //
    //  if value > 0, p2 is on the left side of the line.
    //  if value = 0, p2 is on the same line.
    //  if value < 0, p2 is on the right side of the line.
    //  https://math.stackexchange.com/questions/175896/finding-a-point-along-a-line-a-certain-distance-away-from-another-point
    //======================================================================================================================

    // east bound
    if (angle == 0 || angle == 360) {
        // choose point perpendicularly below x axis
        veins::Coord const p1{p0.x, p0.y + 5};
        return (p1.x - p0.x) * (p2.y - p0.y) - (p1.y - p0.y) * (p2.x - p0.x) > 0;
    }
    // north-east bound
    else if (angle > 0 && angle < 90) {
        // choose point on positive y axis
        veins::Coord const p1{0, p0.y};
        return (p1.x - p0.x) * (p2.y - p0.y) - (p1.y - p0.y) * (p2.x - p0.x) > 0;
    }
    // north bound
    else if (angle == 90) {
        // choose point perpendicularly left of y axis
        veins::Coord const p1{p0.x - 5, p0.y};
        return (p1.x - p0.x) * (p2.y - p0.y) - (p1.y - p0.y) * (p2.x - p0.x) > 0;
    }
    // north-west bound
    else if (angle > 90 && angle < 180) {
        // choose point on positive y axis
        veins::Coord const p1{0, p0.y};
        return (p1.x - p0.x) * (p2.y - p0.y) - (p1.y - p0.y) * (p2.x - p0.x) < 0;
    }
    // west bound
    else if (angle == 180) {
        // choose point perpendicularly above x axis
        veins::Coord const p1{p0.x, p0.y + 5};
        return (p1.x - p0.x) * (p2.y - p0.y) - (p1.y - p0.y) * (p2.x - p0.x) < 0;
    }
    // south-west bound
    else if (angle > 180 && angle < 270) {
        // choose point on negative y axis
        veins::Coord const p1{0, p0.y};
        return (p1.x - p0.x) * (p2.y - p0.y) - (p1.y - p0.y) * (p2.x - p0.x) > 0;
    }
    // south bound
    else if (angle == 270) {
        // choose point perpendicularly right of y-axis
        veins::Coord const p1{p0.x + 5, p0.y};
        return (p1.x - p0.x) * (p2.y - p0.y) - (p1.y - p0.y) * (p2.x - p0.x) > 0;
    }
    // south-east bound
    else if (angle > 270 && angle < 360) {
        // choose point on negative y-axis
        veins::Coord const p1{0, p0.y};
        return (p1.x - p0.x) * (p2.y - p0.y) - (p1.y - p0.y) * (p2.x - p0.x) < 0;
    }
    return false;
}

inline double getSafetyDistance(veins::Coord const& speed)
{
    // https://en.wikipedia.org/wiki/Braking_distance#Total_stopping_distance
    // D_total = D_perceptionToReaction + D_braking
    //         = v*t_perceptionToReaction + v^2/2*mu*g
    // common baseline value for t_perceptionToReaction = 1.5s, mu = 0.7, g = 9.8m/s^2
    // i.e. 2*mu*g = 13.72931 m/s^2

    auto const rmsSpeed = speed.length(); // m/s
    auto constexpr timeBetweenPerceptionToReaction = 1.5; // seconds
    auto constexpr mu = 0.7; // friction coefficient
    auto constexpr g = 9.8; // m/s^2
    auto const distanceTraveledBetweenPerceptionToReaction = rmsSpeed * timeBetweenPerceptionToReaction; // m
    auto const distanceTraveledDuringBraking = (rmsSpeed * rmsSpeed) / (2 * mu * g); // m

    return distanceTraveledBetweenPerceptionToReaction + distanceTraveledDuringBraking;
}

inline veins::Coord getPosOffset(veins::BasicSafetyMessage const* rvBsm, double const& offset)
{
    veins::Coord const heading{rvBsm->getHeading().toCoord()};
    veins::Coord const pos{rvBsm->getSenderPos()};
    return pos + heading * offset; // equivalent to return attacker_pos + (distance + attacker_pos.distance(target_pos)) * u;
}

} // namespace utils
} // namespace vasp