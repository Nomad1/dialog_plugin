# Godot System Dialog plugin
Multi-platform plugin for System Dialog in Godot 3 and 4

This project aims to show system dialogs with customizable buttons on different platforms including mobiles.

# Notes
At the moment only iOS and macOS versions are working and only for Godot 3.x using GDNative. iOS plugin can be compiled both as GDNative module and as iOS plugin. GDNative requires sub-module godot-cpp (3.x branch) and iOS Plugin requires godot headers or source code in godot submodule.
You can also download godot-headers here: https://github.com/godotengine/godot-ios-plugins/releases

To build 3.x branch you need to recursively download godot-cpp branch 3.x to folder godot-cpp-3.x. It should be done automatically when you download the repository with submodules

## OS support
| OS | Plugin | GDNative (3.x) | GDExtension (4.x) |
|-|-|-|-|
|iOS| + | + | - |
|Android| - | - | - |
|macOS|  | + | - |
|Windows| | - | - |
|Linux| | - | - |

# Build
Use scons to build each platform independently. I.e. you can change to `platforms/gdnative` and type

```scons platform=macos target=debug version=3.x```

For plugins there are build scripts taken from https://github.com/godotengine/godot-ios-plugins

```scripts/release_xcframework.sh 3.x```

# Distribution
Compiled plugins are placed in the bin/ directory. For GDNative it's `bin/plugins`, for GDExtension it's `bin/extensions`, for mobiles it's `bin/ios` and `bin/android` correspondingly.

# Usage
Mobile plugins provide global `Dialog` singleton that could be used as is in GDScript and with a call to `Godot.Engine.GetSingleton("Dialog")` in C#. GDNative and GDExtension plugins require loading a NativeScript resource:
C#:
```cs
    NativeScript ns = GD.Load<NativeScript>("res://plugins/dialog/dialog.gdns");
    Object dialogPlugin = (Object)ns.New();
```

GDScript:
```gd
    var ns = preload("res://plugins/dialog/dialog.gdns")
    var dialogPlugin = ns.new()
```

There should be a corresponding gdns file in the project files.


To display the dialog use `show` method:
```cs
    dialogPlugin.Connect("dialog_closed", this, "OnClosed"); // Connecting to the signal to get a callback. Optional
    dialogPlugin.Call("show", name, title, new Array({"Ok", "Cancel"});
```

Signal `dialog_closed` can get a callback when dialog is dismissed or buttons are clicked. valid signature is `dialog_closed(int id, int index)`. Method `show` also returns dialog id that could be used with `hide` method and to distinguish different dialogs in `dialog_closed` signal.

