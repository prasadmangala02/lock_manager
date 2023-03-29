****************************************************** Readme.txt ***************************************************************************************************************************************

Submitted By : Reg no - 21558
               Date   - 11/03/22
Files submitted :
i) lockmanager_21558.cpp
ii) readme.txt


lockmanager_21558.cpp - This .cpp file contains the main code for the lockamanger program.

Goal : Build a lock manager. 
The lock manager should support the following capabilities:
1. Lock a resource in either shared or exclusive mode. 
2. Unlock a resource held by a transaction. 
A resource will be identified by a 10 character string. 

Some of Features of the lockmanager implemented :
1) In the lock function, first we check whether there are entries corresponding to the resource in the lock table
   If not there, then a new entry is created for that resource in the locktable and then the transaction is Granted the lock acces it has requested for.
   If already there, then w.r.t the locktype and the lock status,w.r.t the compatability, transactions are Granted lock access, or are put into waiting status.
2) Duplicate lock request : Transaction asking for a lock access with the same locktype as it is already holding a lock on the resource with the same type, won't create any conflict
   for eg :  lock("DBMS", 21551, lockType::EXCLUSIVE);
             lock("DBMS", 21551, lockType::EXCLUSIVE); 
   But in the case, where the locktype is different,
      if previous locktype was Shared and if the asked locktype is Exclusive, if no other transactions are holding the resource,then the upgrade to exclusive will be granted
                                                                              Otherwise,the lock status will be updated to Waiting and the locktype will be Exclusive.
      if previous locktype was Exclusive and if the asked locktype is Shared,if there are subsequent sequence of one or more Shared locktype access in Waiting, then grant all those locks(till another 
                                                                          Exclusive lock with status Waiting is encountered) along with the requested transaction's lockstatus, with it's locktype downgrad                                                                           ed to Shared.
3) If a transaction is waiting for an exclusive lock on a resource asks for the shared lock on the same resource and the resource is currently held in shared lock, then the transaction will be granted shared lock and the exclusive request will still be in the queue.
4) A transaction can't unlock(remove from itself from the the queue) if it is in waiting state.
 


Compile the code using  : g++ lockmanager_21558.cpp
Execution command : ./a.out

Sample Output : 

................... Welcome to Lock Manager ........................

************************************* Lock Table *********************************************
**********************************************************************************************
 RESOURCE NAME : APUE
**********************************************************************************************
	 Transaction ID (txnid) : 21559	:LockType : Shared	:LockStatus : Granted
	 Transaction ID (txnid) : 21556	:LockType : Exclusive	:LockStatus : Waiting
	 Transaction ID (txnid) : 21558	:LockType : Shared	:LockStatus : Waiting

**********************************************************************************************
**********************************************************************************************
 RESOURCE NAME : DBMS
**********************************************************************************************
	 Transaction ID (txnid) : 21551	:LockType : Exclusive	:LockStatus : Granted
	 Transaction ID (txnid) : 21552	:LockType : Shared	:LockStatus : Waiting
	 Transaction ID (txnid) : 21561	:LockType : Shared	:LockStatus : Waiting

**********************************************************************************************

Choose from the options given below

For LOCK 	=> (Type 'L')
For UNLOCK	=> (Type 'U)
For EXIT	=> (Type 'e')
Enter your choice :  L

Enter the details of the resource to be locked, below

Enter the Resource Name :ML

Enter the Transaction ID :21334

Choose what kind of lock is to be issued 

1)For SHARED lockstatus =>  (Type 'S')
2)For EXCLUSIVE lockstatus => (Type X)

Choice : S

************************************* Lock Table *********************************************
**********************************************************************************************
 RESOURCE NAME : ML
**********************************************************************************************
	 Transaction ID (txnid) : 21334	:LockType : Shared	:LockStatus : Granted

**********************************************************************************************
**********************************************************************************************
 RESOURCE NAME : APUE
**********************************************************************************************
	 Transaction ID (txnid) : 21559	:LockType : Shared	:LockStatus : Granted
	 Transaction ID (txnid) : 21556	:LockType : Exclusive	:LockStatus : Waiting
	 Transaction ID (txnid) : 21558	:LockType : Shared	:LockStatus : Waiting

**********************************************************************************************
**********************************************************************************************
 RESOURCE NAME : DBMS
**********************************************************************************************
	 Transaction ID (txnid) : 21551	:LockType : Exclusive	:LockStatus : Granted
	 Transaction ID (txnid) : 21552	:LockType : Shared	:LockStatus : Waiting
	 Transaction ID (txnid) : 21561	:LockType : Shared	:LockStatus : Waiting

**********************************************************************************************

Choose from the options given below

For LOCK 	=> (Type 'L')
For UNLOCK	=> (Type 'U)
For EXIT	=> (Type 'e')
Enter your choice :  L

Enter the details of the resource to be locked, below

Enter the Resource Name :ML

Enter the Transaction ID :21556

Choose what kind of lock is to be issued 

1)For SHARED lockstatus =>  (Type 'S')
2)For EXCLUSIVE lockstatus => (Type X)

Choice : S

************************************* Lock Table *********************************************
**********************************************************************************************
 RESOURCE NAME : ML
**********************************************************************************************
	 Transaction ID (txnid) : 21334	:LockType : Shared	:LockStatus : Granted
	 Transaction ID (txnid) : 21556	:LockType : Shared	:LockStatus : Granted

**********************************************************************************************
**********************************************************************************************
 RESOURCE NAME : APUE
**********************************************************************************************
	 Transaction ID (txnid) : 21559	:LockType : Shared	:LockStatus : Granted
	 Transaction ID (txnid) : 21556	:LockType : Exclusive	:LockStatus : Waiting
	 Transaction ID (txnid) : 21558	:LockType : Shared	:LockStatus : Waiting

**********************************************************************************************
**********************************************************************************************
 RESOURCE NAME : DBMS
**********************************************************************************************
	 Transaction ID (txnid) : 21551	:LockType : Exclusive	:LockStatus : Granted
	 Transaction ID (txnid) : 21552	:LockType : Shared	:LockStatus : Waiting
	 Transaction ID (txnid) : 21561	:LockType : Shared	:LockStatus : Waiting

**********************************************************************************************

Choose from the options given below

For LOCK 	=> (Type 'L')
For UNLOCK	=> (Type 'U)
For EXIT	=> (Type 'e')
Enter your choice :  L

Enter the details of the resource to be locked, below

Enter the Resource Name :APUE

Enter the Transaction ID :21559

Choose what kind of lock is to be issued 

1)For SHARED lockstatus =>  (Type 'S')
2)For EXCLUSIVE lockstatus => (Type X)

Choice : X

************************************* Lock Table *********************************************
**********************************************************************************************
 RESOURCE NAME : ML
**********************************************************************************************
	 Transaction ID (txnid) : 21334	:LockType : Shared	:LockStatus : Granted
	 Transaction ID (txnid) : 21556	:LockType : Shared	:LockStatus : Granted

**********************************************************************************************
**********************************************************************************************
 RESOURCE NAME : APUE
**********************************************************************************************
	 Transaction ID (txnid) : 21559	:LockType : Exclusive	:LockStatus : Granted
	 Transaction ID (txnid) : 21556	:LockType : Exclusive	:LockStatus : Waiting
	 Transaction ID (txnid) : 21558	:LockType : Shared	:LockStatus : Waiting

**********************************************************************************************
**********************************************************************************************
 RESOURCE NAME : DBMS
**********************************************************************************************
	 Transaction ID (txnid) : 21551	:LockType : Exclusive	:LockStatus : Granted
	 Transaction ID (txnid) : 21552	:LockType : Shared	:LockStatus : Waiting
	 Transaction ID (txnid) : 21561	:LockType : Shared	:LockStatus : Waiting

**********************************************************************************************

Choose from the options given below

For LOCK 	=> (Type 'L')
For UNLOCK	=> (Type 'U)
For EXIT	=> (Type 'e')
Enter your choice : e