#include "MetaheuristicScheduler.h"
#include <stdexcept>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstdlib>
namespace MobiEdgeSim{


MetaheuristicScheduler::MetaheuristicScheduler(const Orchestrator::AppDescriptorInfo &appInfo, const std::vector<MecHostInfo> &mecHostInfos,const std::string &algorithmName ) {
    env = JVMManager::getInstance().getEnv();

    schedulerClass = env->FindClass("Interface/findBestMECHost");
    if (!schedulerClass) {
        env->ExceptionDescribe();
        throw std::runtime_error("Failed to find Java class: Interface/findBestMECHost");
    }

    //set the Host parameters
    jclass hostClass = env->FindClass("MECHosts/Host");
    if (!hostClass) {
        throw std::runtime_error("Failed to find Java class: MECHosts/Host");
    }

    jmethodID hostConstructor = env->GetMethodID(hostClass, "<init>", "(Ljava/lang/String;DDD[Ljava/lang/String;DDDDDD)V");
    if (!hostConstructor) {
        throw std::runtime_error("Failed to find constructor for class: MECHosts/Host");
    }
    jclass stringClass = env->FindClass("java/lang/String");
    if (!stringClass) {
        throw std::runtime_error("Failed to find Java class: java/lang/String");
    }

    EV << "mecHostInfos.size() = " << mecHostInfos.size() << endl;
    jobjectArray hostArray = env->NewObjectArray(mecHostInfos.size(), hostClass, nullptr);
    for (size_t i = 0; i < mecHostInfos.size(); ++i) {
        const auto &info = mecHostInfos[i];
        jstring name = env->NewStringUTF(info.name.c_str());
        jdouble availableRam = info.availableRam;
        jdouble availableDisk = info.availableDisk;
        jdouble availableCpu = info.availableCpu;
        jdouble latitude = info.latitude;
        jdouble longitude = info.longitude;
        jdouble latency = info.latency;
        jdouble ram = info.ram;
        jdouble disk = info.disk;
        jdouble cpu = info.cpu;

//        jobjectArray serviceArray = env->NewObjectArray(info.availableServices.size(), env->FindClass("java/lang/String"), nullptr);
//        for (size_t j = 0; j < info.availableServices.size(); ++j) {
//            jstring service = env->NewStringUTF(info.availableServices[j].c_str());
//            env->SetObjectArrayElement(serviceArray, j, service);
//        }
        //set to default service
        //jclass stringClass = env->FindClass("java/lang/String");
        jobjectArray serviceArray = env->NewObjectArray(1, stringClass, nullptr);
        jstring service = env->NewStringUTF("normalService");
        env->SetObjectArrayElement(serviceArray, 0, service);


        jobject host = env->NewObject(hostClass, hostConstructor, name, availableRam, availableDisk, availableCpu, serviceArray, latitude, longitude, latency, ram, disk, cpu);
        env->SetObjectArrayElement(hostArray, i, host);

        EV << "Host[" << i << "] created: " << "Name=" << info.name << ", AvailableRam=" << availableRam << ", AvailableDisk=" << availableDisk << ", AvailableCpu=" << availableCpu << ", Latitude="
                << latitude << ", Longitude=" << longitude << ", Latency=" << latency << ", maxRam=" << ram << ", maxDisk=" << disk << ", maxCpu=" << cpu << endl;
    }

    //set the Function parameters
    jclass functionClass = env->FindClass("Functions/Function");
    if (!functionClass) {
        throw std::runtime_error("Failed to find Java class: Functions/Function");
    }

    jmethodID functionConstructor = env->GetMethodID(functionClass, "<init>", "(Ljava/lang/String;DDD[Ljava/lang/String;DD)V");
    if (!functionConstructor) {
        throw std::runtime_error("Failed to find constructor for class: Functions/Function");
    }

    jobjectArray functionArray = env->NewObjectArray(1, functionClass, nullptr);
    const auto &info = appInfo;
    jstring name = env->NewStringUTF(info.name.c_str());
    jdouble ram = info.ram;
    jdouble disk = info.disk;
    jdouble cpu = info.cpu;
    jdouble latitude = info.latitude;
    jdouble longitude = info.longitude;

//    jobjectArray serviceArray = env->NewObjectArray(info.requiredServices.size(), env->FindClass("java/lang/String"), nullptr);
//    for (size_t j = 0; j < info.requiredServices.size(); ++j) {
//        jstring service = env->NewStringUTF(info.requiredServices[j].c_str());
//        env->SetObjectArrayElement(serviceArray, j, service);
//    }
    //function service set to default
    jobjectArray reqServiceArray = env->NewObjectArray(1, stringClass, nullptr);
    jstring reqService = env->NewStringUTF("normalService");
    env->SetObjectArrayElement(reqServiceArray, 0, reqService);

    jobject function = env->NewObject(functionClass, functionConstructor, name, ram, disk, cpu, reqServiceArray, latitude, longitude);
    env->SetObjectArrayElement(functionArray, 0, function);
    EV << "Function created: " << "Name=" << info.name << ", Ram=" << ram << ", Disk=" << disk << ", Cpu=" << cpu << ", Latitude=" << latitude << ", Longitude=" << longitude << endl;


    //return  a string
    //jstring result = (jstring) env->CallStaticObjectMethod(schedulerClass, findBestHostMethod, env->NewStringUTF("Random"), hostArray, functionArray);
//    jmethodID findBestHostMethod = env->GetStaticMethodID(schedulerClass, "findBestHost", "(Ljava/lang/String;[LMECHosts/Host;[LFunctions/Function;)Ljava/lang/String;");
//    if (!findBestHostMethod) {
//        throw std::runtime_error("Failed to find findBestHost method");
//    }

    //return a string[]
    jmethodID findBestHostMethod = env->GetStaticMethodID(schedulerClass, "findBestHost", "(Ljava/lang/String;[LMECHosts/Host;[LFunctions/Function;)[Ljava/lang/String;");
    if (!findBestHostMethod) {
        throw std::runtime_error("Failed to find findBestHost method");
    }

    jobjectArray resultArray = (jobjectArray) env->CallStaticObjectMethod(schedulerClass, findBestHostMethod, env->NewStringUTF(algorithmName.c_str()), hostArray, functionArray);
    jstring result = (jstring) env->GetObjectArrayElement(resultArray, 0);//get the firat result

    if (env->ExceptionCheck()) {
        env->ExceptionDescribe();
        env->ExceptionClear();
        throw std::runtime_error("Java exception occurred during findBestHost method call");
    }

    if (!result) {
        throw std::runtime_error("findBestHost method returned null");
    }

    const char *resultCStr = env->GetStringUTFChars(result, nullptr);
    bestHostName = std::string(resultCStr);
    env->ReleaseStringUTFChars(result, resultCStr);
}

MetaheuristicScheduler::~MetaheuristicScheduler() {

}

std::string MetaheuristicScheduler::findBestHost() {
    return bestHostName;
}

}
