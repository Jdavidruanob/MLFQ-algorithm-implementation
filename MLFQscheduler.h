#ifndef MLFQScheduler_H
#define MLFQScheduler_H

#include <vector>
#include <queue>
#include <string>
#include "Process.h"

class MLFQScheduler {
private:
    std::vector<std::queue<Process*>> queues;
    std::vector<int> timeQuantums;
    std::string lastQueueAlgorithm;
    
    int currentTime = 0;
    
    std::vector<Process> allProcesses;

private:
    // Helper methods for the simulation logic
    void admitNewProcesses();
    void findAndPromoteBestProcess();

public:
    // Constructor
    MLFQScheduler(const std::vector<int>& p_quantums, const std::string& p_lastQueueAlgo);

    // Main methods
    void loadProcessesFromFile(const std::string& filename);
    void run();
    void printResults(const std::string& outputFilename);
};

#endif //MLFQScheduler_H