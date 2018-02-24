#include "image_scissor.h"

ImageScissor::ImageScissor()
{
    cout << "[INFO]: ImageScissor Initialize." << endl;
    int d[8][2] = {{0,1},{-1,1},{-1,0},{-1,-1},{0,-1},{1,-1},{1,0},{1,1}};
    _d.resize(8);
    for (int i = 0; i < 8; i++)
    {
        _d[i].resize(2);
        for (int j = 0; j < 2; j++) _d[i][j] = d[i][j];
    }
}

void ImageScissor::iniParam(const cv::Mat &img)
{
    std::cout << "[INFO]: ImageScissor::iniParam" << std::endl;
    img.copyTo(_img_src);
    _row = img.rows;
    _col = img.cols;
    _dest.reset();
    _prev_path.clear();
    _node_graph.clear();
    _node_graph.resize(_row);
    for (size_t i = 0; i < _node_graph.size(); i++)
    {
        _node_graph[i].resize(_col);
        for (size_t j = 0; j < _node_graph[i].size(); j++)
        {
            _node_graph[i][j] = boost::make_shared<Node>(i, j);
        }
    }
    std::cout << "[INFO]: ImageScissor::iniParam_initialize _node_graph" << std::endl;

    _pqueue = pqueue();
    _pixel_node = cv::Mat(_row*3, _col*3, _img_src.type());
    _cost_graph = cv::Mat(_row*3, _col*3, _img_src.type());
    _minipath_tree = cv::Mat(_row*3, _col*3, _img_src.type(), cv::Scalar::all(0));
    _minipath = cv::Mat(_row*3, _col*3, _img_src.type());
    _link_cost = cv::Mat(_row*3, _col*3, _img_src.type());

    std::cout << "[INFO]: Image_ROW_COL: " << _row << " " << _col << std::endl;

}

void ImageScissor::clearParam()
{
    _dest.reset();
    _path.clear();
    for (size_t i = 0; i < _node_graph.size(); i++)
        for (size_t j = 0; j < _node_graph[i].size(); j++)
            _node_graph[i][j]->iniParam();
    _pqueue = pqueue();
}

int ImageScissor::CheckBoundary(const int &i, const int &j)
{
    // out of the boundary
    if ((i < 0) || (i >= _row) || (j < 0) || (j >= _col)) return 0;
    // at the boundary
    if ((i == 0) || (i == _row - 1)) return 1;
    if ((j == 0) || (j == _col - 1)) return 1;
    return 2;
}

void ImageScissor::ComputeDLink(vector<double> &d_link, const int &i, const int &j,
                                cv::Mat &img)
{
    // TODO: something wrong
    vector<int> value;
    for (int k = 0; k < 8; k++)
    {
        int ii = i + _d[k][0], jj = j + _d[k][1];
        if (CheckBoundary(ii, jj) == 0)
        {
            value.push_back(int(img.at<uchar>(i, j)));
        } else
        {
//            std::cout << int(img.at<uchar>(ii, jj)) << " ";
            value.push_back(int(img.at<uchar>(ii, jj)));
        }
    }

    for (int k = 0; k < 8; k++)
    {
        int ii = i + _d[k][0], jj = j + _d[k][1];
        switch (k)
        {
            case 0: d_link[k] = std::abs(value[6] + value[7] - value[1] - value[2]) / 4.0; break;
            case 1: d_link[k] = std::abs(value[0] - value[2]) / std::sqrt(2.0); break;
            case 2: d_link[k] = std::abs(value[0] + value[1] - value[3] - value[4]) / 4.0; break;
            case 3: d_link[k] = std::abs(value[2] - value[4]) / std::sqrt(2.0); break;
            case 4: d_link[k] = std::abs(value[2] + value[3] - value[5] - value[6]) / 4.0; break;
            case 5: d_link[k] = std::abs(value[4] - value[6]) / std::sqrt(2.0); break;
            case 6: d_link[k] = std::abs(value[4] + value[5] - value[7] - value[0]) / 4.0; break;
            case 7: d_link[k] = std::abs(value[6] - value[0]) / std::sqrt(2.0); break;
            default: break;
        }
//        if ((CheckBoundary(i ,j) == 1) && (CheckBoundary(ii, jj) == 1)) d_link[k] = 180;
    }

}

void ImageScissor::ComputeCostGraph()
{
    std::cout << "[INFO]: ComputeCostGraph" << std::endl;
    double max_d = 0;
    vector<vector<double> > v_dlink;
    if (_img_src.type() == CV_8UC1)
    {
        std::cout << "[INFO]: ImageScissor::ComputeCostGraph_8UC1 type" << std::endl;
        for (int i = 0; i < _row; i++)
        {
            for (int j = 0; j < _col; j++)
            {
                // compute d_link
                vector<double> d_link(8);
                ComputeDLink(d_link, i, j, _img_src);
                max_d = std::max(*(std::max_element(d_link.begin(), d_link.end())), max_d);
                v_dlink.push_back(d_link);
            }
        }
        for (int i = 0; i < _row; i++)
        {
            for (int j = 0; j < _col; j++)
            {
                // compute cost_link and create cost_graph
                Node::Ptr p = _node_graph[i][j];
                _pixel_node.at<uchar>(3*i+1, 3*j+1) = _img_src.at<uchar>(i, j);
                _cost_graph.at<uchar>(3*i+1, 3*j+1) = _img_src.at<uchar>(i, j);
                for (int k = 0; k < 8; k++)
                {
                    p->_link_cost[k] = (max_d - v_dlink[i*_col+j][k]) * std::sqrt(std::abs(_d[k][0]) + std::abs(_d[k][1]));
                    _pixel_node.at<uchar>(3*i+1 + _d[k][0], 3*j+1 + _d[k][1]) = 255;
                    _cost_graph.at<uchar>(3*i+1 + _d[k][0], 3*j+1 + _d[k][1]) = p->_link_cost[k];
                }

            }
        }
    }
    else if (_img_src.type() == CV_8UC3)
    {
        std::cout << "[INFO]: ImageScissor::ComputeCostGraph_8UC3 type" << std::endl;
        cv::Mat channel[3];
        cv::split(_img_src, channel);
        for (int i = 0; i < _row; i++)
        {
            for (int j = 0; j < _col; j++)
            {
                // compute d_link
                vector<double> dr_link(8), dg_link(8), db_link(8), d_link(8);
                ComputeDLink(dr_link, i, j, channel[0]);
                ComputeDLink(dg_link, i, j, channel[1]);
                ComputeDLink(db_link, i, j, channel[2]);
                _link_cost.at<cv::Vec3b>(3*i+1, 3*j+1) = _img_src.at<cv::Vec3b>(i, j);
                for (int k = 0; k < 8; k++) d_link[k] = std::sqrt((dr_link[k]*dr_link[k] + dg_link[k]*dg_link[k] + db_link[k]*db_link[k]) / 3.0);
                for (int k = 0; k < 8; k++) _link_cost.at<cv::Vec3b>(3*i+1 + _d[k][0], 3*j+1 + _d[k][1]) = cv::Vec3b(d_link[k], d_link[k], d_link[k]);
                max_d = std::max(*(std::max_element(d_link.begin(), d_link.end())), max_d);
                v_dlink.push_back(d_link);
            }
        }
        std::cout << "ImageScissor::ComputeCostGraph_max_d " << max_d << std::endl;
        std::cout << "ImageScissor::ComputeCostGraph_size_v_dink " << v_dlink.size() << std::endl;
        for (int i = 0; i < _row; i++)
        {
            for (int j = 0; j < _col; j++)
            {
                // compute cost_link and create cost_graph
                Node::Ptr p = _node_graph[i][j];
                _pixel_node.at<cv::Vec3b>(3*i+1, 3*j+1) = _img_src.at<cv::Vec3b>(i, j);
                _cost_graph.at<cv::Vec3b>(3*i+1, 3*j+1) = _img_src.at<cv::Vec3b>(i, j);
                for (int k = 0; k < 8; k++)
                {
                    p->_link_cost[k] = (max_d - v_dlink[i*_col+j][k]) * std::sqrt(std::abs(_d[k][0]) + std::abs(_d[k][1]));
                    _pixel_node.at<cv::Vec3b>(3*i+1 + _d[k][0], 3*j+1 + _d[k][1]) = cv::Vec3b(0, 0, 0);
                    _cost_graph.at<cv::Vec3b>(3*i+1 + _d[k][0], 3*j+1 + _d[k][1]) = cv::Vec3b(p->_link_cost[k]/1.5, p->_link_cost[k]/1.5, p->_link_cost[k]/1.5);
                }
            }
        }
    }
}

void ImageScissor::setInput(const cv::Mat &img)
{
    iniParam(img);
    ComputeCostGraph();
}

void ImageScissor::addSeed(const int &row, const int &col)
{
    std::cout << "[INFO] Add Seed ROW_COL: " << row << " " << col << std::endl;
    for (size_t i = 0; i < _path.size(); i++) _prev_path.push_back(_path[i]);
    _prev_path_end.push(_path.size());

    clearParam();
    _seed.push(_node_graph[row][col]);

    DIJkstra();
}

void ImageScissor::deleteSeed()
{
    _seed.pop();
    if (!_seed.empty())
    {
        _prev_path.erase(_prev_path.end() - _prev_path_end.top(), _prev_path.end());
        _prev_path_end.pop();
        clearParam();
        DIJkstra();
    }
}

void ImageScissor::setDest(const int &row, const int &col)
{
//    std::cout << "[INFO]: Set Dest: " << row << " " << col << std::endl;
    _dest = _node_graph[row][col];
}

void ImageScissor::DrawShortestPath()
{
    _path.clear();
    if (_dest == _seed.top()) return;
    Node::Ptr end = _dest;
    while (end->_prev_node)
    {
//        std::cout << "(" << end->_row << "," << end->_col << ")>";
        vector<int> t(2); t[0] = end->_row; t[1] = end->_col;
        _path.push_back(t);
        end = end->_prev_node;
    }

    _draw_img = _img_src.clone();

    for (size_t i = 0; i < _prev_path.size(); i++)
    {
        if (_draw_img.type() == CV_8UC1) _draw_img.at<uchar>(_prev_path[i][0], _prev_path[i][1]) = 255;
        else if (_draw_img.type() == CV_8UC3) _draw_img.at<cv::Vec3b>(_prev_path[i][0], _prev_path[i][1]) = cv::Vec3b(0, 0, 255);
    }

    for (size_t i = 0; i < _path.size(); i++)
    {
        if (_draw_img.type() == CV_8UC1) _draw_img.at<uchar>(_path[i][0], _path[i][1]) = 255;
        else if (_draw_img.type() == CV_8UC3) _draw_img.at<cv::Vec3b>(_path[i][0], _path[i][1]) = cv::Vec3b(0, 0, 255);
    }
}

/*
 * TODO:
 * fix the wrong usage of priority_queue.
 * The data type should be Node and use _node_graph to record the state of each node.
 * The data value(related to priority) in priority_queue should not be changed(not influence the queue)
 * We can change the state of node in priority_queue
*/
void ImageScissor::DIJkstra()
{
    std::cout << "[INFO]ImageScissor::DIJkstra" << std::endl;
    std::cout << "[INFO]ImageScissor::DIJkstra_pqueue size " << _pqueue.size() <<std::endl;

    Node::Ptr s = _seed.top();
    s->_total_cost = 0;
    _pqueue.push(s);
    while (!_pqueue.empty())
    {
        // Extract the minimum cost node
        Node::Ptr q = _pqueue.top();
        _pqueue.pop();
        if (q->_state == EXPANDEND) continue;
//        if (_node_graph[q->_row][q->_col]->_state == EXPANDEND) continue;

//        std::cout << "(" << q->_row << "," << q->_col << "," << q->_total_cost  << ") " << q->_state << " ";
        // Remove the minimum cost node
        q->_state = EXPANDEND;
//        _node_graph[q->_row][q->_col]->_state = EXPANDEND;
        for (int i = 0; i < 8; i++)
        {
            int r_row = q->_row + _d[i][0];
            int r_col = q->_col + _d[i][1];
            // Not consider the path outside the image
            if ((r_row < 0) || (r_row >= _row) || (r_col < 0) || (r_col >= _col)) continue;

            Node::Ptr r = _node_graph[r_row][r_col];
            if (r->_state == INITIAL)
            {
                r->_prev_node = q;
                r->_total_cost = q->_total_cost + q->_link_cost[i];
                r->_state = ACTIVE;
                _pqueue.push(r);
            }
            else if (r->_state == ACTIVE)
            {
                if (q->_total_cost + q->_link_cost[i] < r->_total_cost)
                {
                    r->_prev_node = q;
                    r->_total_cost = q->_total_cost + q->_link_cost[i];
                    _pqueue.push(r);
                }
            }
            else if (r->_state == EXPANDEND)
            {
                continue;
            }
        }
    }

    // draw minipath tree
    std::cout << "[INFO]ImageScissor::DIJkstra_Draw minipath tree" << std::endl;
    std::cout << "[INFO]ImageScissor::DIJkstra_Draw _node_graph size " << _node_graph.size()
              << " " << _node_graph[0].size() << std::endl;

    for (size_t i = 0; i < _node_graph.size(); i++)
        for (size_t j = 0; j < _node_graph[i].size(); j++)
        {
            Node::Ptr p = _node_graph[i][j];
            if (p->_prev_node)
                _minipath_tree.at<cv::Vec3b>(p->_prev_node->_row*3+1, p->_prev_node->_col*3+1) = cv::Vec3b(0, 255, 0);
        }
}

void ImageScissor::getMask()
{
    // BUG! POINT::x = col; POINT::y = row
    cv::Point start(_seed.top()->_col, _seed.top()->_row);
    cv::Point end;

    cv::Point seed(0, 0);
    int i = 0;
    while (!_seed.empty())
    {
        Node::Ptr s = _seed.top();
        _seed.pop();
        seed.y += s->_row;
        seed.x += s->_col;
        end.y = s->_row;
        end.x = s->_col;
        i++;
    }
    seed.x = int(seed.x / i);
    seed.y = int(seed.y / i);
    std::cout << seed.x << " " << seed.y << std::endl;

    if (_draw_img.type() == CV_8UC1)
        cv::line(_draw_img, start, end, cv::Scalar(255), 1);
    else
        cv::line(_draw_img, start, end, cv::Scalar(0, 0, 255), 1);

    _mask = cv::Mat(_row, _col, _img_src.type(), cv::Scalar(255, 255, 255));
    if (_draw_img.type() == CV_8UC1)
    {
        for (size_t i = 0; i < _prev_path.size(); i++)
            _mask.at<uchar>(_prev_path[i][0], _prev_path[i][1]) = 255;
        cv::line(_mask, start, end, cv::Scalar(255), 1);
    }
    else
    {
        for (size_t i = 0; i < _prev_path.size(); i++)
            _mask.at<cv::Vec3b>(_prev_path[i][0], _prev_path[i][1]) = cv::Vec3b(0, 0, 255);
        cv::line(_mask, start, end, cv::Scalar(0, 0, 255), 1);
    }
    cv::floodFill(_mask, seed, cvScalarAll(0));
//    _img_src.copyTo(_mask, _mask);
    _mask = _mask + _img_src;
}

void ImageScissor::saveMask()
{

}
