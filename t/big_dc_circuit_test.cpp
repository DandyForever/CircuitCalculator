#include <gtest/gtest.h>
#include <fstream>

#include "../circuit.h"

void generate_fully_coherent_subcircuits_test(size_t subcircuit_vertices, size_t subcircuit_number/*, const std::string& test_file_name*/) {
    std::stringstream circuit_, answer_;
    for (size_t subcircuit_index = 0; subcircuit_index < subcircuit_number; subcircuit_index++) {
        size_t subcircuit_vertex_first = subcircuit_vertices * subcircuit_index + 1,
                subcircuit_vertex_last = subcircuit_vertices * (subcircuit_index + 1);
        for (size_t outcoming = subcircuit_vertex_first; outcoming <= subcircuit_vertex_last; outcoming++) {
            for (size_t incoming = outcoming + 1; incoming <= subcircuit_vertex_last; incoming++) {
                circuit_ << outcoming << " -- " << incoming << ", 1.0;\n";
                answer_ << outcoming << " -- " << incoming << ": 0 A;\n";
            }
        }
    }
    circuit<double> test_circuit(circuit_.str());
    test_circuit.calculate_edge_current();
    std::string answer = answer_.str();
    answer.pop_back();
    EXPECT_EQ(answer, test_circuit.get_edge_current_answer());
//    std::ofstream test_file("..\\t\\e2e_dc_test\\" + test_file_name + "_test.txt");
//    std::ofstream answer_file("..\\t\\e2e_dc_test\\" + test_file_name + "_answer.txt");
//    test_file << circuit_.str();
//    answer_file << answer_.str();
}

void generate_fully_coherent_dc_circuit_test(size_t vertex_number/*, const std::string& test_file_name*/) {
    generate_fully_coherent_subcircuits_test(vertex_number, 1/*, test_file_name*/);
}

void generate_consistent_circuit_test(size_t vertex_last/*, const std::string& test_file_name*/) {
    std::stringstream circuit_, answer_;
    circuit_ << 1 << " -- " << vertex_last << ", 1; " << vertex_last << "V;\n";
    answer_ << 1 << " -- " << vertex_last << ": 1 A;\n";
    for (size_t outcoming = 1; outcoming < vertex_last; outcoming++) {
        circuit_ << outcoming << " -- " << outcoming + 1 << ", 1;\n";
        answer_ << outcoming << " -- " << outcoming + 1 << ": -1 A;\n";
    }
    circuit<double> test_circuit(circuit_.str());
    test_circuit.calculate_edge_current();
    std::string answer = answer_.str();
    answer.pop_back();
    EXPECT_EQ(answer, test_circuit.get_edge_current_answer());
//    std::ofstream test_file("..\\t\\e2e_dc_test\\" + test_file_name + "_test.txt");
//    std::ofstream answer_file("..\\t\\e2e_dc_test\\" + test_file_name + "_answer.txt");
//    test_file << circuit_.str();
//    answer_file << answer_.str();
}

TEST(FullyConnectedCircuitTest, FullyConnected10Test) { generate_fully_coherent_dc_circuit_test(10/*, "FullyConnected10"*/); }
TEST(FullyConnectedCircuitTest, FullyConnected20Test) { generate_fully_coherent_dc_circuit_test(20/*, "FullyConnected20"*/); }
TEST(FullyConnectedCircuitTest, FullyConnected30Test) { generate_fully_coherent_dc_circuit_test(30/*, "FullyConnected30"*/); }
TEST(FullyConnectedCircuitTest, FullyConnected40Test) { generate_fully_coherent_dc_circuit_test(40/*, "FullyConnected40"*/); }
TEST(FullyConnectedCircuitTest, FullyConnected50Test) { generate_fully_coherent_dc_circuit_test(50/*, "FullyConnected50"*/); }
TEST(FullyConnectedCircuitTest, FullyConnected60Test) { generate_fully_coherent_dc_circuit_test(60/*, "FullyConnected60"*/); }
TEST(FullyConnectedCircuitTest, FullyConnected70Test) { generate_fully_coherent_dc_circuit_test(70/*, "FullyConnected70"*/); }
TEST(FullyConnectedCircuitTest, FullyConnected80Test) { generate_fully_coherent_dc_circuit_test(80/*, "FullyConnected80"*/); }
TEST(FullyConnectedCircuitTest, FullyConnected90Test) { generate_fully_coherent_dc_circuit_test(90/*, "FullyConnected90"*/); }
TEST(FullyConnectedCircuitTest, FullyConnected100Test) { generate_fully_coherent_dc_circuit_test(100/*, "FullyConnected100"*/); }



TEST(ConsistentCircuitTest, Consistent10Test) { generate_consistent_circuit_test(10/*, "Consistent10"*/); }
TEST(ConsistentCircuitTest, Consistent20Test) { generate_consistent_circuit_test(20/*, "Consistent20"*/); }
TEST(ConsistentCircuitTest, Consistent30Test) { generate_consistent_circuit_test(30/*, "Consistent30"*/); }
TEST(ConsistentCircuitTest, Consistent40Test) { generate_consistent_circuit_test(40/*, "Consistent40"*/); }
TEST(ConsistentCircuitTest, Consistent50Test) { generate_consistent_circuit_test(50/*, "Consistent50"*/); }
TEST(ConsistentCircuitTest, Consistent60Test) { generate_consistent_circuit_test(60/*, "Consistent60"*/); }
TEST(ConsistentCircuitTest, Consistent70Test) { generate_consistent_circuit_test(70/*, "Consistent70"*/); }
TEST(ConsistentCircuitTest, Consistent80Test) { generate_consistent_circuit_test(80/*, "Consistent80"*/); }
TEST(ConsistentCircuitTest, Consistent90Test) { generate_consistent_circuit_test(90/*, "Consistent90"*/); }
TEST(ConsistentCircuitTest, Consistent100Test) { generate_consistent_circuit_test(100/*, "Consistent100"*/); }


TEST(FullyConnectedSubcircuitsTest, FullyConnected10Subcircuit2Test) { generate_fully_coherent_subcircuits_test(10, 2/*, "FullyConnected10Subcircuit2"*/); }
TEST(FullyConnectedSubcircuitsTest, FullyConnected20Subcircuit2Test) { generate_fully_coherent_subcircuits_test(20, 2/*, "FullyConnected20Subcircuit2"*/); }
TEST(FullyConnectedSubcircuitsTest, FullyConnected30Subcircuit2Test) { generate_fully_coherent_subcircuits_test(30, 2/*, "FullyConnected30Subcircuit2"*/); }
TEST(FullyConnectedSubcircuitsTest, FullyConnected40Subcircuit2Test) { generate_fully_coherent_subcircuits_test(40, 2/*, "FullyConnected40Subcircuit2"*/); }
TEST(FullyConnectedSubcircuitsTest, FullyConnected50Subcircuit2Test) { generate_fully_coherent_subcircuits_test(50, 2/*, "FullyConnected50Subcircuit2"*/); }
TEST(FullyConnectedSubcircuitsTest, FullyConnected10Subcircuit5Test) { generate_fully_coherent_subcircuits_test(10, 5/*, "FullyConnected10Subcircuit5"*/); }
TEST(FullyConnectedSubcircuitsTest, FullyConnected20Subcircuit5Test) { generate_fully_coherent_subcircuits_test(20, 5/*, "FullyConnected20Subcircuit5"*/); }
TEST(FullyConnectedSubcircuitsTest, FullyConnected30Subcircuit5Test) { generate_fully_coherent_subcircuits_test(30, 5/*, "FullyConnected30Subcircuit5"*/); }
TEST(FullyConnectedSubcircuitsTest, FullyConnected40Subcircuit5Test) { generate_fully_coherent_subcircuits_test(40, 5/*, "FullyConnected40Subcircuit5"*/); }
TEST(FullyConnectedSubcircuitsTest, FullyConnected50Subcircuit5Test) { generate_fully_coherent_subcircuits_test(50, 5/*, "FullyConnected50Subcircuit5"*/); }
TEST(FullyConnectedSubcircuitsTest, FullyConnected10Subcircuit10Test) { generate_fully_coherent_subcircuits_test(10, 10/*, "FullyConnected10Subcircuit10"*/); }
TEST(FullyConnectedSubcircuitsTest, FullyConnected20Subcircuit10Test) { generate_fully_coherent_subcircuits_test(20, 10/*, "FullyConnected20Subcircuit10"*/); }
TEST(FullyConnectedSubcircuitsTest, FullyConnected30Subcircuit10Test) { generate_fully_coherent_subcircuits_test(30, 10/*, "FullyConnected30Subcircuit10"*/); }
TEST(FullyConnectedSubcircuitsTest, FullyConnected40Subcircuit10Test) { generate_fully_coherent_subcircuits_test(40, 10/*, "FullyConnected40Subcircuit10"*/); }
TEST(FullyConnectedSubcircuitsTest, FullyConnected50Subcircuit10Test) { generate_fully_coherent_subcircuits_test(40, 10/*, "FullyConnected50Subcircuit10"*/); }
TEST(FullyConnectedSubcircuitsTest, FullyConnected10Subcircuit50Test) { generate_fully_coherent_subcircuits_test(10, 50/*, "FullyConnected10Subcircuit50"*/); }
TEST(FullyConnectedSubcircuitsTest, FullyConnected20Subcircuit50Test) { generate_fully_coherent_subcircuits_test(20, 50/*, "FullyConnected20Subcircuit50"*/); }
TEST(FullyConnectedSubcircuitsTest, FullyConnected30Subcircuit50Test) { generate_fully_coherent_subcircuits_test(30, 50/*, "FullyConnected30Subcircuit50"*/); }
TEST(FullyConnectedSubcircuitsTest, FullyConnected40Subcircuit50Test) { generate_fully_coherent_subcircuits_test(40, 50/*, "FullyConnected40Subcircuit50"*/); }
TEST(FullyConnectedSubcircuitsTest, FullyConnected50Subcircuit50Test) { generate_fully_coherent_subcircuits_test(50, 50/*, "FullyConnected50Subcircuit50"*/); }