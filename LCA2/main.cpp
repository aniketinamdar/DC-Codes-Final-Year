#include <iostream>
#include <bits/stdc++.h>
using namespace std;

class node
{
public:
    int id;
    int coordinator;
    bool isAlive;
    bool electionStarted;
    bool message_back;
    bool visited;
    node *next = NULL;

    node()
    {
        this->isAlive = true;
        this->electionStarted = false;
    }

    node(int i, bool c)
    {
        this->id = i;
        this->coordinator = c ? i : -1;
        this->isAlive = true;
        this->electionStarted = false;
        this->message_back = false;
        this->visited = false;
    }

    node *startProcess(int total_nodes)
    {
        int i_d;
        bool isC;
        node *p = NULL;
        node *start = NULL;

        for (int i = 1; i <= total_nodes; i++)
        {
            cout << "Enter id for process " << i << ": ";
            cin >> i_d;
            cout << "Is process " << i_d << " a coordinator (1/0): ";
            cin >> isC;
            node *curr = new node(i_d, isC);

            if (p == NULL)
            {
                start = curr;
            }
            else
            {
                p->next = curr;
            }
            p = curr;
        }
        return start;
    }

    void display_processes(node *n)
    {
        node *head = n;
        while (head != NULL)
        {
            cout << "Process id : " << head->id << endl;
            cout << "Is coordinator : " << (head->coordinator != -1 ? "Yes" : "No") << endl;
            cout << "Election Started : " << (head->electionStarted ? "Yes" : "No") << endl;
            cout << "Is Alive : " << (head->isAlive ? "Yes" : "No") << endl
                 << endl;
            head = head->next;
        }
    }

    void kill_node(int i_d, node *n)
    {
        node *head = n;
        while (head != NULL)
        {
            if (head->id == i_d)
            {
                head->isAlive = false;
            }
            head = head->next;
        }
    }

    void startElection_bully(int total_nodes, node *start_node, node *coppy)
    {

        if (start_node == NULL)
        {
            return;
        }
        node *s = start_node;
        if (!s->electionStarted)
        {
            s->electionStarted = true;
            for (int i = s->id; i < total_nodes; i++)
            {
                cout << start_node->id << " sent message to " << i + 1 << endl;
            }

            s = s->next;
            bool flag = false;

            while (s != NULL)
            {
                if (s->isAlive)
                {
                    cout << s->id << " sent ok back to " << start_node->id << "\n";
                    flag = true;
                }
                s = s->next;
            }
            if (flag)
            {
                start_node->message_back = true;
            }
            if (!start_node->message_back)
            {
                cout << "New coordinator is " << start_node->id << endl;
                while (coppy != NULL)
                {
                    coppy->coordinator = start_node->id;
                    coppy = coppy->next;
                }
            }
            if (start_node->next == NULL)
            {
                return;
            }
            s = start_node;
            while (s->next != NULL)
            {
                if (s->next->isAlive)
                {
                    startElection_bully(total_nodes, s->next, coppy);
                }
                s = s->next;
            }
        }
    }
    void startElection_ring(node *start_node)
    {
        int ans = INT_MIN;
        while (!start_node->visited)
        {
            start_node->visited = true;
            ans = max(ans, start_node->id);
            node *temp = start_node->next;
            while (!temp->isAlive)
            {
                temp = temp->next;
            }
            if (!temp->visited)
            {
                cout << start_node->id << " Sent election e" << start_node->id << " to " << temp->id << endl;
            }
            start_node = temp;
        }
        cout << "New cordinator is " << ans << endl;
    }
};

int main()
{
    cout << "Election Algorithms (Bully and Ring) : " << endl;
    node x;
    node *start_node = NULL;
    int choice = 0;
    int num_nodes = 0;
    do
    {
        cout << "Menu :" << endl;
        cout << "1. Add nodes" << endl;
        cout << "2. Disable a node" << endl;
        cout << "3. Display nodes" << endl;
        cout << "4. Start Bully algorithm election" << endl;
        cout << "5. Start Ring algorithm election" << endl;
        cout << "6. Exit" << endl;
        cin >> choice;

        switch (choice)
        {
        case 1:
            cout << "Number of nodes to be added : ";
            cin >> num_nodes;
            start_node = x.startProcess(num_nodes);
            break;

        case 2:
            if (start_node != NULL)
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
            if (start_node != NULL)
            {
                x.display_processes(start_node);
            }
            else
            {
                cout << "No nodes added yet!" << endl;
            }
            break;

        case 4:
            int node_id;
            cout << "Enter node id that starts the election\n";
            cin >> node_id;
            for (int i = 0; i < node_id - 1; i++)
            {
                start_node = start_node->next;
            }
            x.startElection_bully(num_nodes, start_node, start_node);
            break;

        case 5:
            x.startElection_ring(start_node);
            break;

        default:
            cout << "Enter valid choice" << endl;
            break;
        }
    } while (choice != 6);

    return 0;
}
