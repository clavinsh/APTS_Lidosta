#include <fstream>
#include <iostream>

struct FlightTime {
    int minutesFrom;
    int minutesTo;
};

class FlightTimeList {
private:
    struct Node {
        FlightTime data;
        Node* next;
    };

    Node* head;

public:
    FlightTimeList() { head = nullptr; }

    ~FlightTimeList() {
        Node* current = head;
        while (current != nullptr) {
            Node* nextNode = current->next;
            delete current;
            current = nextNode;
        }
    }

    void insertNode(FlightTime data) {
        Node* newNode = new Node;
        newNode->data = data;
        newNode->next = nullptr;

        if (head == nullptr) {
            head = newNode;
        }
        else {
            Node* temp = head;
            while (temp->next != nullptr) {
                temp = temp->next;
            }
            temp->next = newNode;
        }
    }

    bool empty() {
        return head == nullptr ? true : false;
    }
};

class Graph {
private:



    FlightTimeList** adjacencyMatrix;
    int airportCount;
public:
    Graph(int airportCount) {
        this->airportCount = airportCount;
        adjacencyMatrix = new FlightTimeList *[airportCount];
        for (int i = 0; i < airportCount; i++) {
            adjacencyMatrix[i] = new FlightTimeList[airportCount];
        }
    }

    ~Graph() {
        for (int i = 0; i < airportCount; i++) {
            delete[] adjacencyMatrix[i];
        }
        delete[] adjacencyMatrix;
    }

    void InsertEdge(int from, int to, int elementCount, int* flightTimesArray) {
        if (adjacencyMatrix[from][to].empty()) {
            adjacencyMatrix[from][to] = FlightTimeList();
        }
        for (int i = 0; i < 2 * elementCount; i++) {
            adjacencyMatrix[from][to].insertNode(FlightTime{ flightTimesArray[i], flightTimesArray[++i] });
        }
    }
};

int HHMM_to_total_minutes(char* arr) {
    int hours = (arr[0] - '0') * 10 + arr[1] - '0';
    int minutes = (arr[3] - '0') * 10 + arr[4] - '0';

    minutes += hours * 60;
    return minutes;
}

int main() {
    std::ifstream fin("lidostas.in");

    int airportCount, startIndex, goalIndex, arrivalTime;

    fin >> airportCount;
    fin >> startIndex;
    fin >> goalIndex;

    startIndex--; goalIndex--;

    //std::cout << airportCount << startIndex << goalIndex << std::endl;

    Graph graph = Graph(airportCount);

    char HHMM[6]; // HH:MM\n ir pavisam 6 simboli


    fin >> HHMM;

    arrivalTime = HHMM_to_total_minutes(HHMM);

    int from;

    fin >> from;

    while (from != 0) {
        int to, n;

        fin >> to;
        fin >> n;



    }


    fin.close();

    std::ofstream fout("lidostas.out");


    fout.close();

    return 0;
}


//
//int main()
//{
//    std::cout << "Hello World!\n";
//}
//