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
    Node* tail;

public:
    FlightTimeList() { head = nullptr; tail = nullptr; }

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
            adjacencyMatrix[from][to].insertNode(FlightTime{ flightTimesArray[i], flightTimesArray[++i] });
        }
    }
};

int HHMM_to_total_minutes(char*& arr) {
    //int hours = (*arr++ - '0') * 10 + (*arr++ - '0');
    int hours = 0;
    hours = (*arr++ - '0') * 10;
    hours += *arr++ - '0';

    if (*arr++ != ':') {
        // handle input string format error
        return -1;
    }

    int minutes = 0;
    minutes = (*arr++ - '0') * 10;
    minutes += *arr - '0';


    //int minutes = (*arr++ - '0') * 10 + (*arr - '0');

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

    Graph graph = Graph(airportCount);

    char* HHMM = new char[6]; // HH:MM\n ir pavisam 6 simboli
    fin >> HHMM;

    arrivalTime = HHMM_to_total_minutes(HHMM);

    int from;
    fin >> from;

    while (from != 0) {
        int to, n;

        fin >> to;
        fin >> n;
        fin.get();
        char* buffer = new char[n * 12];
        fin.getline(buffer, static_cast<std::streamsize>(n) * 12);

        char* ptr = buffer;

        int* flightTimes = nullptr;
        try {
            flightTimes = new int[n * 2];
        }
        catch (std::bad_alloc& e) {
            // handle memory allocation error
            delete[] HHMM;
            delete[] buffer;
            throw e;
        }

        for (int i = 0; i < n; i++) {
            flightTimes[i] = HHMM_to_total_minutes(ptr);
            ptr += 2;
            std::cout << flightTimes[i] << ' ';

            flightTimes[++i] = HHMM_to_total_minutes(ptr);
            std::cout << flightTimes[i] << std::endl;
            ptr += 2;
        }

        graph.InsertVertex(from, to, n, flightTimes);

        delete[] flightTimes;
        delete[] buffer;

        fin >> from;
    }

    delete[] HHMM;

    fin.close();

    std::ofstream fout("lidostas.out");

    fout.close();

    return 0;
}

