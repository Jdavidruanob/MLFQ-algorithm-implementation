#include "MLFQScheduler.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <numeric>

// Constructor implementation
MLFQScheduler::MLFQScheduler(const std::vector<int>& p_quantums, const std::string& p_lastQueueAlgo) {
    timeQuantums = p_quantums;
    lastQueueAlgorithm = p_lastQueueAlgo;
    
    int num_queues = (lastQueueAlgorithm == "RR") ? timeQuantums.size() : timeQuantums.size() + 1;
    queues.resize(num_queues);
    
    currentTime = 0;
}

// Method to load processes from a text file
void MLFQScheduler::loadProcessesFromFile(const std::string& filename) {
    allProcesses.clear(); // Clear any previous processes
    std::ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return;
    }

    std::string line;
    std::getline(inputFile, line); // Skip header

    while (std::getline(inputFile, line)) {
        if (line.empty() || line[0] == '#') continue;

        std::stringstream ss(line);
        std::string id_str, burst_str, arrival_str;
        std::getline(ss, id_str, ';');
        std::getline(ss, burst_str, ';');
        std::getline(ss, arrival_str, ';');
        
        try {
            allProcesses.emplace_back(id_str, std::stoi(burst_str), std::stoi(arrival_str));
        } catch (const std::invalid_argument& e) {
            std::cerr << "Warning: Could not parse line, skipping: " << line << std::endl;
        }
    }
}

// Helper method to admit newly arrived processes to the top queue
void MLFQScheduler::admitNewProcesses() {
    for (Process& p : allProcesses) {
        if (p.arrivalTime == currentTime) {
            queues[0].push(&p);
        }
    }
}

// Helper to find the best process in the last queue for SJF/STCF and move it to the front
void MLFQScheduler::findAndPromoteBestProcess() {
    std::queue<Process*>& lastQueue = queues.back();
    if (lastQueue.size() <= 1) return;

    Process* bestProcess = lastQueue.front();
    std::queue<Process*> tempQueue;
    
    while (!lastQueue.empty()) {
        Process* current = lastQueue.front();
        lastQueue.pop();
        tempQueue.push(current);
        if (lastQueueAlgorithm == "STCF" && current->remainingTime < bestProcess->remainingTime) {
            bestProcess = current;
        } else if (lastQueueAlgorithm == "SJF" && current->burstTime < bestProcess->burstTime) {
            bestProcess = current;
        }
    }

    std::queue<Process*> finalQueue;
    finalQueue.push(bestProcess);
    while (!tempQueue.empty()) {
        Process* p = tempQueue.front();
        tempQueue.pop();
        if (p->id != bestProcess->id) {
            finalQueue.push(p);
        }
    }
    lastQueue = finalQueue;
}

// Main simulation loop
void MLFQScheduler::run() {
    int finishedProcessesCount = 0;
    while (finishedProcessesCount < allProcesses.size()) {
        admitNewProcesses();

        int activeQueueIndex = -1;
        for (int i = 0; i < queues.size(); ++i) {
            if (!queues[i].empty()) {
                activeQueueIndex = i;
                break;
            }
        }

        if (activeQueueIndex != -1) {
            bool isLastQueue = (activeQueueIndex == queues.size() - 1);
            if (isLastQueue && (lastQueueAlgorithm == "SJF" || lastQueueAlgorithm == "STCF")) {
                findAndPromoteBestProcess();
            }
            
            Process* processToRun = queues[activeQueueIndex].front();

            if (!processToRun->hasStartedExecution) {
                processToRun->responseTime = currentTime - processToRun->arrivalTime;
                processToRun->hasStartedExecution = true;
            }

            processToRun->remainingTime--;
            processToRun->timeSliceUsed++;

            if (processToRun->remainingTime == 0) {
                processToRun->completionTime = currentTime + 1;
                finishedProcessesCount++;
                queues[activeQueueIndex].pop();
            } else if (!isLastQueue && processToRun->timeSliceUsed == timeQuantums[activeQueueIndex]) {
                queues[activeQueueIndex].pop();
                queues[activeQueueIndex + 1].push(processToRun);
                processToRun->currentQueueIndex++;
                processToRun->timeSliceUsed = 0;
            }
        }
        
        currentTime++;
    }
}

// Method to calculate final metrics and print results to a file
void MLFQScheduler::printResults(const std::string& outputFilename) {
    std::ofstream outputFile(outputFilename);
    if (!outputFile.is_open()) {
        std::cerr << "Error: Could not open output file " << outputFilename << std::endl;
        return;
    }

    double totalWT = 0, totalCT = 0, totalRT = 0, totalTAT = 0;
    for (Process& p : allProcesses) {
        p.turnaroundTime = p.completionTime - p.arrivalTime;
        p.waitingTime = p.turnaroundTime - p.burstTime;
        totalWT += p.waitingTime;
        totalCT += p.completionTime;
        totalRT += p.responseTime;
        totalTAT += p.turnaroundTime;
    }

    double avgWT = totalWT / allProcesses.size();
    double avgCT = totalCT / allProcesses.size();
    double avgRT = totalRT / allProcesses.size();
    double avgTAT = totalTAT / allProcesses.size();

    outputFile << "#etiqueta;BT;AT;Q;Pr;WT;CT;RT;TAT" << std::endl;
    for (const Process& p : allProcesses) {
        outputFile << p.id << ";" << p.burstTime << ";" << p.arrivalTime << ";N/A;N/A;" 
                   << p.waitingTime << ";" << p.completionTime << ";" << p.responseTime << ";" << p.turnaroundTime << std::endl;
    }

    outputFile << std::fixed << std::setprecision(2);
    outputFile << "$WT=" << avgWT << ";$CT=" << avgCT << ";$RT=" << avgRT << ";$TAT=" << avgTAT << ";" << std::endl;
    outputFile.close();
}