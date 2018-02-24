#ifndef IMAGESCISSOR_H
#define IMAGESCISSOR_H

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <cstddef>
#include <iostream>
#include <queue>
#include <vector>
#include <fstream>
#include <stack>
#include <opencv2/opencv.hpp>
#include "mutable_priority_queue.h"

using namespace std;

enum STATE {INITIAL, ACTIVE, EXPANDEND};

class Node
{
public:
    typedef boost::shared_ptr<Node> Ptr;

    Node(const int &row, const int &col)
    {
        _row = row;
        _col = col;
        _state = INITIAL;
        _prev_node.reset();
        _total_cost = 1e6;
    }

    void iniParam()
    {
        _state = INITIAL;
        _prev_node.reset();
        _total_cost = 1e6;
    }

    bool operator == (const Node::Ptr &p)
    {
        return ((p->_row == this->_row) & (p->_col == this->_col));
    }

    double _link_cost[8];
    double _total_cost;
    STATE _state;
    Node::Ptr _prev_node;
    int _row, _col;
};

class NodeComparision
{
public:
    bool operator() (Node::Ptr &a, Node::Ptr &b)
    {
        return (a->_total_cost > b->_total_cost);
    }
};


typedef priority_queue<Node::Ptr, std::vector<Node::Ptr>, NodeComparision> pqueue;
//typedef mutable_priority_queue<int, Node::Ptr, ValueComparision, NodeComparision> pqueue;

class ImageScissor
{
public:
    ImageScissor();
    void iniParam(const cv::Mat &img);
    void clearParam();
    int CheckBoundary(const int &i, const int &j);

    void setInput(const cv::Mat &img);
    void addSeed(const int &row, const int &col);
    void setDest(const int &row, const int &col);
    void deleteSeed();
    void saveMask();
    void getMask();

    void ComputeCostGraph();
    void ComputeDLink(vector<double> &d_link, const int &i, const int &j, cv::Mat &img);
    void DrawShortestPath();
    void DIJkstra();

public:
    cv::Mat _img_src;
    cv::Mat _prev_draw_img, _draw_img, _mask;
    cv::Mat _pixel_node, _cost_graph, _minipath_tree, _minipath, _link_cost;
    int _row, _col;
    vector<vector<int> > _d;

    stack<Node::Ptr> _seed;
    Node::Ptr _dest;
    vector<vector<int> > _path;
    vector<vector<int> > _prev_path;
    stack<int> _prev_path_end;

    vector<vector<Node::Ptr> > _node_graph;

    // TODO: https://en.wikipedia.org/wiki/Priority_queue#Implementation
    pqueue _pqueue;
};

#endif // IMAGESCISSOR_H
