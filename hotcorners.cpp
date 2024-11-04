#include <windows.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <string>
#include <vector>

// Constants
const int HOT_CORNER_OFFSET = 20;       // Corner offset for trigger
const int DEFAULT_ACTION_DELAY = 80;         // Default action delay in milliseconds
const std::string CONFIG_FILE = "hotcorners.ini";  // Configuration file path

// Key mapping table
std::map<std::string, WORD> keyMap = {
    // Modifier keys
    {"Ctrl", VK_CONTROL}, {"LCtrl", VK_LCONTROL}, {"RCtrl", VK_RCONTROL},
    {"Shift", VK_SHIFT}, {"LShift", VK_LSHIFT}, {"RShift", VK_RSHIFT},
    {"Alt", VK_MENU}, {"LAlt", VK_LMENU}, {"RAlt", VK_RMENU},
    {"Win", VK_LWIN}, {"RWin", VK_RWIN},

    // Arrow keys
    {"Left", VK_LEFT}, {"Right", VK_RIGHT}, {"Up", VK_UP}, {"Down", VK_DOWN},

    // Alphabet keys
    {"A", 'A'}, {"B", 'B'}, {"C", 'C'}, {"D", 'D'}, {"E", 'E'}, {"F", 'F'},
    {"G", 'G'}, {"H", 'H'}, {"I", 'I'}, {"J", 'J'}, {"K", 'K'}, {"L", 'L'},
    {"M", 'M'}, {"N", 'N'}, {"O", 'O'}, {"P", 'P'}, {"Q", 'Q'}, {"R", 'R'},
    {"S", 'S'}, {"T", 'T'}, {"U", 'U'}, {"V", 'V'}, {"W", 'W'}, {"X", 'X'},
    {"Y", 'Y'}, {"Z", 'Z'},

    // Number keys (main keyboard)
    {"0", '0'}, {"1", '1'}, {"2", '2'}, {"3", '3'}, {"4", '4'}, {"5", '5'},
    {"6", '6'}, {"7", '7'}, {"8", '8'}, {"9", '9'},

    // Function keys
    {"F1", VK_F1}, {"F2", VK_F2}, {"F3", VK_F3}, {"F4", VK_F4},
    {"F5", VK_F5}, {"F6", VK_F6}, {"F7", VK_F7}, {"F8", VK_F8},
    {"F9", VK_F9}, {"F10", VK_F10}, {"F11", VK_F11}, {"F12", VK_F12},

    // Numpad number keys
    {"Num0", VK_NUMPAD0}, {"Num1", VK_NUMPAD1}, {"Num2", VK_NUMPAD2}, {"Num3", VK_NUMPAD3},
    {"Num4", VK_NUMPAD4}, {"Num5", VK_NUMPAD5}, {"Num6", VK_NUMPAD6}, {"Num7", VK_NUMPAD7},
    {"Num8", VK_NUMPAD8}, {"Num9", VK_NUMPAD9},

    // Numpad operation keys
    {"NumLk", VK_NUMLOCK}, {"Div", VK_DIVIDE}, {"Mul", VK_MULTIPLY},
    {"Sub", VK_SUBTRACT}, {"Add", VK_ADD}, {"Dec", VK_DECIMAL},

    // Control keys
    {"Esc", VK_ESCAPE}, {"Tab", VK_TAB}, {"Caps", VK_CAPITAL},
    {"Space", VK_SPACE}, {"Enter", VK_RETURN}, {"Bksp", VK_BACK},

    // Editing keys
    {"Ins", VK_INSERT}, {"Del", VK_DELETE}, {"Home", VK_HOME}, 
    {"End", VK_END}, {"PgUp", VK_PRIOR}, {"PgDn", VK_NEXT},

    // Punctuation keys
    {"SColon", VK_OEM_1}, {"Plus", VK_OEM_PLUS}, {"Comma", VK_OEM_COMMA},
    {"Minus", VK_OEM_MINUS}, {"Dot", VK_OEM_PERIOD}, {"Slash", VK_OEM_2},
    {"Tilde", VK_OEM_3}, {"LBrack", VK_OEM_4}, {"Bslash", VK_OEM_5},
    {"RBrack", VK_OEM_6}, {"Quote", VK_OEM_7},

    // Other special keys
    {"PrtSc", VK_SNAPSHOT}, {"Scroll", VK_SCROLL}, {"Pause", VK_PAUSE},
    {"Menu", VK_APPS}  // Application Menu (context menu)
};

// Structure for storing the configuration of each hot corner action
struct CornerAction {
    std::string type;                  // "hotkey" or "cmd"
    std::string content;               // Key sequence or command line
    int delay = DEFAULT_ACTION_DELAY;  // Trigger delay in milliseconds
};

// Structure to store the configuration for all four corners
struct HotCornerConfig {
    CornerAction topLeft, topRight, bottomLeft, bottomRight;
} hotCornerConfig;

// Function to simulate key press for a series of keys
void PressKeySequence(const std::vector<WORD>& keys) {
    std::vector<INPUT> inputs;
    for (WORD key : keys) {
        INPUT input = {};
        input.type = INPUT_KEYBOARD;
        input.ki.wVk = key;
        inputs.push_back(input);
    }
    for (WORD key : keys) {
        INPUT input = {};
        input.type = INPUT_KEYBOARD;
        input.ki.wVk = key;
        input.ki.dwFlags = KEYEVENTF_KEYUP;
        inputs.push_back(input);
    }
    SendInput(static_cast<UINT>(inputs.size()), inputs.data(), sizeof(INPUT));
    Sleep(50);
}

// Function to parse and simulate key sequences from a string
void SimulateKeyPress(const std::string& keySequence) {
    std::stringstream ss(keySequence);
    std::string group;
    while (std::getline(ss, group, ',')) {
        std::vector<WORD> keys;
        std::stringstream keyStream(group);
        std::string key;
        while (std::getline(keyStream, key, '+')) {
            key = key.substr(key.find_first_not_of(' '));
            if (keyMap.count(key)) {
                keys.push_back(keyMap[key]);
            } else {
                std::cerr << "Unknown key: " << key << std::endl;
            }
        }
        if (!keys.empty()) {
            PressKeySequence(keys);
        }
    }
}

// Execute custom command
void ExecuteCommand(const std::string& command) {
    STARTUPINFOW si = { sizeof(si) };
    PROCESS_INFORMATION pi;
    std::wstring commandW(command.begin(), command.end());

    if (!CreateProcessW(NULL, &commandW[0], NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        std::wcerr << L"Failed to execute command: " << commandW << std::endl;
    } else {
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }
}

// Load configuration from the ini file
void LoadConfig() {
    std::ifstream config(CONFIG_FILE);
    std::string line;
    CornerAction* currentAction = nullptr;

    while (std::getline(config, line)) {
        if (line == "[TopLeft]") currentAction = &hotCornerConfig.topLeft;
        else if (line == "[TopRight]") currentAction = &hotCornerConfig.topRight;
        else if (line == "[BottomLeft]") currentAction = &hotCornerConfig.bottomLeft;
        else if (line == "[BottomRight]") currentAction = &hotCornerConfig.bottomRight;
        else if (currentAction) {
            if (line.find("type=") == 0)
                currentAction->type = line.substr(5);
            else if (line.find("content=") == 0)
                currentAction->content = line.substr(8);
            else if (line.find("delay=") == 0)
                currentAction->delay = std::stoi(line.substr(6));
        }
    }
}

// Execute either a key sequence or a command based on the action type
void ExecuteCornerAction(const CornerAction& action) {
    if (action.delay > 0) {
        Sleep(action.delay);
    }
    std::cout << "Executing action: " << action.content << std::endl;

    if (action.type == "hotkey") {
        SimulateKeyPress(action.content);
    } else if (action.type == "cmd") {
        ExecuteCommand(action.content);
    }
}

// Define the rectangle areas of hot corners on screen
const RECT hotCorners[4] = {
    { -HOT_CORNER_OFFSET, -HOT_CORNER_OFFSET, HOT_CORNER_OFFSET, HOT_CORNER_OFFSET },
    { GetSystemMetrics(SM_CXSCREEN) - HOT_CORNER_OFFSET, -HOT_CORNER_OFFSET, 
      GetSystemMetrics(SM_CXSCREEN) + HOT_CORNER_OFFSET, HOT_CORNER_OFFSET },
    { -HOT_CORNER_OFFSET, GetSystemMetrics(SM_CYSCREEN) - HOT_CORNER_OFFSET, 
      HOT_CORNER_OFFSET, GetSystemMetrics(SM_CYSCREEN) + HOT_CORNER_OFFSET },
    { GetSystemMetrics(SM_CXSCREEN) - HOT_CORNER_OFFSET, 
      GetSystemMetrics(SM_CYSCREEN) - HOT_CORNER_OFFSET, 
      GetSystemMetrics(SM_CXSCREEN) + HOT_CORNER_OFFSET, 
      GetSystemMetrics(SM_CYSCREEN) + HOT_CORNER_OFFSET }
};

// Hot corner handling thread
DWORD WINAPI CornerHotFunc(LPVOID lpParam) {
    int corner = (int)(intptr_t)lpParam;
    CornerAction action;
    switch (corner) {
        case 0: action = hotCornerConfig.topLeft; break;
        case 1: action = hotCornerConfig.topRight; break;
        case 2: action = hotCornerConfig.bottomLeft; break;
        case 3: action = hotCornerConfig.bottomRight; break;
    }
    if (!action.content.empty()) {
        ExecuteCornerAction(action);
    }
    return 0;
}

HANDLE cornerThreads[4] = { NULL };

// Mouse hook callback with state and thread management
static LRESULT CALLBACK MouseHookCallback(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode >= 0 && wParam == WM_MOUSEMOVE) {
        MSLLHOOKSTRUCT* evt = (MSLLHOOKSTRUCT*)lParam;
        
        for (int i = 0; i < 4; ++i) {
            if (PtInRect(&hotCorners[i], evt->pt)) {
                // If mouse enters the corner and no active thread, create a thread
                if (cornerThreads[i] == NULL) {
                    cornerThreads[i] = CreateThread(NULL, 0, CornerHotFunc, (LPVOID)(intptr_t)i, 0, NULL);
                }
            } else if (cornerThreads[i] != NULL) {
                TerminateThread(cornerThreads[i], 0);
                CloseHandle(cornerThreads[i]);
                cornerThreads[i] = NULL;
            }
        }
    }
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

int main() {
    LoadConfig();
    HHOOK mouseHook = SetWindowsHookEx(WH_MOUSE_LL, MouseHookCallback, NULL, 0);
    if (!mouseHook) {
        std::cerr << "Failed to set mouse hook!" << std::endl;
        return 1;
    }
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    UnhookWindowsHookEx(mouseHook);
    return 0;
}
