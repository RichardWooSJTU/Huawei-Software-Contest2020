#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <string>
using namespace std;
//思路：1.构建有向图，主要是邻接矩阵2.深度优先搜索，根节点返回后删除
class Node
{
public:
    Node(unsigned int data = 0);
    unsigned int m_uData;
    bool m_IsVisited;
};
Node::Node(unsigned int data)
{
    m_uData = data;
    m_IsVisited = false;
}


class Graph
{
public:
    vector<vector<unsigned int>> path;
    Graph(int capacity);
    int getNodeCnt();
    bool addNode(Node* pNode);
    void findNode(unsigned int data1, unsigned int data2, int& i1, int& i2);
    bool setMatrixValue(int row, int col, char val = 1);
    void printMatrix(int size);
    void depthFirstSearch(const int& nodeIndex);
    void disableNode(int nodeIndex);
    void findPath();
	~Graph();

private:
    int m_iNodeCnt;
    int m_iCapacity;
    vector<Node> m_NodeArray;
    char* m_Matrix;
    vector<unsigned int> p;
    int targetIndex;
    bool getValueOfMatrix(int row, int col, char& val);
};

Graph::Graph(int capacity)
{
    targetIndex = 0;
    m_iCapacity = capacity;
    m_iNodeCnt = 0;
    m_Matrix = new char[m_iCapacity * m_iCapacity];
    for (int i = 0; i < m_iCapacity * m_iCapacity; ++i)
    {
        m_Matrix[i] = 0;
    }
}

int Graph::getNodeCnt()
{
    return m_iNodeCnt;
}
bool Graph::getValueOfMatrix(int row, int col, char& val)
{
    if (row < 0 || row >= m_iCapacity) return false;
    if (col < 0 || col >= m_iCapacity) return false;
    val = m_Matrix[row + col * m_iCapacity];
    return true;
}
bool Graph::addNode(Node* pNode)//可优化
{
    if (pNode == NULL)
        return false;
    m_NodeArray.push_back(*pNode);
    m_iNodeCnt++;
    return true;
}
void Graph::findNode(unsigned int data1, unsigned int data2, int& i1, int &i2)
{
    bool flag1 = false, flag2 = false;
    for (int k = 0; k < m_iNodeCnt; ++k)
    {
        if (m_NodeArray[k].m_uData == data1)
        {
            i1 = k;
            flag1 = true;
        }
        if (m_NodeArray[k].m_uData == data2)
        {
            i2 = k;
            flag2 = true;
        }
    }
    if (!flag1) i1 = -1;
    if (!flag2) i2 = -1;
}
bool Graph::setMatrixValue(int row, int col, char val)
{
    if (row < 0 || row >= m_iCapacity) return false;
    if (col < 0 || col >= m_iCapacity) return false;
    m_Matrix[row + col * m_iCapacity] = val;
    return true;
}
void Graph::printMatrix(int size)
{
    for (int i = 0; i < size; i++)
    {
        for (int k = 0; k < size; k++)
        {
            cout << m_Matrix[i + m_iCapacity * k] << " ";
        }
        cout << endl;
    }
}
void Graph::depthFirstSearch(const int& nodeIndex)
{
    char value = 0;
    p.push_back(m_NodeArray[nodeIndex].m_uData);
    m_NodeArray[nodeIndex].m_IsVisited = true;
    //这里在深度为7的时候进行优化
    if (p.size() > 2)
    {
        //getValueOfMatrix(nodeIndex, targetIndex, value);
        value = m_Matrix[nodeIndex + targetIndex * m_iCapacity];
        if (p.size() != 7 && value == 0)
        {//什么都不做但是避免了判断
        }
        else if (p.size() == 7 && value == 1)
        {
            vector<unsigned int> tmp;
            unsigned int min = p[0];
            for (auto u : p)
            {
                if (u < min) min = u;
            }
            auto minIter = find(p.begin(), p.end(), min);
            for (int j = 0; j < p.size(); ++j)
            {
                tmp.push_back(*minIter);
                minIter++;
                if (minIter == p.end()) minIter = p.begin();
            }
            path.push_back(tmp);
            //cout << path.size() << endl;
            m_NodeArray[nodeIndex].m_IsVisited = false;
            p.pop_back();
            return;
        }
        else if (p.size() != 7 && value == 1)
        {
            vector<unsigned int> tmp;
            unsigned int min = p[0];
            for (auto u : p)
            {
                if (u < min) min = u;
            }
            auto minIter = find(p.begin(), p.end(), min);
            for (int j = 0; j < p.size(); ++j)
            {
                tmp.push_back(*minIter);
                minIter++;
                if (minIter == p.end()) minIter = p.begin();
            }
            path.push_back(tmp);
            //cout << path.size() << endl;
        }
        else if (p.size() == 7 && value == 0)
        {
            m_NodeArray[nodeIndex].m_IsVisited = false;
            p.pop_back();
            return;
        }
    }
    
    
    for (int i = 0; i < m_iNodeCnt ; ++i)
    {
        value = m_Matrix[nodeIndex + i * m_iCapacity];
        if (value == 1)
        {
            if (m_NodeArray[i].m_IsVisited == true)
                continue;
            else
            {
                depthFirstSearch(i);
            }
        }
    }

    m_NodeArray[nodeIndex].m_IsVisited = false;
    p.pop_back();
    return;
}
void Graph::disableNode(int nodeIndex)
{
    for (int col = nodeIndex; col < (m_iNodeCnt + 1); ++col)
        setMatrixValue(nodeIndex, col, 0);
    for (int raw = nodeIndex; raw < (m_iNodeCnt + 1); ++raw)
        setMatrixValue(raw, nodeIndex, 0);
}

void Graph::findPath()
{
    for (int i = 0; i < m_iNodeCnt; ++i)
    {
        targetIndex = i;
        depthFirstSearch(i);
        disableNode(i);
    }
}
Graph::~Graph()
{
    delete[]m_Matrix;
}

bool sorFun(const vector<unsigned int>& p1, const vector<unsigned int>& p2)
{
    if (p1.size() < p2.size()) return true;
    else if (p1.size() == p2.size())
    {
        if (p1[0] < p2[0]) return true;
        else if (p1[0] == p2[0])
        {
            if (p1[1] < p2[1]) return true;
            else if (p1[1] == p2[1])
            {
                if (p1[2] < p2[2]) return true;
            }
        }
    }
    return false;
}

int main()
{
    Graph G(50000);
	ifstream file("/data/test_data.txt");
    //ifstream file("test_data.txt");
    string str;
    int cnt = 0;
    //cout << "File opened, start to load data" << endl;
    while (getline(file,str)) //把每一行输入连成图
    {
        
        int dot = str.find(',');
        string n1 = str.substr(0,dot);
        str.erase(0,dot + 1);
    
        dot = str.find(',');
        string n2 = str.substr(0,dot);
        /*

        istringstream readStr(str);
        string n1, n2;
        getline(readStr, n1, ',');
        getline(readStr, n2, ',');
        */

        int i1, i2;
        unsigned int a = atoi(n1.c_str());
        unsigned int b = atoi(n2.c_str());
        
        Node aNode(a), bNode(b);
        //if (G.getNodeCnt() == 0) G.addNode(&aNode);
        G.findNode(a, b, i1, i2);
        if (i1 == -1 && i2 == -1)
        {
            i1 = G.getNodeCnt();
            G.addNode(&aNode);          
            G.addNode(&bNode);
            i2 = i1 + 1;
        }
        else if (i1 != -1 && i2 == -1)
        {
            i2 = G.getNodeCnt();
            G.addNode(&bNode);
        }
        else if (i1 == -1 && i2 != -1)
        {
            i1 = G.getNodeCnt();
            G.addNode(&aNode);
        }
        if (!G.setMatrixValue(i1, i2)) return 0;
        if (cnt % 100 == 0)
            cout << "loaded " << cnt << endl;
	++cnt;
    }
    file.close();
    //cout << "Start to search" << endl;
    G.findPath();

    std::sort(G.path.begin(), G.path.end(), sorFun);//按照输出规则排序

    ofstream output("/projects/student/result.txt");
    output << G.path.size() << endl;
    for (int j = 0; j < G.path.size(); ++j)
    {
        auto it = G.path[j].begin();
        while (it != G.path[j].end())
        {
            output << *it;
            ++it;
            if (it != G.path[j].end())
                output << ',';
        }
        output << endl;
    }
    /*
    cout << G.path.size() << endl;
    for (int j = 0; j < G.path.size(); ++j)
    {
        auto it = G.path[j].begin();
        while (it != G.path[j].end())
        {
            cout << *it;
            ++it;
            
        }
        cout << endl;
    }
    */
    output.close();
    return 0;
}
