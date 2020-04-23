#include <iostream>
#include <fstream>
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
	Graph();
    int getNodeCnt();
    bool addNode(Node pNode);
    int findNode(unsigned int data);
    bool setMatrixValue(int row, int col, int val = 1);
    void printMatrix(int size);
    void depthFirstSearch(int nodeIndex);
    void disableNode(int nodeIndex);
    void findPath();
	~Graph();

private:
    int m_iNodeCnt;
    vector<Node> m_NodeArray;
    vector<vector<int>> m_Matrix;
    vector<unsigned int> p;
    
    bool getValueOfMatrix(int row, int col, int& val);
};

Graph::Graph()
{
    m_iNodeCnt = 0;
    vector<int> v1;
    v1.push_back(0);
    m_Matrix.push_back(v1);
    
}

int Graph::getNodeCnt()
{
    return m_iNodeCnt;
}
bool Graph::getValueOfMatrix(int row, int col, int& val)
{
    if (row < 0 || row >= (m_iNodeCnt + 1)) return false;
    if (col < 0 || col >= (m_iNodeCnt + 1)) return false;
    val = m_Matrix[row][col];
    return true;
}
bool Graph::addNode(Node pNode)//可优化
{
    if (&pNode == NULL)
        return false;
    m_NodeArray.push_back(pNode);
    m_iNodeCnt++;
    vector<int> newV(m_iNodeCnt);
    m_Matrix.push_back(newV);
    for (auto &v : m_Matrix)
        v.push_back(0);

    return true;
}
int Graph::findNode(unsigned int data)//可优化
{
    for (int k = 0; k < m_iNodeCnt; ++k)
    {
        if (m_NodeArray[k].m_uData == data) return k;
    }
    return -1;
}
bool Graph::setMatrixValue(int row, int col, int val)
{
    if (row < 0 || row >= (m_iNodeCnt + 1)) return false;
    if (col < 0 || col >= (m_iNodeCnt + 1)) return false;
    m_Matrix[row][col] = val;
    return true;
}
void Graph::printMatrix(int size)
{
    for (int i = 0; i < size; i++)
    {
        for (int k = 0; k < size; k++)
        {
            cout << m_Matrix[i][k] << " ";
        }
        cout << endl;
    }
}
void Graph::depthFirstSearch(int nodeIndex)
{
    int value = 0;
    p.push_back(m_NodeArray[nodeIndex].m_uData);
    if (p.size() >= 8)
    {
        p.pop_back();
        return;
    }
    m_NodeArray[nodeIndex].m_IsVisited = true;
    for (int i = 0; i < (m_iNodeCnt + 1); ++i)
    {
        getValueOfMatrix(nodeIndex, i, value);
        if (value == 1)
        {
            if (m_NodeArray[i].m_uData == p[0] && p.size() > 2)
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
                continue;
            }
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
    for (int col = 0; col < (m_iNodeCnt + 1); ++col)
        setMatrixValue(nodeIndex, col, 0);
    for (int raw = 0; raw < (m_iNodeCnt + 1); ++raw)
        setMatrixValue(raw, nodeIndex, 0);
}

void Graph::findPath()
{
    for (int i = 0; i < m_iNodeCnt; ++i)
    {
        depthFirstSearch(i);
        disableNode(i);
    }
}
Graph::~Graph()
{
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
    Graph G;
	ifstream file("test_data.txt");
    string str;
    while (getline(file,str)) //把每一行输入连成图
    {
        int dot = str.find(',');
        string sa = str.substr(0,dot);
        str.erase(0,dot + 1);
    
        dot = str.find(',');
        string sb = str.substr(0,dot);

        int i1, i2;
        unsigned int a = atoi(sa.c_str());
        unsigned int b = atoi(sb.c_str());
        
        Node aNode(a), bNode(b);
        //if (G.getNodeCnt() == 0) G.addNode(&aNode);
        i1 = G.findNode(a); i2 = G.findNode(b);
        if (i1 == -1 && i2 == -1)
        {
            i1 = G.getNodeCnt();
            G.addNode(aNode);          
            G.addNode(bNode);
            i2 = i1 + 1;
        }
        else if (i1 != -1 && i2 == -1)
        {
            i2 = G.getNodeCnt();
            G.addNode(bNode);
        }
        else if (i1 == -1 && i2 != -1)
        {
            i1 = G.getNodeCnt();
            G.addNode(aNode);
        }
        G.setMatrixValue(i1, i2);

    }
    file.close();
    G.findPath();
    sort(G.path.begin(), G.path.end(), sorFun);//按照输出规则排序

    ofstream output("result_test.txt");
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