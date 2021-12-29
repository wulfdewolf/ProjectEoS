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
    int average_over = 1000;
    int population_size = 100;
    int games_per_agent = 500;
    double noise = 0.1;
    double new_signal_prob = 0.2/population_size;
    double clean_prob = 0.1;
    vector<int> write_iterations{20, 100, 200, 250, 300, 350, 400, 450, 500};

    // Randomness
    unsigned int seed = static_cast<unsigned int>( time(NULL) ); 
    mt19937 rand_gen(seed);

    try {

        // Network
        BAnetwork network(population_size, rand_gen, "exp2/BAnetwork100.txt");
        // FCnetwork network(population_size, rand_gen, "network.txt");  // --> select for fully-connected network
        // Network network("network.txt");                                 // --> select to read from network file

        // Run the simulation and save the results
        network.simulation(average_over, games_per_agent, noise, new_signal_prob, clean_prob, write_iterations, "BAresults100.txt");

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