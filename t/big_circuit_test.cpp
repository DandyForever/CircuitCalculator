#include <gtest/gtest.h>

#include "../circuit.h"

void generate_fully_coherent_subcircuits_test(size_t subcircuit_vertices, size_t subcircuit_number, bool is_ac) {
    std::stringstream circuit_, answer_;
    for (size_t subcircuit_index = 0; subcircuit_index < subcircuit_number; subcircuit_index++) {
        size_t subcircuit_vertex_first = subcircuit_vertices * subcircuit_index + 1,
                subcircuit_vertex_last = subcircuit_vertices * (subcircuit_index + 1);
        for (size_t outcoming = subcircuit_vertex_first; outcoming <= subcircuit_vertex_last; outcoming++) {
            for (size_t incoming = outcoming + 1; incoming <= subcircuit_vertex_last; incoming++) {
                circuit_ << outcoming << " -- " << incoming << ", 1.0";
                if (is_ac)
                    circuit_ << "R; 1.0C; 1.0L";
                circuit_ << "; ";
                answer_ << outcoming << " -- " << incoming << ": 0 A";
                if (is_ac)
                    answer_ << ", 0";
                answer_ << ";\n";
            }
        }
    }
    circuit test_circuit(circuit_.str());
    test_circuit.calculate_edge_current();
    std::string answer = answer_.str();
    answer.pop_back();
    EXPECT_EQ(answer, test_circuit.get_edge_current_answer());
}

void generate_fully_coherent_dc_circuit_test(size_t vertex_number) {
    generate_fully_coherent_subcircuits_test(vertex_number, 1, false);
}

void generate_fully_coherent_ac_circuit_test(size_t vertex_number) {
    generate_fully_coherent_subcircuits_test(vertex_number, 1, true);
}

enum class CircuitType {
    DC_RESISTANCE,
    AC_RESISTANCE,
    AC_CAPACITY,
    AC_INDUCTANCE
};

void generate_consistent_circuit_test(size_t vertex_last, CircuitType type) {
    std::stringstream circuit_, answer_;
    circuit_ << 1 << " -- " << vertex_last << ", ";
    answer_ << 1 << " -- " << vertex_last << ": 1 A";
    switch (type) {
        case CircuitType::DC_RESISTANCE:
            circuit_ << "1; " << vertex_last << "V; ";
            break;
        case CircuitType::AC_RESISTANCE:
            circuit_ << "1R; 0C; 0L; " << vertex_last << "V, 0; ";
            answer_ << ", 0";
            break;
        case CircuitType::AC_CAPACITY:
            circuit_ << "0R; 1C; 0L; " << vertex_last << "V, 0; ";
            answer_ << ", 1.5708";
            break;
        case CircuitType::AC_INDUCTANCE:
            circuit_ << "0R; 0C; 1L; " << vertex_last << "V, 0; ";
            answer_ << ", -1.5708";
            break;
    }
    answer_ << ";\n";
    for (size_t outcoming = 1; outcoming < vertex_last; outcoming++) {
        circuit_ << outcoming << " -- " << outcoming + 1 << ", ";
        answer_ << outcoming << " -- " << outcoming + 1 << ": ";
        switch (type) {
            case CircuitType::DC_RESISTANCE:
                circuit_ << "1; ";
                answer_ << "-1 A";
                break;
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
        }
        answer_ << ";\n";
    }
    circuit test_circuit(circuit_.str());
    test_circuit.calculate_edge_current();
    std::string answer = answer_.str();
    answer.pop_back();
    EXPECT_EQ(answer, test_circuit.get_edge_current_answer());
}


TEST(FullyConnectedDCCircuitTest, FullyConnected10Test) { generate_fully_coherent_dc_circuit_test(10); }
TEST(FullyConnectedDCCircuitTest, FullyConnected20Test) { generate_fully_coherent_dc_circuit_test(20); }
TEST(FullyConnectedDCCircuitTest, FullyConnected30Test) { generate_fully_coherent_dc_circuit_test(30); }
TEST(FullyConnectedDCCircuitTest, FullyConnected40Test) { generate_fully_coherent_dc_circuit_test(40); }
TEST(FullyConnectedDCCircuitTest, FullyConnected50Test) { generate_fully_coherent_dc_circuit_test(50); }


TEST(ConsistentDCCircuitTest, Consistent10Test) { generate_consistent_circuit_test(10, CircuitType::DC_RESISTANCE); }
TEST(ConsistentDCCircuitTest, Consistent20Test) { generate_consistent_circuit_test(20, CircuitType::DC_RESISTANCE); }
TEST(ConsistentDCCircuitTest, Consistent30Test) { generate_consistent_circuit_test(30, CircuitType::DC_RESISTANCE); }
TEST(ConsistentDCCircuitTest, Consistent40Test) { generate_consistent_circuit_test(40, CircuitType::DC_RESISTANCE); }
TEST(ConsistentDCCircuitTest, Consistent50Test) { generate_consistent_circuit_test(50, CircuitType::DC_RESISTANCE); }
TEST(ConsistentDCCircuitTest, Consistent60Test) { generate_consistent_circuit_test(60, CircuitType::DC_RESISTANCE); }


TEST(FullyConnectedDCSubcircuitsTest, FullyConnected10Subcircuit2Test) { generate_fully_coherent_subcircuits_test(10, 2, false); }
TEST(FullyConnectedDCSubcircuitsTest, FullyConnected20Subcircuit2Test) { generate_fully_coherent_subcircuits_test(20, 2, false); }
TEST(FullyConnectedDCSubcircuitsTest, FullyConnected30Subcircuit2Test) { generate_fully_coherent_subcircuits_test(30, 2, false); }
TEST(FullyConnectedDCSubcircuitsTest, FullyConnected40Subcircuit2Test) { generate_fully_coherent_subcircuits_test(40, 2, false); }
TEST(FullyConnectedDCSubcircuitsTest, FullyConnected50Subcircuit2Test) { generate_fully_coherent_subcircuits_test(50, 2, false); }
TEST(FullyConnectedDCSubcircuitsTest, FullyConnected10Subcircuit5Test) { generate_fully_coherent_subcircuits_test(10, 5, false); }
TEST(FullyConnectedDCSubcircuitsTest, FullyConnected20Subcircuit5Test) { generate_fully_coherent_subcircuits_test(20, 5, false); }
TEST(FullyConnectedDCSubcircuitsTest, FullyConnected30Subcircuit5Test) { generate_fully_coherent_subcircuits_test(30, 5, false); }
TEST(FullyConnectedDCSubcircuitsTest, FullyConnected40Subcircuit5Test) { generate_fully_coherent_subcircuits_test(40, 5, false); }
TEST(FullyConnectedDCSubcircuitsTest, FullyConnected10Subcircuit10Test) { generate_fully_coherent_subcircuits_test(10, 10, false); }
TEST(FullyConnectedDCSubcircuitsTest, FullyConnected20Subcircuit10Test) { generate_fully_coherent_subcircuits_test(20, 10, false); }
TEST(FullyConnectedDCSubcircuitsTest, FullyConnected30Subcircuit10Test) { generate_fully_coherent_subcircuits_test(30, 10, false); }
TEST(FullyConnectedDCSubcircuitsTest, FullyConnected40Subcircuit10Test) { generate_fully_coherent_subcircuits_test(40, 10, false); }
TEST(FullyConnectedDCSubcircuitsTest, FullyConnected10Subcircuit50Test) { generate_fully_coherent_subcircuits_test(10, 50, false); }
TEST(FullyConnectedDCSubcircuitsTest, FullyConnected20Subcircuit50Test) { generate_fully_coherent_subcircuits_test(20, 50, false); }
TEST(FullyConnectedDCSubcircuitsTest, FullyConnected30Subcircuit50Test) { generate_fully_coherent_subcircuits_test(30, 50, false); }

TEST(FullyConnectedACCircuitTest, FullyConnected10Test) { generate_fully_coherent_ac_circuit_test(10); }
TEST(FullyConnectedACCircuitTest, FullyConnected20Test) { generate_fully_coherent_ac_circuit_test(20); }
TEST(FullyConnectedACCircuitTest, FullyConnected30Test) { generate_fully_coherent_ac_circuit_test(30); }
TEST(FullyConnectedACCircuitTest, FullyConnected40Test) { generate_fully_coherent_ac_circuit_test(40); }
TEST(FullyConnectedACCircuitTest, FullyConnected50Test) { generate_fully_coherent_ac_circuit_test(50); }

TEST(FullyConnectedACSubcircuitsTest, FullyConnected10Subcircuit2Test) { generate_fully_coherent_subcircuits_test(10, 2, true); }
TEST(FullyConnectedACSubcircuitsTest, FullyConnected20Subcircuit2Test) { generate_fully_coherent_subcircuits_test(20, 2, true); }
TEST(FullyConnectedACSubcircuitsTest, FullyConnected30Subcircuit2Test) { generate_fully_coherent_subcircuits_test(30, 2, true); }
TEST(FullyConnectedACSubcircuitsTest, FullyConnected40Subcircuit2Test) { generate_fully_coherent_subcircuits_test(40, 2, true); }
TEST(FullyConnectedACSubcircuitsTest, FullyConnected50Subcircuit2Test) { generate_fully_coherent_subcircuits_test(50, 2, true); }
TEST(FullyConnectedACSubcircuitsTest, FullyConnected10Subcircuit5Test) { generate_fully_coherent_subcircuits_test(10, 5, true); }
TEST(FullyConnectedACSubcircuitsTest, FullyConnected20Subcircuit5Test) { generate_fully_coherent_subcircuits_test(20, 5, true); }
TEST(FullyConnectedACSubcircuitsTest, FullyConnected30Subcircuit5Test) { generate_fully_coherent_subcircuits_test(30, 5, true); }
TEST(FullyConnectedACSubcircuitsTest, FullyConnected40Subcircuit5Test) { generate_fully_coherent_subcircuits_test(40, 5, true); }
TEST(FullyConnectedACSubcircuitsTest, FullyConnected10Subcircuit10Test) { generate_fully_coherent_subcircuits_test(10, 10, true); }
TEST(FullyConnectedACSubcircuitsTest, FullyConnected20Subcircuit10Test) { generate_fully_coherent_subcircuits_test(20, 10, true); }
TEST(FullyConnectedACSubcircuitsTest, FullyConnected30Subcircuit10Test) { generate_fully_coherent_subcircuits_test(30, 10, true); }
TEST(FullyConnectedACSubcircuitsTest, FullyConnected40Subcircuit10Test) { generate_fully_coherent_subcircuits_test(40, 10, true); }
TEST(FullyConnectedACSubcircuitsTest, FullyConnected10Subcircuit50Test) { generate_fully_coherent_subcircuits_test(10, 50, true); }
TEST(FullyConnectedACSubcircuitsTest, FullyConnected20Subcircuit50Test) { generate_fully_coherent_subcircuits_test(20, 50, true); }
TEST(FullyConnectedACSubcircuitsTest, FullyConnected30Subcircuit50Test) { generate_fully_coherent_subcircuits_test(30, 50, true); }

TEST(ConsistentACCircuitTest, ResistanceConsistent10Test) { generate_consistent_circuit_test(10, CircuitType::AC_RESISTANCE); }
TEST(ConsistentACCircuitTest, ResistanceConsistent20Test) { generate_consistent_circuit_test(20, CircuitType::AC_RESISTANCE); }
TEST(ConsistentACCircuitTest, ResistanceConsistent30Test) { generate_consistent_circuit_test(30, CircuitType::AC_RESISTANCE); }
TEST(ConsistentACCircuitTest, ResistanceConsistent40Test) { generate_consistent_circuit_test(40, CircuitType::AC_RESISTANCE); }
TEST(ConsistentACCircuitTest, ResistanceConsistent50Test) { generate_consistent_circuit_test(50, CircuitType::AC_RESISTANCE); }
TEST(ConsistentACCircuitTest, ResistanceConsistent60Test) { generate_consistent_circuit_test(60, CircuitType::AC_RESISTANCE); }

TEST(ConsistentACCircuitTest, CapacityConsistent10Test) { generate_consistent_circuit_test(10, CircuitType::AC_CAPACITY); }
TEST(ConsistentACCircuitTest, CapacityConsistent20Test) { generate_consistent_circuit_test(20, CircuitType::AC_CAPACITY); }
TEST(ConsistentACCircuitTest, CapacityConsistent30Test) { generate_consistent_circuit_test(30, CircuitType::AC_CAPACITY); }
TEST(ConsistentACCircuitTest, CapacityConsistent40Test) { generate_consistent_circuit_test(40, CircuitType::AC_CAPACITY); }
TEST(ConsistentACCircuitTest, CapacityConsistent50Test) { generate_consistent_circuit_test(50, CircuitType::AC_CAPACITY); }
TEST(ConsistentACCircuitTest, CapacityConsistent60Test) { generate_consistent_circuit_test(60, CircuitType::AC_CAPACITY); }

TEST(ConsistentACCircuitTest, InductanceConsistent10Test) { generate_consistent_circuit_test(10, CircuitType::AC_INDUCTANCE); }
TEST(ConsistentACCircuitTest, InductanceConsistent20Test) { generate_consistent_circuit_test(20, CircuitType::AC_INDUCTANCE); }
TEST(ConsistentACCircuitTest, InductanceConsistent30Test) { generate_consistent_circuit_test(30, CircuitType::AC_INDUCTANCE); }
TEST(ConsistentACCircuitTest, InductanceConsistent40Test) { generate_consistent_circuit_test(40, CircuitType::AC_INDUCTANCE); }
TEST(ConsistentACCircuitTest, InductanceConsistent50Test) { generate_consistent_circuit_test(50, CircuitType::AC_INDUCTANCE); }
TEST(ConsistentACCircuitTest, InductanceConsistent60Test) { generate_consistent_circuit_test(60, CircuitType::AC_INDUCTANCE); }
