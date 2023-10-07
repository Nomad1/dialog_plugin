//
//  Dialog.cpp
//  Dialog Godot plugin
//
//  Created by Alex Harbuzenko on 04.10.2023.
//

#include "Dialog.h"

#import <Foundation/Foundation.h>
#include <AppKit/AppKit.h>

Dialog *Dialog::instance = NULL;
int Dialog::dialog_index = 0; // not used on mac

NSString * ToNSString(String str) {
    return [[NSString alloc] initWithUTF8String:str.utf8().get_data()];
}

int Dialog::show(String title, String message, Array buttons)
{
    NSString *strTitle = ToNSString(title);
    NSString *strMessage = ToNSString(message);
    
    NSAlert * alert = [[NSAlert alloc] init];
    
    [alert setMessageText:strTitle];
    [alert setInformativeText:strMessage];
    
    // alert.view.tag = id;
    
    if (!buttons.size()) {
        [alert addButtonWithTitle:@"OK"];
    } else
    {
        for(int i = buttons.size() - 1; i >=0; i--)
        {
            NSString *strButton = ToNSString(buttons[i]);

            [alert addButtonWithTitle:strButton];
        }
    }

    int id = (int)alert.hash;

    NSWindow *currentWindow = [[NSApplication sharedApplication] keyWindow];


    dispatch_async(dispatch_get_main_queue(), ^{
        [alert beginSheetModalForWindow:currentWindow completionHandler:^(NSModalResponse returnCode) { this->handle_result(id, (int)(returnCode - NSAlertFirstButtonReturn)); }];
    });
    
    return id;
}

void Dialog::hide(int id) {
    NSWindow *activeModalWindow = [[NSApplication sharedApplication] modalWindow];
    if (activeModalWindow && activeModalWindow.hash == id) {
        [activeModalWindow close];
        [[NSApplication sharedApplication] stopModalWithCode:NSModalResponseCancel];
    }
}

void Dialog::handle_result(int id, int index) {
    emit_signal("dialog_closed", id, index < 0 ? -1 : index);
}

#ifdef GDNATIVE
void Dialog::_init() {
}

void Dialog::_register_methods() {
    register_method("show", &Dialog::show);
    register_method("hide", &Dialog::hide);
    
    register_signal<Dialog>("dialog_closed", "index", GODOT_VARIANT_TYPE_INT);
}
#else
void Dialog::_bind_methods() {
    ClassDB::bind_method(D_METHOD("show"), &Dialog::show);
    ClassDB::bind_method(D_METHOD("hide"), &Dialog::hide);
    
    ADD_SIGNAL(MethodInfo("dialog_closed", PropertyInfo(Variant::INT, "index")));
}
#endif

Dialog *Dialog::get_singleton() {
    return instance;
}

Dialog::Dialog() {
    ERR_FAIL_COND(instance != NULL);

    instance = this;
}


Dialog::~Dialog() {
    instance = NULL;
}
