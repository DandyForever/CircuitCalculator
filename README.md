# Circuit Calculator
## Problem statement
For a given electrical circuit, calculate currents in
its branches.
## Input format
The electrical circuit is entered in the following
format: *"n_out_1 -- n_in_1, r_1;[ v_1V;] n_out_2 --*
*n_in_2, r_2;[ v_2V;] ..."*, where *n_out_i* and
*n_in_i* are nodes of the circuit, *r_i* is resistance
in Ohms between *n_out_i* and *n_in_i*, *v_i* is voltage
in Volts between *n_out_i* and *n_in_i*. *n_out_i* and
*n_in_i* are positive integers, *r_i* is a positive
float, *v_i* is a float. *v_i* is not necessary for each
branch.
## Output format
Branch currents display in the following format:
*"n_out_1 -- n_in_1: c_1 A; n_out_2 -- n_in_2: c_2 A;*
*..."*, where *c_i* is a current in Amperes between
*n_out_i* and *n_in_i*.
