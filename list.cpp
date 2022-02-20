#include<iostream>
#define CHAR_SIZE 14
using namespace std;

struct Node
{
    int data;
    Node* npx;
};

Node* XOR(Node* a, Node* b)
{
    return reinterpret_cast<Node*>(
        reinterpret_cast<uintptr_t>(a) ^
        reinterpret_cast<uintptr_t>(b));
}

Node* FIND(Node* pointer, Node* act)
{
    if (pointer == NULL)
        return NULL;
    else if (pointer == act)
        return pointer;
    Node* curr = pointer;
    Node* prev = NULL;
    Node* next;

    while (curr != NULL)
    {
        next = XOR(prev, curr->npx);
        if (next == act)
            return curr;
        prev = curr;
        curr = next;
    }
    return 0;
}

void MOVE(Node** act_ref, Node** n_act_ref, Node** p_act_ref, Node** actr, Node** actl, Node* fpointer, Node* spointer)
{

    if (fpointer == NULL || spointer == NULL)
        cout << "NULL\n";
    else if (*act_ref == spointer)
    {
        *act_ref = fpointer;
        *p_act_ref = spointer;
        *n_act_ref = XOR(NULL, fpointer->npx);
        cout << (*act_ref)->data << '\n';
    }
    else
    {
        Node* curr = *act_ref;
        Node* next = *actr;
        cout << next->data << '\n';
        *act_ref = next;
        *actl = curr;
        *actr = XOR(next->npx, curr->npx);
    }

}

void ADD_EDGE(Node** fpointer, Node** spointer, Node** act_ref, int data)
{
    Node* new_node = new Node();
    new_node->data = data;


    new_node->npx = *fpointer;

    if (*fpointer != NULL)
    {
        (*fpointer)->npx = XOR(new_node, (*fpointer)->npx);
    }
    else
    {
        *spointer = new_node;
        *act_ref = new_node;
    }

    *fpointer = new_node;

}

void ADD_ACT(Node* beg, Node* act, Node** p_act_ref, int data)
{
    Node* curr = beg;
    Node* prev = NULL;
    Node* next;
    while (curr != NULL)
    {
        next = XOR(prev, curr->npx);
        if (next == act)
        {
            Node* new_node = new Node();
            new_node->data = data;
            new_node->npx = XOR(curr, next);
            (*p_act_ref)->npx = XOR(XOR(next, (*p_act_ref)->npx), new_node);
            act->npx = XOR(XOR(curr, act->npx), new_node);
            *p_act_ref = new_node;
            break;
        }
        prev = curr;
        curr = next;
    }
}


void DEL_EDGE(Node** fpointer, Node** spointer, Node* del, Node** act_ref)
{
    if (*fpointer == *spointer)
    {
        *fpointer = NULL;
        *spointer = NULL;
        *act_ref = NULL;
    }
    else
    {
        if (*fpointer == *act_ref)
            *act_ref = *spointer;
        Node* curr = *fpointer;
        Node* prev = NULL;
        Node* next = XOR(prev, curr->npx);
        prev = curr;
        curr = next;
        next = XOR(prev, curr->npx);
        *fpointer = curr;
        (*fpointer)->npx = XOR(NULL, next);
    }
    delete del;
}

void DEL_ACT(Node** act_ref, Node** p_act_ref, Node* n_act, Node* del)
{
    Node* n_next = XOR(*act_ref, n_act->npx);
    *p_act_ref = XOR((*p_act_ref)->npx, *act_ref);
    *act_ref = XOR((*act_ref)->npx, n_act);
    (*act_ref)->npx = XOR(*p_act_ref, n_act);
    n_act->npx = XOR(*act_ref, n_next);
    delete del;
}

void DEL_VAL(Node** beg_ref, Node** end_ref, Node** act_ref, Node** p_act_ref, Node** n_act_ref, int data)
{
    if (*beg_ref == *end_ref)
        DEL_EDGE(beg_ref, end_ref, *beg_ref, act_ref);
    Node* curr = *beg_ref;
    Node* prev = NULL;
    Node* next;
    Node* p_prev = NULL;
    while (curr != NULL)
    {
        next = XOR(prev, curr->npx);
        Node* del = curr;
        if (curr->data == data)
        {
            if (curr == *beg_ref)
            {
                curr = next;
                next = XOR(next->npx, del);
                DEL_EDGE(beg_ref, end_ref, del, act_ref);
                continue;
            }
            else if (curr == *act_ref)
            {
                curr = next;
                next = XOR(next->npx, del);
                DEL_ACT(act_ref, p_act_ref, *n_act_ref, *act_ref);
                continue;
            }

            else if (curr == *end_ref)
            {
                DEL_EDGE(end_ref, beg_ref, *end_ref, act_ref);
                break;
            }
            else
            {
                curr = next;
                next = XOR(next->npx, del);
                prev->npx = XOR(p_prev, curr);
                curr->npx = XOR(prev, next);
                delete del;
                continue;
            }

        }
        p_prev = prev;
        prev = curr;
        curr = next;
    }

}

void PRINT(Node* pointer)
{
    if (pointer == NULL)
        cout << "NULL\n";
    else
    {
        Node* curr = pointer;
        Node* prev = NULL;
        Node* next;

        while (curr != NULL)
        {
            cout << curr->data << " ";

            next = XOR(prev, curr->npx);

            prev = curr;
            curr = next;
        }
        cout << '\n';
    }
}

int main()
{
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    Node* beg = NULL;
    Node* end = NULL;
    Node* act = NULL;
    Node* n_act = NULL;
    Node* p_act = NULL;
    char command[CHAR_SIZE];
    int N;
    while (cin >> command)
    {
        n_act = FIND(end, act);
        p_act = FIND(beg, act);

        if (!strcmp(command, "ACTUAL"))
        {
            if (act == NULL)
                cout << "NULL\n";
            else
                cout << act->data << '\n';
        }
        if (!strcmp(command, "NEXT"))
        {
            MOVE(&act, &n_act, &p_act, &n_act, &p_act, beg, end);
        }
        if (!strcmp(command, "PREV"))
        {
            MOVE(&act, &n_act, &p_act, &p_act, &n_act, end, beg);
        }
        if (!strcmp(command, "ADD_BEG"))
        {
            cin >> N;
            ADD_EDGE(&beg, &end, &act, N);
        }
        if (!strcmp(command, "ADD_END"))
        {
            cin >> N;
            ADD_EDGE(&end, &beg, &act, N);
        }
        if (!strcmp(command, "ADD_ACT"))
        {
            cin >> N;
            if (beg == act)
                ADD_EDGE(&beg, &end, &act, N);
            else
                ADD_ACT(beg, act, &p_act, N);
        }
        if (!strcmp(command, "DEL_BEG"))
        {
            DEL_EDGE(&beg, &end, beg, &act);
        }
        if (!strcmp(command, "DEL_END"))
        {
            if (act == end)
            {
                act = p_act;
            }
            DEL_EDGE(&end, &beg, end, &act);
        }
        if (!strcmp(command, "DEL_ACT"))
        {
            if (beg == act)
                DEL_EDGE(&beg, &end, beg, &act);
            else if (act == end)
            {
                act = p_act;
                DEL_EDGE(&end, &beg, end, &act);
            }
            else
                DEL_ACT(&act, &p_act, n_act, act);
        }
        if (!strcmp(command, "DEL_VAL"))
        {
            cin >> N;
            DEL_VAL(&beg, &end, &act, &p_act, &n_act, N);
        }
        if (!strcmp(command, "PRINT_FORWARD"))
        {
            PRINT(beg);
        }
        if (!strcmp(command, "PRINT_BACKWARD"))
        {
            PRINT(end);
        }

    }

    return 0;
}
