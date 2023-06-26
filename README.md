# detectionplugin
Example library for image recognation

# Video Plugin Usage Guide

This guide provides instructions on how to use and implement a video plugin in your application.

## Using the Plugin

To use the video plugin in your application, follow these steps:

1. Make sure you have the necessary dependencies installed. The plugin is built using C++ and requires the OpenCV library.

2. Load the plugin dynamically in your application using platform-specific functions or libraries. The process typically involves providing the path to the plugin shared object file (e.g., DLL file on Windows, SO file on Linux) and obtaining a handle or pointer to the loaded library.

3. Access the plugin's functionality through the defined interface. The plugin provides methods for processing video frames and retrieving detection information. The main application can use these methods to check for frame changes, get detected coordinates, and make decisions about showing or saving frames.

4. Implement the necessary logic in your application to handle the detection information provided by the plugin. For example, you can decide whether to save frames based on the detection results or update the user interface to highlight the detected coordinates.

5. Clean up and release resources when you're done using the plugin. This includes deleting the plugin instance and unloading the library.

## Implementing a Video Plugin

To implement a video plugin, follow these steps:

1. Define an interface for the plugin. The interface should include methods for processing video frames, retrieving detection information, and providing metadata such as the plugin name and version.

2. Implement the video plugin class by inheriting from the interface and providing the necessary functionality. The implementation should include frame processing logic, detection algorithms, and any other functionality required by the plugin.

3. Build the plugin as a shared object library (DLL file on Windows, SO file on Linux) using the appropriate build system and compiler. Make sure to link against the necessary dependencies, such as the OpenCV library.

4. Distribute the plugin shared object file along with any required dependencies. Provide documentation on how to use the plugin, including the expected interface, methods, and any specific requirements or considerations.

## Example Plugin

As an example, we provide a simple plugin implementation called `SimplePlugin`. The plugin demonstrates basic detection functionality and provides detection information through the defined interface.

You can find the example plugin code and usage instructions in the following repository: [Example Video Plugin](https://github.com/example-plugin-repo)

## Plugin Interface

The video plugin interface provides the following methods:

### `std::string getName() const`

Returns the name of the plugin as a string.

### `std::string getVersion() const`

Returns the version of the plugin as a string.

### `void processFrame(const cv::Mat& frame)`

Processes a video frame. The frame parameter is an input frame in OpenCV's `cv::Mat` format.

### `bool isDetected() const`

Checks if a frame has been detected for any changes. Returns `true` if a change is detected, `false` otherwise.

### `std::vector<DetectionInfo> getDetections() const`

Returns a vector of `DetectionInfo` objects containing information about the detected frames. Each `DetectionInfo` object provides the detected coordinates, the modified frame (if applicable), and flags indicating whether the frame should be shown or saved.

## Conclusion

Using a video plugin in your application allows you to extend its functionality by adding custom video processing and detection capabilities. By following the provided guide, you can effectively use and implement video plugins in your application.

If you encounter any issues or have further questions, feel free to reach out to our support team for assistance.

