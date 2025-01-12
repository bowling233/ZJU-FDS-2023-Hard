import networkx as nx
import matplotlib.pyplot as plt

filename='./extreme.in'
output="../../documents/src/extreme.pdf"

def line_prepender(filename, line):
    with open(filename, 'r+') as f:
        content = f.read()
        f.seek(0, 0)
        f.write(line.rstrip('\r\n') + content)

line_prepender(filename, "#")
g = nx.read_edgelist(filename, nodetype=int,
  data=(('weight',int),), create_using=nx.DiGraph())

# For small graphs

#pos=nx.spring_layout(g)
#pos=nx.circular_layout(g)
#pos=nx.spectral_layout(g)
#pos=nx.shell_layout(g)
pos=nx.random_layout(g)
#print(g.edges(data=True))
#labels = nx.get_edge_attributes(g,'weight')
#options = {
#    'node_color': '#abdbe3',
#    'node_size': 100,
#    'width': 1,
#    'arrowstyle': '-|>',
#    'arrowsize': 3,
#    'font_size': 5,
#    #'with_labels': True,
#}
#nx.draw_networkx(g, pos, arrows=True, **options)
#nx.draw_networkx_edge_labels(g,pos,edge_labels=labels, font_size=5)

## For large graphs
options = {
    'node_color': '#abdbe3',
    'node_size': 10,
    'width': 1,
    'with_labels': False,
}
plt.figure().set_size_inches(20,20)
nx.draw_networkx(g, pos=pos, arrows=False, **options)
plt.savefig(output, format="pdf", bbox_inches="tight")
plt.show()