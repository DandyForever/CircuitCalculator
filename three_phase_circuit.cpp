#include <fstream>
#include "circuit.h"

int main(int argc, char* argv[]) {
    std::cout << argc << std::endl;
    if (argc > 2) {
        std::cout << "Invalid number of arguments: only input file required" << std::endl;
        return 1;
    }
    if (argc == 2) {
        std::ifstream input_file;
        input_file.open(argv[1]);
        std::cout << argv[1] << std::endl;
        if (!input_file.is_open()) {
            std::cout << "File " << argv[1] << " failed to open" << std::endl;
            return 2;
        }
        calculate_complex_circuit<std::complex<double>>(input_file, argv[1]);
        input_file.close();
        return 0;
    }
    calculate_complex_circuit<std::complex<double>>();
    return 0;
}
