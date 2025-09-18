#include "MLFQScheduler.h"
#include <fstream>   // For file handling (ifstream)
#include <sstream>   // For string parsing (stringstream)
#include <iostream>  // For error messages

// Constructor implementation
MLFQScheduler::MLFQScheduler(const std::vector<int>& p_quantums, const std::string& p_lastQueueAlgo) {
    timeQuantums = p_quantums;
    lastQueueAlgorithm = p_lastQueueAlgo;

    // Create the specified number of queues
    // The number of queues is equal to the number of time quantums provided.
    queues.resize(timeQuantums.size());
    
    currentTime = 0;
}


// We will add the other methods to this file below...