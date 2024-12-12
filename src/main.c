#include <windows.h>
#include <commdlg.h>
#include <stdlib.h>
#include <stdio.h>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

char selectedFile[MAX_PATH] = "";

void obfuscateBatchFile(const char *filePath) {
    char tempFile[MAX_PATH] = "temp.~b64";
    char outputFile[MAX_PATH];
    sprintf(outputFile, "%s___obfuscated.bat", filePath);
    FILE *temp = fopen(tempFile, "w");
    fprintf(temp, "//4mY2xzDQo=\n");
    fclose(temp);
    char cmd[512];
    sprintf(cmd, "certutil.exe -f -decode \"%s\" \"%s\"", tempFile, outputFile);
    system(cmd);
    remove(tempFile);
    sprintf(cmd, "copy /b \"%s\" + \"%s\" \"%s\"", outputFile, filePath, outputFile);
    system(cmd);
}

void centerWindow(HWND hwnd) {
    RECT rect, windowRect;
    GetWindowRect(GetDesktopWindow(), &rect);
    GetWindowRect(hwnd, &windowRect);
    int screenWidth = rect.right - rect.left;
    int screenHeight = rect.bottom - rect.top;
    int windowWidth = windowRect.right - windowRect.left;
    int windowHeight = windowRect.bottom - windowRect.top;
    SetWindowPos(hwnd, NULL, (screenWidth - windowWidth) / 2, (screenHeight - windowHeight) / 2, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}

void createControls(HWND hwnd) {
    HFONT hFont = CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Segoe UI");
    HWND hTitle = CreateWindow("STATIC", "BatOfus", WS_VISIBLE | WS_CHILD | SS_CENTER, 20, 20, 360, 30, hwnd, NULL, NULL, NULL);
    SendMessage(hTitle, WM_SETFONT, (WPARAM)CreateFont(24, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Segoe UI"), TRUE);
    HWND hDesc = CreateWindow("STATIC", "An open-source tool to obfuscate your batch files using C.", WS_VISIBLE | WS_CHILD | SS_CENTER, 20, 60, 360, 20, hwnd, NULL, NULL, NULL);
    SendMessage(hDesc, WM_SETFONT, (WPARAM)hFont, TRUE);
    HWND hGithub = CreateWindow("STATIC", "github.com/MeLegend5555/BatOfus", WS_VISIBLE | WS_CHILD | SS_CENTER, 20, 90, 360, 20, hwnd, NULL, NULL, NULL);
    SendMessage(hGithub, WM_SETFONT, (WPARAM)hFont, TRUE);
    HWND hSelectBtn = CreateWindow("BUTTON", "Select Batch File", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 50, 130, 300, 40, hwnd, (HMENU)1, NULL, NULL);
    SendMessage(hSelectBtn, WM_SETFONT, (WPARAM)hFont, TRUE);
    HWND hObfuscateBtn = CreateWindow("BUTTON", "Obfuscate", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 50, 180, 300, 40, hwnd, (HMENU)2, NULL, NULL);
    SendMessage(hObfuscateBtn, WM_SETFONT, (WPARAM)hFont, TRUE);
    HWND hExitBtn = CreateWindow("BUTTON", "Exit", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 50, 230, 300, 40, hwnd, (HMENU)3, NULL, NULL);
    SendMessage(hExitBtn, WM_SETFONT, (WPARAM)hFont, TRUE);
    HWND hFooter = CreateWindow("STATIC", "BatOfus - @MeLegend5555", WS_VISIBLE | WS_CHILD | SS_CENTER, 20, 290, 360, 20, hwnd, NULL, NULL, NULL);
    SendMessage(hFooter, WM_SETFONT, (WPARAM)hFont, TRUE);
}

void handleFileSelection(HWND hwnd) {
    OPENFILENAME ofn;
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFilter = "Batch Files\0*.bat;*.cmd\0";
    ofn.lpstrFile = selectedFile;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
    if (GetOpenFileName(&ofn)) {
        MessageBox(hwnd, "Batch file selected successfully!\nNow click 'Obfuscate' to proceed.", "Info", MB_OK | MB_ICONINFORMATION);
    }
}

void handleObfuscateAction(HWND hwnd) {
    if (selectedFile[0] == '\0') {
        MessageBox(hwnd, "No batch file selected. Please select a file first.", "Warning", MB_OK | MB_ICONWARNING);
    } else {
        obfuscateBatchFile(selectedFile);
        MessageBox(hwnd, "Batch file obfuscated successfully!", "Success", MB_OK | MB_ICONINFORMATION);
    }
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASS wc = {0};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "BatOfusClass";
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    RegisterClass(&wc);
    HWND hwnd = CreateWindowEx(WS_EX_CLIENTEDGE, "BatOfusClass", "BatOfus", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT, 400, 400, NULL, NULL, hInstance, NULL);
    if (hwnd == NULL) return 0;
    centerWindow(hwnd);
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);
    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE:
            createControls(hwnd);
            break;
        case WM_COMMAND:
            if (LOWORD(wParam) == 1) handleFileSelection(hwnd);
            if (LOWORD(wParam) == 2) handleObfuscateAction(hwnd);
            if (LOWORD(wParam) == 3) PostQuitMessage(0);
            break;
        case WM_CTLCOLORSTATIC: {
            HDC hdcStatic = (HDC)wParam;
            SetTextColor(hdcStatic, RGB(0, 0, 0));
            SetBkMode(hdcStatic, TRANSPARENT);
            return (LRESULT)GetStockObject(NULL_BRUSH);
        }
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}