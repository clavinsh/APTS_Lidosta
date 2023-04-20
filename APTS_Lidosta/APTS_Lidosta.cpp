#include <fstream>
#include <iostream>
#include <chrono>

struct FlightTime {
    int minutesFrom;
    int minutesTo;

    FlightTime() : minutesFrom(-1), minutesTo(-1) {}

    FlightTime(int f, int t) : minutesFrom(f), minutesTo(t) {}

    FlightTime& operator=(const FlightTime& other) {
        if (this != &other) {
            minutesFrom = other.minutesFrom;
            minutesTo = other.minutesTo;
        }
        return *this;
    }

    bool operator<(const FlightTime& other) const {
        return minutesFrom < other.minutesFrom;
    }

    bool operator==(const FlightTime& other) const {
        return (minutesFrom == other.minutesFrom && minutesTo == other.minutesTo);
    }
};

class PriorityQueue {
private:
    struct Node {
        FlightTime data;
        Node* next;
        Node() : next(nullptr) {}
        //Node(const Node& other) : data(other.data), next(nullptr) {}
        Node(const FlightTime& f) : data(f), next(nullptr) {}
    };

    Node* head;
public:
    PriorityQueue() : head(nullptr) {}

    void push(const FlightTime& f) {
        Node* newNode = new Node(f);

        if(head == nullptr || f < head->data) {
            newNode->next = head;
            head = newNode;
        }
        else {
            Node* curr = head;
            while (curr->next != nullptr && curr->next->data < f) {
                curr = curr->next;
            }
            newNode->next = curr->next;
            curr->next = newNode;
        }
    }

    FlightTime pop() {
        FlightTime result = std::move(head->data);

        Node* temp = head;
        head = head->next;
        delete temp;
        return result;
    }

    FlightTime remove(const FlightTime& f) {
        Node* curr = head;
        Node* prev = nullptr;

        FlightTime cpy = f;

        while (curr != nullptr && !(curr->data == f)) {
            prev = curr;
            curr = curr->next;
        }

        // not found in queue
        if (curr == nullptr) {
            return f;
        }


        // is in the queue as the head or other
        if (prev == nullptr) {
            head = curr->next;
        }
        else {
            prev->next = curr->next;
        }

        delete curr;

        return cpy;
    }

    FlightTime top() {
        return head != nullptr ? head->data : FlightTime();
    }

    bool empty() {
        return head == nullptr;
    }

    int size() const {
        int count = 0;
        Node* curr = head;
        while (curr != nullptr) {
            count++;
            curr = curr->next;
        }

        return count;
    }

    void printQueue() {
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

    int earliestTime(int arrivalTime) {

        //jaizveido prioritates rinda, kurai uz bridi pieliek so elementu, 
        //un skatas kura pozicija vins ir, 
        //un nem nakamo ka atrako, ja pozicija ir pedejais, tad pirmo elementu saraksta

        int earliest = -1;

        Node* current = head;

        while (current != nullptr) {
            if (current->data.minutesFrom < earliest && current->data.minutesFrom > arrivalTime) {
                
            }
        }
        


    }
};

class Graph {
private:
    PriorityQueue** adjacencyMatrix;
    int airportCount;
public:
    Graph(int airportCount) {
        this->airportCount = airportCount;
        adjacencyMatrix = new PriorityQueue *[airportCount];
        for (int i = 0; i < airportCount; i++) {
            adjacencyMatrix[i] = new PriorityQueue[airportCount];
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
            adjacencyMatrix[from][to] = PriorityQueue();
        }
        for (int i = 0; i < 2 * elementCount; i++) {
            int s = flightTimesArray[i];
            int e = flightTimesArray[++i];

            adjacencyMatrix[from][to].push(FlightTime(s, e));
        }
    }

    void printGraph() {
        for (int i = 0; i < airportCount; i++) {
            for (int j = 0; j < airportCount; j++) {
                std::cout << i << ' ' << j << ":";
                adjacencyMatrix[i][j].printQueue();
            }
        }
    }


    void FindPath(int from, int to, int arrivalTime) {

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
    auto start_time = std::chrono::high_resolution_clock::now();
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


    // Get the current time again
    auto end_time = std::chrono::high_resolution_clock::now();

    // Compute the elapsed time in microseconds
    auto elapsed_time = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);

    // Print the elapsed time
    std::cout << "Elapsed time: " << elapsed_time.count() << " us\n";

    graph.printGraph();

    int currentMoney = 1000;









    std::ofstream fout("lidostas.out");

    fout.close();




    return 0;
}

