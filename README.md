A Graph Theoretic Approach for Multi-Objective Budget
Constrained Capsule Wardrobe Recommendation
====

Usage
----
We make use of three real world e-commerce datasets.
1) Datasets/Amazon-1
2) Datasets/Amazon-2
3) Datasets/Polyvore

To generate graph for each dataset csv, run the *script.sh* from the respective folder. And to generate real-value edge-weighted graph for each dataset csv, run the *script_prob.sh* from the respective folder.

For example, in case of Amazon-1 dataset:

        $cd Datasets/Amazon-1
        $bash script.sh
        $bash script_prob.sh
        
The graphs in required format are stored in *input_graph.txt* for binary weighted edges, and *input_graph_prob.txt* for real-value weighted edges.

To run the code, execute the run.sh script

For example, in case of Amazon-1 dataset:
    
    $bash run.sh
    Enter Dataset File Path: ./Datasets/Amazon-1/input_graph.txt
    
Results are stored in *result/result.csv*

To get the results for real-value edge-weighted graph, execute run_prob.sh script.

For example, in case of Amazon-1 dataset:

    $bash run_prob.sh
    Enter Dataset File Path: ./Datasets/Amazon-1/input_graph_prob.txt

Results are stored in *result/result.csv*
