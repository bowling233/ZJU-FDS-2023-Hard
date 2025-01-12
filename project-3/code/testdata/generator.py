from cyaron import *
import random

io = IO("dij.in")

n, m = 100, 200

graph = Graph.DAG(n - 1, m - 5, weight_limit=10)

linklist = random.sample(range(1, n), 5)
for i in linklist:
    graph.add_edge(i, n, weight=random.randint(1, 100))

io.input_writeln(n, m)
io.input_writeln(graph)