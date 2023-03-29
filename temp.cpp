/* 
Goal : Build a lock manager. 
The lock manager should support the following capabilities:
1. Lock a resource in either shared or exclusive mode. 
2. Unlock a resource held by a transaction. 
A resource will be identified by a 10 character string. 

         *** Done By :
	                 Reg no : 21558
                         Date   : 10/03/22
*/

#include <iostream>
#include <unordered_map>
#include <list>

using namespace std;

// Abstraction of a resource that can be locked. 
// A resource is locked in a 'mode' by a 'transaction'. 
// The lock request may be granted or put on wait based
// on a lock compatibility matrix. 
class lockable_resource
{
private:
  uint32_t txn_id_;
  uint8_t lock_type_; // SHARED, EXCLUSIVE
  uint8_t lock_status_; // GRANTED, WAITING
public:
  lockable_resource(){}
  lockable_resource(
    uint32_t txn_id,
    uint8_t lock_type,
    uint8_t lock_status):
    txn_id_(txn_id),
    lock_type_(lock_type),
    lock_status_(lock_status)
  {}
  uint8_t getLockType() 
  {
    return(lock_type_);
  }
  uint8_t getStatus()
  {
    return(lock_status_);
  }
  uint32_t getTxnId()
  {
    return(txn_id_);
  }
  bool setLockStatus(uint8_t st)
  {
    lock_status_ = st;
    return true;
  }
  bool setLockType(uint8_t lt)
  {
    lock_type_= lt;
    return true;
  }
};

enum lockType
{
  SHARED,
  EXCLUSIVE
};
enum lockStatus
{
  GRANTED,
  WAITING
};

// Resource
// type of lock
// txnid

bool lock(std::string resource_name, 
          std::uint32_t txn_id,
          uint8_t lock_type);
void display_lockTable();

bool unlock(std::string resource_name,
              std::uint32_t txn_id);

unordered_map<std::string, list<lockable_resource *>*> lock_table;

void display_lockTable()   //For displaying the contents of the locktable
{
  unordered_map<std::string, list<lockable_resource *>*>::iterator lock_table_itr;
  cout<<"\n************************************* Lock Table *********************************************\n";
  for (lock_table_itr=lock_table.begin(); lock_table_itr!=lock_table.end(); lock_table_itr++)
  {
    cout<<"**********************************************************************************************\n";
    cout <<" RESOURCE NAME : "<<lock_table_itr->first <<endl;
    cout<<"**********************************************************************************************\n";
    list<lockable_resource *>::iterator lt_itr= lock_table[lock_table_itr->first]->begin();
    for(;lt_itr != lock_table[lock_table_itr->first]->end(); lt_itr++)
    {
      lockable_resource resource= *(*(lt_itr));
      cout<<"\t Transaction ID (txnid) : "<<resource.getTxnId()<<"\t:";
      if (resource.getLockType() == lockType::EXCLUSIVE)
        cout<<"LockType : Exclusive";
      else
        cout<<"LockType : Shared";
      cout <<"\t:";
      if(resource.getStatus()==lockStatus::WAITING)
        cout<<"LockStatus : Waiting";
      else
        cout <<"LockStatus : Granted";
      cout<<endl;


    }
    cout<<"\n**********************************************************************************************\n";
  }
}


int count_Exclusive(string resource_name)     //For counting how many Exclusive lock requests which have been either (Granted or waiting) is there for the resource
{
  int count;
  list<lockable_resource *>::iterator l_itr= lock_table[resource_name]->begin();
  for (; l_itr != lock_table[resource_name]->end(); l_itr++)
  {
    lockable_resource resource= **l_itr;
    if (resource.getLockType() == lockType::EXCLUSIVE)
      count++;

  }
  return count;
}


int count_Shared(string resource_name)     //Retuurns the number of Shared locktype requests on the resource which are granted
{
  int count;
  list<lockable_resource *>::iterator l_itr= lock_table[resource_name]->begin();
  for (; l_itr != lock_table[resource_name]->end(); l_itr++){
    lockable_resource resource= **l_itr;
    if (resource.getLockType() == lockType::SHARED && resource.getStatus() == lockStatus::GRANTED)
      count++;

  }
  return count;
}

int main()
{
  char choice;
  uint32_t txnid;
  uint8_t ret;
  string resource_name;

  ret = lock("DBMS", 21551, lockType::EXCLUSIVE);  //For some initial entries into the locktable
  ret = lock("DBMS", 21552, lockType::SHARED);
  ret = lock("APUE", 21559, lockType::SHARED);
  ret = lock("APUE", 21556, lockType::EXCLUSIVE);
  ret = lock("APUE", 21558, lockType::SHARED);
  ret = lock("DBMS", 21561, lockType::SHARED);


  cout<<"\n................... Welcome to Lock Manager ........................\n";

  while(true)
  {
    display_lockTable();
    cout<<"\nChoose from the options given below\n";
    cout <<"\nFor LOCK \t=> (Type 'L')\nFor UNLOCK\t=> (Type 'U)\nFor EXIT\t=> (Type 'e')\nEnter your choice :  ";
    cin >> choice;

    switch (choice)
    {
      case 'L':
        cout <<"\nEnter the details of the resource to be locked, below"<<endl;
        cout <<"\nEnter the Resource Name :";
        cin>>resource_name;
        cout <<"\nEnter the Transaction ID :";
        cin >>txnid;
	char choice1;
	cout<<"\nChoose what kind of lock is to be issued ";
        cout <<"\n\n1)For SHARED lockstatus =>  (Type 'S')\n2)For EXCLUSIVE lockstatus => (Type X)\n\n";
	cout<<"Choice : ";
        cin >>choice1;
        lockType lockt;
        if (choice1 == 'S')
          lockt = lockType::SHARED;
	else if (choice1 == 'X')
          lockt = lockType::EXCLUSIVE;
	else
	{
		cout<<"\nWrong option, no locktype corresponding to it,requested lock accees denied\n";
		continue;
	}
        lock(resource_name,txnid,lockt);
        break;

      case 'U':
         cout <<"\nEnter the details of the resource to be unlocked,with txnid of transaction that holds a lock on it, below"<<endl;
         cout <<"\nEnter the Resource Name :";
         cin>>resource_name;
         cout <<"\nEnter the Transaction ID :";
         cin >>txnid;
         unlock(resource_name,txnid);
         break;

      case 'e':
         return 0;

      default:
         cout<<"Wrong option choice,Option not found.";
         break;
    }
  }
  display_lockTable();
  return 0;

}


bool unlock(std::string resource_name,std::uint32_t txn_id)  //To unlock the lock held by the transaction with txn_id, on the resource
{
  bool flag_share;
  int countS;
  if(lock_table.find(resource_name) != lock_table.end())
  {
    std::list<lockable_resource*>::iterator iter = lock_table[resource_name]->begin();
    for( ; iter != lock_table[resource_name]->end(); iter++)
    {
      if((*iter)->getTxnId() == txn_id)
      {
        if((*iter)->getStatus() == lockStatus::WAITING)    //Scenario : where the requested txnid's lockstatus is still waiting,implies,there is noting to unlock
        {
          std::cout << "cannot be unlocked as txnId " << txn_id << " is not holding the lock\n";
          return false;   
        }
        uint8_t lockT = (*iter)->getLockType();
	//countS = count_Shared(resource_name);
        std::list<lockable_resource*>::iterator it = iter;
        it++;
        lock_table[resource_name]->erase(iter);  //the pointer to that transaction's entry for that particular resource is removed from the locktable
        flag_share = false;
       
        if(!lock_table[resource_name]->empty())   //if the entries in lock table for that resource are still there, implies some transactions have requested 
        {                                        // for a lock access on that particular resource
           
          if(lockT == lockType::EXCLUSIVE)   //Suppose if the released lock was an Exclusive lock
          {
            (*it)->setLockStatus(lockStatus::GRANTED);  //here it points to the next transaction's entry in the lock table and it is given acces to have a lock on the resource
     
            if((*it)->getLockType() == lockType::SHARED)  //if the next in line was for a shared lock,then look for other shared lock accesses requested and grant it
            {
              std::list<lockable_resource*>::iterator tmp_it = it;
              tmp_it++;
              for( ; tmp_it != lock_table[resource_name]->end(); tmp_it++)  //going through the rest of the entries for the resource in the lock table
              {
                if((*tmp_it)->getLockType() == lockType::SHARED && (*tmp_it)->getStatus() != lockStatus::GRANTED)  //if a subsequent lock is shared and not granted yet,then grant access
                {
                  (*tmp_it)->setLockStatus(lockStatus::GRANTED);
                  std::cout << "SHARED lock being GRANTED for txnId " << (*tmp_it)->getTxnId() << " on resource AAA" << std::endl;
                }
              }
            }
          }

     
          else                 //Suppose if the released lock was a shared lock
          {
	    countS = count_Shared(resource_name);
            
            if((*it)->getLockType() == lockType::SHARED)  //if next subsequent locktype is also shared
            {
              std::list<lockable_resource*>::iterator tmp_it = it;
              for( ; tmp_it != lock_table[resource_name]->end(); tmp_it++)   //going through the rest of the entries for the resource in the lock table
              {
                
                if((*tmp_it)->getLockType() == lockType::SHARED && (*tmp_it)->getStatus() != lockStatus::GRANTED)
                {
                  (*tmp_it)->setLockStatus(lockStatus::GRANTED);
                }
              }
            }

            
            else                 //Next Scenario, where the released lock is shared and the next subsequent locktype is exclusive 
            {
              std::list<lockable_resource*>::iterator tmp_it = it;
          
              for( ; tmp_it != lock_table[resource_name]->end(); tmp_it++)  //If in the subsequent list of transaction requests,seeing whether a shared lock request
              {                                                             //is there or not
                if((*tmp_it)->getLockType() == lockType::SHARED)
                {
                  flag_share = true;
                  break;
                }
              }

      
              if(flag_share == false && countS == 0)  //checks whether there are some other transaction ID's with a granted shared lock,if not,grant exclusive access      
              {
                (*it)->setLockStatus(lockStatus::GRANTED);
              }
            }
          }
        }

        
        else // Scenario where there is no lock request entries for that resource, then remove the pointer to that rrsource 
        {
          lock_table.erase(resource_name);
        }
        break;
      }
    }
  }
  return true;
}


bool lock(std::string resource_name,std::uint32_t txn_id,uint8_t lock_type)
{
  
  uint8_t retval = lockStatus::WAITING;
  int count;

  // Check if lock exists. 
  // if No then Add to lock_table, create new list and add lockable_resource to list
  
  if (lock_table.find(resource_name) == lock_table.end())  //Checking whether the entries related to the resource exists in the lock table or not
  {
    // In lock table the entry does not exist. 
    //  => locable resource has to be created. 
    //     list of lockable resources has to be created. 
    //     lock table should be updated with resource. 

    lockable_resource *lr = new lockable_resource(txn_id,lock_type,lockStatus::GRANTED);

    retval = lockStatus::GRANTED;
    list<lockable_resource*> *lst = new list<lockable_resource*>;
    lst->emplace_back(lr);
    lock_table[resource_name] = lst;
  }
  else        //Scenario where already entries related to the resource is already there in the locktable
  {
    
    count = count_Shared(resource_name);                               //This also implies there exists transactions which holds a lock on this resource
    list<lockable_resource *>::iterator lr = lock_table[resource_name]->begin(); //iterator of the list of above mentioned transactions

    bool dup_flag= false;
    lockable_resource *resource;
    for(; lr != lock_table[resource_name]->end(); lr++)    //iterating through the list of those transactions who has a lock access(waiting or granted) on the resource
    {
      resource= *lr;                                       //and checks whether the current requested transaction already has a lock issued on the resource 
      if (resource->getTxnId() == txn_id)                 //if found , dup_flag is set to true.
      {
        dup_flag= true;
        break;
      }
    }

    int countE = count_Exclusive(resource_name);

    if(dup_flag == false)     //If the request is not from a transaction which already has a lock on the resource (implying not a duplicate txn request)
    {
      

      list <lockable_resource*> * resourceP= lock_table[resource_name];
      if (lock_type == lockType::SHARED) //If the request is for a locktype Shared
      {
        if (count > 0 && countE == 0)                    //if there are already transactions with lock combo "Shared + Granted", then set the lockstatus as 'Granted'
        {
          resourceP->emplace_back(new lockable_resource(txn_id,lock_type,lockStatus::GRANTED));
          retval= lockStatus::GRANTED;
        }
        else                             //if there is no other transactions with a "Shared + Granted" lock, then just set the status as 'Waiting'
        {
          resourceP->emplace_back(new lockable_resource(txn_id,lock_type,lockStatus::WAITING));
          retval= lockStatus::WAITING;
        }
      }
      else            //if the requested locktype is exclusive, then irrespective of any granted locks,we have to set the lockstatus as 'Waiting' only as exclusive is incompatible
      {
        resourceP->emplace_back(new lockable_resource(txn_id,lock_type,lockStatus::WAITING));
        retval= lockStatus::WAITING;
      }

    }

   
    else                                //If the request was a duplicate request
    {
      if (lock_type != resource->getLockType() )       
      {
        if (lock_type == lockType::EXCLUSIVE)         //Here, if the lock access requested transaction had a locktype of shared previously and asked for an exclusive currently
	{
          if (count > 1)                                       //if there are more txns with shared lock on the resource with granted status
	  {
	    unlock(resource_name,txn_id);
	    list <lockable_resource*> * resourceP= lock_table[resource_name];
	    resourceP->emplace_back(new lockable_resource(txn_id,lock_type,lockStatus::WAITING));
            retval= lockStatus::WAITING;
                  //then update the locktype of the transaction to exclusive and lock status as Waiting
          }
	  else
            retval= lockStatus::GRANTED;                     //if this was the only transaction with shared lock type and granted status,then upgrade the txn locktype to exclusive
                                                              // and granted status
          resource->setLockType(lockType::EXCLUSIVE);
        }
	else                                  //if the old transaction had an exclusive lock and if the new request is for a shared lock
        {
          if (count > 0 && countE == 0)                      //Downgrade the locktype to Shared and if there are other txns with Shared + Granted,then set lockstatus as Granted (to Granted)
	  {
            resource->setLockStatus(lockStatus::GRANTED);    //here checking for countE (which gives count of exclusive lock requests), is to avoid starvation for exclusive
            retval= lockStatus::GRANTED;
          }
	  else
		  retval= lockStatus::WAITING;              //if no other txnx with Shared locktype request, then just make it to Shared + Waiting
          resource->setLockType(lockType::SHARED);
       
	  if (resource->getLockType() == lockType::SHARED && resource->getStatus() == lockStatus::GRANTED) //Now,if the newly updated transaction has a lock combo "Shared + Granted "
	  {
            lr = lock_table[resource_name]->begin();
            for(; lr != lock_table[resource_name]->end(); lr++)            //then the subsequent lock requests of type 'Shared + 'Waiting' also should be granted
	    {
              resource= *(lr);
              if (resource->getLockType() == lockType::SHARED && resource->getStatus() == lockStatus::WAITING)
	      {
                resource->setLockStatus(lockStatus::GRANTED);
                retval= lockStatus::GRANTED;
              }
              else              
	      {
                if (resource->getLockType()==lockType::EXCLUSIVE)   //this is to avoid starvation as if an exclusive comes in between, then the rest of the shared shouldn't be granted 
                  break;
              }
            }
	  }
        }
        
      }
    }

  }

  return(retval);

}
