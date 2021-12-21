/*
*
*  EoS project:  Self-organisation in vowel systems in scale-free networks
*
*  Author: Wolf De Wulf
*  Code based on:  de Boer, B. (2000). Self-organization in vowel systems. Journal of Phonetics, 28(4), 441-465. doi: https://doi.org/10.1006/jpho.2000.0125
* 
*/
#include "include/BAnetwork.h"
#include "include/FCnetwork.h"

int main(int argc, char* argv[]) {

    // Parameters
    int N = 100;
    int I = 10000;
    double noise = 0.1;
    double new_signal_prob = 0.01;
    double clean_prob = 0.1;

    // Randomness
    unsigned int seed = static_cast<unsigned int>( time(NULL) ); 
    mt19937 rand_gen(seed);

    // Network
    BAnetwork network;
    try {

        network.initialise(N, rand_gen);

        // Generate the scale-free network
        network.generate();
        
        // Print the network to a .txt file
        network.print_network("network.txt");

        // Run the imitation game simulation and print the results to a .txt file
        network.simulation("results.txt", I, noise, new_signal_prob, clean_prob);

        // Clean up
        network.free_memory();

        // Print seed
        cout << "Used seed: " << seed << "\n";

    } catch (const char* msg) {
        cerr << msg << endl;
        exit(-1);
    }

    return 0;
}