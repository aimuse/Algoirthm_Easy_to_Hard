#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
#include <Windows.h>
#include "hashTable.h"
#include "rbtree.h"
using namespace std;

// microsecond timer
class MyTimer {

private:
    LARGE_INTEGER large_integer;
    __int64 IntStart;
    __int64 IntEnd;
    double DobDff;
    double DobMillseconds;

public:
    MyTimer() {};

    void TimerStart() {
        QueryPerformanceFrequency(&large_integer);
        DobDff = large_integer.QuadPart;

        QueryPerformanceCounter(&large_integer);
        IntStart = large_integer.QuadPart;
    }

    double TimerFinish() {
        QueryPerformanceCounter(&large_integer);
        IntEnd = large_integer.QuadPart;
        DobMillseconds = (IntEnd - IntStart) * 1000000 / DobDff; //
        return DobMillseconds;
    }
};

class MyData
{
public:
    void ReadData(string filename);
    void SaveData();
    void PrintData(vector<vector<int>> num);
    void PrintDataColumn(vector<vector<int>> num);
    void Change(vector<vector<int>>& a, vector<vector<int>>& b);
    void InitialHash();
    void CopyNum();
    void CreateRBT();
    void Search(int x);
    void Update(int x, int y, int newNum);

    vector<vector<int>> num;
    vector<vector<int>> numLie;

    RBTree* tree = new RBTree();

    HashTable hash;
};

void Heap_build(vector<vector<int>>& a, int root, int length, int location);
void Heap_sort(vector<vector<int>>& a, int location);

void printMenu()
{
    cout << ">>> Menu" << endl;
    cout << ">>> 1.read txt" << endl;
    cout << ">>> 2.save txt" << endl;
    cout << ">>> 3.Output data as a two-dimensional matrix" << endl;
    cout << ">>> 4.Output data as a red-black tree" << endl;
    cout << ">>> 5.Output data as a hashtable" << endl;
    cout << ">>> 6.Use Red-Black Tree to find maximum and minimum" << endl;
    cout << ">>> 7.Use Red-Black Tree to modify datasheet" << endl;
    cout << ">>> 8.Use Red-Black Tree to search datasheet for the input value" << endl;
    cout << ">>> 9.Use Hashtable to search datasheet for the input value" << endl;
    cout << ">>> 10.Use heap to sort rows in ascending order" << endl;
    cout << ">>> 11.Use heap to sort column in ascending order " << endl;
    cout << ">>> 0.Exit " << endl;
    return;
}

int main()
{
    MyData res;
    bool exit = false;

    string filename;
    cout << "Please input the filename without \".txt\"" << endl;
    cin >> filename;

    printMenu();

    while (exit == false)
    {
        cout << endl << endl << ">>> Please enter what do you want to do:";
        int ch;
        cin >> ch;
        switch (ch)
        {
        case 1:
        {
            res.ReadData(filename);
            break;
        }
        case 2:
        {
            res.SaveData();
            break;
        }
        case 3:
        {
            res.PrintData(res.num);
            break;
        }
        case 4:
        {
            res.tree->print();
            break;
        }
        case 5:
        {
            res.hash.print();
            break;
        }
        case 6:
        {
            res.tree->maximum();
            res.tree->minimum();
            break;
        }
        case 7: // function1 : using the red black tree to modify the target row/col number
        {
            int row, col, newNum;
            cout << "Please enter the row and column where the data will be modified: ";
            cin >> row >> col;
            cout << "Please enter the new data: ";
            cin >> newNum;

            MyTimer timer;
            timer.TimerStart();
            for (int i = 0; i < res.num[0].size(); i++) {
                res.Update(res.num[0].size() / 2, i, newNum);
            }
            for (int i = 0; i < res.num[0].size(); i++) {
                res.Update(i, res.num[0].size() / 2, newNum);
            }

            double tm = timer.TimerFinish();


            cout << "Successfully modified! " << endl << endl;

            //                res.PrintData(res.num);
            cout << tm / (2 * res.num[0].size()) << " microseconds" << std::endl;
            break;
        }
        case 8: // use red-black tree to search the target number
        {
            cout << "Please enter the data you want search by Red-Black Tree:";
            int value;
            cin >> value;
            cout << value << " is at:  " << endl;
            res.Search(value);
            break;
        }
        case 9: // function2: use the hash table to search the wanted number
        {
            cout << "Please enter the data you want search by Hashtable:";
            int value;
            cin >> value;
            cout << value << " is at:  " << endl;
            MyTimer timer;
            timer.TimerStart();
            res.hash.find(value);
            double tm = timer.TimerFinish();
            cout << tm / (2 * res.num[0].size()) << " mircroseconds " << std::endl;
            break;
        }
        case 10: // function 3: use the heap sort to sort the target row in the asecending roder
        {
            cout << "Please enter the ranking row:";
            int ch;
            cin >> ch;
            MyTimer timer;
            timer.TimerStart();
            Heap_sort(res.numLie, ch);
            res.Change(res.num, res.numLie);
            double tm = timer.TimerFinish();
            cout << tm / (2 * res.num[0].size()) << " mircroseconds " << std::endl;
            //res.PrintDataColumn(res.numLie);
            break;
        }
        case 11:
        {
            cout << "Please enter the ranking column:";
            int ch;
            cin >> ch;
            Heap_sort(res.num, ch);
            res.Change(res.numLie, res.num);
            res.PrintData(res.num);
            break;
        }
        case 0:
        {
            res.tree->destroy(); // Radically destroy the RB tree
            delete res.tree;
            res.tree = NULL;
            exit = true;
            cout << "Exit successfully!";
            break;
        }
        default:
            printMenu();
        }
    }
}


void MyData::Change(vector<vector<int>>& a, vector<vector<int>>& b)
{
    for (int i = 0; i < a.size(); i++)
        for (int j = 0; j < b.size(); j++)
            a[i][j] = b[j][i];
}

void Heap_build(vector<vector<int>>& a, int root, int length, int location)
{
    int lchild = root * 2 + 1;
    if (lchild < length)
    {
        int flag = lchild;
        int rchild = lchild + 1;
        if (rchild < length)
        {
            if (a[rchild][location] > a[flag][location])
            {
                flag = rchild;
            }
        }
        if (a[root][location] < a[flag][location])
        {

            int t = a[root][location];
            a[root][location] = a[flag][location];
            a[flag][location] = t;

            Heap_build(a, flag, length, location);
        }
    }
}
void Heap_sort(vector<vector<int>>& a, int location)
{
    int len = a.size();
    for (int i = len / 2; i >= 0; --i)
    {
        Heap_build(a, i, len, location);
    }
    for (int j = len - 1; j > 0; --j)
    {

        int t = a[0][location];
        a[0][location] = a[j][location];
        a[j][location] = t;
        Heap_build(a, 0, j, location);
    }
}

void MyData::CopyNum()
{
    numLie = num;
    for (int i = 0; i < num.size(); i++)
        for (int j = 0; j < num.size(); j++)
        {
            numLie[j][i] = num[i][j];
        }
}

void MyData::ReadData(string filename)
{
    ifstream fin;
    fin.open(filename + ".txt");

    int row, col;
    fin >> row;
    fin >> col;

    hash.init(row * col);

    if (row == NULL)
    {
        printf("Data is wrong!\n");
        return;
    }

    vector<int> temp(col);
    num.resize(row, temp);

    for (int i = 0; i < row; i++)
    {
        vector<int> temp;
        for (int j = 0; j < col; j++)
        {
            int n;
            fin >> n;
            num[i][j] = n; // insert into vector
            tree->insert(n, i, j); // insert into RBtree
            hash.insert(i, j, n); // insert into hashTable
        }
    }

    CopyNum();
    fin.close();

    // PrintData(num);
    cout << endl << "Data spreadsheet(" << row << "x" << col << ") has been successfully loaded!";
}

void MyData::SaveData()
{
    ofstream fout;
    fout.open("NewDatasheet.txt");
    fout << num.size() << " " << num[0].size() << endl;
    for (int i = 0; i < num.size(); i++)
    {
        for (int j = 0; j < num[0].size(); j++)
        {
            fout << setw(2) << left << num[i][j] << " ";
        }
        fout << endl;
    }
    fout.close();
    cout << "Saved successfully at NewDatasheet.txt !" << endl;
}

// print the data on the interfeace(do not use this for large n)
void MyData::PrintData(vector<vector<int>> num)
{
    for (int i = 0; i < num.size(); i++)
    {
        for (int j = 0; j < num[0].size(); j++)
        {
            cout << setw(2) << left << num[i][j] << " ";
        }
        cout << endl;
    }
}

void MyData::PrintDataColumn(vector<vector<int>> numLie)
{
    for (int i = 0; i < num.size(); i++)
    {
        for (int j = 0; j < num[0].size(); j++)
        {
            cout << setw(2) << left << numLie[j][i] << " ";
        }
        cout << endl;
    }
}


void MyData::Search(int x)
{
    tree->iterativeSearch(x);
}

void MyData::Update(int x, int y, int newNum)
{
    // update the red black tree
    tree->remove(x, y, num[x][y]);
    tree->insert(newNum, x, y);
    // update the vector (matrix)
    num[x][y] = newNum;
}