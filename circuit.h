#ifndef CIRCUITS_CIRCUIT_H
#define CIRCUITS_CIRCUIT_H

#include <sstream>
#include "matrix.h"
#include "graph.h"

class circuit {
public:
    circuit(const std::string& input_string);

    void calculate();

private:
    matrix<double> conductivity_matrix;
    matrix<double> emf_matrix;
    matrix<double> incidence_matrix;
    graph circuit_graph;
};


#endif //CIRCUITS_CIRCUIT_H
