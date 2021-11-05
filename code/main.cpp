/*
*
*  Evolution of speech 
* 
*/
#include "include/BAnetwork.h"
#include "include/FCnetwork.h"

int main(int argc, char* argv[]) {

    int N = 20;
    int I = 5000;
    float noise = 0.1;

    FCnetwork network;

    // BA NETWORK
    try {

        network.initialise(N);

        // Generate the scale-free network
        network.generate();
        
        // Print the network to a .txt file
        network.print_network("network.txt");

        // Run the imitation game simulation and print the results to a .txt file
        network.simulation("results.txt", I, noise);

        // Clean up
        network.free_memory();

    } catch (const char* msg) {
        cerr << msg << endl;
        exit(-1);
    }

    return 0;
}