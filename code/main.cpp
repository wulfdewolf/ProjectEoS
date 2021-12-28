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
    int average_over = 100;
    int population_size = 20;
    int games = 5000;
    double noise = 0.1;
    double new_signal_prob = 0.01;
    double clean_prob = 0.1;
    vector<int> write_iterations{20, 500, 2000, 5000, 10000};

    // Randomness
    unsigned int seed = static_cast<unsigned int>( time(NULL) ); 
    mt19937 rand_gen(seed);

    try {

        // Network
        FCnetwork network(population_size, rand_gen, "network.txt");
        // FCnetwork network(population_size, rand_gen, "network.txt");  // --> select for fully-connected network
        // Network network("network.txt");                                 // --> select to read from network file

        // Run the simulation and save the results
        network.simulation(average_over, games, noise, new_signal_prob, clean_prob, write_iterations, "results.txt");

        // Clean up
        network.free();

        // Print seed
        cout << "Used seed: " << seed << "\n";

    } catch (const char* msg) {
        cerr << msg << endl;
        exit(-1);
    }

    return 0;
}