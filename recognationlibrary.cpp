
#include "recognationlibrary.h"
#include <QString>


RecognationLibrary::RecognationLibrary()
{
}

std::string RecognationLibrary::getName() const
{
    return std::string("Square Detection");
}

std::string RecognationLibrary::getVersion() const
{

    return std::string("1.0.3");
}

extern "C" RECOGNATIONLIBRARY_EXPORT IRecognation *getPlugin()
{
    static RecognationLibrary * lib = new RecognationLibrary();
    return lib;
}
