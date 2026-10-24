#include <bits/stdc++.h>
using namespace std;
/*
    Jorge Arturo Montiel Navarro | A01278612
*/


namespace {
    string inputs_folder = "./inputs";
}

/*
    maxFlow -> This function receives a list of adjacency of a pondered and directed graph,
                 with a source and sink. Based on that, calculates the maximum flow that can pass through the  graph until the sink/destination
                
                Takes inspiration of the following algorithm  ->  Edmons-Karp

    Time complexity O(V *E²) ->This algorithm runs BFS O(V+E) (on a dense graph this is resumed as O(E))
                                 at most V*E times in the worst case (dense graph, multiple power paths).

    Space complexity O(V). We store at most on the queue the exact same nodes as the full graph
*/
pair< unordered_map<int, unordered_map<int, int>>,int> maxFlow(vector<vector<int>> connections, int sink, int source){
    
    unordered_map<int, unordered_map<int, int>> graph;
    for(vector<int> connection : connections){
        int u = connection[0], v=connection[1], bandwidth=connection[2];
        graph[u][v]=  bandwidth; // Available
        graph[v][u]=  0;        // Used

    }

    int maxFlow=0;
    
    while(true){    // While there's a path from the source to the end    
        queue<int> q;
        q.push(source);

        unordered_map<int,int> previouses;

        unordered_set<int> visited; // Avoid infinyte loops
        visited.insert(source);

        while(!q.empty()){ // This is BFS, we use a queue to process the nodes in the graph
            int current = q.front();
            q.pop();

            if (current == sink){break;} // We have found the sink, we stop to trace the path

            // Check if the neighbors have bandwidth available and add them to the queue
            if(graph.find(current) != graph.end()){
                for(auto neighbor : graph[current]){
                    
                    int next_node = neighbor.first;
                    int available_capacity = neighbor.second;

                    if(available_capacity>0 && visited.find(next_node) == visited.end()){
                        q.push(next_node);
                        visited.insert(next_node);
                        previouses[next_node]= current;
                    }
                }
            }
        }

        if(previouses.find(sink) == previouses.end() ) {
            return {graph, maxFlow}; // No path all over the graph was found, we already found the maxflow
        }

        // Reconstruct the way back
        vector<int> path;
        int current = sink;
        while(current != source){
            path.push_back(current);
            current = previouses[current];
        }
        path.push_back(source);
        reverse(path.begin(),path.end());

        // Find the bottleneck
        int bottleneck=INT_MAX;
        for(int i=0; i<path.size()-1; i++){
            int u = path[i]; 
            int v = path[i+1];
            bottleneck = min(graph[u][v], bottleneck);
        }

        // Rest bottleneck
        for(int i=0; i<path.size()-1; i++){
            int u = path[i]; 
            int v = path[i+1];
            graph[u][v] -=  bottleneck;
            graph[v][u] +=  bottleneck;
        }
        
        maxFlow += bottleneck; // The bottleneck is the max flow that can pass through a pipe, so we add it inmediately to the max
    }

    return {graph, maxFlow};
}

/*
    readMaxFlowFile -> Read the file of format
            
    nodes -> nodes in the graph
    n -> unkwown number of lines representing the adjacency list of the graph

    Time complexity O(n) -> Process the list of size n
    Space complexity O(n) -> Store a list of size n
*/
pair<int, vector<vector<int>>> readMaxFlowFile(string filename){
    ifstream file;
    file.open(filename);
    vector<vector<int>> connections;
    int nodes;
    file >> nodes;                       // number of nodes
    int u, v, w;
    while (file >> u >> v >> w) {    // read every remaining edge/connections
        connections.push_back({u, v, w});
    }
    file.close();
    return {nodes, connections};
}


/*
    drawGraph -> This function uses the tool graphviz to help display and create the diagrams of max flow.
                Install it with sudo apt install graphviz or 
                Receives, the graph, the connections and some other data to represent graphically the result of maxFlow
    
    Time Complexity O(n) -> Just process the adjacency list of size n
    Space complexity O(1) -> No aditional space but, it creates artifacts of different sizes.

*/
void drawGraph(unordered_map<int, unordered_map<int, int>> &graph, vector<vector<int>> &connections, string &filename, int sink, int source, int maxFlow){
    ofstream dot("route.dot");
    dot << "digraph G {\n  rankdir=LR;\n";
    for(auto connection : connections){
        int u=connection[0], v=connection[1];
        int available = graph[u][v], used=graph[v][u];
        dot << "  " << u << " -> " << v << " [label=\"" << used <<"/"<<available << "\""  << "];\n";
    }
    dot << "  " << source << " [style=filled, fillcolor=orange];\n";
    dot << "  " << sink << " [style=filled, fillcolor=orange];\n";
    dot << "}\n";
    dot.close();

    string title = filename + "- maxFlow:" + to_string(maxFlow);
    string command = "dot -Tpng route.dot -o " +filename+".png -Glabel=\"" +filename + "-> maxFlow:" + to_string(maxFlow) + "\"";
    system(command.c_str());
    cout << "Graph image saved to route.png" << endl;
}

int main(){
    for( const auto & entry : filesystem::directory_iterator(inputs_folder)){
        string file = entry.path();
        pair<int, vector<vector<int>>> result = readMaxFlowFile(file);
        pair< unordered_map<int, unordered_map<int, int>>,int> r = maxFlow( result.second, result.first-1, 0);
        drawGraph(r.first, result.second, file, result.first-1, 0 , r.second);
    }
    return 0;
}