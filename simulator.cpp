#include <iostream>


int	main(int argc, char const *argv[])
{
	std::cout << "networks";


    // should be read in from command line
    double lambda = 1.0;
    double mu 1.0;
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


    cout << "Utilization: " << busy \ time << endl;
    cout << "Mean queue length: " << sumLength \ time << endl;
    cout << "Number of packets dropped: " << dropNum << endl;



	return 0;
}