//
//  Dialog.cpp
//  Dialog Godot plugin
//
//  Created by Alex Harbuzenko on 04.10.2023.
//

#include "Dialog.h"

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

Dialog *Dialog::instance = NULL;
int Dialog::dialog_index = 0;

NSString * ToNSString(String str) {
    return [[NSString alloc] initWithUTF8String:str.utf8().get_data()];
}

int Dialog::show(String title, String message, Array buttons)
{
    const int id = ++dialog_index;
    
    NSString *strTitle = ToNSString(title);
    NSString *strMessage = ToNSString(message);
    
    UIAlertController* alert = [UIAlertController alertControllerWithTitle:strTitle
                                                                   message:strMessage
                                                            preferredStyle:UIAlertControllerStyleAlert];
    
    alert.view.tag = id;
    
    if (!buttons.size()) {
        UIAlertAction* defaultAction = [UIAlertAction actionWithTitle:@"OK" style:UIAlertActionStyleDefault handler:^(UIAlertAction * action) { this->handle_result(id, 0); }];
        [alert addAction:defaultAction];
    } else
    {
        for(int i = buttons.size() - 1; i >=0; i--)
        {
            NSString *strButton = ToNSString(buttons[i]);
            
            const int number = i;
            
            UIAlertAction* action = [UIAlertAction actionWithTitle:strButton style:UIAlertActionStyleDefault handler:^(UIAlertAction * action) { this->handle_result(id, number); }];
            
            [alert addAction:action];
        }
    }
    
    UIViewController *rootController = [[[[UIApplication sharedApplication] delegate] window] rootViewController];
    
    dispatch_async(dispatch_get_main_queue(), ^{
        [rootController presentViewController:alert animated:YES completion:nil];
    });
    
    return id;
}

void Dialog::hide(int id) {
    UIViewController *currentViewController = [[[[UIApplication sharedApplication] delegate] window] rootViewController];
    
    // traverse all presented view controllers
    while (currentViewController.presentedViewController) {
        currentViewController = currentViewController.presentedViewController;
        
        // process only UIAlertController instances
        if ([currentViewController isKindOfClass:[UIAlertController class]]) {
            
            // we are closing everything or id matches
            if (id == -1 || currentViewController.view.tag == id) {
                [currentViewController dismissViewControllerAnimated:YES completion:nil];
                
                if (id != -1)
                    return;
            }
        }
    }
}

void Dialog::handle_result(int id, int index) {
    emit_signal("dialog_closed", id, index);
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
