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

void ADD_EDGE(Node** fpointer, Node** spointer, int data)
{
    Node* new_node = new Node();
    new_node->data = data;


    new_node->npx = *fpointer;


    if (*fpointer != NULL)
        (*fpointer)->npx = XOR(new_node, (*fpointer)->npx);
    else
        *spointer = new_node;

    *fpointer = new_node;

}

void DEL_EDGE(Node** fpointer, Node** spointer, Node* del)
{
    if (*fpointer == *spointer)
    {
        *fpointer = NULL;
        *spointer = NULL;

    }
    else
    {
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

void PUSH(Node** beg, Node* end, Node** back, Node** front, int data)
{
    Node* curr = *beg;
    Node* prev = NULL;
    Node* next;

    if (*beg == end && *front == NULL)
    {
        *back = end;
        *front = end;
        (*back)->data = data;
        return;
    }
    else if (*back == *beg && end != *front)
    {
        *back = end;
        (*back)->data = data;
        return;
    }
    else if (*back != NULL && *front == NULL)
    {
        *front = *back;
        (*front)->data = data;
        return;
    }
    while (curr != *back)
    {
        next = XOR(prev, curr->npx);
        if (next == *back && curr == *front)
        {
            Node* new_node = new Node();
            new_node->data = data;
            new_node->npx = XOR(curr, next);
            (*front)->npx = XOR(XOR(next, (*front)->npx), new_node);
            (*back)->npx = XOR(XOR(curr, (*back)->npx), new_node);
            *back = new_node;
            return;
        }
        prev = curr;
        curr = next;
    }
    if (curr == *back)
    {
        prev->data = data;
        *back = prev;
    }
    if (*back == end)
        *front = end;
}

void POP(Node** front, Node** back, Node* end)
{
    if (*front == NULL)
        cout << "NULL" << '\n';
    else if (*front == *back)
    {
        cout << (*front)->data << '\n';
        *front = NULL;
    }
    else
    {
        Node* curr = end;
        Node* prev = NULL;
        Node* next;

        while (curr != *front)
        {

            next = XOR(prev, curr->npx);

            prev = curr;
            curr = next;
        }
        next = XOR(prev, curr->npx);
        cout << (*front)->data << '\n';
        if (next == NULL)
            *front = end;
        else
            *front = next;
    }
}

int DATA(Node* fpointer, Node* spointer, Node* start, bool print)
{
    if (fpointer == NULL)
    {
        if(print)
            cout << "NULL\n";
        return 0;
    }
    else
    {
        Node* curr = start;
        Node* prev = NULL;
        Node* next;
        int count = 1;

        while (curr != fpointer)
        {

            next = XOR(prev, curr->npx);

            prev = curr;
            curr = next;
        }
        while (curr != spointer)
        {
            if (print)
                cout << curr->data << " ";
            count += 1;

            next = XOR(prev, curr->npx);
            if (next == NULL)
            {
                next = start;
                curr = start;
                prev = NULL;
            }
            else
            {
                prev = curr;
                curr = next;
            }
        }
        if (print)
            cout << curr->data << '\n';
        return count;
    }
}

void GARBAGE_SOFT(Node* beg, Node* front, Node* back)
{
    Node* curr = beg;
    Node* prev = NULL;
    Node* next;
    if (front == NULL)
    {
        while (curr != NULL)
        {
            next = XOR(prev, curr->npx);
            curr->data = 0;
            prev = curr;
            curr = next;
        }
    }
    else if (back == front)
    {
        while (curr != NULL)
        {
            next = XOR(prev, curr->npx);
            if (curr != back)
                curr->data = 0;
            prev = curr;
            curr = next;
        }
    }
    else
    {
        while (curr != front)
        {
            next = XOR(prev, curr->npx);

            prev = curr;
            curr = next;

        }
        while (curr != back)
        {
            next = XOR(prev, curr->npx);
            if (next == NULL)
            {
                next = beg;
                curr = beg;
                prev = NULL;
                if (next != back)
                    next->data = 0;
                else
                    return;
            }
            else if (next == back)
                return;
            else
            {
                next->data = 0;
                prev = curr;
                curr = next;
            }
        }
    }
}

void GARBAGE_HARD(Node** beg, Node** end, Node* back, Node* front)
{
    Node* curr = *end;
    Node* prev = NULL;
    Node* next;

    if (front == NULL)
    {
        *beg = NULL;
        *end = NULL;
        next = XOR(prev, curr->npx);
        while (next != NULL)
        {
            Node* del = curr;
            curr = next;
            next = XOR(next->npx, del);
            curr->npx = XOR(next, prev);
            delete del;
        }
        delete curr;
    }
    else if(*beg!=back || *end!=front)
    {
        while (curr != back)
        {

            next = XOR(prev, curr->npx);

            prev = curr;
            curr = next;
        }
        next = XOR(prev, curr->npx);
        while (next != front)
        {
            if (next == NULL && *beg == front)
            {
                next = *end;
                prev = NULL;
                curr = prev;
            }
            Node* del = next;
            next = XOR(next->npx, curr);
            if (next == NULL)
            {
                *beg = back;
                back->npx = XOR(NULL, prev);
                prev = NULL;
                curr = prev;
                next = *end;
                delete del;
                continue;
            }
            curr = next;
            next = XOR(curr->npx, del);
            if (prev != NULL)
            {
                back->npx = XOR(curr, prev);
                curr->npx = XOR(next, back);
            }
            else
            {
                curr->npx = XOR(next, prev);
            }
            if (*beg == back)
                *end = curr;
            delete del;
            next = curr;
            if (prev == NULL)
                curr = prev;
            if (prev != NULL)
                curr = back;
        }
    }
}

int main()
{
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    Node* beg = NULL;
    Node* end = NULL;
    Node* front = NULL;
    Node* back = NULL;
    char command[CHAR_SIZE];
    int N;
    while (cin >> command)
    {
        if (!strcmp(command, "ADD_BEG"))
        {
            cin >> N;
            ADD_EDGE(&beg, &end, N);
        }
        if (!strcmp(command, "ADD_END"))
        {
            cin >> N;
            ADD_EDGE(&end, &beg, N);
        }
        if (!strcmp(command, "DEL_BEG"))
        {
            if (beg == end && front != NULL)
            {
                front = NULL;
                back = NULL;
            }
            if (back == front && beg != end)
            {
                front = NULL;
                back = NULL;
            }
            if (back == beg)
                back = XOR(NULL, beg->npx);
            if (front == beg)
                front = end;
            DEL_EDGE(&beg, &end, beg);
        }
        if (!strcmp(command, "DEL_END"))
        {
            if (beg == end && front != NULL)
            {
                front = NULL;
                back = NULL;
            }
            if (back == front && beg != end)
            {
                front = NULL;
                back = NULL;
            }
            if (front == end)
                front = XOR(NULL, end->npx);
            if (back == end)
                back = beg;
            DEL_EDGE(&end, &beg, end);
        }
        if (!strcmp(command, "PRINT_FORWARD"))
        {
            DATA(beg, end, beg, 1);
        }
        if (!strcmp(command, "PRINT_BACKWARD"))
        {
            DATA(end, beg, end, 1);
        }
        if (!strcmp(command, "SIZE"))
        {
            cout << DATA(end, beg, end, 0) << '\n';
        }
        if (!strcmp(command, "PUSH"))
        {
            cin >> N;
            if (beg == NULL)
            {
                ADD_EDGE(&beg, &end, N);
                front = beg;
                back = beg;
            }
            else if (beg == back && end == front)
            {
                ADD_EDGE(&beg, &end, N);
                back = beg;
            }
            else
                PUSH(&beg, end, &back, &front, N);
        }
        if (!strcmp(command, "POP"))
        {
            POP(&front, &back, end);
        }
        if (!strcmp(command, "PRINT_QUEUE"))
        {
            DATA(front, back, end, 1);
        }
        if (!strcmp(command, "COUNT"))
        {
            cout << DATA(front, back, end, 0) << '\n';
        }
        if (!strcmp(command, "GARBAGE_SOFT"))
        {
            GARBAGE_SOFT(beg, front, back);
        }
        if (!strcmp(command, "GARBAGE_HARD"))
        {
            if (beg == end && front == NULL)
            {
                DEL_EDGE(&beg, &end, beg);
            }
            else
                GARBAGE_HARD(&beg, &end, back, front);
        }

    }
    return 0;
}
