#include <iostream>
#include <fstream>
#include <sstream>
#include <list>
#include <string>

using namespace std;

struct Vertex {
    string segment;
    int outDegree;
};

class Graph {
private:
    list<Vertex> vertices;
    list<list<int>> adjacencyList;

public:
    void addEdge(const string& source, const string& destination) {
        int sourceIndex = getOrCreateVertexIndex(source);
        int destIndex = getOrCreateVertexIndex(destination);

        while (adjacencyList.size() <= sourceIndex || adjacencyList.size() <= destIndex) {
            adjacencyList.push_back(list<int>());
        }

        auto it = adjacencyList.begin();
        advance(it, sourceIndex);
        auto& adjacency = *it;

        while (adjacency.size() <= destIndex) {
            adjacency.push_back(0);
        }

        auto adj_it = adjacency.begin();
        advance(adj_it, destIndex);
        (*adj_it)++;

        auto vert_it = vertices.begin();
        advance(vert_it, sourceIndex);
        vert_it->outDegree++;
    }

    void displayTop10OutDegree() {
        sortTop10();

        cout << "Top 10 vertices with more out-degree:\n";
        int count = 0;
        for (const auto& vertex : vertices) {
            if (count >= 10) break;
            cout << "IP segment: " << vertex.segment << ", Out-degree value: " << vertex.outDegree << endl;
            count++;
        }
    }

    string findBootMaster() {
        int maxOutDegree = 0;
        string bootMasterIP;

        for (const auto& vertex : vertices) {
            if (vertex.outDegree > maxOutDegree) {
                maxOutDegree = vertex.outDegree;
                bootMasterIP = vertex.segment;
            }
        }

        return bootMasterIP;
    }

private:
    int getOrCreateVertexIndex(const string& segment) {
        int index = 0;
        for (const auto& vert : vertices) {
            if (vert.segment == segment) {
                return index;
            }
            index++;
        }

        vertices.push_back({ segment, 0 });
        return index;
    }

    void sortTop10() {
        vertices.sort([](const Vertex& a, const Vertex& b) {
            return a.outDegree > b.outDegree;
        });
    }
};

int main() {
    ifstream inputFile("bitacora.txt");
    if (!inputFile.is_open()) {
        cerr << "Unable to open the file." << endl;
        return 1;
    }

    Graph graph;

    string line;
    while (getline(inputFile, line)) {
        istringstream iss(line);
        string month, day, time, ipAddress, reason;

        iss >> month >> day >> time >> ipAddress;
        getline(iss, reason);

        istringstream ipAddressStream(ipAddress);
        string segment;
        while (getline(ipAddressStream, segment, '.')) {
            graph.addEdge(segment, reason);
        }
    }

    graph.displayTop10OutDegree();

    string bootMasterIP = graph.findBootMaster();
    cout << "Boot master is presumably located at IP address segment: " << bootMasterIP << endl;

    inputFile.close();
    return 0;
}