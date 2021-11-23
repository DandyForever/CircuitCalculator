#include <gtest/gtest.h>

#include "../circuit.h"

void generate_fully_coherent_subcircuits_test(size_t subcircuit_vertices, size_t subcircuit_number) {
    std::stringstream circuit_, answer_;
    for (size_t subcircuit_index = 0; subcircuit_index < subcircuit_number; subcircuit_index++) {
        size_t subcircuit_vertex_first = subcircuit_vertices * subcircuit_index + 1,
                subcircuit_vertex_last = subcircuit_vertices * (subcircuit_index + 1);
        for (size_t outcoming = subcircuit_vertex_first; outcoming <= subcircuit_vertex_last; outcoming++) {
            for (size_t incoming = outcoming + 1; incoming <= subcircuit_vertex_last; incoming++) {
                circuit_ << outcoming << " -- " << incoming << ", 1.0; ";
                answer_ << outcoming << " -- " << incoming << ": 0 A;\n";
            }
        }
    }
    circuit<double> test_circuit(circuit_.str());
    test_circuit.calculate_edge_current();
    std::string answer = answer_.str();
    answer.pop_back();
    EXPECT_EQ(answer, test_circuit.get_edge_current_answer());
}

void generate_fully_coherent_dc_circuit_test(size_t vertex_number) {
    generate_fully_coherent_subcircuits_test(vertex_number, 1);
}

void generate_consistent_circuit_test(size_t vertex_last) {
    std::stringstream circuit_, answer_;
    circuit_ << 1 << " -- " << vertex_last << ", 1; " << vertex_last << "V; ";
    answer_ << 1 << " -- " << vertex_last << ": 1 A;\n";
    for (size_t outcoming = 1; outcoming < vertex_last; outcoming++) {
        circuit_ << outcoming << " -- " << outcoming + 1 << ", 1; ";
        answer_ << outcoming << " -- " << outcoming + 1 << ": -1 A;\n";
    }
    circuit<double> test_circuit(circuit_.str());
    test_circuit.calculate_edge_current();
    std::string answer = answer_.str();
    answer.pop_back();
    EXPECT_EQ(answer, test_circuit.get_edge_current_answer());
}


TEST(FullyConnectedCircuitTest, FullyConnected10Test) { generate_fully_coherent_dc_circuit_test(10); }
TEST(FullyConnectedCircuitTest, FullyConnected20Test) { generate_fully_coherent_dc_circuit_test(20); }
TEST(FullyConnectedCircuitTest, FullyConnected30Test) { generate_fully_coherent_dc_circuit_test(30); }
TEST(FullyConnectedCircuitTest, FullyConnected40Test) { generate_fully_coherent_dc_circuit_test(40); }
TEST(FullyConnectedCircuitTest, FullyConnected50Test) { generate_fully_coherent_dc_circuit_test(50); }
TEST(FullyConnectedCircuitTest, FullyConnected60Test) { generate_fully_coherent_dc_circuit_test(60); }
TEST(FullyConnectedCircuitTest, FullyConnected70Test) { generate_fully_coherent_dc_circuit_test(70); }
TEST(FullyConnectedCircuitTest, FullyConnected80Test) { generate_fully_coherent_dc_circuit_test(80); }
TEST(FullyConnectedCircuitTest, FullyConnected90Test) { generate_fully_coherent_dc_circuit_test(90); }
TEST(FullyConnectedCircuitTest, FullyConnected100Test) { generate_fully_coherent_dc_circuit_test(100); }


TEST(ConsistentCircuitTest, Consistent10Test) { generate_consistent_circuit_test(10); }
TEST(ConsistentCircuitTest, Consistent20Test) { generate_consistent_circuit_test(20); }
TEST(ConsistentCircuitTest, Consistent30Test) { generate_consistent_circuit_test(30); }
TEST(ConsistentCircuitTest, Consistent40Test) { generate_consistent_circuit_test(40); }
TEST(ConsistentCircuitTest, Consistent50Test) { generate_consistent_circuit_test(50); }
TEST(ConsistentCircuitTest, Consistent60Test) { generate_consistent_circuit_test(60); }
TEST(ConsistentCircuitTest, Consistent70Test) { generate_consistent_circuit_test(70); }
TEST(ConsistentCircuitTest, Consistent80Test) { generate_consistent_circuit_test(80); }
TEST(ConsistentCircuitTest, Consistent90Test) { generate_consistent_circuit_test(90); }
TEST(ConsistentCircuitTest, Consistent100Test) { generate_consistent_circuit_test(100); }


TEST(FullyConnectedSubcircuitsTest, FullyConnected10Subcircuit2Test) { generate_fully_coherent_subcircuits_test(10, 2); }
TEST(FullyConnectedSubcircuitsTest, FullyConnected20Subcircuit2Test) { generate_fully_coherent_subcircuits_test(20, 2); }
TEST(FullyConnectedSubcircuitsTest, FullyConnected30Subcircuit2Test) { generate_fully_coherent_subcircuits_test(30, 2); }
TEST(FullyConnectedSubcircuitsTest, FullyConnected40Subcircuit2Test) { generate_fully_coherent_subcircuits_test(40, 2); }
TEST(FullyConnectedSubcircuitsTest, FullyConnected50Subcircuit2Test) { generate_fully_coherent_subcircuits_test(50, 2); }
TEST(FullyConnectedSubcircuitsTest, FullyConnected10Subcircuit5Test) { generate_fully_coherent_subcircuits_test(10, 5); }
TEST(FullyConnectedSubcircuitsTest, FullyConnected20Subcircuit5Test) { generate_fully_coherent_subcircuits_test(20, 5); }
TEST(FullyConnectedSubcircuitsTest, FullyConnected30Subcircuit5Test) { generate_fully_coherent_subcircuits_test(30, 5); }
TEST(FullyConnectedSubcircuitsTest, FullyConnected40Subcircuit5Test) { generate_fully_coherent_subcircuits_test(40, 5); }
TEST(FullyConnectedSubcircuitsTest, FullyConnected50Subcircuit5Test) { generate_fully_coherent_subcircuits_test(50, 5); }
TEST(FullyConnectedSubcircuitsTest, FullyConnected10Subcircuit10Test) { generate_fully_coherent_subcircuits_test(10, 10); }
TEST(FullyConnectedSubcircuitsTest, FullyConnected20Subcircuit10Test) { generate_fully_coherent_subcircuits_test(20, 10); }
TEST(FullyConnectedSubcircuitsTest, FullyConnected30Subcircuit10Test) { generate_fully_coherent_subcircuits_test(30, 10); }
TEST(FullyConnectedSubcircuitsTest, FullyConnected40Subcircuit10Test) { generate_fully_coherent_subcircuits_test(40, 10); }
TEST(FullyConnectedSubcircuitsTest, FullyConnected50Subcircuit10Test) { generate_fully_coherent_subcircuits_test(50, 10); }
TEST(FullyConnectedSubcircuitsTest, FullyConnected10Subcircuit50Test) { generate_fully_coherent_subcircuits_test(10, 50); }
TEST(FullyConnectedSubcircuitsTest, FullyConnected20Subcircuit50Test) { generate_fully_coherent_subcircuits_test(20, 50); }
TEST(FullyConnectedSubcircuitsTest, FullyConnected30Subcircuit50Test) { generate_fully_coherent_subcircuits_test(30, 50); }
TEST(FullyConnectedSubcircuitsTest, FullyConnected40Subcircuit50Test) { generate_fully_coherent_subcircuits_test(40, 50); }
TEST(FullyConnectedSubcircuitsTest, FullyConnected50Subcircuit50Test) { generate_fully_coherent_subcircuits_test(50, 50); }

