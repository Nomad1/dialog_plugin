//
//  Dialog.cpp
//  Dialog Godot plugin
//
//  Created by Alex Harbuzenko on 04.10.2023.
//

#include "Dialog.h"

Dialog *Dialog::instance = NULL;
int Dialog::dialog_index = 0;


void handle_result(int id, int index) {
    Dialog::get_singleton()->handle_result(id, index);
}

int Dialog::show(godot::String title, godot::String message, godot::Array buttons)
{
    const int id = ++dialog_index;

    Godot::print("Showing dialog ", title, ", message ", title, message);
    
/*    NSString *strTitle = ToNSString(title);
    NSString *strMessage = ToNSString(message);
    
    UIAlertController* alert = [UIAlertController alertControllerWithTitle:strTitle
                                                                   message:strMessage
                                                            preferredStyle:UIAlertControllerStyleAlert];
    
    alert.view.tag = id;
    
    if (!buttons.size()) {
        UIAlertAction* defaultAction = [UIAlertAction actionWithTitle:@"OK" style:UIAlertActionStyleDefault handler:^(UIAlertAction * action) { handle_result(id, 0); }];
        [alert addAction:defaultAction];
    } else
    {
        for(int i = buttons.size() - 1; i >=0; i--)
        {
            NSString *strButton = ToNSString(buttons[i]);
            
            const int number = i;
            
            UIAlertAction* action = [UIAlertAction actionWithTitle:strButton style:UIAlertActionStyleDefault handler:^(UIAlertAction * action) { handle_result(id, number); }];
            
            [alert addAction:action];
        }
    }
    
    UIViewController *rootController = [[[[UIApplication sharedApplication] delegate] window] rootViewController];
    
    dispatch_async(dispatch_get_main_queue(), ^{
        [rootController presentViewController:alert animated:YES completion:nil];
    });*/
    
    return id;
}

void Dialog::hide(int id) {
/*
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
    }*/
}

void Dialog::_init() {
    Godot::print("Got _init call");
}

void Dialog::handle_result(int id, int index) {
  //  emit_signal("dialog_closed", id, index);
}

void Dialog::_register_methods() {
    Godot::print("Got _register_methods call");

    register_method("show", &Dialog::show);
    register_method("hide", &Dialog::hide);
    
    register_signal<Dialog>("dialog_closed", "index", GODOT_VARIANT_TYPE_INT);
}

Dialog *Dialog::get_singleton() {
    Godot::print("Got get_singleton call");

    return instance;
}

Dialog::Dialog() {
    //ERR_FAIL_COND(instance != NULL);
    instance = this;
    
    //    products_request_delegate = [[GodotProductsDelegate alloc] init];
    //  transactions_observer = [[GodotTransactionsObserver alloc] init];
}


Dialog::~Dialog() {
    instance = NULL;
    //[products_request_delegate reset];
    //[transactions_observer reset];
    
    //    products_request_delegate = nil;
    //  [[SKPaymentQueue defaultQueue] removeTransactionObserver:transactions_observer];
    //transactions_observer = nil;
}
