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

#include <veins/base/utils/Coord.h>
#include <veins/base/utils/Heading.h>
#include <vasp/messages/BasicSafetyMessage_m.h>
#include <vasp/safetyapps/EEBL.h>
#include <vasp/utils/SupportFunctions.h>

namespace vasp {
namespace safetyapps {

// EEBL application implemented as defined in SAE J2945/1 standard
bool EEBL::warning(
    veins::BasicSafetyMessage const* bsm,
    veins::Coord const& myPos,
    veins::Heading const& myDirection,
    veins::Coord const& mySpeed,
    int const myId)
{
    if (!bsm->getEventHardBraking()) {
        return false;
    }

    auto isSenderBehind = utils::isBehind(myPos, bsm->getSenderPos(), myDirection);
    if (isSenderBehind) {
        return false;
    }

    // J2945/1 mentions that an EEBL is raised "if the distance between the vehicles is less than an
    // implementation-specific threshold value." Here the threshold value is the normal stopping
    // distance based on speed.
    if (myPos.distance(bsm->getSenderPos()) > utils::getSafetyDistance(mySpeed)) {
        return false;
    }

    return true;
}

} // namespace safetyapps
} // namespace vasp
