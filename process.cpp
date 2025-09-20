#include "Process.h"

// Constructor implementation
Process::Process(std::string p_id, int p_burstTime, int p_arrivalTime) {
    // Initialize with values from the file
    id = p_id;
    burstTime = p_burstTime;
    arrivalTime = p_arrivalTime;

    // Set default/initial states for the scheduler
    remainingTime = burstTime;
    currentQueueIndex = 0;
    
    // Initialize metrics to zero or default values
    completionTime = 0;
    waitingTime = 0;
    responseTime = -1; // Use -1 to indicate it hasn't been calculated yet
    turnaroundTime = 0;
    hasStartedExecution = false;
    timeSliceUsed = 0;
}