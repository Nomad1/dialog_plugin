//
//  Dialog.h
//  Dialog Godot plugin
//
//  Created by Alex Harbuzenko on 04.10.2023.
//

#ifndef DIALOG_H
#define DIALOG_H

#ifdef GDNATIVE
  #include <Godot.hpp>
  using namespace godot;
#elif defined(GDEXTENSION)
  #include <godot_cpp/classes/object.hpp>
  #include <godot_cpp/core/class_db.hpp>

  using namespace godot;
#else // plugin
  #include "core/version.h"
  #if VERSION_MAJOR == 4
    #include "core/object/class_db.h"
  #else
    #include "core/object.h"
  #endif
#endif

#if !defined(GDCLASS)
  #define GDCLASS(arg1, arg2) GODOT_CLASS(arg1, arg2)
#endif

class Dialog : public Object
{
    GDCLASS(Dialog, Object);

    static Dialog *instance;
    static int dialog_index;

public: // exported
    int show(String title, String message, Array buttons);
    
    void hide(int id);
public: // for internal use
    void handle_result(int id, int index);

    Dialog();
    ~Dialog();

    static Dialog *get_singleton();

#ifdef GDNATIVE
    static void _register_methods();    
    void _init();
#else
    static void _bind_methods();    
#endif
};

#endif
