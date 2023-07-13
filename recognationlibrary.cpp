
#include "recognationlibrary.h"
#include <opencv2/opencv.hpp>
#include <fstream>
#include <Windows.h>

void RecognationLibrary::set_Mutex(QMutex* lock) {
    data_lock = lock; 
}

RecognationLibrary::RecognationLibrary() : running(false), frame_available_(false) {
    try {
        py::initialize_interpreter();       
              
        // Assign Python objects and functions to shared pointers
        script = std::make_shared<py::module>(py::module::import("track_v7"));
        class_obj = std::make_shared<py::object>(script->attr("FishDetection"));
        py_instance = std::make_shared<py::object>((*class_obj)());
        process_frame = std::make_shared<py::function>(py_instance->attr("detect").cast<py::function>());
        initializeTracking = std::make_shared<py::function>(py_instance->attr("initializeTracking").cast<py::function>());
        getBeepStatus = std::make_shared<py::function>(py_instance->attr("get_beep_status").cast<py::function>());
        getCalibStatus = std::make_shared<py::function>(py_instance->attr("get_calib_status").cast<py::function>());

    }
    catch (const py::error_already_set& e) {
        qDebug() << "Python error:" << e.what();
        std::ofstream logfile("error.log", std::ios_base::app); // open log file in append mode
        if (logfile.is_open()) {
            logfile << "Python error: " << e.what() << std::endl; // write error message to log file
            logfile.close(); // close log file
        }
    }
    // Load the PNG image with transparency
    calib_image = cv::imread("calib.png", cv::IMREAD_UNCHANGED);
    resized = false;
    qDebug() << "Library Initialized" ;
}

void RecognationLibrary::run() {
    while (!isInterruptionRequested()) {
        if (running) {
            data_lock->lock();
            if (!frame_available_) {
                cond_.wait(data_lock);
            }
            cv::Mat frame = frame_.clone();
            frame_available_ = false;
            data_lock->unlock();

            // Initialize Strong Sort Tracking on restart
            if (initialize) {
                initialize_Tracking(CalibrationTime);
                initialize = false;
            }

            processFrame(frame);
            frame.copyTo(processedFrame);

            cvtColor(processedFrame, processedFrame, cv::COLOR_BGR2RGB);
            // Return processedFrame
            emit frameProcessed(&processedFrame);
            //Beep if there is a new detection
            beep_status = get_Beep_Status();
            if (beep_status) {
                Beep(1000, 100);
            }
            // Sleep this thread for 5 ms
            msleep(5);
        }
        else {
            initialize = true;
            msleep(10);
        }
    }
}

void RecognationLibrary::frameProcessed(cv::Mat* frame) {
    emit frameProcessed(frame); // Emit the signal
}

void RecognationLibrary::setFrame(cv::Mat* frame) {

    data_lock->lock();
    if (frame != nullptr) {
        frame_ = *frame;
        frame_available_ = true;
        cond_.wakeOne();
    }
    else {
        frame_available_ = false;
        qDebug() << "Null Frame";
    }
    data_lock->unlock();
}


void RecognationLibrary::setStatus(bool status) {
    running = status;
}

void RecognationLibrary::plotOneBox(const cv::Rect& box,const cv::Mat& img, const cv::Scalar& color = cv::Scalar(), const std::string& label = "", int lineThickness = 3) {
    int tl = lineThickness > 0 ? lineThickness : cvRound(0.002 * (img.rows + img.cols) / 2) + 1;
    cv::Scalar boxColor = color.val[0] == 0 && color.val[1] == 0 && color.val[2] == 0
        ? cv::Scalar(std::rand() % 256, std::rand() % 256, std::rand() % 256)
        : color;
    cv::rectangle(img, box, boxColor, tl, cv::LINE_AA);

    bool drawLabel = !label.empty();
    if (drawLabel) {
        int fontThickness = MAX(tl - 1, 1);
        int fontScale = tl / 3;
        cv::Size textSize = cv::getTextSize(label, cv::FONT_HERSHEY_SIMPLEX, fontScale, fontThickness, nullptr);
        cv::Point labelBottomLeft(box.x, box.y - textSize.height - 3);
        cv::Point labelTopRight(box.x + textSize.width, box.y);
        cv::rectangle(img, labelBottomLeft, labelTopRight, boxColor, cv::FILLED, cv::LINE_AA);
        cv::putText(img, label, cv::Point(box.x, box.y - 2), cv::FONT_HERSHEY_SIMPLEX, fontScale,
            cv::Scalar(225, 255, 255), fontThickness, cv::LINE_AA);
    }
}

void RecognationLibrary::setRunning(bool run) {
    running = run;
}
void RecognationLibrary::calibrate(bool init, int CalibTime) {
     initialize = init; 
     CalibrationTime = CalibTime; 
}

void RecognationLibrary::initialize_Tracking(int CalibrationTime) {
    if (!freed) {
        qDebug() << "Calibration Time: " << CalibrationTime;
        //initializeTracking(CalibrationTime);
        (*initializeTracking)(CalibrationTime);
    }
}

bool RecognationLibrary::get_Beep_Status() {
    if (!freed) {        
        beep_status = (*getBeepStatus)().cast<bool>();
        return beep_status;
    }
    else return false;
}

void RecognationLibrary::add_calib_image(const cv::Mat& img) {
    if (!resized) {
        // Get the size of the image
        width = img.cols;
        height = img.rows;

        // Get the dimensions of the image
        channels = img.channels();

        // Define the size of the PNG image
        png_size = static_cast<int>(MIN(width, height) * 0.1);

        // Define the margin size (relative to the image size)
        margin = static_cast<int>(MIN(width, height) * 0.05);

        // Define the location of the PNG image (top right corner with margin)
        png_x = width - png_size - margin;
        png_y = margin;

        // Resize the PNG image to fit the defined size
        cv::resize(calib_image, calib_image, cv::Size(png_size, png_size));               
        resized = true;
    }
    // Create a region of interest (ROI) for the overlay
    cv::Rect roi(png_x, png_y, png_size, png_size);
    cv::Mat overlay = img(roi);

    // Paste the PNG image onto the overlay
    for (int c = 0; c < channels; c++) {
        for (int i = 0; i < png_size; i++) {
            for (int j = 0; j < png_size; j++) {
                float alpha = calib_image.at<cv::Vec4b>(i, j)[3] / 255.0f;
                overlay.at<cv::Vec3b>(i, j)[c] = static_cast<uint8_t>(
                    calib_image.at<cv::Vec4b>(i, j)[c] * alpha +
                    overlay.at<cv::Vec3b>(i, j)[c] * (1.0f - alpha)
                    );
            }
        }
    }
    // Blend the overlay with the original image
    cv::addWeighted(overlay, 0.5, img(roi), 0.5, 0.0, img(roi));
} 

std::string RecognationLibrary::getName() const
{
    return std::string("Fish Detection");
}

std::string RecognationLibrary::getVersion() const
{
    return std::string("1.0.3");
}

void RecognationLibrary::processFrame(const cv::Mat& frame) {
    // Perform frame processing and detection here
    try{
        if (!freed) {
            
            // Convert the OpenCV Mat to a NumPy array
            py::array_t<uint8_t> np_frame({ frame.rows, frame.cols, 3 }, frame.data);

            //py::gil_scoped_acquire acquire{};
            // Call Python Function detect()            
            py::array_t<uint16_t> bboxes = (*process_frame)(np_frame).cast<py::array_t<uint16_t>>();
            calib_status = (*getCalibStatus)().cast<bool>();          
            
            // Check if any bounding box is detected
            if (bboxes.size() > 0) {
                auto bboxes_info = bboxes.mutable_unchecked<2>();
                // Iterate over each bounding box
                for (ssize_t i = 0; i < bboxes_info.shape(0); i++) {
                    int x1 = static_cast<int>(bboxes_info(i, 0));
                    int y1 = static_cast<int>(bboxes_info(i, 1));
                    int x2 = static_cast<int>(bboxes_info(i, 2));
                    int y2 = static_cast<int>(bboxes_info(i, 3));

                    cv::Rect box(x1, y1, x2 - x1, y2 - y1);
                    // Plot the bounding box
                    plotOneBox(box, frame, cv::Scalar(0, 234, 255), "", 2);
                }
                detection = true;
            }
            else {
                // Handle case when no bounding box is detected
                std::cout << "No bounding box detected." << std::endl;
                // Simulate detection for demonstration purposes
                detection = false;
            }
           
            if (calib_status)   add_calib_image(frame);
                        
            // Release Global Interpreter Lock (Commented for testing)
            //py::gil_scoped_release release{};
        }       
    }
    catch (const py::error_already_set& e) {
        qDebug() << "Python error:" << e.what();
        std::ofstream logfile("error.log", std::ios_base::app); // open log file in append mode
        if (logfile.is_open()) {
            logfile << "Python error: " << e.what() << std::endl; // write error message to log file
            logfile.close(); // close log file
        }
    }
}


void RecognationLibrary::free_Resources() {
    try {
        if (!freed) {
            
            qDebug() << "Freeing Resources";
            script.reset();
            class_obj.reset();
            py_instance.reset();
            process_frame.reset();
            initializeTracking.reset();
            getBeepStatus.reset();
            getCalibStatus.reset();
            qDebug() << "Freed Object";
            // Release the GIL before finalizing the interpreter (Commented for testing)
            //py::gil_scoped_acquire acquire;
            py::finalize_interpreter();
            //py::gil_scoped_release release{};            
            qDebug() << "Freed Resources";
            freed = true;
        }
    }
    catch (const py::error_already_set& e) {
        qDebug() << "Python error:" << e.what();
        std::ofstream logfile("error.log", std::ios_base::app); // open log file in append mode
        if (logfile.is_open()) {
            logfile << "Python error: " << e.what() << std::endl; // write error message to log file
            logfile.close(); // close log file
        }
    }
}

extern "C" RECOGNATIONLIBRARY_EXPORT IRecognation *getPlugin()
{
    static RecognationLibrary * lib = new RecognationLibrary();
    return lib;
}
