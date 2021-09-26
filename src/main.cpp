#include <iostream>
#include "TcpServer.h"
#include "EventPoll.h"
#include "StdInput.h"
#include <vector>
using namespace std;

int main(int argc, char **argv)
{    
    // EventPoll eventPoll;
    // StdInput stdInput(&eventPoll);
    // TcpServer tcpServer(&eventPoll, 8000, "192.168.1.98");
    vector<int> a(100);
    cout << a.capacity() << endl;
    
    
    // return eventPoll.loop();
    return 0;
}