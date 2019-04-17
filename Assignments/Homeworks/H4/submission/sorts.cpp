#include <iostream>
#include <algorithm>
#include <numeric>  // for std::accumulate
#include <vector>
#include <string>
#include <cstdlib>  // for std::rand
#include <cassert>

using namespace std;

//========================================================================
//  Set this to false to skip the insertion sort tests; you'd do this if
//  you're sorting so many items that insertion_sort would take more time
//  than you're willing to wait.

const bool TEST_INSERTION_SORT = true;

//========================================================================

//========================================================================
// Timer t;                 // create a timer
// t.start();               // start the timer
// double d = t.elapsed();  // milliseconds since timer was last started
//========================================================================

#if __cplusplus >= 201103L  // C++11

#include <chrono>

class Timer
{
public:
    Timer()
    {
        start();
    }
    void start()
    {
        m_time = std::chrono::high_resolution_clock::now();
    }
    double elapsed() const
    {
        std::chrono::duration<double,std::milli> diff =
        std::chrono::high_resolution_clock::now() - m_time;
        return diff.count();
    }
private:
    std::chrono::high_resolution_clock::time_point m_time;
};

#elif defined(_MSC_VER)  // not C++11, but Windows

#include <windows.h>

class Timer
{
public:
    Timer()
    {
        QueryPerformanceFrequency(&ticksPerSecond);
        start();
    }
    void start()
    {
        QueryPerformanceCounter(&m_time);
    }
    double elapsed() const
    {
        LARGE_INTEGER now;
        QueryPerformanceCounter(&now);
        return (1000.0 * (now.QuadPart - m_time.QuadPart)) / ticksPerSecond.QuadPart;
    }
private:
    LARGE_INTEGER m_time;
    LARGE_INTEGER ticksPerSecond;
};

#else // not C++11 or Windows, so C++98

#include <ctime>

class Timer
{
public:
    Timer()
    {
        start();
    }
    void start()
    {
        m_time = std::clock();
    }
    double elapsed() const
    {
        return (1000.0 * (std::clock() - m_time)) / CLOCKS_PER_SEC;
    }
private:
    std::clock_t m_time;
};

#endif

//========================================================================

// Here's a class that is not cheap to copy because the objects contain
// a large array.

// We'll simplify writing our timing tests by declaring everything public
// in this class.  (We wouldn't make data public in a class intended for
// wider use.)

typedef int IdType;

const int NREADINGS = 150;

struct Sensor
{
    IdType id;
    double avg;
    double readings[NREADINGS];
    Sensor(IdType i) : id(i)
    {
        // create random sensor readings (from 0 to 99)
        for (size_t k = 0; k < NREADINGS; k++)
            readings[k] = rand() % 100;
        // (accumulate computes 0.0 + readings[0] + readings[1] + ...)
        avg = accumulate(readings, readings + NREADINGS, 0.0) / NREADINGS;
    }
};

inline
bool compareSensor(const Sensor& lhs, const Sensor& rhs)
{
    // The Sensor with the higher average should come first.  If they have
    // the same average, then the Sensor with the smaller id number should
    // come first.  Return true iff lhs should come first.  Notice that
    // this means that a false return means EITHER that rhs should come
    // first, or there's a tie, so we don't care which comes first,
    
    if (lhs.avg > rhs.avg)
        return true;
    if (lhs.avg < rhs.avg)
        return false;
    return lhs.id < rhs.id;
}

inline
bool compareSensorPtr(const Sensor* lhs, const Sensor* rhs)
{
    // TODO: You implement this.  Using the same criteria as compareSensor,
    //       compare two Sensors POINTED TO by lhs and rhs.  Think about
    //       how you can do it in one line by calling compareSensor.
    
    // Just so this will compile, we'll pretend every comparison results in
    // a tie, so there's no preferred ordering.
    
    return compareSensor(*lhs, *rhs);
}

bool isSorted(const vector<Sensor>& s)
{
    // Return true iff the vector is sorted according to the ordering
    // relationship compareSensor
    
    for (size_t k = 1; k < s.size(); k++)
    {
        if (compareSensor(s[k], s[k-1]))
            return false;
    }
    return true;
}

void insertion_sort(vector<Sensor>& s, bool comp(const Sensor&, const Sensor&))
{
    // TODO: Using the insertion sort algorithm (pp. 311-313), sort s
    //       according to the ordering relationship passed in as the
    //       parameter comp.
    
    // Note:  The insertion sort algorithm on pp. 312-313 of the Carrano
    // book 6th edition is incorrect; someone made a change from the 5th
    // edition and messed it up.  See the errata page entry for page 313 at
    // http://homepage.cs.uri.edu/~carrano/WMcpp6e
    
    // Just to show you how to use the second parameter, we'll write code
    // that sorts only a vector of 2 elements.  (This is *not* the
    // insertion sort algorithm.)
    
    // Note that if comp(x,y) is true, it means x must end up before y in the
    // final ordering.
    //if (s.size() == 2  &&  comp(s[1], s[0]))
      //  swap(s[0], s[1]);
    
    for (int i = 1; i < s.size(); i++)
    {
        int j = i;
        while ((j > 0) && comp(s[j], s[j - 1]))
        {
            swap(s[j - 1], s[j]);
            j--;
        }
    }
}

// Report the results of a timing test

void report(string caption, double t, const vector<Sensor>& s)
{
    cout << t << " milliseconds; " << caption
    << "; first few sensors are\n\t";
    size_t n = s.size();
    if (n > 5)
        n = 5;
    for (size_t k = 0; k < n; k++)
        cout << " (" << s[k].id << ", " << s[k].avg << ")";
    cout << endl;
}

int main()
{
    size_t nsensors;
    cout << "Enter number of sensors to sort: ";
    cin >> nsensors;
    if ( !cin  ||  nsensors <= 0)
    {
        cout << "You must enter a positive number.  Goodbye." << endl;
        return 1;
    }
    
    // Create a random ordering of id numbers 0 through nsensors-1
    vector<IdType> ids;
    for (size_t j = 0; j < nsensors; j++)
        ids.push_back(IdType(j));
    random_shuffle(ids.begin(), ids.end());  // from <algorithm>
    
    // Create a bunch of Sensors
    vector<Sensor> unorderedSensors;
    for (size_t k = 0; k < ids.size(); k++)
        unorderedSensors.push_back(Sensor(ids[k]));
    
    // Create a timer
    
    Timer timer;
    
    // Sort the Sensors using the STL sort algorithm.  It uses a variant
    // of quicksort called introsort.
    
    vector<Sensor> sensors(unorderedSensors);
    timer.start();
    sort(sensors.begin(), sensors.end(), compareSensor);
    double elapsed = timer.elapsed();
    assert(isSorted(sensors));
    report("STL sort", elapsed, sensors);
    
    // Sort the already sorted array using the STL sort.  This should be
    // fast.
    
    timer.start();
    sort(sensors.begin(), sensors.end(), compareSensor);
    elapsed = timer.elapsed();
    assert(isSorted(sensors));
    report("STL sort if already sorted", elapsed, sensors);
    
    if (TEST_INSERTION_SORT)
    {
        // Sort the original unsorted array using insertion sort.  This
        // should be really slow.  If you have to wait more than a minute,
        // try rerunning the program with a smaller number of Sensors.
        
        sensors = unorderedSensors;
        timer.start();
        insertion_sort(sensors, compareSensor);
        elapsed = timer.elapsed();
        assert(isSorted(sensors));
        report("insertion sort if not already sorted", elapsed, sensors);
        
        // Sort the already sorted array using insertion sort.  This should
        // be fast.
        
        timer.start();
        insertion_sort(sensors, compareSensor);
        elapsed = timer.elapsed();
        assert(isSorted(sensors));
        report("insertion sort if already sorted", elapsed, sensors);
    }
    
    // Since Sensors are expensive to copy, and since the STL's sort copies
    // Sensors O(N log N) times, let's sort POINTERS to the Sensors, then
    // make one final pass to rearrange the Sensors according to the
    // reordered pointers.  We'll write some code; you write the rest.
    
    // Set sensors to the original unsorted sequence
    sensors = unorderedSensors;
    
    // Start the timing
    timer.start();
    
    // Create an auxiliary copy of sensors, to faciliate the later reordering.
    // We create it in a local scope so that we also account for the
    // destruction time.
    {
        vector<Sensor> auxSensors(sensors);
        
        // TODO:  Create a vector of Sensor pointers, and set each pointer
        //        to point to the corresponding Sensor in auxSensors.
        
        vector<Sensor*> ptrSensors;
        for (int i = 0; i < sensors.size(); i++)
        {
            Sensor* curr = &(auxSensors.at(i));
            ptrSensors.push_back(curr);
        }
        
        // TODO:  Sort the vector of pointers using the STL sort algorithm
        //        with compareSensorPtr as the ordering relationship.
        
        sort (ptrSensors.begin(), ptrSensors.end(), compareSensorPtr);
        
        // TODO:  Using the now-sorted vector of pointers, replace each Sensor
        //        in sensors with the Sensors from auxSensors in the correct order.
        
        for (int i = 0; i < sensors.size(); i++)
        {
            sensors.at(i) = *ptrSensors.at(i);
        }
        
    } // auxSensors will be destroyed here
    
    // Report the timing and verify that the sort worked
    elapsed = timer.elapsed();
    assert(isSorted(sensors));
    report("STL sort of pointers", elapsed, sensors);
}

/*
 
 The file sorts.cpp contains an almost complete program that creates a randomly ordered array, sorts it in a few ways, and reports on the elapsed times. Your job is to complete it and experiment with it.

You can run the program as is to get some results for the STL sort algorithm. The insertion sort will give you an assertion violation, because the insertion sort function right now doesn't do anything. That's one thing for you to write.

The objects in the array are not cheap to copy, which makes a sort that does a lot of moving objects around expensive. Your other task will be to create a vector of pointers to the objects, sort the pointers using the same criterion as was used to sort the objects, and then make one pass through the vector to put the objects in the proper order.

Your two tasks are thus:

Implement the insertion_sort function.

Implement the compareStorePtr function and the code in main to create and sort the array of pointers.

The places to make modifications are indicated by TODO: comments. You should not have to make modifications anywhere else. (Our solution doesn't.)
                                                                                                                            
Try the program with about 10000 items. Depending on the speed of your processor, this number may be too large or small to get meaningful timings in a reasonable amount of time. Experiment. Once you get insertion sort working, observe the O(N2) behavior by sorting, say, 10000, 20000, and 40000 items.
                                                                                                                            
To further observe the performance behavior of the STL sort algorithm, try sorting, say, 100000, 200000, and 400000 items, or even ten times as many. Since this would make the insertion sort tests take a long time, skip them by setting the TEST_INSERTION_SORT constant at the top of sorts.cpp to false.
                                                                                                                            
Notice that if you run the program more than once, you may not get the same timings each time. This is not because you're not getting the same sequence of random numbers each time; you are. Instead, factors like caching by the operating system are the cause.
 
 
*/