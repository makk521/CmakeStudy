#pragma once
#include <string>
#include <vector>
#include <mutex>
//#include "Hungarian.h"
#include "Hungarian.h"
#include "dataPack.h"
class Commander {
public:
    Commander(){}
    bool isCommandProcessed = true;
    //void process();
    
    int vehicle_num;
    struct UAV {
        int id;
        std::array<double, 3> position; //原始位置
        std::array<double, 3> target;   //目标位置
        // UAV(int id, std::array<double, 3> pos, std::array<double, 3> tgt)
        // : id(id), position(pos), target(tgt) {}

    };
    //测试
    void setTestData(const std::vector<UAV>& testUavs, const std::vector<std::array<double, 3>>& testTargets) {
        uavs = testUavs;
        targets = testTargets;
    }
    // const std::vector<UAV>& getUAVs() const {
    //     return uavs;
    // }

    std::mutex mtx; // 

    std::vector<UAV> uavs;
    std::vector<std::array<double, 3>> targets;  // 目标点位置集合
    int cmd;  // 命令
   // std::mutex mtx;
    //void process(const std::array<double, 3>& center, double angle, int n_uavs, double distance,const std::vector<Commander::UAV>& testUavs);
    void process(const std::array<double, 3>& center, double angle, int n_uavs, double distance,const std::vector<Commander::UAV>& testUavs,int cmd);

    void getUserCommand();
    void xiongAlgorithm();
    void broadcastUAVData();
    void broadcastPauseCommand();
    void broadcastResumeCommand();
    void testAlgorithm() ;
    double distance_uav(const std::array<double, 3>& a, const std::array<double, 3>& b);
    std::vector<std::array<double, 3>> calculate_formation(std::array<double, 3> center, double angle, int n_uavs, double distance, int cmd);
    void addToQueue(const std::vector<UAV>& uavs, ThreadSafeQueue<DataPack>& queue);

    std::vector<std::vector<double>> calculateCostMatrix();  // 不需要参数
};

size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output);
bool performPostRequest(const std::string& url, const std::string& postData, std::string& response);

