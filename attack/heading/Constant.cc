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

#include <vasp/attack/heading/Constant.h>
#include <vasp/messages/BasicSafetyMessage_m.h>

namespace vasp {
namespace attack {
namespace heading {

void Constant::update(veins::Heading const& prevHeading, simtime_t_cref prevBeaconTime)
{
    prevHeading_ = prevHeading;
    prevBeaconTime_ = prevBeaconTime;
}

void Constant::attack(veins::BasicSafetyMessage* bsm)
{
    switch (type_) {
    case kHyraTypeHeading:
        bsm->setAttackType("ConstantHeading");
        bsm->setHeading(veins::Heading(M_PI_2));
        break;
    case kHyraTypeYawRate: {
        bsm->setAttackType("ConstantYawRate");
        bsm->setYawRate(M_PI_2);
        break;
    }
    case kHyraTypeBoth: {
        bsm->setAttackType("ConstantHeadingYawRate");
        double constexpr kYawRate{0};
        bsm->setYawRate(kYawRate);

        veins::Heading newHeading{getNewHeading(prevHeading_, kYawRate, prevBeaconTime_)};
        bsm->setHeading(newHeading);
        break;
    }
    }
}

} // namespace heading
} // namespace attack
} // namespace vasp