#include<iostream>
using namespace std;

class node {
public:
    int id;
    int coordinator;
    bool isAlive;
    bool electionStarted;
    node* next = NULL;
    
    node() {
        this->isAlive = true;
        this->electionStarted = false;   
    }
    
    node(int i, bool c) {
        this->id = i;
        this->coordinator = c ? i : -1;
        this->isAlive = true;
        this->electionStarted = false;
    }
    
    node* startProcess(int total_nodes) {
        int i_d;
        bool isC;
        node* p = NULL;
        node* start = NULL;
        
        for(int i = 1 ; i <= total_nodes ; i++) {
            cout << "Enter id for process " << i << ": ";
            cin >> i_d;
            cout << "Is process " << i_d << " a coordinator (1/0): ";
            cin >> isC;
            node* curr = new node(i_d, isC);
            
            if(p == NULL) {
                start = curr;
            } else {
                p->next = curr;
            }
            p = curr;
        }
        return start;
    }
    
    void display_processes(node *n) {
        node* head = n;
        while(head != NULL) {
            cout << "Process id : " << head->id << endl;
            cout << "Is coordinator : " << (head->coordinator != -1 ? "Yes" : "No") << endl;
            cout << "Election Started : " << (head->electionStarted ? "Yes" : "No") << endl;
            cout << "Is Alive : " << (head->isAlive ? "Yes" : "No") << endl<<endl;
            head = head->next;
        }
    }
    
    void kill_node(int i_d, node *n) {
        node* head = n;
        while (head != NULL) {
            if(head->id == i_d) {
                head->isAlive = false;
            }
            head = head->next;
        }
    }
    
    void startElection_bully(node* NodeStarted) {
        int i_d = NodeStarted->id;
        int c_n = NodeStarted->coordinator;
        
        if(NodeStarted->isAlive && !(NodeStarted->electionStarted)) {
            for(int i = i_d + 1; i <= c_n; i++) {
                cout << i_d << " sends election started to " << i << endl;
            }
        }
    }
};

int main() {
    cout<<"Bully Election ALgorithm : "<<endl;
    node x;
    node* start_node = NULL;
    int choice = 0 ;
    do
    {
        cout<<"Menu :"<<endl;
        cout << "1. Add nodes" << endl;
        cout << "2. Disable a node" << endl;
        cout << "3. Display nodes" << endl;
        cout << "4. Start Election" << endl;
        cout << "5. Exit" << endl;
        cin >> choice;

        switch (choice)
        {
        case 1:
            int num_nodes;
            cout<<"Number of nodes to be added : ";
            cin>>num_nodes;
            start_node = x.startProcess(num_nodes);
            break;

        case 2:
            if(start_node != NULL) 
            { 
                int node_idx_to_kill;
                cout << "Enter node to disable: ";
                cin >> node_idx_to_kill;
                x.kill_node(node_idx_to_kill, start_node);
            } 
            else 
            {
                cout << "No nodes added yet " << endl;
            }
            break;

        case 3:
            if(start_node != NULL) 
            { 
                x.display_processes(start_node);
            } 
            else 
            {
                cout << "No nodes added yet!" << endl;
            }               
            break;

        case 4:
            break;

        default:
            cout<<"Enter valid choice"<<endl;
            break;
        
    } 
    }while (choice!=5);
    

    return 0;
}

