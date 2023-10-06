//
//  dialog_module.cpp
//  Dialog Godot plugin
//
//  Devived from in_app_store_module.cpp on 04.10.2023.
//  https://github.com/godotengine/godot-ios-plugins/
//

#include "dialog_module.h"

#include "core/version.h"

#if VERSION_MAJOR == 4
#include "core/config/engine.h"
#else
#include "core/engine.h"
#endif

#include "Dialog.h"

void register_dialog_types() {
    Dialog * dialog = memnew(Dialog);
	Engine::get_singleton()->add_singleton(Engine::Singleton("Dialog", dialog));
}

void unregister_dialog_types() {
    Dialog * dialog = Dialog::get_singleton();
	if (dialog) {
		memdelete(dialog);
	}
}
