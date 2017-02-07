#include <iostream>
#include <list>
#include <cmath>

using namespace std;


// randomly calculates negative-exponenetially-distributed-time
double nedt(double rate);

class Event {
	double eventTime;
	bool isArrival; // type 0 
	bool isDeparture; // type 1

public:
	Event(double etime, bool eventType) {
		eventTime = etime;

		if (eventType == 0) {
			isArrival = true;
		}
		else if (eventType == 1) {
			isDeparture = false;
		}
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
	GEL() {}
	void insert(Event event) {

		std::list<Event>::iterator indexIterator;
		//auto& indexIterator;
		if (GlobalEventList.size() == 0) {
			GlobalEventList.push_front(event);
		}

		if (GlobalEventList.front().getEventTime() >= event.getEventTime()) { 
			GlobalEventList.push_front(event);
		}
		else {
			for (auto& element : GlobalEventList) {
				indexIterator++;
				
				if (element.getEventTime() < event.getEventTime()) {
					GlobalEventList.insert(indexIterator, event);
				}
			}
		}

	} // insert sorted by events time

	Event removeFirst() {
		Event firstElement = GlobalEventList.front();
		GlobalEventList.remove(firstElement);

		return firstElement;
	}
};

int	main(int argc, char const *argv[])
{
    // should be read in from command line
    double lambda = 1.0;
    double mu = 1.0;
    double maxbuffer = -1;

    // initialize
    int length = 0;
    int dropNum = 0;
    int sumLength = 0;
    double time = 0;
    double busy = 0;
    double packet = 0;


    Event e = Event(time, true);
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
        // may want to chech it should be length, not length - 1
        sumLength += length * (e.getEventTime() - time);

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
                packet = nedt(mu);
                busy += packet;
                eventList.insert(Event(time + packet, false));
                length ++;
                // this assumes maxbuffer is at least one, 
                // which is a good assumption because no buffer 
                // would have max buffer equal to 1
            }
            // else if room in buffer
            // maxbuffer = -1 denotes infinite buffer
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
