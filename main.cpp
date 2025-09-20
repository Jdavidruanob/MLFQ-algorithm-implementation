#include "MLFQScheduler.h"
#include <iostream>
#include <vector>
#include <string>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <input_file.txt>" << std::endl;
        return 1;
    }
    std::string inputFilename = argv[1];

    std::cout << "Starting MLFQ Scheduler Simulation..." << std::endl;
    std::cout << "=======================================" << std::endl;
    std::cout << "Input file: " << inputFilename << std::endl << std::endl;

    // --- Scheme 1: RR(1), RR(3), RR(4), SJF ---
    std::cout << "--- Running Simulation for Scheme 1: RR(1), RR(3), RR(4), SJF ---" << std::endl;
    std::vector<int> quantums1 = {1, 3, 4};
    MLFQScheduler scheduler1(quantums1, "SJF");
    scheduler1.loadProcessesFromFile(inputFilename);
    scheduler1.run();
    scheduler1.printResults("output_scheme1.txt");
    std::cout << "Scheme 1 finished. Results saved to output_scheme1.txt" << std::endl << std::endl;

    // --- Scheme 2: RR(2), RR(3), RR(4), STCF ---
    std::cout << "--- Running Simulation for Scheme 2: RR(2), RR(3), RR(4), STCF ---" << std::endl;
    std::vector<int> quantums2 = {2, 3, 4};
    MLFQScheduler scheduler2(quantums2, "STCF");
    scheduler2.loadProcessesFromFile(inputFilename);
    scheduler2.run();
    scheduler2.printResults("output_scheme2.txt");
    std::cout << "Scheme 2 finished. Results saved to output_scheme2.txt" << std::endl << std::endl;

    // --- Scheme 3: RR(3), RR(5), RR(6), RR(20) ---
    std::cout << "--- Running Simulation for Scheme 3: RR(3), RR(5), RR(6), RR(20) ---" << std::endl;
    std::vector<int> quantums3 = {3, 5, 6, 20};
    MLFQScheduler scheduler3(quantums3, "RR");
    scheduler3.loadProcessesFromFile(inputFilename);
    scheduler3.run();
    scheduler3.printResults("output_scheme3.txt");
    std::cout << "Scheme 3 finished. Results saved to output_scheme3.txt" << std::endl << std::endl;

    std::cout << "All simulations completed." << std::endl;
    return 0;
}