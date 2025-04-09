#include "JVMManager.h"

JavaVM* JVMManager::jvm = nullptr;
JNIEnv* JVMManager::env = nullptr;
bool JVMManager::jvmInitialized = false;

JVMManager::JVMManager() {
    initializeJVM();
}

JVMManager::~JVMManager() {
}

JNIEnv* JVMManager::getEnv() {
    if (!jvmInitialized) {
        initializeJVM();
    }
    return env;
}

void JVMManager::destroyJVM() {
    if (jvm && jvmInitialized) {
        std::cout << "JVMManager::destroyJVM - Destroying Java VM..." << std::endl;
        jint res = jvm->DestroyJavaVM();
        if (res != JNI_OK) {
            std::cout << "JVMManager::destroyJVM - Failed to destroy Java VM, error code: " << res << std::endl;
            throw std::runtime_error("Failed to destroy Java VM");
        }
        jvmInitialized = false;
        jvm = nullptr;
        env = nullptr;
        std::cout << "JVMManager::destroyJVM - Java VM destroyed successfully" << std::endl;
    } else {
        std::cout << "JVMManager::destroyJVM - No JVM to destroy or JVM not initialized" << std::endl;
    }
}

void JVMManager::initializeJVM() {
    if (jvmInitialized) {
        std::cout << "JVMManager::initializeJVM - JVM is already initialized" << std::endl;
        return;
    }

    std::cout << "JVMManager::initializeJVM - Initializing JVM..." << std::endl;
    JavaVMInitArgs vm_args;
    JavaVMOption options[4];
    //change this path to your own
    options[0].optionString = const_cast<char*>("-Djava.class.path=/home/tianhao/Documents/workspace/MetaHeuristicAlgorithms/out/production/MetaHeuristicAlgorithms:/home/tianhao/Documents/workspace/MetaHeuristicAlgorithms/lib/gson-2.11.0.jar");
    options[1].optionString = const_cast<char*>("-Xms1g");  // Set minimum heap memory
    options[2].optionString = const_cast<char*>("-Xmx4g"); //Set max heap memory
    options[3].optionString = const_cast<char*>("-verbose:jni"); // Enable verbose JNI log

    vm_args.version = JNI_VERSION_1_6;
    vm_args.nOptions = 4;
    vm_args.options = options;
    vm_args.ignoreUnrecognized = false;

    jint res = JNI_CreateJavaVM(&jvm, (void**)&env, &vm_args);
    if (res != JNI_OK) {
        std::cout << "JVMManager::initializeJVM - Failed to create Java VM, error code: " << res << std::endl;
        throw std::runtime_error("Failed to create Java VM");
    }

    jvmInitialized = true;
    std::cout << "JVMManager::initializeJVM - JVM initialized successfully" << std::endl;


    //this part is for test
    jclass systemClass = env->FindClass("java/lang/System");
    jmethodID getPropertyMethod = env->GetStaticMethodID(systemClass, "getProperty", "(Ljava/lang/String;)Ljava/lang/String;");
    jstring propertyKey = env->NewStringUTF("java.class.path");
    jstring classpath = (jstring)env->CallStaticObjectMethod(systemClass, getPropertyMethod, propertyKey);
    const char *classpathCStr = env->GetStringUTFChars(classpath, nullptr);
    std::cout << "JVM Class Path: " << classpathCStr << std::endl;
    env->ReleaseStringUTFChars(classpath, classpathCStr);

    jclass testClass = env->FindClass("Interface/findBestMECHost");
    if (!testClass) {
        std::cout << "Failed to load Interface/findBestMECHost" << std::endl;
    } else {
        std::cout << "Class Interface/findBestMECHost loaded successfully" << std::endl;
    }

}
