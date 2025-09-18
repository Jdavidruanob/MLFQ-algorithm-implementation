// In a new file: Process.h
#include <string>

class Process {
    public:
        // Attributes from the input file
        std::string id;
        int burstTime;
        int arrivalTime;

        // Attributes for the scheduler to manage
        int remainingTime;
        int currentQueueIndex; // e.g., 0 for the highest priority queue
        int timeSliceUsed = 0; // Tracks time used in the current queue's quantum

        // Attributes for the final metrics
        int completionTime;
        int waitingTime;
        int responseTime;
        int turnaroundTime;
        bool hasStartedExecution = false; // To help calculate response time
        

        // Constructor to initialize the process
        Process(std::string p_id, int p_burstTime, int p_arrivalTime);
};