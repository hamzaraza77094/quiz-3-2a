#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <cstdlib>

// include additional necessary headers
using namespace std;

mutex mtx;
condition_variable cv;
bool syn_done = false;

void query(/*add necessary parameters*/ int count) {
    // Should print: the print number (starting from 0), "SYN", and the three dots "..."
    for (int i = 0; i < count; ++i) {
        unique_lock<mutex> lock(mtx);
        cv.wait(lock, [] { return !syn_done; });
        cout << "[" << i << "] SYN ... ";
        syn_done = true;
        cv.notify_one();
    }
}

void response(/*add necessary parameters*/int count) {
    // Should print "ACK"
    for (int i = 0; i < count; ++i) {
        unique_lock<mutex> lock(mtx);
        cv.wait(lock, [] { return syn_done; });
        cout << "ACK" << endl;
        syn_done = false;
        cv.notify_one();
    }
}

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("Usage: ./handshake <count>\n");
        exit(1);
    }

    /**
     * Steps to follow:
     * 1. Get the "count" from cmd args
     * 2. Create necessary synchronization primitive(s)
     * 3. Create two threads, one for "SYN" and the other for "ACK"
     * 4. Provide the threads with necessary args
     * 5. Update the "query" and "response" functions to synchronize the output
    */
    int count = stoi(argv[1]);

    thread query_thread(query, count);
    thread response_thread(response, count);

    query_thread.join();
    response_thread.join();
   
    return 0;
}

