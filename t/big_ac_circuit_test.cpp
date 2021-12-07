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
                circuit_ << outcoming << " -- " << incoming << ", 1.0R; 1.0C; 1.0L;\n";
                answer_ << outcoming << " -- " << incoming << ": 0 A, 0;\n";
            }
        }
    }
    circuit<std::complex<double>> test_circuit(circuit_.str());
    test_circuit.calculate_edge_current();
    std::string answer = answer_.str();
    answer.pop_back();
    EXPECT_EQ(answer, test_circuit.get_edge_current_answer());
//    std::ofstream test_file("..\\t\\e2e_ac_test\\" + test_file_name + "_test.txt");
//    std::ofstream answer_file("..\\t\\e2e_ac_test\\" + test_file_name + "_answer.txt");
//    test_file << circuit_.str();
//    answer_file << answer_.str();
}

void generate_fully_coherent_circuit_test(size_t vertex_number/*, const std::string& test_file_name*/) {
    generate_fully_coherent_subcircuits_test(vertex_number, 1/*, test_file_name*/);
}

enum class CircuitType {
    AC_RESISTANCE,
    AC_CAPACITY,
    AC_INDUCTANCE,
    AC_RESISTANCE_CAPACITY,
    AC_RESISTANCE_INDUCTANCE
};

void generate_consistent_circuit_test(size_t vertex_last, CircuitType type/*, const std::string& test_file_name*/) {
    std::stringstream circuit_, answer_;
    circuit_ << 1 << " -- " << vertex_last << ", ";
    answer_ << 1 << " -- " << vertex_last << ": ";
    switch (type) {
        case CircuitType::AC_RESISTANCE:
            circuit_ << "1R; 0C; 0L; ";
            answer_ << "1 A, 0";
            break;
        case CircuitType::AC_CAPACITY:
            circuit_ << "0R; 1C; 0L; ";
            answer_ << "1 A, 90";
            break;
        case CircuitType::AC_INDUCTANCE:
            circuit_ << "0R; 0C; 1L; ";
            answer_ << "1 A, -90";
            break;
        case CircuitType::AC_RESISTANCE_CAPACITY:
            circuit_ << "1R; 1C; 0L; ";
            answer_ << "0.707107 A, 45";
            break;
        case CircuitType::AC_RESISTANCE_INDUCTANCE:
            circuit_ << "1R; 0C; 1L; ";
            answer_ << "0.707107 A, -45";
            break;
    }
    circuit_ << vertex_last << "V, 0;\n";
    answer_ << ";\n";
    for (size_t outcoming = 1; outcoming < vertex_last; outcoming++) {
        circuit_ << outcoming << " -- " << outcoming + 1 << ", ";
        answer_ << outcoming << " -- " << outcoming + 1 << ": ";
        switch (type) {
            case CircuitType::AC_RESISTANCE:
                circuit_ << "1R; 0C; 0L;";
                answer_ << "1 A, 180";
                break;
            case CircuitType::AC_CAPACITY:
                circuit_ << "0R; 1C; 0L;";
                answer_ << "1 A, -90";
                break;
            case CircuitType::AC_INDUCTANCE:
                circuit_ << "0R; 0C; 1L;";
                answer_ << "1 A, 90";
                break;
            case CircuitType::AC_RESISTANCE_CAPACITY:
                circuit_ << "1R; 1C; 0L;";
                answer_ << "0.707107 A, -135";
                break;
            case CircuitType::AC_RESISTANCE_INDUCTANCE:
                circuit_ << "1R; 0C; 1L;";
                answer_ << "0.707107 A, 135";
                break;
        }
        circuit_ << '\n';
        answer_ << ";\n";
    }
    circuit<std::complex<double>> test_circuit(circuit_.str());
    test_circuit.calculate_edge_current();
    std::string answer = answer_.str();
    answer.pop_back();
    EXPECT_EQ(answer, test_circuit.get_edge_current_answer());
//    std::ofstream test_file("..\\t\\e2e_ac_test\\" + test_file_name + "_test.txt");
//    std::ofstream answer_file("..\\t\\e2e_ac_test\\" + test_file_name + "_answer.txt");
//    test_file << circuit_.str();
//    answer_file << answer_.str();
}

TEST(FullyConnectedCircuitTest, FullyConnected10Test) { generate_fully_coherent_circuit_test(10/*, "FullyConnected10"*/); }
TEST(FullyConnectedCircuitTest, FullyConnected20Test) { generate_fully_coherent_circuit_test(20/*, "FullyConnected20"*/); }
TEST(FullyConnectedCircuitTest, FullyConnected30Test) { generate_fully_coherent_circuit_test(30/*, "FullyConnected30"*/); }
TEST(FullyConnectedCircuitTest, FullyConnected40Test) { generate_fully_coherent_circuit_test(40/*, "FullyConnected40"*/); }
TEST(FullyConnectedCircuitTest, FullyConnected50Test) { generate_fully_coherent_circuit_test(50/*, "FullyConnected50"*/); }
TEST(FullyConnectedCircuitTest, FullyConnected60Test) { generate_fully_coherent_circuit_test(60/*, "FullyConnected60"*/); }
TEST(FullyConnectedCircuitTest, FullyConnected70Test) { generate_fully_coherent_circuit_test(70/*, "FullyConnected70"*/); }
TEST(FullyConnectedCircuitTest, FullyConnected80Test) { generate_fully_coherent_circuit_test(80/*, "FullyConnected80"*/); }
TEST(FullyConnectedCircuitTest, FullyConnected90Test) { generate_fully_coherent_circuit_test(90/*, "FullyConnected90"*/); }
TEST(FullyConnectedCircuitTest, FullyConnected100Test) {
    generate_fully_coherent_circuit_test(100/*, "FullyConnected100"*/); }

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

TEST(ConsistentCircuitTest, ResistanceConsistent10Test) { generate_consistent_circuit_test(10, CircuitType::AC_RESISTANCE/*, "ResistanceConsistent10"*/); }
TEST(ConsistentCircuitTest, ResistanceConsistent20Test) { generate_consistent_circuit_test(20, CircuitType::AC_RESISTANCE/*, "ResistanceConsistent20"*/); }
TEST(ConsistentCircuitTest, ResistanceConsistent30Test) { generate_consistent_circuit_test(30, CircuitType::AC_RESISTANCE/*, "ResistanceConsistent30"*/); }
TEST(ConsistentCircuitTest, ResistanceConsistent40Test) { generate_consistent_circuit_test(40, CircuitType::AC_RESISTANCE/*, "ResistanceConsistent40"*/); }
TEST(ConsistentCircuitTest, ResistanceConsistent50Test) { generate_consistent_circuit_test(50, CircuitType::AC_RESISTANCE/*, "ResistanceConsistent50"*/); }
TEST(ConsistentCircuitTest, ResistanceConsistent60Test) { generate_consistent_circuit_test(60, CircuitType::AC_RESISTANCE/*, "ResistanceConsistent60"*/); }
TEST(ConsistentCircuitTest, ResistanceConsistent70Test) { generate_consistent_circuit_test(70, CircuitType::AC_RESISTANCE/*, "ResistanceConsistent70"*/); }
TEST(ConsistentCircuitTest, ResistanceConsistent80Test) { generate_consistent_circuit_test(80, CircuitType::AC_RESISTANCE/*, "ResistanceConsistent80"*/); }
TEST(ConsistentCircuitTest, ResistanceConsistent90Test) { generate_consistent_circuit_test(90, CircuitType::AC_RESISTANCE/*, "ResistanceConsistent90"*/); }
TEST(ConsistentCircuitTest, ResistanceConsistent100Test) { generate_consistent_circuit_test(100, CircuitType::AC_RESISTANCE/*, "ResistanceConsistent100"*/); }

TEST(ConsistentCircuitTest, CapacityConsistent10Test) { generate_consistent_circuit_test(10, CircuitType::AC_CAPACITY/*, "CapacityConsistent10"*/); }
TEST(ConsistentCircuitTest, CapacityConsistent20Test) { generate_consistent_circuit_test(20, CircuitType::AC_CAPACITY/*, "CapacityConsistent20"*/); }
TEST(ConsistentCircuitTest, CapacityConsistent30Test) { generate_consistent_circuit_test(30, CircuitType::AC_CAPACITY/*, "CapacityConsistent30"*/); }
TEST(ConsistentCircuitTest, CapacityConsistent40Test) { generate_consistent_circuit_test(40, CircuitType::AC_CAPACITY/*, "CapacityConsistent40"*/); }
TEST(ConsistentCircuitTest, CapacityConsistent50Test) { generate_consistent_circuit_test(50, CircuitType::AC_CAPACITY/*, "CapacityConsistent50"*/); }
TEST(ConsistentCircuitTest, CapacityConsistent60Test) { generate_consistent_circuit_test(60, CircuitType::AC_CAPACITY/*, "CapacityConsistent60"*/); }
TEST(ConsistentCircuitTest, CapacityConsistent70Test) { generate_consistent_circuit_test(70, CircuitType::AC_CAPACITY/*, "CapacityConsistent70"*/); }
TEST(ConsistentCircuitTest, CapacityConsistent80Test) { generate_consistent_circuit_test(80, CircuitType::AC_CAPACITY/*, "CapacityConsistent80"*/); }
TEST(ConsistentCircuitTest, CapacityConsistent90Test) { generate_consistent_circuit_test(90, CircuitType::AC_CAPACITY/*, "CapacityConsistent90"*/); }
TEST(ConsistentCircuitTest, CapacityConsistent100Test) { generate_consistent_circuit_test(100, CircuitType::AC_CAPACITY/*, "CapacityConsistent100"*/); }

TEST(ConsistentCircuitTest, InductanceConsistent10Test) { generate_consistent_circuit_test(10, CircuitType::AC_INDUCTANCE/*, "InductanceConsistent10"*/); }
TEST(ConsistentCircuitTest, InductanceConsistent20Test) { generate_consistent_circuit_test(20, CircuitType::AC_INDUCTANCE/*, "InductanceConsistent20"*/); }
TEST(ConsistentCircuitTest, InductanceConsistent30Test) { generate_consistent_circuit_test(30, CircuitType::AC_INDUCTANCE/*, "InductanceConsistent30"*/); }
TEST(ConsistentCircuitTest, InductanceConsistent40Test) { generate_consistent_circuit_test(40, CircuitType::AC_INDUCTANCE/*, "InductanceConsistent40"*/); }
TEST(ConsistentCircuitTest, InductanceConsistent50Test) { generate_consistent_circuit_test(50, CircuitType::AC_INDUCTANCE/*, "InductanceConsistent50"*/); }
TEST(ConsistentCircuitTest, InductanceConsistent60Test) { generate_consistent_circuit_test(60, CircuitType::AC_INDUCTANCE/*, "InductanceConsistent60"*/); }
TEST(ConsistentCircuitTest, InductanceConsistent70Test) { generate_consistent_circuit_test(70, CircuitType::AC_INDUCTANCE/*, "InductanceConsistent70"*/); }
TEST(ConsistentCircuitTest, InductanceConsistent80Test) { generate_consistent_circuit_test(80, CircuitType::AC_INDUCTANCE/*, "InductanceConsistent80"*/); }
TEST(ConsistentCircuitTest, InductanceConsistent90Test) { generate_consistent_circuit_test(90, CircuitType::AC_INDUCTANCE/*, "InductanceConsistent90"*/); }
TEST(ConsistentCircuitTest, InductanceConsistent100Test) { generate_consistent_circuit_test(100, CircuitType::AC_INDUCTANCE/*, "InductanceConsistent100"*/); }

TEST(MixedConsistentCircuitTest, ResistanceCapacitanceConsistent10Test) { generate_consistent_circuit_test(10, CircuitType::AC_RESISTANCE_CAPACITY/*, "ResistanceCapacitanceConsistent10"*/); }
TEST(MixedConsistentCircuitTest, ResistanceCapacitanceConsistent20Test) { generate_consistent_circuit_test(20, CircuitType::AC_RESISTANCE_CAPACITY/*, "ResistanceCapacitanceConsistent20"*/); }
TEST(MixedConsistentCircuitTest, ResistanceCapacitanceConsistent30Test) { generate_consistent_circuit_test(30, CircuitType::AC_RESISTANCE_CAPACITY/*, "ResistanceCapacitanceConsistent30"*/); }
TEST(MixedConsistentCircuitTest, ResistanceCapacitanceConsistent40Test) { generate_consistent_circuit_test(40, CircuitType::AC_RESISTANCE_CAPACITY/*, "ResistanceCapacitanceConsistent40"*/); }
TEST(MixedConsistentCircuitTest, ResistanceCapacitanceConsistent50Test) { generate_consistent_circuit_test(50, CircuitType::AC_RESISTANCE_CAPACITY/*, "ResistanceCapacitanceConsistent50"*/); }
TEST(MixedConsistentCircuitTest, ResistanceCapacitanceConsistent60Test) { generate_consistent_circuit_test(60, CircuitType::AC_RESISTANCE_CAPACITY/*, "ResistanceCapacitanceConsistent60"*/); }
TEST(MixedConsistentCircuitTest, ResistanceCapacitanceConsistent70Test) { generate_consistent_circuit_test(70, CircuitType::AC_RESISTANCE_CAPACITY/*, "ResistanceCapacitanceConsistent70"*/); }
TEST(MixedConsistentCircuitTest, ResistanceCapacitanceConsistent80Test) { generate_consistent_circuit_test(80, CircuitType::AC_RESISTANCE_CAPACITY/*, "ResistanceCapacitanceConsistent80"*/); }
TEST(MixedConsistentCircuitTest, ResistanceCapacitanceConsistent90Test) { generate_consistent_circuit_test(90, CircuitType::AC_RESISTANCE_CAPACITY/*, "ResistanceCapacitanceConsistent90"*/); }
TEST(MixedConsistentCircuitTest, ResistanceCapacitanceConsistent100Test) { generate_consistent_circuit_test(100, CircuitType::AC_RESISTANCE_CAPACITY/*, "ResistanceCapacitanceConsistent100"*/); }

TEST(MixedConsistentCircuitTest, ResistanceInductanceConsistent10Test) { generate_consistent_circuit_test(10, CircuitType::AC_RESISTANCE_INDUCTANCE/*, "ResistanceInductanceConsistent10"*/); }
TEST(MixedConsistentCircuitTest, ResistanceInductanceConsistent20Test) { generate_consistent_circuit_test(20, CircuitType::AC_RESISTANCE_INDUCTANCE/*, "ResistanceInductanceConsistent20"*/); }
TEST(MixedConsistentCircuitTest, ResistanceInductanceConsistent30Test) { generate_consistent_circuit_test(30, CircuitType::AC_RESISTANCE_INDUCTANCE/*, "ResistanceInductanceConsistent30"*/); }
TEST(MixedConsistentCircuitTest, ResistanceInductanceConsistent40Test) { generate_consistent_circuit_test(40, CircuitType::AC_RESISTANCE_INDUCTANCE/*, "ResistanceInductanceConsistent40"*/); }
TEST(MixedConsistentCircuitTest, ResistanceInductanceConsistent50Test) { generate_consistent_circuit_test(50, CircuitType::AC_RESISTANCE_INDUCTANCE/*, "ResistanceInductanceConsistent50"*/); }
TEST(MixedConsistentCircuitTest, ResistanceInductanceConsistent60Test) { generate_consistent_circuit_test(60, CircuitType::AC_RESISTANCE_INDUCTANCE/*, "ResistanceInductanceConsistent60"*/); }
TEST(MixedConsistentCircuitTest, ResistanceInductanceConsistent70Test) { generate_consistent_circuit_test(70, CircuitType::AC_RESISTANCE_INDUCTANCE/*, "ResistanceInductanceConsistent70"*/); }
TEST(MixedConsistentCircuitTest, ResistanceInductanceConsistent80Test) { generate_consistent_circuit_test(80, CircuitType::AC_RESISTANCE_INDUCTANCE/*, "ResistanceInductanceConsistent80"*/); }
TEST(MixedConsistentCircuitTest, ResistanceInductanceConsistent90Test) { generate_consistent_circuit_test(90, CircuitType::AC_RESISTANCE_INDUCTANCE/*, "ResistanceInductanceConsistent90"*/); }
TEST(MixedConsistentCircuitTest, ResistanceInductanceConsistent100Test) { generate_consistent_circuit_test(100, CircuitType::AC_RESISTANCE_INDUCTANCE/*, "ResistanceInductanceConsistent100"*/); }