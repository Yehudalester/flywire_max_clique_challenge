# solution checker for https://codex.flywire.ai/app/max_clique_challenge

import csv

# adjust these if reading from disk
with open("edges.csv") as csv_f1:
    read_edges = [row for row in csv.reader(csv_f1)]
with open("combined.csv") as csv_f2:
    read_solution = [row for row in csv.reader(csv_f2)];

calc_score = lambda kis: sum(k / 2**i for i, k in enumerate(kis))
calc_density = lambda e, n: sum(1 for a, b in e if a in n and b in n) / (len(n)*(len(n)-1)/2)

def eval(rows, edges, d=12):
    kis = [0]*d
    for r in rows:
        k, δ = len(r), calc_density(edges, set(r))
        for i in range(d):
            if δ >= 1 / 2**i: kis[i] = max(kis[i], k)
        print(f"- found subgraph of size {k} with density {δ}; {kis=} {calc_score(kis)=}")
    return calc_score(kis)

s, e = read_solution, read_edges
print(f"Final score: {eval(s, e)}")