#include "logger.h"

namespace Example::Infra {

    Logger::Logger(std::string format)
        : Format_(std::move(format)) {
    }

    void Logger::Info(std::string_view message) {
        time_t currentTime;
        struct tm* timeInfo;
        char timeBuffer[80];

        time(&currentTime);
        timeInfo = localtime(&currentTime);

        // Format the time as a string
        strftime(timeBuffer, sizeof(timeBuffer), "%Y-%m-%d %H:%M:%S", timeInfo);

        // Print the time and the message
        printf(Format_.data(), timeBuffer, "INFO", message.data());
    }

}
