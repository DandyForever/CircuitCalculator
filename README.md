# Circuit Calculator
## Problem statement
For a given electrical (AC/DC) circuit, calculate currents in
its branches.
## Input format
### DC circuit
The electrical circuit is entered in the following
format:
```
n_out_1 -- n_in_1, r_1;[ v_1V;]
n_out_2 -- n_in_2, r_2;[ v_2V;]
...
```
where
  * *n_out_i* and *n_in_i* are nodes of the circuit, positive integers
  * *r_i* is resistance in Ohms between *n_out_i* and *n_in_i*, a float 
  * *v_i* is voltage in Volts between *n_out_i* and *n_in_i*, a float, not necessary for each
    branch
### AC circuit
The electrical circuit is entered in the following
format:
```
n_out_1 -- n_in_1, r_1R; c_1C; l_1L;[ v_1V, p_1;]
n_out_2 -- n_in_2, r_2R; c_2C; l_2L;[ v_2V, p_2;]
...
```
where
* *n_out_i* and *n_in_i* are nodes of the circuit, positive integers
* *r_i* is resistance in Ohms between *n_out_i* and *n_in_i*, a float
* *c_i* is capacitance in Ohms between *n_out_i* and *n_in_i*, a float
* *l_i* is inductance in Ohms between *n_out_i* and *n_in_i*, a float
* *v_i* is voltage in Volts between *n_out_i* and *n_in_i*, a float, not necessary for each
  branch
* *p_i* is voltage phase in radians between *n_out_i* and *n_in_i*, a float, not necessary
  for each branch, but required if *v_i* is given
## Output format
### DC circuit
Branch currents display in the following format:
```
n_out_1 -- n_in_1: I_1 A;
n_out_2 -- n_in_2: I_2 A;
...
```
where *I_i* is a current in Amperes between *n_out_i* and *n_in_i*.
### AC circuit
Branch currents display in the following format:
```
n_out_1 -- n_in_1: I_1 A, pI_1;
n_out_2 -- n_in_2: I_2 A, pI_2;
...
```
where
  * *I_i* is a current in Amperes between *n_out_i* and *n_in_i*
  * *pI_i* is a current phase in radians between *n_out_i* and *n_in_i*
## How to build
Requirements to build and run are CMake version 3.16 and C++ 17
### Build and run project
#### DC circuit
```
$ cmake --build . --target dc_circuit
$ ./dc_circuit
```
#### AC circuit
```
$ cmake --build . --target ac_circuit
$ ./ac_circuit
```
### Build and run End-To-End tests
#### DC circuit
*Big DC Circuit test* consists of 3 families of tests on large circuits:
* *Fully coherent circuits* consisting of *n* vertices, where *n* varies
  from 10 to 100 with step 10
* *Fully coherent subcircuits* are circuits consisting of *m* fully
  coherent subcircuits with *n* vertices, where *m* is *{2, 5, 10, 50}* and *n*
  varies from 10 to 50 with step 10
* *Consistent circuits* consisting of *n* vertices, where *n* varies from
  10 to 100 with step 10

**Running all tests takes ~15 minutes**
```
$ cmake --build . --target big_dc_circuit_test
$ ./big_dc_circuit_test
```
#### AC circuit
*Big AC Circuit test* consists of 3 families of tests on large circuits:
* *Fully coherent circuits* consisting of *n* vertices, where *n* varies
  from 10 to 100 with step 10
* *Fully coherent subcircuits* are circuits consisting of *m* fully
  coherent subcircuits with *n* vertices, where *m* is *{2, 5, 10, 50}* and *n*
  varies from 10 to 50 with step 10
* *Consistent circuits* consisting of *n* vertices, where *n* varies from
  10 to 100 with step 10. Includes 3 subfamilies, depending on the resistance element
  * *Resistant consistent circuits*
  * *Capacitance consistent circuits*
  * *Inductance consistent circuits*
* *Mixed consistent circuits* consisting of *n* vertices, where *n* varies from
  10 to 100 with step 10. Includes 2 subfamilies, depending on the impedance elements
  * *Resistance and Capacitance consistent circuits*
  * *Resistance and Inductance consistent circuits*

**Running all tests takes ~1 hour**
```
$ cmake --build . --target big_ac_circuit_test
$ ./big_ac_circuit_test
```
### Build and run Unit tests
#### Matrix tests
```
$ cmake --build . --target matrix_test
$ ./matrix_test
```
#### Graph tests
```
$ cmake --build . --target graph_test
$ ./graph_test
```
#### Input Parser tests
```
$ cmake --build . --target input_parser_test
$ ./input_parser_test
```
#### DC Circuit tests
```
$ cmake --build . --target dc_circuit_test
$ ./dc_circuit_test
```
#### AC Circuit tests
```
$ cmake --build . --target ac_circuit_test
$ ./ac_circuit_test
```