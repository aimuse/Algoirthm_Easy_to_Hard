#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
#include <map>
using namespace std;


class Point
{
public:
    Point(int m, int n)
    {
        x = m;
        y = n;
    }
    bool operator<(const Point& a) const
    {
        if (this->x + this->y < a.x + a.y)
            return true;
        return false;
    }
    int x;
    int y;
};
class MyData
{
public:

    void ReadData();
    void SaveData();
    void PrintData(vector<vector<int> > num);
    void PrintDataLie(vector<vector<int> > num);


    void CopyNum(); // Convert rows and columns vector

    void CreateMap();  //Initialize the Red-Black Tree Map

    int Search(int x, int y);              //Use Hashing to search two-dimensional data tables
    void Update(int x, int y, int newNum); //Use red-black tree to update two-dimensional data table

    vector<vector<int> > num;    //A vector inside is a row
    vector<vector<int> > numLie; //A vector inside is a column
    vector<int> hash;           //hashing search 
    map<Point, int> mapNum;     //red-black tree update 
};

	void Heap_build(vector<vector<int> >& a, int root, int length, int location); //first screening process
	void Heap_sort(vector<vector<int> >& a, int location);	

int main()
{
    MyData res;

    cout << "1.read txt" << endl;
    cout << "2.save txt" << endl;
    cout << "3.Output two-dimensional data table" << endl;
    cout << "4.Use the red-black tree to modify the two-dimensional data table" << endl;
    cout << "5.Use hashing to search two-dimensional data tables" << endl;
    cout << "6.Use heap to sort rows in ascending order" << endl;
    cout << "7.Use heap to sort column in ascending order " << endl;

    while (true)
    {
        cout << endl
            << "Please enter what do you want to do:";
        int ch;
        cin >> ch;
        switch (ch)
        {
        case 1:
            res.ReadData();
            break;
        case 2:
            res.SaveData();
            break;
        case 3:
            res.PrintData(res.num);
            break;
        case 4:
        {
            cout << "Please enter the row and column where the data is modified:";
            int row, col;
            cin >> row >> col;
            cout << "Please enter the revised data:";
            int newNum;
            cin >> newNum;
            res.Update(row - 1, col - 1, newNum);
            cout << "Successfully modified! "<< endl;
        }
        break;
        case 5:
        {
            cout << "Please enter the row and column of the  data you want search:";
            int row, col;
            cin >> row >> col;
            cout << "The data you want to find is:" << res.Search(row - 1, col - 1) << endl;
        }
        break;

        case 6:
        {
            cout << "Please enter the ranking row:";
            int ch;
            cin >> ch;
            vector<vector<int> > temp = res.numLie;
            Heap_sort(temp, ch - 1);
            res.PrintDataLie(temp);
            break;
        }
        case 7:
        {
            cout << "Please enter the ranking column:";
            int ch;
            cin >> ch;
            vector<vector<int> > temp = res.num;
            Heap_sort(temp, ch - 1);
            res.PrintData(temp);
            break;
        }
        }
    }
    return 0;
}

void Heap_build(vector<vector<int> >& a, int root, int length, int location) //first screening process
{
    int lchild = root * 2 + 1; // left child node of the root node
    if (lchild < length)       //The subscript of the left child node cannot exceed the length of the array
    {
        int flag = lchild;       //flag saves the subscript of the maximum value in the left and right nodes
        int rchild = lchild + 1; //The subscript of the right child node of the root node
        if (rchild < length)     //The subscript of the right child node cannot exceed the length of the array (if any)
        {
            if (a[rchild][location] > a[flag][location]) //Find the maximum value of the left and right child nodes
            {
                flag = rchild;
            }
        }
        if (a[root][location] < a[flag][location])
        {
            //Exchange the parent node and the largest child node larger than the parent node
            vector<int> t = a[root];
            a[root] = a[flag];
            a[flag] = t;
            //Recursively build a heap from the position of the largest child node 
            Heap_build(a, flag, length, location);
        }
    }
}
void Heap_sort(vector<vector<int> >& a, int location)
{
    int len = a.size();
    for (int i = len / 2; i >= 0; --i) //Build a heap from the parent node of the last non-leaf node
    {
        Heap_build(a, i, len, location);
    }
    for (int j = len - 1; j > 0; --j) //j represents the length of the array at this time, because the length of len has been built, starting from len-1
    {
        //swap(a[0], a[j]);    //Swap the first and last elements, and swap the maximum value to the last position of the array to save
        vector<int> t = a[0];
        a[0] = a[j];
        a[j] = t;
        Heap_build(a, 0, j, location); //Remove the element at the last position to rebuild the heap, where j represents the length of the array, and the index at the last position becomes len-2
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

void MyData::ReadData()
{
    ifstream fin;
    fin.open("data.txt");
    int row, col;
    fin >> row >> col;

    for (int i = 0; i < row; i++)
    {
        vector<int> temp;
        for (int j = 0; j < col; j++)
        {
            int n;
            fin >> n;
            temp.push_back(n);
        }
        num.push_back(temp);
    }
    CreateMap();
    CopyNum();
    fin.close();
}

void MyData::SaveData()
{
    ofstream outfile("output.txt");
    outfile << num.size() << " " << num[0].size() << endl;
    for (int i = 0; i < num.size(); i++)
    {
        for (int j = 0; j < num[0].size(); j++)
        {
            outfile << left << setw(2) << num[i][j] << " ";
        }
        outfile << endl;
    }
    outfile.close();
    cout << "save successfully ! " << endl;
}

void MyData::PrintData(vector<vector<int> > num)
{
    for (int i = 0; i < num.size(); i++)
    {
        for (int j = 0; j < num[0].size(); j++)
        {
            cout << left << setw(2) << num[i][j] << " ";
        }
        cout << endl;
    }
}

void MyData::PrintDataLie(vector<vector<int> > numLie)
{
    for (int i = 0; i < num.size(); i++)
    {
        for (int j = 0; j < num[0].size(); j++)
        {
            cout << left << setw(2) << numLie[j][i] << " ";
        }
        cout << endl;
    }
}

// only for the consistent of the program, have not finished the hashing part yet. 
int MyData::Search(int x, int y)
{
     return num[x][y];
}

void MyData::Update(int x, int y, int newNum)
{
    Point temp(x, y);
    mapNum.at(temp) = newNum; 
    num[x][y] = newNum;       
    CopyNum();
}

void MyData::CreateMap()
{
    for (int i = 0; i < num.size(); i++)
        for (int j = 0; j < num[0].size(); j++)
        {
            Point temp(i, j);
            mapNum.insert(pair<Point, int>(temp, num[i][j]));
        }
}
