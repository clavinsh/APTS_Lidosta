#include <fstream>
#include <iostream>

struct FlightTime {
    int minutesFrom;
    int minutesTo;

    FlightTime() : minutesFrom(0), minutesTo(0) {}

    FlightTime(int f, int t) : minutesFrom(f), minutesTo(t) {}

    FlightTime& operator=(const FlightTime& other) {
        if (this != &other) {
            minutesFrom = other.minutesFrom;
            minutesTo = other.minutesTo;
        }
        return *this;
    }
};

class FlightTimeList {
private:
    struct Node {
        FlightTime data;
        Node* next;

        Node() : next(nullptr) {}

        Node(const Node& other) : data(other.data), next(nullptr) {}

        Node& operator=(const Node& other) {
            if (this != &other) {
                data = other.data;
                next = nullptr;
            }
            return *this;
        }
    };

    Node* head;
    Node* tail;

public:
    FlightTimeList() : head(nullptr), tail(nullptr) {}

    ~FlightTimeList() {
        Node* current = head;
        while (current != nullptr) {
            Node* nextNode = current->next;
            delete current;
            current = nextNode;
        }
    }

    FlightTimeList(const FlightTimeList& other) : head(nullptr), tail(nullptr) {
        Node* current = other.head;
        while (current != nullptr) {
            insertNode(current->data);
            current = current->next;
        }
    }

    void insertNode(FlightTime data) {
        Node* newNode = new Node;
        newNode->data = data;
        newNode->next = nullptr;

        if (head == nullptr) {
            head = newNode;
            tail = head;
        }
        else {
            tail->next = newNode;
            tail = newNode;
        }
    }

    bool empty() {
        return head == nullptr ? true : false;
    }

    void printList() {
        if (empty()) {
            std::cout << std::endl;
        }

        Node* current = head;

        while (current != nullptr) {
            std::cout << current->data.minutesFrom << ' ' << current->data.minutesTo << ' ';
            current = current->next;
        }

        std::cout << std::endl;
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

    void InsertVertex(int from, int to, int elementCount, int* flightTimesArray) {
        if (adjacencyMatrix[from][to].empty()) {
            adjacencyMatrix[from][to] = FlightTimeList();
        }
        for (int i = 0; i < 2 * elementCount; i++) {
            int s = flightTimesArray[i];
            int e = flightTimesArray[++i];

            adjacencyMatrix[from][to].insertNode(FlightTime(s, e));
        }
    }

    void printGraph() {
        for (int i = 0; i < airportCount; i++) {
            for (int j = 0; j < airportCount; j++) {
                std::cout << i << ' ' << j << ":";
                adjacencyMatrix[i][j].printList();
            }
        }
    }

    
};

int HHMM_to_total_minutes(char*& arr) {
    int hours = (*arr++ - '0') * 10;
    hours += (*arr++ - '0');
    arr++; // skip the colon

    int minutes = (*arr++ - '0') * 10;
    minutes += (*arr++ - '0');

    int returnable = hours * 60 + minutes;
    return returnable;
}


int main() {
    std::ifstream fin("lidostas.in");

    int airportCount, startIndex, goalIndex, arrivalTime;

    fin >> airportCount;
    fin >> startIndex;
    fin >> goalIndex;

    startIndex--; goalIndex--;

    Graph graph = Graph(airportCount);

    char* HHMM = new char[6]; // HH:MM\n ir pavisam 6 simboli
    char* ptr = HHMM;

    fin >> HHMM;

    arrivalTime = HHMM_to_total_minutes(HHMM);

    delete[] (HHMM-5);

    int from;
    fin >> from;

    while (from != 0) {
        int to, n;

        fin >> to;
        fin >> n;
        fin.get();
        char* buffer = new char[n * 12];
        fin.getline(buffer, static_cast<std::streamsize>(n) * 12);

        ptr = buffer;

        int* flightTimes = new int[n * 2];

        for (int i = 0; i < n*2; i++) {
            flightTimes[i] = HHMM_to_total_minutes(ptr);
            ptr++;

            flightTimes[++i] = HHMM_to_total_minutes(ptr);
            ptr++;
        }

        graph.InsertVertex(--from, --to, n, flightTimes);

        delete[] flightTimes;
        delete[] buffer;

        fin >> from;
    }

    fin.close();


    graph.printGraph();

    int currentMoney = 1000;





    std::ofstream fout("lidostas.out");

    fout.close();

    return 0;
}

