NAME - Mangala Prasad Sahu
REG NO - 22560
DATE - 29th March 2023
Github link - 

Info >> The lock_manager.cpp file contains code for the following exercise
Compile Using >> g++ -o lock lock_manager.cpp
Run Using >> ./lock


Goal : Build a lock manager. 
The lock manager should support the following capabilities:
1. Lock a resource in either shared or exclusive mode. 
2. Unlock a resource held by a transaction. 
A resource will be identified by a 10 character string. 

The lock_manager does the following jobs 
(i) The lock manager first checks if the resource requested is already present previously. If the lockable
    resource is not present previously then it creates an entry for the resource in the locktable and 
    grants the required access. If the lockable resource is present in the table previously then it iterators
    the locktable to find its location and then continues accoordingly.

    if there are no Duplicate requests then only the program continues. If a transaction is waiting for 
    an exclusive lock on a resource asks for the shared lock on the same resource and the resource is 
    currently held in shared lock, then the transaction will be granted shared lock and the exclusive 
    request will still be in the queue.
    A transaction can't unlock(remove from itself from the the queue) if it is in waiting state.


TODO : Need to implement the UPGRADE and DOWNGRADE feature of the in the presence of the duplicate requests.
       Need to implement code for Deadlock detection.

