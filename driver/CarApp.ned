//
// MIT License
//
// Copyright (c) 2022 Qualcomm Innovation Center, Inc., SPDX-License-Identifier: MIT
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of
// this software and associated documentation files (the "Software"), to deal in
// the Software without restriction, including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
// of the Software, and to permit persons to whom the Software is furnished to do
// so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
// Project: V2X Application Spoofing Platform (VASP)
// Author: Raashid Ansari
// Email: quic_ransari@quicinc.com
//

package vasp.driver;
import org.car2x.veins.modules.application.ieee80211p.DemoBaseApplLayer;

simple CarApp extends DemoBaseApplLayer
{
    parameters:
        @display("i=block/app2");
        @class(vasp::driver::CarApp);

        double sporadicInsertionRate = default(0.0);

        string mapFile;
        double maliciousProbability;

        string resultDir = default("results");
        string runID;
        string bsmData = default("genuine"); // gives knowledge of car type on receiving a BSM

        int attackType = default(0);	// 0 - no attack

        int beaconPriority = default(1);
        int nDosMessages = default(1);

        double posAttackOffset @unit(m) = default(10m);
        double dimensionAttackOffset @unit(m) = default(4m);
        double accelerationAttackOffset = default(2);
}
