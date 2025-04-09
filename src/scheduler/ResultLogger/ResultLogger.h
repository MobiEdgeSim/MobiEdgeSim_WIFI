#ifndef SCHEDULER_RESULTLOGGER_RESULTLOGGER_H_
#define SCHEDULER_RESULTLOGGER_RESULTLOGGER_H_

#include <string>
#include <vector>
#include <mutex>
#include <unordered_map>
#include <memory>
#include <fstream>
#include "orchestrator.h"
#include "mecHost.h"

namespace MobiEdgeSim {

class ResultLogger {
private:
    // 单例指针与互斥锁
    static ResultLogger* instance;
    static std::mutex initMutex;

    // 构造函数私有化，防止外部实例化
    ResultLogger() = default;

    // 键：算法名称   值：对应的文件输出流(用unique_ptr封装)
    std::unordered_map<std::string, std::unique_ptr<std::ofstream>> fileMap;

public:
    // 获取单例实例
    static ResultLogger& getInstance();
    ~ResultLogger();

    // 禁止拷贝与赋值
    ResultLogger(const ResultLogger&) = delete;
    ResultLogger& operator=(const ResultLogger&) = delete;

    /**
     * 记录放置结果
     * @param algorithmName    算法名称
     * @param appInfo          UE需求
     * @param bestHostName     选择的主机
     * @param hosts            所有可选主机列表
     * @param schedulingTimeMs 调度算法耗时(毫秒)
     */
    void logPlacementResult(
        const std::string& algorithmName,
        const Orchestrator::AppDescriptorInfo& appInfo,
        const std::string& bestHostName,
        const std::vector<MecHostInfo>& hosts,
        double schedulingTimeMs
    );

    // 主动刷新文件的buffer
    void flush(const std::string& algorithmName);
private:
    // 内部方法：获取(或创建)某个算法对应的输出文件流
    std::ofstream& getFileStream(const std::string& algorithmName);
};

} // namespace MobiEdgeSim

#endif /* SCHEDULER_RESULTLOGGER_RESULTLOGGER_H_ */
