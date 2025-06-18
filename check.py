from pandas import *
# load data
edges = {(r[0], r[1]) for r in read_csv("edges.csv")}
nodes = sorted([r[0] for r in read_csv("result.csv")])
# verify no duplicate nodes
assert len(nodes) == len(set(nodes)), "Duplicate nodes detected"
# verify that edge density is above 0.25
edge_count, pair_count = 0, 0
for i1, n1 in enumerate(nodes):
  for n2 in nodes[i1:]:
    pair_count += 1
    if (n1, n2) in edges:
      edge_count += 1
assert edge_count / pair_count > 0.25, "Insufficient density"