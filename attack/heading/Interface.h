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

#include <omnetpp/simtime_t.h>
#include <vasp/attack/Interface.h>
#include <vasp/attack/heading/Type.h>

namespace veins {
class Heading;
} // namespace veins

namespace vasp {
namespace attack {
namespace heading {
class Interface : public attack::Interface {
public:
    virtual void attack(veins::BasicSafetyMessage* bsm) = 0;
    void setType(Type const type)
    {
        type_ = type;
    }

protected:
    veins::Heading const getNewHeading(veins::Heading const& prevHeading, double const yawRate, omnetpp::simtime_t_cref prevBeaconTime) const;

protected:
    Type type_;
};
} // namespace heading
} // namespace attack
} // namespace vasp