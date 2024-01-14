#include "Commander.h"

int main() {
    Commander commander;
    std::array<double, 3> center = {0.0, 0.0, 0.0};
    double angle = 45;
    int n_uavs = 6;
    double distance = 1.0;
    int cmd = 2;
    std::vector<Commander::UAV> testUavs = {
        {1, {1.8, 2.8, 3.8}, {0, 0, 0}},
        {2, {1.0, 2, 2}, {0, 0, 0}},
        {3, {2, 2, 2}, {0, 0, 0}},
        {4, {2, 2, 2}, {0, 0, 0}},
        {5, {2, 2, 2}, {0, 0, 0}},
        {6, {2, 2, 2}, {0, 0, 0}}
    };
    ////ceshi
   // commander.testAlgorithm();

    commander.process(center, angle, n_uavs, distance,testUavs,cmd);


    std::string url = "http://192.168.10.83:8005/foo/";
    std::string postData = "hello";
    std::string response;

    if (performPostRequest(url, postData, response)) {
        std::cout << "Server response: " << response << std::endl;
    }

    return 0;
}