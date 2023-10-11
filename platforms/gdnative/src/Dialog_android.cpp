//
//  Dialog.cpp
//  Dialog Godot plugin
//
//  Created by Alex Harbuzenko on 04.10.2023.
//

#include "Dialog.h"

#include <core/GodotGlobal.hpp>
#include <android/looper.h>
#include <jni.h>

Dialog *Dialog::instance = NULL;

int Dialog::dialog_index = 0;

extern "C" JNIEXPORT void JNICALL onClick(JNIEnv *env, jobject instance, jobject dialog, jint which) {
    Dialog::get_singleton()->handle_result(0, which);
}

void Dialog::do_show() {

    Godot::print("Got do_show call");

    JNIEnv* env = godot::android_api->godot_android_get_env();
    jobject activity = godot::android_api->godot_android_get_activity();


/*
    jobject listenerInstance = env->NewLocalRef(env->ToReflectedMethod(env->FindClass("android/content/DialogInterface$OnClickListener"), env->GetMethodID(env->FindClass("android/content/DialogInterface$OnClickListener"), "onClick", "(Landroid/content/DialogInterface;I)V"), JNI_TRUE));
    jobject onClickListener = env->NewObject(env->FindClass("android/content/DialogInterface$OnClickListener"), env->GetMethodID(env->FindClass("android/content/DialogInterface$OnClickListener"), "<init>", "(J)V"), reinterpret_cast<jlong>(this));


    jmethodID setPositiveButtonMethod = env->GetMethodID(alertDialogBuilderClass, "setPositiveButton", "(Ljava/lang/CharSequence;Landroid/content/DialogInterface$OnClickListener;)Landroid/app/AlertDialog$Builder;");

    if (lastButtons.size() == 0)
    {
        jstring buttonText = env->NewStringUTF("OK");
        env->CallObjectMethod(builderInstance, setPositiveButtonMethod, buttonText, onClickListener);
    } else
    {
        String button = buttons[0];
        jstring buttonText = env->NewStringUTF(button.utf8().get_data());
        env->CallObjectMethod(builderInstance, setPositiveButtonMethod, buttonText, onClickListener);
    }*/

    // Convert Godot::String to jstring
    jstring title = env->NewStringUTF(lastTitle.utf8().get_data());
    jstring message = env->NewStringUTF(lastMessage.utf8().get_data());

    Godot::print("Converted strings");

    jclass alertDialogBuilderClass = env->FindClass("android/app/AlertDialog$Builder");

    // Get the constructor method
    jmethodID constructor = env->GetMethodID(alertDialogBuilderClass, "<init>", "(Landroid/content/Context;)V");

    // Get the setTitle, setMessage and show methods
    jmethodID setTitleMethod = env->GetMethodID(alertDialogBuilderClass, "setTitle", "(Ljava/lang/CharSequence;)Landroid/app/AlertDialog$Builder;");
    jmethodID setMessageMethod = env->GetMethodID(alertDialogBuilderClass, "setMessage", "(Ljava/lang/CharSequence;)Landroid/app/AlertDialog$Builder;");
    jmethodID showMethod = env->GetMethodID(alertDialogBuilderClass, "show", "()Landroid/app/AlertDialog;");

    Godot::print("Got methods, ready to process");

    // Create an instance of AlertDialog.Builder
    jobject builderInstance = env->NewObject(alertDialogBuilderClass, constructor, activity);

    Godot::print("AlertDialog.Builder instance created");

    // Call setTitle and setMessage methods
    env->CallObjectMethod(builderInstance, setTitleMethod, title);
    env->CallObjectMethod(builderInstance, setMessageMethod, message);

    Godot::print("Strings set");

    // Show the AlertDialog
    jobject result = env->CallObjectMethod(builderInstance, showMethod);

    // Clean up local references (optional but good practice)
    env->DeleteLocalRef(alertDialogBuilderClass);
    env->DeleteLocalRef(builderInstance);
    env->DeleteLocalRef(title);
    env->DeleteLocalRef(message);

    if (env->ExceptionCheck()) {
        env->ExceptionDescribe();
        env->ExceptionClear();
        // or handle the error as needed
        Godot::print("Failed do_show call");
    } else
    {
        Godot::print("Completed do_show call");
       // Godot::print((long)result);
    }
}

int Dialog::show(godot::String titleStr, godot::String messageStr, godot::Array buttons) {
    int id = 0;

    lastTitle = titleStr;
    lastMessage = messageStr;
    lastButtons = buttons;

    JNIEnv* env = godot::android_api->godot_android_get_env();
    jobject activity = godot::android_api->godot_android_get_activity();

    jclass godotLibClass = env->FindClass("org/godotengine/godot/GodotLib");

    jclass classClass = env->FindClass("java/lang/Class");
    jmethodID getDeclaredMethodsMethod = env->GetMethodID(classClass, "getDeclaredMethods", "()[Ljava/lang/reflect/Method;");
    jobjectArray methodsArray = (jobjectArray)env->CallObjectMethod(godotLibClass, getDeclaredMethodsMethod);


    jobject targetMethod;

    int methodCount = env->GetArrayLength(methodsArray);

    // 2. Iterate through all methods to find the correct one
    for (int i = 0; i < methodCount; i++) {
        jobject method = env->GetObjectArrayElement(methodsArray, i);
        
        // Get the method's name and signature to compare
        jclass methodClass = env->GetObjectClass(method);
        jmethodID getNameMethod = env->GetMethodID(methodClass, "getName", "()Ljava/lang/String;");
        jstring methodNameJString = (jstring)env->CallObjectMethod(method, getNameMethod);
        const char* methodNameCStr = env->GetStringUTFChars(methodNameJString, NULL);
        
        if (strcmp(methodNameCStr, "callobject") == 0) {
            targetMethod = method;
            env->ReleaseStringUTFChars(methodNameJString, methodNameCStr);
            break;
            
        }
        
        env->ReleaseStringUTFChars(methodNameJString, methodNameCStr);
    }

    // 2. Construct a MethodAndArgsCaller object which targets the myNativeFunction method
    jclass macClass = env->FindClass("android/internal/os/RuntimeInit$MethodAndArgsCaller");
    jmethodID macCtor = env->GetMethodID(macClass, "<init>", "(Ljava/lang/reflect/Method;[Ljava/lang/Object;)V");

    jobjectArray jparams = env->NewObjectArray(0, env->FindClass("java/lang/Object"), NULL);

    jobject jid = env->NewObject(env->FindClass("java/lang/Long"), env->GetMethodID(env->FindClass("java/lang/Long"), "<init>", "(J)V"), get_instance_id());
    jstring jname = env->NewStringUTF("do_show");

    jobjectArray args = env->NewObjectArray(3, env->FindClass("java/lang/Object"), NULL);
    env->SetObjectArrayElement(args, 0, jid);
    env->SetObjectArrayElement(args, 0, jname);
    env->SetObjectArrayElement(args, 0, jparams);

    env->DeleteLocalRef(jname);

    jobject macObj = env->NewObject(macClass, macCtor, targetMethod, args);
    if (!macObj) {
        // Handle error
        return -1;
    }

    // 3. Use the Activity.runOnUiThread() method to execute the MethodAndArgsCaller
    jclass activityClass = env->FindClass("android/app/Activity");
    jmethodID runOnUiThreadMethod = env->GetMethodID(activityClass, "runOnUiThread", "(Ljava/lang/Runnable;)V");
    env->CallVoidMethod(activity, runOnUiThreadMethod, macObj);

    return id;
}

void Dialog::hide(int id) {

}

void Dialog::handle_result(int id, int index) {
  //  emit_signal("dialog_closed", id, index);
}

#ifdef GDNATIVE
void Dialog::_init() {
    Godot::print("Got _init call");
}

void Dialog::_register_methods() {
    register_method("show", &Dialog::show);
    register_method("do_show", &Dialog::do_show);
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
