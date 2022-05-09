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

import veins.base.utils.Coord;
import vasp.messages.Heading;
import veins.base.utils.SimpleAddress;
import veins.modules.messages.DemoSafetyMessage;

namespace veins;

packet BasicSafetyMessage extends veins::DemoSafetyMessage {
    int msgCount;
	double msgGenerationTime;
    veins::LAddress::L2Type address = -1;
	double acceleration;
	Heading heading;
	string attackType = "Genuine";
	double width = 2.0; //meters
	double length = 5.0; //meters
	double height = 1.8; //meters
	veins::LAddress::L2Type recipientId = -1;
	double yawRate;
	string data;

	// Vehicle Safety Extensions
	bool eventHardBraking = false;
}
