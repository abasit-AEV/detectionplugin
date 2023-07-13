#pragma once
#include <opencv2/core/core.hpp>
#include <string>
#include <vector>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QDebug>

/**
 * @brief Structure to hold information about a detection.
 */
struct DetectionInfo {
    std::vector<cv::Point> detectedCoordinates;  ///< Detected coordinates of the object.
    bool frameChanged;                           ///< Indicates if the frame has changed.
    bool shouldShow;                             ///< Indicates if the frame should be shown.
    bool shouldSave;                             ///< Indicates if the frame should be saved.
    cv::Mat* modifiedFrame;                      ///< Pointer to the modified frame (nullptr if frame not changed).
};

/**
 * @brief Abstract base class for a video plugin.
 *
 * This class defines the interface for a video plugin that processes frames from a video source and provides
 * detection information to the main application.
 */
class IRecognation : public QThread {
    Q_OBJECT //  Q_OBJECT macro to enable signals and slots

signals:
    virtual void frameProcessed(cv::Mat* frame);

public slots:
    virtual void setFrame(cv::Mat* frame) = 0;

    virtual void setStatus(bool status) = 0;

public:
    
    virtual void set_Mutex(QMutex* lock) = 0;
    /**
     * @brief Destructor.
     */
    virtual ~IRecognation() {
        qDebug() << "Destructor Called IR";
    }

    virtual void run() override = 0;

    virtual void setRunning(bool run) = 0;

    virtual void calibrate(bool init, int CalibTime) = 0;
        
    /**
     * @brief Get the name of the plugin.
     *
     * @return The name of the plugin.
     */
    virtual std::string getName() const = 0;

    /**
     * @brief Get the version of the plugin.
     *
     * @return The version of the plugin.
     */
    virtual std::string getVersion() const = 0;

    /**
     * @brief Process a frame from the video source.
     *
     * This method is called by the main application to process a frame from the video source.
     *
     * @param frame The frame to process.
     */
    virtual void processFrame(const cv::Mat& frame) = 0;

    /**
     * @brief Check if the frame should be saved.
     *
     * This method is called by the main application to determine if the current frame should be saved.
     *
     * @return void.
     */

    virtual void initialize_Tracking(int CalibrationTime) = 0;
    
    /**
    * @brief Initialize Tracking for the designated Calibration Time.
    *
    * This method is called by the main application Initialize tracking for the calibration time to reset detections.
    *
    * @return void.
    */

    virtual void free_Resources() = 0;

    virtual bool get_Beep_Status() = 0;

    /**
   * @brief Return Beep Status.
   *
   * This method is called by the main application to sound a beep upon Fish Detection.
   *
   * @return bool.
   */
    virtual bool shouldSaveFrame() const = 0;

    /**
     * @brief Check if the frame should be shown.
     *
     * This method is called by the main application to determine if the current frame should be displayed.
     *
     * @return True if the frame should be shown, false otherwise.
     */
    virtual bool shouldShowFrame() const = 0;

    /**
     * @brief Check if any detection has occurred.
     *
     * This method is called by the main application to determine if any detection has occurred.
     *
     * @return True if a detection has occurred, false otherwise.
     */
    virtual bool isDetected() const = 0;

    /**
     * @brief Get the detection information.
     *
     * This method is called by the main application to get the detection information.
     *
     * @return A vector of DetectionInfo objects containing the detection information.
     */
    virtual std::vector<DetectionInfo> getDetectionInfo() const = 0;
    
};
