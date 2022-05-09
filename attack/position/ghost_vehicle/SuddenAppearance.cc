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

#include <vasp/attack/position/ghost_vehicle/SuddenAppearance.h>
#include <vasp/messages/BasicSafetyMessage_m.h>
#include <vasp/utils/SupportFunctions.h>

namespace vasp {
namespace attack {
namespace position {

SuddenAppearance::SuddenAppearance(veins::BasicSafetyMessage const* rvBsm)
{
    // get target's safety distance
    auto const targetSafetyDistance = utils::getSafetyDistance(rvBsm->getSenderSpeed());
    // put ghost within target's safety distance
    auto const ghostVehicleOffset = targetSafetyDistance - 0.1;
    // calculate attacker's position w.r.t. target's safety distance
    posOffset_ = utils::getPosOffset(rvBsm, ghostVehicleOffset);
}

void SuddenAppearance::attack(veins::BasicSafetyMessage* bsm)
{
    bsm->setAttackType("SuddenAppearance");
    bsm->setData("ghost");
    bsm->setSenderPos(posOffset_);
    bsm->setSenderSpeed(veins::Coord::ZERO);
}

} // namespace position
} // namespace attack
} // namespace vasp