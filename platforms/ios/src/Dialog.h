//
//  Dialog.h
//  Dialog Godot plugin
//
//  Created by Alex Harbuzenko on 04.10.2023.
//

#ifndef DIALOG_H
#define DIALOG_H

#include "core/version.h"

#if VERSION_MAJOR == 4
#include "core/object/class_db.h"
#else
#include "core/object.h"
#endif

class Dialog : public Object {
    GDCLASS(Dialog, Object);

    static Dialog *instance;
    static int dialog_index;
    
    static void _bind_methods();
public: // exported
    int show(String title, String message, Array buttons);
    
    void hide(int id);
public: // for internal use
    static Dialog *get_singleton();
    
    void handle_result(int id, int index);

    Dialog();
    ~Dialog();
};

#endif
