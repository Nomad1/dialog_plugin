#ifndef GDEXAMPLE_H
#define GDEXAMPLE_H

#include <Godot.hpp>

#include <gen/Node.hpp>

using namespace godot;

class Dialog : public Object
{
    GODOT_CLASS(Dialog, Object);

    static Dialog *instance;
    static int dialog_index;

public: // exported
    int show(String title, String message, Array buttons);
    
    void hide(int id);
public: // for internal use
    static void _register_methods();
    static Dialog *get_singleton();
    
    void _init();

    void handle_result(int id, int index);

    Dialog();
    ~Dialog();
};

#endif
