# control_flow_checking
This program is the implementation of the control flow checking by software signatures.CFCSS is a software method that checks the control flow of the program using assigned signatures.Signatures are embedded in the program during compilation time using the constant field of the instructions and compared with run-time signatures when the program is executed.

1.Initially we identify all the basic blocks, build a program flow graph and number all the nodes in the program flow graph. 2.Compute si for each of the node in the flow graph 3.Compute D if there are a set of predecessors and add an an instruction g^D at the node

In this program, we take the input in the form of a text file and generate a matrix from it and later we construct a graph based on the available information.Once the signature for all the nodes is generated we call a function to print the nodes in the way they are connected.This is redirected to a text file ,using the dot command we generate a graph with signatures as its records.

Here the program is written in cpp, the name of the file is "cfcss.cpp", the text file in which the input is present is "in.txt",the output which is redirected to generate graph is "diagram.txt" and the generated graph is named "diag.png" in this repositary.The repositary also consists of a script to compile,run the program and generate the graph from the output of the program.
