//
// Created by giannis on 26/05/16.
//

#include "callSpatialLibrary.h"

using std::cout;
using std::endl;
using std::vector;

int CallSpatialLib::height_traversed = 0;

void
CallSpatialLib::insert_point(std::unique_ptr<SpatialIndex::ISpatialIndex>::pointer index, double coords[],
                             unsigned int num_of_coordinates, int id, uint32_t cluster_id) {

    std::string string_cluster(std::to_string(cluster_id));
    uint32_t len = string_cluster.length();
    char const *p = string_cluster.c_str();

    index->insertData(len, (byte *) p, SpatialIndex::Point(coords, num_of_coordinates), id);

}


void CallSpatialLib::readPoints() {

    int i = 0;
    cout << "reading file now" << endl;

    in_file_stream.clear();
    in_file_stream.seekg(0, std::ifstream::beg);

    if (!in_file_stream) {
        cout << "failed to read input" << endl;
        exit(-1);
    }
    std::string line, word;
    vector<std::string> tokens;
    while (getline(in_file_stream, line)) {

        //ignore comment lines
        if (line[0] == '#') {
            continue;
        }
        //istringstream makes the line string an input string stream
        std::istringstream record(line);

        /*a record contains a number of coordinates and a cluster id:
        2.58119273751133 -3.0897997256242977 1*/
        while (record >> word) {
            tokens.push_back(word);
        }

        Point point(tokens);

        if (random_indices.find(i) != random_indices.end()) {
            random_points.push_back(point);
        }
        if (point.getCluster_id() > max_clusterid) {
            max_clusterid = point.getCluster_id();
        }

        //for now the cluster is a singleton
        auto point_coords = point.getCoordinates();
        //convert vector to an array
        double coordinates[point_coords.size()];
        std::copy(point_coords.cbegin(), point_coords.cend(), coordinates);
//        insert_point(tree_.get(), coordinates, point_coords.size(), i);
        insert_point(tree_.get(), coordinates, point_coords.size(), point.getPoint_id(), point.getCluster_id());
        ++i;

        tokens.clear();

    }

    cout << "random points size:" << random_points.size() << endl;
    cout << "reading file done" << endl;
    cout << "creating rtree done:" << i << endl;

}


uint32_t CallSpatialLib::call_lib_spatial(Point const &point) {
    MyVisitor visitor(max_clusterid);
//    int k=1;
    std::vector<double> const &point_coordinates = point.getCoordinates();
    ulong dimen = point_coordinates.size();
    double coordinates[dimen];


    std::copy(point_coordinates.begin(), point_coordinates.end(), coordinates);

//    tree_->nearestNeighborQuery(k, SpatialIndex::Point(coordinates, dimen), visitor);
    tree_->pointLocationQuery(SpatialIndex::Point(coordinates, dimen), visitor);

    return std::stol(visitor.getCluster_id());
}


std::vector<Point> CallSpatialLib::getRandomPoints() {

    return random_points;

}


//void CallSpatialLib::printRtreeStatistics() {
//
//    SpatialIndex::IStatistics *stats = new SpatialIndex::RTree::Statistics();
//
//    tree_->getStatistics(&stats);
//
//
//    cout << "#########################" << endl;
//    cout << "#of nodes:" << stats->getNumberOfNodes() << endl;
//    cout << "#of reads:" << dynamic_cast<SpatialIndex::RTree::Statistics *>(stats)->getReads() << endl;
//    cout << "#of hits:" << dynamic_cast<SpatialIndex::RTree::Statistics *>(stats)->getHits() << endl;
//    cout << "#of splits:" << dynamic_cast<SpatialIndex::RTree::Statistics *>(stats)->getSplits() << endl;
//    cout << "#of adjustments:" << dynamic_cast<SpatialIndex::RTree::Statistics *>(stats)->getAdjustments() << endl;
//    cout << "#of misses:" << dynamic_cast<SpatialIndex::RTree::Statistics *>(stats)->getMisses() << endl;
//    cout << "tree height:" << dynamic_cast<SpatialIndex::RTree::Statistics *>(stats)->getTreeHeight() << endl;
//    cout << "#of writes:" << dynamic_cast<SpatialIndex::RTree::Statistics *>(stats)->getWrites() << endl;
//    cout << "#of data:" << dynamic_cast<SpatialIndex::RTree::Statistics *>(stats)->getNumberOfData() << endl;
//    cout << "#########################" << endl;
//}
//
//double CallSpatialLib::getTreeHeight() {
//    SpatialIndex::IStatistics *stats = new SpatialIndex::RTree::Statistics();
//
//    tree_->getStatistics(&stats);
//
//    double height = dynamic_cast<SpatialIndex::RTree::Statistics *>(stats)->getTreeHeight();
//    return height;
//}