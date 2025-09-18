// In a new file: MLFQScheduler.h
#include <vector>
#include <queue>
#include <string>
#include "Process.h"


class MLFQScheduler {
private:
    std::vector<std::queue<Process*>> queues; // A vector of queues, each holding pointers to Processes
    std::vector<int> timeQuantums;            // Stores the time quantum for each respective queue
    std::string lastQueueAlgorithm;           // e.g., "SJF" or "STCF"
    
    int currentTime = 0;
    
    // This vector will hold all processes for loading and final reporting
    std::vector<Process> allProcesses; 

public:
    // Constructor to set up the scheduler with specific queue rules
    MLFQScheduler(const std::vector<int>& p_quantums, const std::string& p_lastQueueAlgo);

    // Main methods
    void loadProcessesFromFile(const std::string& filename);
    void run();
    void printResults();

private:
    // Helper methods for the simulation logic
    void admitNewProcesses();
    Process* getNextSTCFProcess(); // Example helper for the last queue
};