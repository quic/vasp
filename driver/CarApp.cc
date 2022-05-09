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

#include <CSVWriter.h>
#include <vasp/connection/Manager.h>
#include <vasp/driver/CarApp.h>
#include <vasp/logging/TraceManager.h>
#include <vasp/messages/BasicSafetyMessage_m.h>

// V2X Applications
#include <vasp/safetyapps/EEBL.h>
#include <vasp/safetyapps/IMA.h>

// attacks
#include <vasp/attack/Type.h>
#include <vasp/attack/dimension/Type.h>
// self telemetry based attacks
#include <vasp/attack/acceleration/Constant.h>
#include <vasp/attack/acceleration/ConstantOffset.h>
#include <vasp/attack/acceleration/High.h>
#include <vasp/attack/acceleration/Low.h>
#include <vasp/attack/acceleration/Random.h>
#include <vasp/attack/acceleration/RandomOffset.h>
#include <vasp/attack/channel/DenialOfService.h>
#include <vasp/attack/dimension/BadRatio.h>
#include <vasp/attack/dimension/ConstantOffset.h>
#include <vasp/attack/dimension/High.h>
#include <vasp/attack/dimension/Low.h>
#include <vasp/attack/dimension/Random.h>
#include <vasp/attack/dimension/RandomOffset.h>
#include <vasp/attack/position/self_telemetry/ConstantOffset.h>
#include <vasp/attack/position/self_telemetry/PlaygroundConstantPosition.h>
#include <vasp/attack/position/self_telemetry/Random.h>
#include <vasp/attack/position/self_telemetry/RandomOffset.h>
#include <vasp/attack/position/self_telemetry/SuddenDisappearance.h>
#include <vasp/attack/safetyapp/ima/HighAcceleration.h>
#include <vasp/attack/safetyapp/ima/HighSpeed.h>
#include <vasp/attack/safetyapp/ima/JunctionPosition.h>
#include <vasp/attack/safetyapp/ima/LowAcceleration.h>
#include <vasp/attack/safetyapp/ima/LowSpeed.h>
#include <vasp/attack/safetyapp/ima/PositionOffset.h>

namespace vasp {
namespace driver {

Define_Module(CarApp);

void CarApp::initialize(int stage)
{
    DemoBaseApplLayer::initialize(stage);

    if (stage == 0) {
        attackType_ = par("attackType");
        maliciousProbability_ = attackType_ == attack::kAttackNo ? 0.0 : par("maliciousProbability");
        bsmData_ = par("bsmData").stdstringValue();
        simRunID_ = par("runID").stdstringValue();
        resultDir_ = par("resultDir").stdstringValue();
        mapFile_ = par("mapFile").stdstringValue();
    }

    if (stage == 1) {
        world_ = veins::FindModule<veins::BaseWorldUtility*>::findGlobalModule();
        traceManager_ = veins::FindModule<logging::TraceManager*>::findGlobalModule();

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

        isMalicious_ = maliciousProbability_ >= dblrand();

        // only initialize attack if malicious
        if (!isMalicious_) {
            return;
        }
        posAttackOffset_ = par("posAttackOffset");
        accelerationAttackOffset_ = par("accelerationAttackOffset");
        nDosMessages_ = par("nDosMessages");
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

        if (isMalicious_) {
                injectAttack(hvBsm);

            prevBeaconTime_ = simTime();
            if (attackType_ != attack::kAttackSuddenDisappearance) {
                prevHvHeading_ = hvBsm->getHeading();
                sendDown(hvBsm);
            }
        }
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

void CarApp::injectAttack(veins::BasicSafetyMessage* hvBsm)
{
    using namespace vasp::attack;

    if (generatedBSMs == 0) {
        prevHvHeading_ = hvBsm->getHeading();
    }

    // Select attack according to the attackType_
    // do nothing if NoAttacks or any one of the ghost attacks is selected
    switch (attackType_) {
    case attack::kAttackPlaygroundConstantPosition: {
        attack_ = std::make_unique<position::PlaygroundConstantPosition>(world_);
        break;
    }
    case attack::kAttackConstantPositionOffset: {
        attack_ = std::make_unique<position::ConstantOffset>(posAttackOffset_);
        break;
    }
    case attack::kAttackRandomPosition: {
        attack_ = std::make_unique<position::Random>(world_);
        break;
    }
    case attack::kAttackRandomPositionOffset: {
        attack_ = std::make_unique<position::RandomOffset>(posAttackOffset_);
        break;
    }
    case attack::kAttackSuddenDisappearance: {
        attack_ = std::make_unique<position::SuddenDisappearance>();
        break;
    }
    case attack::kAttackDenialOfService: {
        attack_ = std::make_unique<channel::DenialOfService>(beaconInterval, nDosMessages_);
        break;
    }
    case attack::kAttackIMAPosOffset: {
        attack_ = std::make_unique<safetyapp::ima::PositionOffset>(approachingIntersection_);
        break;
    }
    case attack::kAttackIMAJunctionPos: {
        attack_ = std::make_unique<safetyapp::ima::JunctionPosition>(approachingIntersection_, junctionPos_);
        break;
    }
    case attack::kAttackIMAHighSpeed: {
        attack_ = std::make_unique<safetyapp::ima::HighSpeed>(approachingIntersection_);
        break;
    }
    case attack::kAttackIMALowSpeed: {
        attack_ = std::make_unique<safetyapp::ima::LowSpeed>(approachingIntersection_);
        break;
    }
    case attack::kAttackIMAHighAcceleration: {
        attack_ = std::make_unique<safetyapp::ima::HighAcceleration>(approachingIntersection_);
        break;
    }
    case attack::kAttackIMALowAcceleration: {
        attack_ = std::make_unique<safetyapp::ima::LowAcceleration>(approachingIntersection_);
        break;
    }
    // Dimension attacks
    case attack::kAttackHighDimension: {
        auto highDimension = std::make_unique<dimension::High>();
        highDimension->setType(dimension::kDimensionAttackTypeBoth);
        attack_ = std::move(highDimension);
        break;
    }
    case attack::kAttackLowDimension: {
        auto lowDimension = std::make_unique<dimension::Low>();
        lowDimension->setType(dimension::kDimensionAttackTypeBoth);
        attack_ = std::move(lowDimension);
        break;
    }
    case attack::kAttackRandomDimension: {
        auto randomDimension = std::make_unique<dimension::Random>();
        randomDimension->setType(dimension::kDimensionAttackTypeBoth);
        attack_ = std::move(randomDimension);
        break;
    }
    case attack::kAttackRandomDimensionOffset: {
        auto randomDimensionOffset = std::make_unique<dimension::RandomOffset>();
        randomDimensionOffset->setType(dimension::kDimensionAttackTypeBoth);
        attack_ = std::move(randomDimensionOffset);
        break;
    }
    case attack::kAttackConstantDimensionOffset: {
        auto constantDimensionOffset = std::make_unique<dimension::ConstantOffset>();
        constantDimensionOffset->setType(dimension::kDimensionAttackTypeBoth);
        attack_ = std::move(constantDimensionOffset);
        break;
    }
    case attack::kAttackBadRatioDimension: {
        auto badRatioDimension = std::make_unique<dimension::BadRatio>();
        badRatioDimension->setType(dimension::kDimensionAttackTypeBoth);
        attack_ = std::move(badRatioDimension);
        break;
    }
    // Length attacks
    case attack::kAttackHighLength: {
        auto highLength = std::make_unique<dimension::High>();
        highLength->setType(dimension::kDimensionAttackTypeLength);
        attack_ = std::move(highLength);
        break;
    }
    case attack::kAttackLowLength: {
        auto lowLength = std::make_unique<dimension::Low>();
        lowLength->setType(dimension::kDimensionAttackTypeLength);
        attack_ = std::move(lowLength);
        break;
    }
    case attack::kAttackRandomLength: {
        auto randomLength = std::make_unique<dimension::Random>();
        randomLength->setType(dimension::kDimensionAttackTypeLength);
        attack_ = std::move(randomLength);
        break;
    }
    case attack::kAttackRandomLengthOffset: {
        auto randomLengthOffset = std::make_unique<dimension::RandomOffset>();
        randomLengthOffset->setType(dimension::kDimensionAttackTypeLength);
        attack_ = std::move(randomLengthOffset);
        break;
    }
    case attack::kAttackConstantLengthOffset: {
        auto constantLengthOffset = std::make_unique<dimension::ConstantOffset>();
        constantLengthOffset->setType(dimension::kDimensionAttackTypeLength);
        attack_ = std::move(constantLengthOffset);
        break;
    }
    case attack::kAttackBadRatioLength: {
        auto badRatioLength = std::make_unique<dimension::BadRatio>();
        badRatioLength->setType(dimension::kDimensionAttackTypeLength);
        attack_ = std::move(badRatioLength);
        break;
    }
    // Width attacks
    case attack::kAttackHighWidth: {
        auto highWidth = std::make_unique<dimension::High>();
        highWidth->setType(dimension::kDimensionAttackTypeWidth);
        attack_ = std::move(highWidth);
        break;
    }
    case attack::kAttackLowWidth: {
        auto lowWidth = std::make_unique<dimension::Low>();
        lowWidth->setType(dimension::kDimensionAttackTypeWidth);
        attack_ = std::move(lowWidth);
        break;
    }
    case attack::kAttackRandomWidth: {
        auto randomWidth = std::make_unique<dimension::Random>();
        randomWidth->setType(dimension::kDimensionAttackTypeWidth);
        attack_ = std::move(randomWidth);
        break;
    }
    case attack::kAttackRandomWidthOffset: {
        auto randomWidthOffset = std::make_unique<dimension::RandomOffset>();
        randomWidthOffset->setType(dimension::kDimensionAttackTypeWidth);
        attack_ = std::move(randomWidthOffset);
        break;
    }
    case attack::kAttackConstantWidthOffset: {
        auto constantWidthOffset = std::make_unique<dimension::ConstantOffset>();
        constantWidthOffset->setType(dimension::kDimensionAttackTypeWidth);
        attack_ = std::move(constantWidthOffset);
        break;
    }
    case attack::kAttackBadRatioWidth: {
        auto badRatioWidth = std::make_unique<dimension::BadRatio>();
        badRatioWidth->setType(dimension::kDimensionAttackTypeWidth);
        attack_ = std::move(badRatioWidth);
        break;
    }
    case attack::kAttackHighAcceleration: {
        attack_ = std::make_unique<acceleration::High>();
        break;
    }
    case attack::kAttackLowAcceleration: {
        attack_ = std::make_unique<acceleration::Low>();
        break;
    }
    case attack::kAttackConstantAcceleration: {
        attack_ = std::make_unique<acceleration::Constant>();
        break;
    }
    case attack::kAttackRandomAcceleration: {
        attack_ = std::make_unique<acceleration::Random>();
        break;
    }
    case attack::kAttackRandomAccelerationOffset: {
        attack_ = std::make_unique<acceleration::RandomOffset>(accelerationAttackOffset_);
        break;
    }
    case attack::kAttackConstantAccelerationOffset: {
        attack_ = std::make_unique<acceleration::ConstantOffset>(accelerationAttackOffset_);
        break;
    }
    }

    if (attack_) {
        attack_->attack(hvBsm);
    }
}

void CarApp::onBSM(veins::DemoSafetyMessage* dsm)
{
    auto rvBsm = dynamic_cast<veins::BasicSafetyMessage*>(dsm);
    if (rvBsm == nullptr) {
        return;
    }

    simtime_t const rvBsmReceiveTime{simTime()};
    executeV2XApplications(rvBsm);
    writeTrace(rvBsm, rvBsmReceiveTime);
}

void CarApp::writeTrace(veins::BasicSafetyMessage const* rvBsm, simtime_t_cref rvBsmReceiveTime)
{
    auto hvBsm = std::make_unique<veins::BasicSafetyMessage>();
    populateWSM(hvBsm.get());

    traceManager_->logTrace(rvBsm, hvBsm.get(), rvBsmReceiveTime, eeblWarning_, imaWarning_);
}

void CarApp::executeV2XApplications(veins::BasicSafetyMessage const* rvBsm)
{
    // EEBL
    vasp::safetyapps::EEBL eebl{};
    eeblWarning_ = eebl.warning(
        rvBsm,
        mobility->getPositionAt(simTime()),
        mobility->getHeading(),
        mobility->getHostSpeed(),
        myId);

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