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
  #define GDCLASS GODOT_CLASS
#else
  #include "core/version.h"
  #if VERSION_MAJOR == 4
    #include "core/object/class_db.h"
  #else
    #include "core/object.h"
  #endif
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
