#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Node{
    public:
    int val = -1;
    int height = 0;
    Node* left = nullptr;
    Node* right = nullptr;
};

void internal_inc(string* inc, Node *pt)
{
    if (pt->left != nullptr) internal_inc(inc, pt->left);
    *inc += to_string(pt->val) + " ";
    if (pt->right != nullptr) internal_inc(inc, pt->right);
    return;
}
/* DECISION
0 - Sem rotação necessária
1 - Rotação simples à esquerda (os bf's de A e B são positivos)
2 - Rotação simples à direita (os bf's de A e B são negativos)
3 - Rotação direita-esquerda (o bf de A é negativo e o de B é positivo)
4 - Rotação esquerda-direita (o bf de A é positivo e o de B é negativo)
*/
int bf(Node* pt)
{
    int left = 0, right = 0, bf;
    if (pt->left != nullptr) left = (pt->left)->height;
    if (pt->right != nullptr) right = (pt->right)->height;
    bf = right - left; //for usual reasons, right is positive while left is negative
    return bf;
}

int MAX(Node* p, Node* q)
{
    int x, y;

    if (p == nullptr) x = 0;
    else x = p->height;
    if (q == nullptr) y = 0;
    else y = q->height;
    return max(x, y);
}

Node* rotation_left(Node* a)
{
    Node* b = a->right;
    a->right = b->left;
    b->left = a;
    a->height = 1 + MAX(a->left, a->right);
    b->height = 1 + MAX(b->left, b->right);
    return b;
}

Node* rotation_right(Node* a)
{
    Node* b = a->left;
    a->left = b->right;
    b->right = a;
    a->height = 1 + MAX(a->left, a->right);
    b->height = 1 + MAX(b->left, b->right);
    return b;
}

Node* rotation_left_right(Node *a)
{
    Node* b = a->left;
    Node* c = b->right;

    a->left = c;
    b->right = c->left;
    c->left = b;
    a->height = 1 + MAX(a->left, a->right);
    b->height = 1 + MAX(b->left, b->right);
    c->height = 1 + MAX(c->left, c->right);
    return rotation_right(a);

}

Node* rotation_right_left(Node *a)
{
    Node* b = a->right;
    Node* c = b->left;

    a->right = c;
    b->left = c->right;
    c->right = b;
    a->height = 1 + MAX(a->left, a->right);
    b->height = 1 + MAX(b->left, b->right);
    c->height = 1 + MAX(c->left, c->right);
    return rotation_left(a);
}

class AVL{
    public:
    Node root;
    int height;
    int qty = 0;

    bool insert(Node* node)
    {
        if (root.val == -1)
        {
            root.val = node->val;
            root.height = 1;
            node->height = 1;
            height = 1;
            return true;
        }

        else
        {
            //cout << "Entrou no else" << endl;
            Node* pt = &root;
            vector<Node *> pile;

            while (true)
            {
                //cout << "Entrou no while " << endl;
                //cout << pt->val << endl;
                pile.push_back(pt);

                if (node->val < pt->val)
                {
                    if (pt->left == nullptr)
                    {
                        //node->height = h;
                        pt->left = node; 
                        break;
                    }
                    else pt = pt->left;
                }    

                else if (node->val > pt->val) 
                {
                    if (pt->right == nullptr)
                    {
                        //cout << "Ponteiro direito eh nulo " << endl;
                        //node->height = h;
                        pt->right = node;
                        break;
                    }

                    else 
                    {
                        pt = pt->right; 
                        //cout << "Ponteiro direito existe com valor " << pt->val << endl;
                    }
                    //cout << "Nodulo direito: " << (pt->right)->val << endl;                   
                }
                else return false;
            }

            qty++;
            pile.push_back(node);
            int calc = 1, pilesize = pile.size() - 1;
            Node* pt2; 

            while (pilesize >= 0)
            {
                pt2 = pile[pilesize];
                pt2->height = calc;
                pile.pop_back();
                pilesize--;
                calc++;

                int factor = bf(pt2);
                cout << "Fator de " << pt2->val << ": " << factor << endl;

                if (factor < -1)
                {
                    calc--;
                    int sonfactor = bf(pt2->left);
                    Node* track = pile[pilesize];
                    Node* temp;

                    if (sonfactor < 0)
                    {
                        temp = rotation_right(pt2);
                    }

                    else if (sonfactor > 0)
                    {
                        temp = rotation_left_right(pt2);
                        cout << "Rotacao left right realizada" << endl;
                    }

                    if (pilesize < 0)
                    {
                        cout << "Track é nulo" << endl;
                        break;
                    } 

                    else
                    {
                        if (pt2 == track->left)
                        {
                            track->left = temp;
                        }

                        else
                        {
                            track->right = temp;
                        }
                    }   
                }

                else if (factor > 1)
                {
                    calc--;
                    int sonfactor = bf(pt2->right);
                    Node* track = pile[pilesize];
                    Node* temp;

                    if (sonfactor < 0)
                    {
                       temp = rotation_right_left(pt2);
                    }

                    else if (sonfactor > 0)
                    {
                        temp = rotation_left(pt2);
                    }

                    if (pt2 == track->left)
                    {
                        track->left = temp;
                    }

                    else
                    {
                        track->right = temp;
                    }
                }
            }
            //cout << "Nodulo salvo com valor " << (pt->right)->val << " na altura " << (pt->right)->height << endl;
            //cout << "Root.right: " << (*root.right).val << endl;
            //cout << "Root.val: " << root.val << endl;
            return true;
        }
    }

    int search(int val)
    {
        Node* pt = &root;

        while (true)
        {
            if (val < pt->val)
            {
                if (pt->left == nullptr)
                {
                    break;
                }
                else pt = pt->left;
            }    

            else if (val > pt->val) 
            {
                if (pt->right == nullptr)
                {
                    break;
                }

                else 
                {
                    pt = pt->right; 
                }                
            }

            else
            {
                return pt->height;
            }
        }
        return -1;
    }

    string inc_order()
    {
        string inc = "";
        Node *pt = &root;
        internal_inc(&inc, &root);
        return inc;
    }
};

int main()
{
    AVL tree;
    int n, i = 0;
    vector<Node> x;

    while (true)
    {
        do
        {
            cout << "1 - Inserir numero\n2 - Procurar numero\n3 - Ordem crescente\n4 - Encerrar Programa" << endl;
            cin >> n;
        }  while (n < 1 || n > 4);

        if (n == 4) break;

        if (n == 1)
        {
            cout << "Insira um numero: ";
            int b;
            cin >> b;
            Node y;
            y.val = b;
            x.push_back(y);
            if (tree.insert(&x[i]))
            {
                cout << "Nodulo de valor " << x[i].val << " salvo com altura " << x[i].height << endl;
                i++;
            }
            
            else 
            {
                cout << "Valor ja salvo" << endl;
                x.pop_back();
            }
        }

        else if (n == 2)
        {
            cout << "Numero a ser procurado: ";
            int b;
            cin >> b;
            int x = tree.search(b);
            if (x == -1) cout << "Numero nao encontrado" << endl;
            else cout << "Numero encontrado na altura " << x << endl;
        }

        else if (n == 3)
        {
            string f = tree.inc_order();
            cout << f << endl;
        }
    }
}