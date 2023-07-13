#pragma once
#include <pybind11/pybind11.h>
#include <pybind11/embed.h>
#include <pybind11/numpy.h>
#include "RecognationLibrary_global.h"
#include "IRecognation.h"
#include <QString>
#include <QDebug> 
#include <memory>

namespace py = pybind11;

class RecognationLibrary: public IRecognation
{
signals:
    void frameProcessed(cv::Mat* frame) override;

public slots:
    void setFrame(cv::Mat* frame) override;
    void setStatus(bool status) override;

private:
    bool detection;
    cv::Mat* modifiedFrame;
    std::vector<DetectionInfo> detectionInfo;
    cv::Mat calib_image;
    bool resized = false;
    bool beep_status = false;
    bool calib_status = true;
    bool freed = false;
    //Image info
    int width;
    int height;
    int channels;
    int png_size;
    int margin;
    int png_x;
    int png_y;

    //QMutex mutex_;
    QMutex* data_lock;
    QWaitCondition cond_;
    cv::Mat frame_;
    bool frame_available_ = false;
    bool running = false;
    cv::Mat processedFrame;
    float fps = 30;
    int CalibrationTime = 5;
    bool initialize;

public:
    
    std::shared_ptr<py::module> script;
    std::shared_ptr<py::object> class_obj;
    std::shared_ptr<py::object> py_instance;
    std::shared_ptr<py::function> process_frame;
    std::shared_ptr<py::function> initializeTracking;
    std::shared_ptr<py::function> getBeepStatus;
    std::shared_ptr<py::function> getCalibStatus;

    RecognationLibrary();

    void set_Mutex(QMutex* lock) override;
    void run() override;

    std::string getName() const override;
    std::string getVersion() const override;
    ~RecognationLibrary() override {
        qDebug() << "Destructor Called reclib";
        py::finalize_interpreter();
    }
    void setRunning(bool run) override;
    void calibrate(bool init, int CalibTime) override;
    void processFrame(const cv::Mat& frame) override;
    void add_calib_image(const cv::Mat& img);
    void plotOneBox(const cv::Rect& box, const cv::Mat& img, const cv::Scalar& color, const std::string& label, int lineThickness);
    void initialize_Tracking(int CalibrationTime) override;
    void free_Resources() override;
    bool get_Beep_Status() override;

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

