#include <fstream>
#include <iostream>
#include <chrono>

const int MINUTES_IN_A_DAY = 24 * 60;

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


struct FlightTimeWaitPair {
    FlightTime ft;
    int waitTime;
};

struct FlightTimeIndexPair {
    FlightTime ft;
    int index;
    //FlightTimeIndexPair() : ft(FlightTime()), index(-1) {}
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

    Node* push(const FlightTime& f) {
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

        return newNode;
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

    bool IsLast(const FlightTime& f) {

        Node* curr = head;

        if (curr == nullptr) return false;

        while (curr->next != nullptr) {
            curr = curr->next;
        }

        return (curr->data.minutesFrom == f.minutesFrom);
    }


    FlightTimeWaitPair earliestFlight(int arrivalTime) {
        if (empty()) return FlightTimeWaitPair{FlightTime(), -1};

        FlightTime temp_t = FlightTime(arrivalTime, 0);

        Node* tempNode = push(temp_t);

        FlightTimeWaitPair pair;

        if (!IsLast(temp_t)) {
            pair = FlightTimeWaitPair{ tempNode->next->data, tempNode->next->data.minutesFrom - arrivalTime };
        }
        else {
            remove(temp_t);
            pair = FlightTimeWaitPair{ head->data, MINUTES_IN_A_DAY - head->data.minutesFrom + arrivalTime };
        }


        remove(temp_t);

        return pair;
    }
};

char* minutesToHHMMFull(int minutesFrom, int minutesTo) {
    int hoursFrom = minutesFrom / 60;
    minutesFrom -= hoursFrom * 60;

    int hoursTo = minutesTo / 60;
    minutesTo -= hoursTo * 60;


    char buffer[12];

    snprintf(buffer, 12,"%02d:%02d-%02d:%02d",hoursFrom, minutesFrom, hoursTo, minutesTo);

    return buffer;
}


struct Node {
    FlightTimeIndexPair data;
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

class List {
private:
    Node* head;
    Node* tail;

public:
    List() : head(nullptr), tail(nullptr) {}

    ~List() {
        Node* current = head;
        while (current != nullptr) {
            Node* nextNode = current->next;
            delete current;
            current = nextNode;
        }
    }

    List(const List& other) : head(nullptr), tail(nullptr) {
        Node* current = other.head;
        while (current != nullptr) {
            insertNode(current->data);
            current = current->next;
        }
    }

    void insertNode(FlightTimeIndexPair data) {
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

    void pop() {
        if (head == nullptr) {
            return;
        }
        Node* tmp = head;
        head = head->next;
        delete tmp;
        if (head == nullptr) {
            tail = nullptr;
        }
    }

    Node* getHead() {
        return head;
    }

    bool empty() {
        return head == nullptr ? true : false;
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
                std::cout << i+1 << ' ' << j+1 << ":";
                adjacencyMatrix[i][j].printQueue();
            }
        }
    }

    int FindPath(int from, int to, int arrivalTime, int& money, List*& path) {
        //if (money <= 0) return -1; // no more money for travel

        FlightTimeWaitPair earliestPair = { FlightTime(), MINUTES_IN_A_DAY + 1 };
        int nextAirportIndex = -1;

        for (int i = 0; i < airportCount; i++) {
            // atrasts kaimiņš
            if (from != i && !adjacencyMatrix[from][i].empty()) {
                FlightTimeWaitPair pair = adjacencyMatrix[from][i].earliestFlight(arrivalTime);
                if (earliestPair.waitTime > pair.waitTime) {
                    earliestPair = pair;
                    nextAirportIndex = i;
                }
            }
        }

        if (nextAirportIndex != -1) {
            // save the chosen airport index and the flight, 
            money = money - (earliestPair.waitTime);

            path->insertNode(FlightTimeIndexPair{ earliestPair.ft, nextAirportIndex});

            adjacencyMatrix[from][nextAirportIndex].remove(earliestPair.ft);

            if (nextAirportIndex == to) {
                return 1;
            }

            return FindPath(nextAirportIndex, to, earliestPair.ft.minutesTo, money, path);

        }
        else {
            // path does not exist
            return -1;
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

char* minutesToHHMM(int minutes) {
    int hours = minutes / 60;
    minutes -= hours * 60;

    char buffer[6];

    sprintf_s(buffer, "%d:%d", hours, minutes);

    return buffer;
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

    delete[](HHMM - 5);

    int from;
    fin >> from;

    while (from != 0) {
        int n, to;

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

    List* path = new List();

    std::ofstream fout("lidostas.out");

    if (graph.FindPath(startIndex, goalIndex, arrivalTime, currentMoney, path) == -1) {
        fout << "Impossible\n";
        return -1;
    }

    fout << from+1 << ' ' << minutesToHHMM(arrivalTime) << '\n';

    Node* curr = path->getHead();

    fout << from+1<< "->" << curr->data.index+1 << ' ' << minutesToHHMMFull(curr->data.ft.minutesFrom, curr->data.ft.minutesTo) << '\n';

    from = curr->data.index;

    curr = curr->next;

    while (curr != nullptr) {
        fout << from+1 << "->" << curr->data.index+1 << ' ' << minutesToHHMMFull(curr->data.ft.minutesFrom, curr->data.ft.minutesTo) << '\n';
        from = curr->data.index;
        curr = curr->next;
    }
   
    fout.close();

    // Get the current time again
    auto end_time = std::chrono::high_resolution_clock::now();

    // Compute the elapsed time in microseconds
    auto elapsed_time = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);

    // Print the elapsed time
    std::cout << "Elapsed time: " << elapsed_time.count() << " us\n";




    return 0;
}

