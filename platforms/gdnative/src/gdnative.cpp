#include "Dialog.h"

extern "C" void GDN_EXPORT godot_gdnative_init(godot_gdnative_init_options *o) {
	godot::Godot::gdnative_init(o);
}

extern "C" void GDN_EXPORT godot_gdnative_terminate(godot_gdnative_terminate_options *o) {
	godot::Godot::gdnative_terminate(o);

	Dialog * instance = Dialog::get_singleton();
	if (instance)
		godot::api->godot_free((void*)instance);
}

extern "C" void GDN_EXPORT godot_nativescript_init(void *handle) {
	godot::Godot::nativescript_init(handle);

	Godot::print("Registering class Dialog");
	godot::register_class<Dialog>();
}