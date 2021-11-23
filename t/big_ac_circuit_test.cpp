#include <gtest/gtest.h>

#include "../circuit.h"


void generate_fully_coherent_subcircuits_test(size_t subcircuit_vertices, size_t subcircuit_number) {
    std::stringstream circuit_, answer_;
    for (size_t subcircuit_index = 0; subcircuit_index < subcircuit_number; subcircuit_index++) {
        size_t subcircuit_vertex_first = subcircuit_vertices * subcircuit_index + 1,
                subcircuit_vertex_last = subcircuit_vertices * (subcircuit_index + 1);
        for (size_t outcoming = subcircuit_vertex_first; outcoming <= subcircuit_vertex_last; outcoming++) {
            for (size_t incoming = outcoming + 1; incoming <= subcircuit_vertex_last; incoming++) {
                circuit_ << outcoming << " -- " << incoming << ", 1.0R; 1.0C; 1.0L; ";
                answer_ << outcoming << " -- " << incoming << ": 0 A, 0;\n";
            }
        }
    }
    circuit<std::complex<double>> test_circuit(circuit_.str());
    test_circuit.calculate_edge_current();
    std::string answer = answer_.str();
    answer.pop_back();
    EXPECT_EQ(answer, test_circuit.get_edge_current_answer());
}

void generate_fully_coherent_ac_circuit_test(size_t vertex_number) {
    generate_fully_coherent_subcircuits_test(vertex_number, 1);
}

enum class CircuitType {
    AC_RESISTANCE,
    AC_CAPACITY,
    AC_INDUCTANCE,
    AC_RESISTANCE_CAPACITY,
    AC_RESISTANCE_INDUCTANCE
};

void generate_consistent_circuit_test(size_t vertex_last, CircuitType type) {
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
            answer_ << "1 A, 1.5708";
            break;
        case CircuitType::AC_INDUCTANCE:
            circuit_ << "0R; 0C; 1L; ";
            answer_ << "1 A, -1.5708";
            break;
        case CircuitType::AC_RESISTANCE_CAPACITY:
            circuit_ << "1R; 1C; 0L; ";
            answer_ << "0.707107 A, 0.785398";
            break;
        case CircuitType::AC_RESISTANCE_INDUCTANCE:
            circuit_ << "1R; 0C; 1L; ";
            answer_ << "0.707107 A, -0.785398";
            break;
    }
    circuit_ << vertex_last << "V, 0; ";
    answer_ << ";\n";
    for (size_t outcoming = 1; outcoming < vertex_last; outcoming++) {
        circuit_ << outcoming << " -- " << outcoming + 1 << ", ";
        answer_ << outcoming << " -- " << outcoming + 1 << ": ";
        switch (type) {
            case CircuitType::AC_RESISTANCE:
                circuit_ << "1R; 0C; 0L; ";
                answer_ << "1 A, 3.14159";
                break;
            case CircuitType::AC_CAPACITY:
                circuit_ << "0R; 1C; 0L; ";
                answer_ << "1 A, -1.5708";
                break;
            case CircuitType::AC_INDUCTANCE:
                circuit_ << "0R; 0C; 1L; ";
                answer_ << "1 A, 1.5708";
                break;
            case CircuitType::AC_RESISTANCE_CAPACITY:
                circuit_ << "1R; 1C; 0L; ";
                answer_ << "0.707107 A, -2.35619";
                break;
            case CircuitType::AC_RESISTANCE_INDUCTANCE:
                circuit_ << "1R; 0C; 1L; ";
                answer_ << "0.707107 A, 2.35619";
                break;
        }
        answer_ << ";\n";
    }
    circuit<std::complex<double>> test_circuit(circuit_.str());
    test_circuit.calculate_edge_current();
    std::string answer = answer_.str();
    answer.pop_back();
    EXPECT_EQ(answer, test_circuit.get_edge_current_answer());
}

TEST(FullyConnectedCircuitTest, FullyConnected10Test) { generate_fully_coherent_ac_circuit_test(10); }
TEST(FullyConnectedCircuitTest, FullyConnected20Test) { generate_fully_coherent_ac_circuit_test(20); }
TEST(FullyConnectedCircuitTest, FullyConnected30Test) { generate_fully_coherent_ac_circuit_test(30); }
TEST(FullyConnectedCircuitTest, FullyConnected40Test) { generate_fully_coherent_ac_circuit_test(40); }
TEST(FullyConnectedCircuitTest, FullyConnected50Test) { generate_fully_coherent_ac_circuit_test(50); }
TEST(FullyConnectedCircuitTest, FullyConnected60Test) { generate_fully_coherent_ac_circuit_test(60); }
TEST(FullyConnectedCircuitTest, FullyConnected70Test) { generate_fully_coherent_ac_circuit_test(70); }
TEST(FullyConnectedCircuitTest, FullyConnected80Test) { generate_fully_coherent_ac_circuit_test(80); }
TEST(FullyConnectedCircuitTest, FullyConnected90Test) { generate_fully_coherent_ac_circuit_test(90); }
TEST(FullyConnectedCircuitTest, FullyConnected100Test) { generate_fully_coherent_ac_circuit_test(100); }

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
TEST(FullyConnectedSubcircuitsTest, FullyConnected50Subcircuit10Test) { generate_fully_coherent_subcircuits_test(40, 10); }
TEST(FullyConnectedSubcircuitsTest, FullyConnected10Subcircuit50Test) { generate_fully_coherent_subcircuits_test(10, 50); }
TEST(FullyConnectedSubcircuitsTest, FullyConnected20Subcircuit50Test) { generate_fully_coherent_subcircuits_test(20, 50); }
TEST(FullyConnectedSubcircuitsTest, FullyConnected30Subcircuit50Test) { generate_fully_coherent_subcircuits_test(30, 50); }
TEST(FullyConnectedSubcircuitsTest, FullyConnected40Subcircuit50Test) { generate_fully_coherent_subcircuits_test(40, 50); }
TEST(FullyConnectedSubcircuitsTest, FullyConnected50Subcircuit50Test) { generate_fully_coherent_subcircuits_test(50, 50); }

TEST(ConsistentCircuitTest, ResistanceConsistent10Test) { generate_consistent_circuit_test(10, CircuitType::AC_RESISTANCE); }
TEST(ConsistentCircuitTest, ResistanceConsistent20Test) { generate_consistent_circuit_test(20, CircuitType::AC_RESISTANCE); }
TEST(ConsistentCircuitTest, ResistanceConsistent30Test) { generate_consistent_circuit_test(30, CircuitType::AC_RESISTANCE); }
TEST(ConsistentCircuitTest, ResistanceConsistent40Test) { generate_consistent_circuit_test(40, CircuitType::AC_RESISTANCE); }
TEST(ConsistentCircuitTest, ResistanceConsistent50Test) { generate_consistent_circuit_test(50, CircuitType::AC_RESISTANCE); }
TEST(ConsistentCircuitTest, ResistanceConsistent60Test) { generate_consistent_circuit_test(60, CircuitType::AC_RESISTANCE); }
TEST(ConsistentCircuitTest, ResistanceConsistent70Test) { generate_consistent_circuit_test(70, CircuitType::AC_RESISTANCE); }
TEST(ConsistentCircuitTest, ResistanceConsistent80Test) { generate_consistent_circuit_test(80, CircuitType::AC_RESISTANCE); }
TEST(ConsistentCircuitTest, ResistanceConsistent90Test) { generate_consistent_circuit_test(90, CircuitType::AC_RESISTANCE); }
TEST(ConsistentCircuitTest, ResistanceConsistent100Test) { generate_consistent_circuit_test(100, CircuitType::AC_RESISTANCE); }

TEST(ConsistentCircuitTest, CapacityConsistent10Test) { generate_consistent_circuit_test(10, CircuitType::AC_CAPACITY); }
TEST(ConsistentCircuitTest, CapacityConsistent20Test) { generate_consistent_circuit_test(20, CircuitType::AC_CAPACITY); }
TEST(ConsistentCircuitTest, CapacityConsistent30Test) { generate_consistent_circuit_test(30, CircuitType::AC_CAPACITY); }
TEST(ConsistentCircuitTest, CapacityConsistent40Test) { generate_consistent_circuit_test(40, CircuitType::AC_CAPACITY); }
TEST(ConsistentCircuitTest, CapacityConsistent50Test) { generate_consistent_circuit_test(50, CircuitType::AC_CAPACITY); }
TEST(ConsistentCircuitTest, CapacityConsistent60Test) { generate_consistent_circuit_test(60, CircuitType::AC_CAPACITY); }
TEST(ConsistentCircuitTest, CapacityConsistent70Test) { generate_consistent_circuit_test(70, CircuitType::AC_CAPACITY); }
TEST(ConsistentCircuitTest, CapacityConsistent80Test) { generate_consistent_circuit_test(80, CircuitType::AC_CAPACITY); }
TEST(ConsistentCircuitTest, CapacityConsistent90Test) { generate_consistent_circuit_test(90, CircuitType::AC_CAPACITY); }
TEST(ConsistentCircuitTest, CapacityConsistent100Test) { generate_consistent_circuit_test(100, CircuitType::AC_CAPACITY); }

TEST(ConsistentCircuitTest, InductanceConsistent10Test) { generate_consistent_circuit_test(10, CircuitType::AC_INDUCTANCE); }
TEST(ConsistentCircuitTest, InductanceConsistent20Test) { generate_consistent_circuit_test(20, CircuitType::AC_INDUCTANCE); }
TEST(ConsistentCircuitTest, InductanceConsistent30Test) { generate_consistent_circuit_test(30, CircuitType::AC_INDUCTANCE); }
TEST(ConsistentCircuitTest, InductanceConsistent40Test) { generate_consistent_circuit_test(40, CircuitType::AC_INDUCTANCE); }
TEST(ConsistentCircuitTest, InductanceConsistent50Test) { generate_consistent_circuit_test(50, CircuitType::AC_INDUCTANCE); }
TEST(ConsistentCircuitTest, InductanceConsistent60Test) { generate_consistent_circuit_test(60, CircuitType::AC_INDUCTANCE); }
TEST(ConsistentCircuitTest, InductanceConsistent70Test) { generate_consistent_circuit_test(70, CircuitType::AC_INDUCTANCE); }
TEST(ConsistentCircuitTest, InductanceConsistent80Test) { generate_consistent_circuit_test(80, CircuitType::AC_INDUCTANCE); }
TEST(ConsistentCircuitTest, InductanceConsistent90Test) { generate_consistent_circuit_test(90, CircuitType::AC_INDUCTANCE); }
TEST(ConsistentCircuitTest, InductanceConsistent100Test) { generate_consistent_circuit_test(100, CircuitType::AC_INDUCTANCE); }

TEST(MixedConsistentCircuitTest, ResistanceCapacitanceConsistent10Test) { generate_consistent_circuit_test(10, CircuitType::AC_RESISTANCE_CAPACITY); }
TEST(MixedConsistentCircuitTest, ResistanceCapacitanceConsistent20Test) { generate_consistent_circuit_test(20, CircuitType::AC_RESISTANCE_CAPACITY); }
TEST(MixedConsistentCircuitTest, ResistanceCapacitanceConsistent30Test) { generate_consistent_circuit_test(30, CircuitType::AC_RESISTANCE_CAPACITY); }
TEST(MixedConsistentCircuitTest, ResistanceCapacitanceConsistent40Test) { generate_consistent_circuit_test(40, CircuitType::AC_RESISTANCE_CAPACITY); }
TEST(MixedConsistentCircuitTest, ResistanceCapacitanceConsistent50Test) { generate_consistent_circuit_test(50, CircuitType::AC_RESISTANCE_CAPACITY); }
TEST(MixedConsistentCircuitTest, ResistanceCapacitanceConsistent60Test) { generate_consistent_circuit_test(60, CircuitType::AC_RESISTANCE_CAPACITY); }
TEST(MixedConsistentCircuitTest, ResistanceCapacitanceConsistent70Test) { generate_consistent_circuit_test(70, CircuitType::AC_RESISTANCE_CAPACITY); }
TEST(MixedConsistentCircuitTest, ResistanceCapacitanceConsistent80Test) { generate_consistent_circuit_test(80, CircuitType::AC_RESISTANCE_CAPACITY); }
TEST(MixedConsistentCircuitTest, ResistanceCapacitanceConsistent90Test) { generate_consistent_circuit_test(90, CircuitType::AC_RESISTANCE_CAPACITY); }
TEST(MixedConsistentCircuitTest, ResistanceCapacitanceConsistent100Test) { generate_consistent_circuit_test(100, CircuitType::AC_RESISTANCE_CAPACITY); }

TEST(MixedConsistentCircuitTest, ResistanceInductanceConsistent10Test) { generate_consistent_circuit_test(10, CircuitType::AC_RESISTANCE_INDUCTANCE); }
TEST(MixedConsistentCircuitTest, ResistanceInductanceConsistent20Test) { generate_consistent_circuit_test(20, CircuitType::AC_RESISTANCE_INDUCTANCE); }
TEST(MixedConsistentCircuitTest, ResistanceInductanceConsistent30Test) { generate_consistent_circuit_test(30, CircuitType::AC_RESISTANCE_INDUCTANCE); }
TEST(MixedConsistentCircuitTest, ResistanceInductanceConsistent40Test) { generate_consistent_circuit_test(40, CircuitType::AC_RESISTANCE_INDUCTANCE); }
TEST(MixedConsistentCircuitTest, ResistanceInductanceConsistent50Test) { generate_consistent_circuit_test(50, CircuitType::AC_RESISTANCE_INDUCTANCE); }
TEST(MixedConsistentCircuitTest, ResistanceInductanceConsistent60Test) { generate_consistent_circuit_test(60, CircuitType::AC_RESISTANCE_INDUCTANCE); }
TEST(MixedConsistentCircuitTest, ResistanceInductanceConsistent70Test) { generate_consistent_circuit_test(70, CircuitType::AC_RESISTANCE_INDUCTANCE); }
TEST(MixedConsistentCircuitTest, ResistanceInductanceConsistent80Test) { generate_consistent_circuit_test(80, CircuitType::AC_RESISTANCE_INDUCTANCE); }
TEST(MixedConsistentCircuitTest, ResistanceInductanceConsistent90Test) { generate_consistent_circuit_test(90, CircuitType::AC_RESISTANCE_INDUCTANCE); }
TEST(MixedConsistentCircuitTest, ResistanceInductanceConsistent100Test) { generate_consistent_circuit_test(100, CircuitType::AC_RESISTANCE_INDUCTANCE); }