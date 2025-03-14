# Synchronization Solutions for CalTrain and Chemical Reaction Problems
# A problem set in OS 2012 course of John Ousterhout.

Overview
* This repository contains solutions for two classic synchronization problems implemented using Pintos locks and condition variables:
1. CalTrain Passenger Synchronization
2. Chemical Reaction Synchronization (Water Formation)
* Each problem requires proper synchronization mechanisms to ensure orderly execution without race conditions, deadlocks, or busy-waiting.

# Problem 1: CalTrain Passenger Synchronization

A. Problem Description:
CalTrain has automated its trains and passengers (now robots). Each train and passenger is represented as a thread. The goal is to synchronize train boarding in an orderly manner using condition variables and locks.

B. Functional Requirements:

    * When a train arrives at the station, it calls station_load_train(station, count), where count is the number of available seats.
    * Passenger robots call station_wait_for_train(station) when they are ready to board, waiting for an available seat.
    * Once a passenger boards a train, it calls station_on_board(station) to notify the train that it has taken a seat.
    * The train must not depart until it is full or there are no more waiting passengers.
    * Multiple passengers must be able to board simultaneously.
    * Busy-waiting must be avoided.

C. Solution Design:

   1. The solution uses:
   
     * A lock to protect access to shared station data.
     * Condition variables for synchronizing train arrival and passenger boarding.
     * A counter-based approach to track available seats and waiting passengers.

   2. Implementation Details:

      a. The following functions are implemented:
      
          * station_init(struct station *station): Initializes the station structure.
          * station_load_train(struct station *station, int count): Waits until the train is fully loaded or no more passengers are waiting.
          * station_wait_for_train(struct station *station): Blocks passengers until a train with available seats arrives.
          * station_on_board(struct station *station): Signals that a passenger has taken a seat, allowing the train to track remaining passengers.
      b. The solution ensures efficient synchronization by:
      
          * Using condition variables to avoid busy-waiting.
          * Using targeted signaling (cond_signal instead of cond_broadcast) to reduce context switches.
          * Ensuring a train departs only when the right conditions are met.
          
# Problem 2: Chemical Reaction Synchronization (Water Formation)

A. Problem Description:
In this problem, we synchronize chemical reactions to form water molecules (H₂O). Each atom (Hydrogen or Oxygen) is represented by a thread. The goal is to ensure exactly two hydrogen atoms and one oxygen atom react together to form one molecule of water.

B. Functional Requirements:

    * Hydrogen threads call reaction_h(struct reaction *r) when they are ready to bond.
    * Oxygen threads call reaction_o(struct reaction *r) when they are ready to bond.
    * When two H atoms and one O atom are available, one thread calls make_water() exactly once.
    * After forming one water molecule, the participating threads return.
    * No busy-waiting is allowed.

C. Solution Design:

   1. The solution utilizes:

     * Locks to protect access to shared reaction data.
     * Multiple condition variables (cv_h, cv_o, cv_barrier) to synchronize thread execution efficiently.
     * A barrier mechanism to ensure exactly three atoms synchronize before proceeding.

   2. Implementation Details:

      a. The following functions are implemented:
      
          * reaction_init(struct reaction *r): Initializes the reaction synchronization structure.
          * reaction_h(struct reaction *r): Blocks hydrogen threads until they can react.
          * reaction_o(struct reaction *r): Blocks oxygen threads until they can react.
          * make_water(): Called exactly once per valid combination (2H + 1O) to simulate water formation.

      b. Optimizations for Speed:
      
          * Separate condition variables (cv_h and cv_o) ensure that only the required threads wake up, reducing unnecessary wakeups and contention.
          * Barrier synchronization prevents any thread from proceeding before all required atoms are ready.
          * Efficient signaling (cond_signal instead of cond_broadcast) reduces CPU overhead.

      c. Key Challenges Solved:
      
          * Avoiding deadlocks: Ensuring hydrogen and oxygen atoms wait in a coordinated manner.
          * Preventing starvation: Ensuring all atoms eventually react and no thread waits indefinitely.
          * Minimizing contention: Using efficient wake-up strategies to avoid unnecessary thread blocking.

# Testing The Code

    * These programs have been tested on the myth linux cluster and OS X and should be pretty portable to other UNIX-like OSes.
    * Run : 'make run' to run the testing program.
    * Testing program will run both problems' testcases.
    * Each problem's testcase will be in caltrain-runner.c & reaction-runner.c file.

# Conclusion

These solutions demonstrate proper use of locks and condition variables to solve real-world synchronization challenges. They ensure efficient thread coordination without busy-waiting, race conditions, or deadlocks.

Further Enhancements

    * Extend CalTrain to support multiple simultaneous trains.
    * Optimize the Chemical Reaction solution further with adaptive thread pooling.
    * For any questions or improvements, feel free to contribute!
          
