# Our submission to the Flywire Max Quasi-Cliques Challenge #
[The Challenge](https://codex.flywire.ai/app/max_clique_challenge) - find maximal quasi-cliques for densities 1/2**i for i=0,...,11. We thank the flywire team at princeton university for this challenge and opportunity.
## How to use ##
Download the `edges.csv` file from the [challenge website](https://codex.flywire.ai/app/max_clique_challenge), or use your own `edges.csv` file, representing an undirected graph.<br />
Compile and Run the code: `g++ -Ofast -march=native -funroll-loops code.cpp -o code` `./code`.<br />
Wait :)
## Our implementation ##
In the `code.cpp` file we implemented the following:
- A naive greedy swap algorithm that improves a current quasi-clique
- An improved version of the greedy algorithm that optimally breaks ties
- A random process that utilizes large random swaps to slightly improves a current quasi-clique <br />

More details can be found in our [presentation](https://docs.google.com/presentation/d/17UcqfBVe-hsSrSR0ssjPFgzZglBz29-JVWS0ORLp0XU/).
## Our results ##
On the given connectome we were able to achieve clique sizes of \[40, 175, 369, 723, 1541, 3109, 6207, 11757, 20841, 35828, 60430, 97272\] for given densities, giving us the second place with the same score as first place:
<img width="1711" height="422" alt="image" src="https://github.com/user-attachments/assets/47d09095-43bf-4977-b053-2171f95a38d2" />
