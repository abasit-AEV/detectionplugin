#pragma once
#include "RecognationLibrary_global.h"
#include "IRecognation.h"


class RecognationLibrary: public IRecognation
{
private:
    bool detection;
    std::vector<DetectionInfo> detectionInfo;
public:
    RecognationLibrary();
    std::string getName() const override;
    std::string getVersion() const override;
    //~RecognationLibrary(){}
    void processFrame(const cv::Mat& frame) override {
        // Perform frame processing and detection here

        // Simulate detection for demonstration purposes
        detection = true;

        // Create a dummy detection info
        DetectionInfo info;
        info.detectedCoordinates = { cv::Point(100, 100), cv::Point(200, 200) };
        info.frameChanged = true;
        info.shouldShow = true;
        info.shouldSave = true;
        info.modifiedFrame = nullptr;

        detectionInfo.push_back(info);
    }

    bool shouldSaveFrame() const override {
        // Simulate decision to save frame for demonstration purposes
        return true;
    }

    bool shouldShowFrame() const override {
        // Simulate decision to show frame for demonstration purposes
        return true;
    }

    bool isDetected() const override {
        return detection;
    }

    std::vector<DetectionInfo> getDetectionInfo() const override {
        return detectionInfo;
    }
};

//using pluginGetter = RecognationLibrary *();
typedef IRecognation* (*pluginGetter)();

extern "C" RECOGNATIONLIBRARY_EXPORT IRecognation * getPlugin();

