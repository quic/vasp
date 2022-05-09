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

#include <vasp/driver/CarApp.h>
#include <vasp/messages/BasicSafetyMessage_m.h>

// V2X Applications
#include <vasp/safetyapps/IMA.h>

namespace vasp {
namespace driver {

Define_Module(CarApp);

void CarApp::initialize(int stage)
{
    DemoBaseApplLayer::initialize(stage);

    if (stage == 0) {
        bsmData_ = par("bsmData").stdstringValue();
        mapFile_ = par("mapFile").stdstringValue();
    }

    if (stage == 1) {

        // Load MAP
        std::ifstream mapFileStream{mapFile_};
        std::stringstream buffer{};
        if (mapFileStream) {
            buffer << mapFileStream.rdbuf();
            mapFileStream.close();
        }
        else {
            std::string errorMsg = "Unable to open map JSON file: \"" + mapFile_ + "\"";
            throw cRuntimeError(errorMsg.c_str());
        }
        mapJson_ = json::parse(buffer);

        // start IMA
        runIMA_ = std::make_shared<cMessage>("runIMA");
        scheduleAt(simTime() + 2, runIMA_.get());
    }
}

void CarApp::finish()
{
    DemoBaseApplLayer::finish();
    cancelEvent(runIMA_.get());
}

void CarApp::handleSelfMsg(cMessage* msg)
{
    if (msg == runIMA_.get()) {
        runIMA();
        scheduleAt(simTime() + 2, runIMA_.get());
    }

    if (msg == sendBeaconEvt) {
        veins::BasicSafetyMessage* hvBsm = new veins::BasicSafetyMessage();
        populateWSM(hvBsm);

        scheduleAt(simTime() + beaconInterval, sendBeaconEvt);
    }

    if (msg == sendWSAEvt) {
        veins::DemoServiceAdvertisment* wsa = new veins::DemoServiceAdvertisment();
        populateWSM(wsa);
        sendDown(wsa);
        scheduleAt(simTime() + wsaInterval, sendWSAEvt);
    }
}

void CarApp::populateWSM(veins::BaseFrame1609_4* wsm, veins::LAddress::L2Type rcvId, int serial)
{
    DemoBaseApplLayer::populateWSM(wsm, rcvId, serial);

    if (veins::BasicSafetyMessage* bsm = dynamic_cast<veins::BasicSafetyMessage*>(wsm)) {
        bsm->setMsgCount(generatedBSMs % 128);
        bsm->setMsgGenerationTime(simTime().dbl());
        bsm->setAddress(myId);
        bsm->setRecipientId(rcvId);
        bsm->setAttackType("Genuine");
        bsm->setData(bsmData_.c_str());
        bsm->setHeading(mobility->getHeading());
        bsm->setYawRate(curYawRate_);
        bsm->setLength(traciVehicle->getLength());
        bsm->setWidth(traciVehicle->getWidth());
        bsm->setHeight(traciVehicle->getHeight());

        double const acceleration{traciVehicle->getAcceleration()};
        bsm->setAcceleration(acceleration);

        // AASHTO defines hard braking as a deceleration greater than 4.5 m/s^2
        double constexpr kDecelerationThreshold{-4.5}; // m/s^2
        bsm->setEventHardBraking(acceleration < kDecelerationThreshold);
    }
}

void CarApp::handlePositionUpdate(cObject* obj)
{
    DemoBaseApplLayer::handlePositionUpdate(obj);

    if (lastUpdate_ == -1.0) {
        lastUpdate_ = simTime();
        return;
    }

    auto const updateInterval{simTime() - lastUpdate_};

    // calculate yaw rate
    auto const curAngleRad{mobility->getHeading().getRad()};
    if (lastAngleRad_ != -1.0) {
        curYawRate_ = (curAngleRad - lastAngleRad_) / updateInterval.dbl();
    }
    lastAngleRad_ = curAngleRad;
    lastUpdate_ = simTime();
}


void CarApp::onBSM(veins::DemoSafetyMessage* dsm)
{
    auto rvBsm = dynamic_cast<veins::BasicSafetyMessage*>(dsm);
    if (rvBsm == nullptr) {
        return;
    }

    executeV2XApplications(rvBsm);
}

void CarApp::executeV2XApplications(veins::BasicSafetyMessage const* rvBsm)
{
    // IMA
    vasp::safetyapps::IMA ima{};
    imaWarning_ = approachingIntersection_ ? ima.warning(curPosition, curSpeed, rvBsm, junctionPos_) : false;
}

void CarApp::runIMA()
{
    auto currentRoad = mobility->getRoadId();

    for (auto& roadObj : mapJson_["roads"]) {
        auto road = roadObj["road"];
        // check if approaching an intersection
        approachingIntersection_ = road["id"] == currentRoad;
        if (approachingIntersection_) {
            // find junctionPos
            auto junction = road["junction"];
            junctionPos_ = veins::Coord(junction["x"], junction["y"]);
        }
    }
}

} // namespace driver
} // namespace vasp