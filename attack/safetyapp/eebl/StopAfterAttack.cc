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

#include <vasp/attack/safetyapp/eebl/StopAfterAttack.h>
#include <vasp/messages/BasicSafetyMessage_m.h>
#include <veins/base/utils/Coord.h>
#include <vasp/utils/SupportFunctions.h>

namespace vasp {
namespace attack {
namespace safetyapp {
namespace eebl {

bool StopAfterAttack::attackFlag_ = false;

StopAfterAttack::StopAfterAttack(veins::BasicSafetyMessage const* rvBsm)
{
    if (!attackFlag_) {
        return;
    }

    auto constexpr ghostVehicleOffset = 0.2; // put ghost just within target's safety distance
    // calculate ghost vehicle's position w.r.t. target's safety distance
    ghostPos_ = utils::getPosOffset(rvBsm, ghostVehicleOffset);
    attackFlag_ = false;
}

void StopAfterAttack::attack(veins::BasicSafetyMessage* bsm)
{
    bsm->setAttackType("FakeEEBLStopPositionUpdateAfterAttack");
    bsm->setSenderSpeed(veins::Coord::ZERO); // creates a speed object with speed = 0
    bsm->setEventHardBraking(true);
    bsm->setSenderPos(ghostPos_);

    // since FakeEEBL is sent, send the same position as that at previous transmission
    auto constexpr kXAccel = 4.6;
    bsm->setAcceleration(kXAccel);
}

} // namespace eebl
} // namespace safetyapp
} // namespace attack
} // namespace vasp