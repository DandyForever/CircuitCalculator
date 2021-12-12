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
* *p_i* is voltage phase in degrees between *n_out_i* and *n_in_i*, a float, not necessary
  for each branch, but required if *v_i* is given
### Three Phase circuit
It is possible to define custom electrical elements. For example, Resistance 50 Ohms
could be defined as follows:
```
define R50(A, B)
  A -- B, 50R; 0C; 0L;
```
After custom element defined you could use it:
```
R50(1, 2)
```
That example means that between vertices 1 and 2 there is an element R50 defined
above.

Also, it is possible to use internal variables inside your custom elements. For
example, in such way three phase circuit could be conveniently defined:
```
define ThreePhase(A, B, C, N)
  internal Z
  Z -- A, 0R; 0C; 0L; 220V, 0
  Z -- B, 0R; 0C; 0L; 220V, 120
  Z -- C, 0R; 0C; 0L; 220V, -120
  Z -- N, 0R; 0C; 0L;
```

If your elements defined in several files, you could include them. For example, if
element ThreePhase above defined in file three_phase.txt, you could include and use
it as follows:
```
include three_phase.txt
ThreePhase(1, 2, 3, 4)
...
```
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
  * *pI_i* is a current phase in degrees between *n_out_i* and *n_in_i*
### Three Phase circuit
Branch currents display in the same way as for *AC circuit* 
## How to build
Requirements to build and run are Flex, CMake version 3.16 and C++ 17
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
#### Three Phase circuit
```
$ cmake --build . --target three_phase_circuit
$ ./three_phase_circuit INPUT_FILE
```
### Build and run End-To-End tests
#### DC circuit
DC circuit E2E tests consist of 4 families of tests on circuits:
* *Simple test* consists of 6 tests from internet
* *Fully coherent circuits* consisting of *n* vertices, where *n* varies
  from 10 to 100 with step 10
* *Fully coherent subcircuits* are circuits consisting of *m* fully
  coherent subcircuits with *n* vertices, where *m* is *{2, 5, 10, 50}* and *n*
  varies from 10 to 50 with step 10
* *Consistent circuits* consisting of *n* vertices, where *n* varies from
  10 to 100 with step 10

```
$ cmake --build . --target dc_circuit
$ ./dc_circuit < t/e2e_dc_test/NAME_test.txt
```

**Running all tests automatically takes ~15 minutes**
```
$ cmake --build . --target big_ac_circuit_test
$ ./big_dc_circuit_test
```
#### AC circuit
*Big AC Circuit test* consists of 4 families of tests on large circuits:
* *Simple test* consists of 6 tests from internet
* *Fully coherent circuits* consisting of *n* vertices, where *n* varies
  from 10 to 100 with step 10
* *Fully coherent subcircuits* are circuits consisting of *m* fully
  coherent subcircuits with *n* vertices, where *m* is *{2, 5, 10, 50}* and *n*
  varies from 10 to 50 with step 10
* *Consistent circuits* consisting of *n* vertices, where *n* varies from 10 to 100
  with step 10. Includes 5 subfamilies, depending on the resistance elements
  * *Resistant consistent circuits*
  * *Capacitance consistent circuits*
  * *Inductance consistent circuits*
  * *Resistance and Capacitance consistent circuits*
  * *Resistance and Inductance consistent circuits*

```
$ cmake --build . --target ac_circuit
$ ./ac_circuit < t/e2e_dc_test/NAME_test.txt
```

**Running all tests automatically takes ~1 hour**
```
$ cmake --build . --target big_ac_circuit_test
$ ./big_ac_circuit_test
```
#### Three Phase circuit
*Big Three Phase Circuit test* consist of the same tests as *AC Circuit test*.
The only difference between them is the input format: for all edges of three
phase circuits custom elements are defined.
```
$ cmake --build . --target three_phase_circuit
$ ./three_phase_circuit t/e2e_three_phase_test/NAME_test.txt
```

**Running all tests automatically takes ~1 hour**
```
$ cmake --build . --target big_three_phase_circuit_test
$ ./big_three_phase_circuit_test
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
#### Input Preworker tests
```
$ cmake --build . --input_preworker_test
$ ./input_preworker_test
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
#### Three Phase Circuit tests
```
$ cmake --build . --target three_phase_circuit_test
$ ./three_phase_circuit_test
```