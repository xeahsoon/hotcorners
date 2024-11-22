# Hotcorners

## 💡 Introduction

A lightweight hot corners tool for Windows. You can use "trigger corners" as in macOS to quickly launch actions when moving the pointer to a corner. For example, you can set it so that when you move the pointer to the top right corner, it switches to the next virtual desktop, or when you move it to the bottom right corner, it opens Notepad. It supports not only custom hotkeys but also custom commands.

‍![gif](assets/screen.gif)


## 🔎 Usage

Modify the `hotcorners.ini`​ file to customize hot corner actions. Here is a sample configuration:

```ini
[TopLeft]
type=hotkey
content=Ctrl+Win+Left

[TopRight]
type=hotkey
content=Ctrl+Win+Right

[BottomLeft]
type=cmd
content=D:\hotkeys.exe Win

[BottomRight]
type=cmd
content=notepad.exe
delay=1000
```

Explanation of configuration items:

|**Config**|Value|**Description**|
| ---------| --------| ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
|type|​`hotkey`​|Hotkey action|
||​`cmd`​|Execute command|
|content|-|When type is "hotkey", it means hotkeys combination. Keys in the same group are connected by a "+" sign, and multiple groups of keys are separated by a ",". When type is "cmd", it means a custom command line.|
|delay|number|Action delay, in milliseconds|

‍

## 🗺️ Keymap

This table organizes the keys into categories to make it easier to identify their purpose and usage in the code.

|**Category**|**Key Name**|**Description**|
| --| ----------------------------------------------| -------------------------------------------------------|
|**Modifier Keys**|​`Ctrl`​, `LCtrl`​, `RCtrl`​|Control keys (general, left, right)|
||​`Shift`​, `LShift`​, `RShift`​|Shift keys (general, left, right)|
||​`Alt`​, `LAlt`​, `RAlt`​|Alt keys (general, left, right)|
||​`Win`​, `RWin`​|Windows keys (left, right)|
|**Arrow Keys**|​`Left`​, `Right`​, `Up`​, `Down`​|Directional arrow keys|
|**Alphabet Keys**|​`A`​ - `Z`​|Letters A through Z|
|**Number Keys**|​`0`​ - `9`​|Number keys on the main keyboard|
|**Function Keys**|​`F1`​ - `F12`​|Function keys|
|**Numpad Keys**|​`Num0`​ - `Num9`​|Number keys on the numpad|
||​`NumLk`​, `Div`​, `Mul`​, `Sub`​, `Add`​, `Dec`​|Numpad Lock, Divide, Multiply, Subtract, Add, Decimal|
|**Control Keys**|​`Esc`​, `Tab`​, `Caps`​, `Space`​, `Enter`​, `Bksp`​|Escape, Tab, Caps Lock, Space, Enter, Backspace|
|**Editing Keys**|​`Ins`​, `Del`​, `Home`​, `End`​, `PgUp`​, `PgDn`​|Insert, Delete, Home, End, Page Up, Page Down|
|**Punctuation Keys**|​`SColon`​, `Plus`​, `Comma`​, `Minus`​, `Dot`​, `Slash`​, `Tilde`​, `LBrack`​, `Bslash`​, `RBrack`​, `Quote`​|Various punctuation marks like ; + , - . / ~ [ \ ] '|
|**Multimedia keys**|`MediaNext`, `MediaPrev`, `MediaStop`, `MediaPlayPause`, `VolumeMute`, `VolumeUp`, `VolumeDown`|Multimedia keys|
|**Special Keys**|​`PrtSc`​, `Scroll`​, `Pause`​, `Menu`​|Print Screen, Scroll Lock, Pause, Application Menu|

‍

## 🙏 Acknowledgement

Inspired by:

* [https://github.com/taviso/hotcorner](https://github.com/taviso/hotcorner)
