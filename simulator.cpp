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

	} // sorted insert

	Event removeFirst() {
		Event firstElement = GlobalEventList.front();
		GlobalEventList.remove(firstElement);

		return firstElement;
	}
};

int	main(int argc, char const *argv[])
{
	double lambda;
	double mu;
	int maxBufferSize;

	std::cout << "lambda: ";
	std::cin >> lambda;

	std::cout << "mu: ";
	std::cin >> mu;

	std::cout << "Buffer Size: ";
	std::cin >> maxBufferSize;

	return 0;
}


double nedt(double rate)
{
     double u;
     u = drand48();
     return ((-1/rate)*log(1-u));
}
