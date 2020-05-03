import networkx as nx
import sys, time


def main(out_dir):
    nodes = [5000, 10000, 20000, 50000, 100000]

    for n in nodes:
        graph = nx.barabasi_albert_graph(n, 3)
        edge_list = [e for e in graph.edges]
        
        with open("{}/graph-{}.txt".format(out_dir, str(n)), "w") as f:
            f.write("{} {}\n".format(n, graph.number_of_edges()))
            for e in edge_list:
                f.write("{} {}\n".format(e[0], e[1]))

        # with open("{}/stats-{}.txt".format(out_dir, str(n)), "w") as f:
        #     bc_val = nx.betweenness_centrality(graph)
        #     for i in sorted(bc_val.keys()):
        #         f.write("{} ".format(str(bc_val[i])))
        #     f.write("\n")


        


if __name__ == '__main__':
    main(sys.argv[1])