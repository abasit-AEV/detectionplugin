#include <iostream>
#include <opencv2/opencv.hpp>
#include "recognationlibrary.h"

#include "IRecognation.h"



int main() {
    // Create an instance of the plugin
    IRecognation* plugin = new RecognationLibrary();

     
    std::string name = plugin->getName();
    std::string version = plugin->getVersion();
    bool save = plugin->shouldShowFrame();

    std::cout << "Name: " << name << std::endl;
    std::cout << "Version: " << version << std::endl;
    std::cout << "Save: " << save << std::endl;

    cv::VideoCapture video("D:/2aa.mp4");
    while (true) {
        // Read the next frame
        cv::Mat frame;
        video >> frame;
        if (frame.empty()) {
            break;
        }

        // Process the frame using the plugin
        plugin->processFrame(frame);

        // Display the frame
        cv::imshow("Frame", frame);
        cv::waitKey(1);

        // Exit if the user presses 'q'
        if (cv::waitKey(1) == 'q') {
            break;
        }
    }

    return 0;
}
