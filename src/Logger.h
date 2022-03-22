//
// Created by jeremy on 2022-03-22.
//

#ifndef CAPSTONE_FIRMWARE_LOGGER_H
#define CAPSTONE_FIRMWARE_LOGGER_H


#include <Arduino.h>


enum LogLevel {
    Info,
    Warn,
    Error
};


struct Logger {
    String name;

    explicit Logger(String _name) : name(_name) { }

    void PrintLog(LogLevel level, String message)
    {
        String tag;
        switch (level) {
            case Info:
                tag = "INFO ";
                break;
            case Warn:
                tag = "WARN ";
                break;
            case Error:
                tag = "ERROR";
                break;
            default:
                tag = "     ";
        }
        String msg = String(millis()) + "\t[" + tag + "]\t" + name + ": " + message;
        Serial.println(msg);
    }

};


#endif //CAPSTONE_FIRMWARE_LOGGER_H
