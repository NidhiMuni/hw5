#ifndef RECCHECK
#include <set>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
// add or remove necessary headers as you please

#endif

#include "schedwork.h"

using namespace std;

// a constant that can be used to indicate an INVALID 
// worker ID if that is useful to your implementation.
// Feel free to not use or delete.
static const Worker_T INVALID_ID = (unsigned int)-1;


// Add prototypes for any helper functions here


// Add your implementation of schedule() and other helper functions here

bool schedule(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched
)
{
    if(avail.size() == 0U){
        return false;
    }
    sched.clear();
    // Add your code below

    //a vector to track the number of shifts per worker
    std::vector<int> shiftsScheduled(avail[0].size(), 0);

    for (int i = 0; i < (int)avail.size(); i++){
      std::vector<Worker_T> temp(dailyNeed, INVALID_ID);
      sched.push_back(temp);
    }

    return scheduleHelper(avail, dailyNeed, maxShifts, sched, 0, 0, shiftsScheduled);
}

bool scheduleHelper(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched, 
    int day, 
    int col, 
    std::vector<int>& shiftsScheduled
)
{
  //If the last row has the correct number of employees, done
  if (sched[avail.size()-1][dailyNeed-1] != INVALID_ID){
    return true;
  }

  //Get the next location
  int nextDay;
  int nextCol;
  if(col < (int)sched[0].size()-1){
    nextCol = col + 1;
    nextDay = day;
  } else {
    nextCol = 0;
    nextDay = day + 1;
  }

  for (Worker_T workerId = 0; workerId < avail[0].size(); workerId++){

    //if that worker is available for that day
    if (avail[day][(int)workerId] == 1){
      
      sched[day][col] = workerId;

      shiftsScheduled[workerId] ++;

      int scheduledAlreadyCount = count(sched[day].begin(), sched[day].end(), workerId); 

      //valid solution
      if (shiftsScheduled[workerId] <= (int)maxShifts && scheduledAlreadyCount == 1){
        if (scheduleHelper(avail, dailyNeed, maxShifts, sched, nextDay, nextCol, shiftsScheduled) == true){
          return true;
        }
      }

      //backtrack if it reaches here
      sched[day][col] = INVALID_ID;
      shiftsScheduled[workerId] --;
    }

  }

  return false;

}

