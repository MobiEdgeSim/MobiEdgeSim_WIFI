#ifndef JVM_MANAGER_H
#define JVM_MANAGER_H

#include <jni.h>
#include <stdexcept>
#include <string>
#include <iostream>

class JVMManager {
public:
    static JVMManager& getInstance() {
        static JVMManager instance;
        return instance;
    }

    JNIEnv* getEnv();
    static void destroyJVM();

private:
    JVMManager();
    ~JVMManager();

    void initializeJVM();

    static JavaVM* jvm;
    static JNIEnv* env;
    static bool jvmInitialized;


    JVMManager(const JVMManager&) = delete;
    JVMManager& operator=(const JVMManager&) = delete;
};

#endif // JVM_MANAGER_H
