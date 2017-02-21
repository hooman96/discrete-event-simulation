#include <iostream>
#include <list>
#include <cmath>
#include <algorithm> // max

using namespace std;


// randomly calculates negative-exponenetially-distributed-time
double nedt(double rate);

class Event {
	double eventTime;
	bool isArrival; // type 0 

public:
	Event(double etime, bool arrival) {
		eventTime = etime;

        isArrival = arrival;
	}

	double getEventTime() {
		return eventTime;
	}

	bool getIsArrival() {
		return isArrival;
	}

	bool operator==(const Event &rhs) const {
        return rhs.eventTime == eventTime;
    }

	bool operator>=(const Event &rhs) const {
        return rhs.eventTime >= eventTime;
    }

    bool operator>(const Event &rhs) const {
        return rhs.eventTime > eventTime;
    }
};


class GEL { // Global Event List

	std::list<Event> GlobalEventList;

public:
	GEL() {
        GlobalEventList = std::list<Event>();
    }
	void insert(Event event) {
        //cerr << "begin insert" << endl;

        //cerr << "insert event.isArrival: " << event.getIsArrival() << endl;
        //cerr << "size = " << GlobalEventList.size() << endl;

		if (GlobalEventList.size() == 0) {
            GlobalEventList.push_front(event);
            return;
        }

		for (std::list<Event>::iterator itr = GlobalEventList.begin(); itr != GlobalEventList.end(); itr++) {
			if (itr->getEventTime() > event.getEventTime()) {
                GlobalEventList.insert(itr, event);
                return;
            }
        }

        GlobalEventList.push_back(event);
	

        //cerr << "end insert " << endl;

	} // insert sorted by events time

	Event removeFirst() {

        //cerr << "begin removeFirst" << endl;
		Event firstElement = GlobalEventList.front();
		GlobalEventList.pop_front();

        //cerr << "end removeFirst" << endl;

		return firstElement;
	}
};

int	main(int argc, char const *argv[])
{
    // should be read in from command line
    double lambda;
    double mu;
    double maxbuffer;

    std::cout << "lambda: ";
    std::cin >> lambda;

    std::cout << "mu: ";
    std::cin >> mu;

    std::cout << "Buffer Size: ";
    std::cin >> maxbuffer; 

    // initialize
    int length = 0; // length of queue
    int dropNum = 0; // number of droped packets
    double sumLength = 0; // integration for average queue length
    double time = 0; // current time  
    double busy = 0; // packets waiting in queue?
    double packet = 0; // service time of packet // only thing needed as concept of queue 


    Event e = Event(time + nedt(lambda), true);
    GEL eventList = GEL();
    eventList.insert(e);

    // for 100000 events 
    // process event
    // just going by the number given
    for (int i = 0; i < 100000; i++)
    {
        // get closest event and update time
        e = eventList.removeFirst();

        // sums length by multiplying length by elapsed time
        // since length = 1 could still be considered empty queue
        // may want to check it should be length, not length - 1
        sumLength += max(0, length - 1) * (e.getEventTime() - time); // area under graph of packets in queue (len - 1) vs time

        // updates time
        time = e.getEventTime();

        // handles Arrival event
        if (e.getIsArrival())
        {
            // insert new arrival event
            eventList.insert(Event(time + nedt(lambda), true));

            // if server is free, schedule a departure event, and update length
            if (length == 0)
            {
                //cerr << "hello from length = 0" << endl;
                packet = nedt(mu);
                //cerr << "packet: " << packet << endl;
                busy += packet;
                eventList.insert(Event(time + packet, false));
                length ++;
                // this assumes maxbuffer is at least one, 
                // which is a good assumption because no buffer 
                // would have max buffer equal to 1
            }
            // else if room in buffer
            // maxbuffer -1 denotes infinite buffer
            else if (maxbuffer == -1 || length - 1 < maxbuffer)
            {
                length ++;
                // handles generating of service time when departure event created
            }
            else // no room in buffer
            {
                dropNum ++;
            }
        }

        // handles departure event
        else 
        {
            length --;

            // if packet still in queue, create a departure event
            if (length > 0)
            {
                packet = nedt(mu);

                busy += packet;
                eventList.insert(Event(time + packet, false));
            }
        }

    }


    cout << "Utilization: " << busy / time << endl;
    cout << "Mean queue length: " << sumLength / time << endl;
    cout << "Number of packets dropped: " << dropNum << endl;


	return 0;
}


double nedt(double rate)
{
     double u;
     u = drand48();
     return ((-1/rate)*log(1-u));
}