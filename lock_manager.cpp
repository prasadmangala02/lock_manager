// Name - Mangala Prasad Sahu
// Reg No - 22560
// Date - 29th March 2023

/* 
Goal : Build a lock manager. 
The lock manager should support the following capabilities:
1. Lock a resource in either shared or exclusive mode. 
2. Unlock a resource held by a transaction. 
A resource will be identified by a 10 character string. 
*/

#include <iostream>
#include <unordered_map>
#include <list>

using namespace std;

// Abstraction of a resource that can be locked. 
// A resource is locked in a 'mode' by a 'transaction'. 
// The lock request may be granted or put on wait based
// on a lock compatibility matrix. 

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

const char* type_txt[] = {"shared", "exclusive"};
const char* stat_txt[] = {"granted", "waiting"};
class lockable_resource
{
private:
  uint32_t txn_id_;
  lockType lock_type_; // SHARED, EXCLUSIVE
  lockStatus lock_status_; // GRANTED, WAITING
public:
  lockable_resource(
    uint32_t txn_id,
    lockType lock_type,
    lockStatus lock_status):
    txn_id_(txn_id),
    lock_type_(lock_type),
    lock_status_(lock_status)
  {}
  lockType getLockType() 
  {
    return(lock_type_);
  }
  lockStatus getStatus()
  {
    return(lock_status_);
  }
  uint32_t getTxnId()
  {
    return(txn_id_);
  }
  void setLockStatus(lockStatus st)
  {
    lock_status_ = st;
  }

  // void setLockStatus(uint8_t st)
  //   {
  //     lock_status_ = st;
  //   }

};



// Resource
// type of lock
// txnid

bool lock(std::string resource_name, 
          std::uint32_t txn_id,
          lockType lock_type);

void unlock(std::string resource_name,
              std::uint32_t txn_id);


void display_lockTable();

unordered_map<std::string, list<lockable_resource *>*> lock_table;

int main()
{

  string resource_name;
  int transaction_ID = 0;
  // lockType lt;
  bool ret;

  cout << "--------------------- LOCK MANAGER ---------------------\n\n" << endl;
  while (true){

    char choice;
    cout << "------OPTIONS AVAILABLE------" << endl;
    cout << "-----------------------------" << endl;
    cout << "for LOCK -> type 'L' or 'l'" << endl;
    cout << "for UNLOCK -> type 'U or 'u'" << endl;
    cout << "for EXIT -> type 'E' or 'e'" << endl;
    cout << "-----------------------------" << endl;
    
    cout << "Enter >> ";
    cin >> choice;
    choice = tolower(choice);

    switch(choice){

      case 'l':
        cout << "YOU HAVE CHOSEN TO LOCK A RESOURCE"<<endl;
        cout << "ENTER THE RESOURCE NAME >> ";
        cin >> resource_name;
        cout << "ENTER THE TRANSACTION ID >> ";
        cin >> transaction_ID;
        cout << "ENTER THE LOCKTYPE 's' for SHARED or 'x' for EXCLUSIVE >> ";
      
        lockType lt;
        char lockChoice;
        cin >> lockChoice;
        lockChoice = tolower(lockChoice);

        if(lockChoice == 's'){
          lt = lockType::SHARED;
        }
        else if(lockChoice == 'x'){
          lt = lockType::EXCLUSIVE;
        }
        else {
          cout << "INVALID OPTION..." << endl;
          continue;
        }
        ret = lock(resource_name, transaction_ID, lt);
        break;
      case 'u':
        cout << "YOU HAVE CHOSEN TO UNLOCK A RESOURCE"<<endl;
        cout << "ENTER THE RESOURCE NAME >> ";
        cin >> resource_name;
        cout << "ENTER THE TRANSACTION ID >> ";
        cin >> transaction_ID;
        unlock(resource_name, transaction_ID);

        break;

      case 'e':
        cout << "EXITING..." << endl;
        return 0;

      default:
        cout << "INVALID CHOICE ENTERED..." << endl;
        break;
    }
    cout << "\n\n\n";
    display_lockTable();
    cout << "\n\n\n";

  }
  // ret = lock("AAA", 1234, lockType::SHARED);
  // ret = lock("AAB", 1234, lockType::SHARED);
  // ret = lock("AAC", 1236, lockType::SHARED);
  // ret = lock("AAD", 4567, lockType::EXCLUSIVE);
  // ret = lock("AAA", 1235, lockType::SHARED);
  // ret = lock("AAA", 1243, lockType::EXCLUSIVE);
  // ret = lock("AAD", 1236, lockType::SHARED);
  // ret = lock("AAE", 1237, lockType::SHARED); // for case - when the resource is not held by none of the transaction.

  // display_lockTable();

  // unlock("AAC", 1236);
  // unlock("AAD", 4567);
  // unlock("AAE", 1237);

  // display_lockTable();

  return 0;

}

void display_lockTable(){

  // iterator for each of the element in the hashtable

  cout << "***************LOCK TABLE***************"<<endl;
  
  unordered_map<std::string, list<lockable_resource *>*>::iterator itr;
  itr = lock_table.begin();

  for ( ; itr != lock_table.end(); itr++){

    cout <<"resource name: "<< itr->first << endl;

    // iterator for the list .
    list<lockable_resource*>::iterator itr_lst =  (itr->second)->begin();

    for(; itr_lst != (itr->second)->end(); itr_lst++){

      lockable_resource resource = **itr_lst;
      cout <<"Txn Id :: "<<resource.getTxnId()<<" , lock type :: "<< type_txt[resource.getLockType()] << " , lock status :: " << stat_txt[resource.getStatus()] <<  endl;
    }
  }
}
bool lock(std::string resource_name,
          std::uint32_t txn_id,
          lockType lock_type)
{
  lockStatus retval = lockStatus::WAITING;
  // Check if lock exists. 
  //   No: Add to map, create new list and add lockable_resource to list
  if (lock_table.find(resource_name) == lock_table.end())
  {
    // lock table does not exist. 
    //  => locable resource has to be created. 
    //     list of lockable resources has to be created. 
    //     lock table should be updated wit resource. 
    lockable_resource *lr = new lockable_resource(
      txn_id,
      lock_type,
      lockStatus::GRANTED
    );
    retval = lockStatus::GRANTED;
    list<lockable_resource*> *lst = new list<lockable_resource*>;
    lst->emplace_back(lr);
    lock_table[resource_name] = lst;
    // cout<<lock_table[resource_name]<<endl;
  }

  // else if the resource is found in the hashtable
  else {

    // pointer to the begin of the list with the resource name 
    list<lockable_resource*>::iterator it = lock_table[resource_name]->begin();

    // duplicate flag
    bool isDuplicate = false;
    // pointer to the lockable resource
    lockable_resource* resource;
    // We need to check if the requesting transaction has any other lock on the resource in the list
    // That is to check whether there is any duplication.
    for (;it != lock_table[resource_name]->end(); it++){
      if((*it)->getTxnId() == txn_id){
        isDuplicate = true;   //i.e. duplicate transaction
        break;
      }
    }

    // i.e. the transaction request is NOT duplicate
    if(isDuplicate == false){
      
      list<lockable_resource*>* lr = lock_table[resource_name];
      list<lockable_resource*>::reverse_iterator lr_end = lock_table[resource_name]->rbegin();
      lockable_resource resource = **lr_end;

      // If the data item is locked in shared mode
      // then we need to check whether the requested transaction is asking for
      // shared mode or exclusive mode
      if(resource.getLockType() == lockType::SHARED) {
        if(lock_type == lockType::SHARED){

          lr->emplace_back(new lockable_resource(txn_id, lock_type, lockStatus::GRANTED));
        }
        else{
          lr->emplace_back(new lockable_resource(txn_id, lock_type, lockStatus::WAITING));
        }
      }


      // If the dataitem is locked in the exclusive mode 
      // then inspite of the request type we put the transaction in
      // WAITING mode
      else{
        lr->emplace_back(new lockable_resource(txn_id, lock_type, lockStatus::WAITING));
      }

    }

    }


  return(retval);
}


// Unlock function for the lock manager
void unlock(std::string resource_name,std::uint32_t txn_id){


  bool shared_lock_flag = false;

  if (lock_table.find(resource_name) != lock_table.end()){
    

    list<lockable_resource*>::iterator iter = lock_table[resource_name]->begin();

    for( ; iter != lock_table[resource_name]->end() ; iter++){
      
      lockable_resource resource = **iter;
      // uint32_t txn_id_r = resource.getTxnId();

      // lockType lockt = resource.getLockType();
      if(resource.getTxnId() == txn_id){

        // cout << "inside first if"<< endl;
        // cout << "status : " << resource.getStatus() << endl;
        if(resource.getStatus() == lockStatus::WAITING){
          cout << "Cannot be unlocked because";
          cout << " transaction: " << txn_id << "is NOT holding the lock" << endl;
        }

        // if the requested data item is held by some other transaction
        else{

          // At first find out the lock type of the element which is going to be unlocked

          lockType deleted_lock = resource.getLockType();
          list<lockable_resource*>::iterator it = iter;

          // Before removing the lockable_resource
          it++;
          lock_table[resource_name]->erase(iter);

          bool check = lock_table[resource_name]->empty();
          // cout << resource_name <<"check : " << check << endl;
          
          // Check if the resource_name list is non-empty and then proceed
          if(!lock_table[resource_name]->empty()){
            

            // if the released lock type was exclusive
            if(deleted_lock == lockType::EXCLUSIVE){

              // cout <<"before : "<<(**it).getStatus() << endl;
              // Next line in the list gets the lock
              (**it).setLockStatus(lockStatus::GRANTED);
              // display_lockTable();

              // cout <<"after : "<<(**it).getStatus() << endl;

              // cout << "locktype : " << (**it).getLockType() <<  endl;
              // If other resources are requesting for shared lock mode

              if((**it).getLockType() == lockType::SHARED){

                list<lockable_resource*>::iterator middle_itr = it;

                bool temp = (**middle_itr).getLockType();
                bool stat = (**middle_itr).getStatus();
                for(; middle_itr != lock_table[resource_name]->end() ; middle_itr++){
                  if((**middle_itr).getLockType() == lockType::SHARED && (**middle_itr).getStatus() != lockStatus::GRANTED){
                    (**middle_itr).setLockStatus(lockStatus::GRANTED);
                    cout<<"Waiting shared locks are granted" << endl;
                    cout<<"for the transaction id: " << (**middle_itr).getTxnId() << endl;
                  }
                }
              }
              else {
                cout << "Waiting lock is exclusive" << endl;
              }
            }

            // if the released lock type was shared 

            else {
              
              // if the released lock was shared and the lock type in the next is shared
              if((**it).getLockType() == lockType::SHARED){

                list<lockable_resource*>::iterator middle_itr = it;

                // Check if in the next lockable resources has any shared lock requests
                for( ; middle_itr != lock_table[resource_name]->end() ; middle_itr++){

                  if((**middle_itr).getLockType() == lockType::SHARED && (**middle_itr).getStatus() != lockStatus::GRANTED){
                    (**middle_itr).setLockStatus(lockStatus::GRANTED);
                  }
                if(middle_itr!=lock_table[resource_name]->end())
                  break;
                }
                
              }
              // if the released lock was shared and the lock type in the next is exclusive

              else{
                // check if any other shared locks are waiting for it

                list<lockable_resource*>::iterator middle_itr = it;
                for( ; middle_itr != lock_table[resource_name]->end() ; middle_itr++){
                  if((**middle_itr).getLockType() == SHARED){
                    shared_lock_flag = true;
                    break;
                  }
                }

                // check if the next transaction is asking the dataitem in an exclusive mode
                if(shared_lock_flag == false && (lock_table[resource_name]->begin() == it)){
                  // cout << "something";
                  (**it).setLockStatus(lockStatus::GRANTED);
                }
              }
            }
          }
        
        
          // if the resource is NOT held by any transaction
          else{
          lock_table.erase(resource_name);
          }
          


          // when the request transaction id and the current transaction id matches with
          // the help of iterator then we break from the initial for loop.
          break;
          
          }
          
        }

       

      }
    }
}


