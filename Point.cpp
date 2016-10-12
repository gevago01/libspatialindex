//
// Created by giannis on 15/04/16.
//

#include "Point.h"

using std::istringstream;
using std::cerr;
using std::endl;
using std::string;
using std::vector;
using std::ostream;

int Point::id_generator = 0;

/**
 * gets records of this form:
 * 2.58119273751133 -3.0897997256242977 1
 */
Point::Point(vector<double> records) : point_id(id_generator) {
    coordinates = records;
    cluster_id = static_cast<long>(coordinates.back());
    coordinates.pop_back();
    ++id_generator;
}

/**
 * gets records of this form:
 * 2.58119273751133 -3.0897997256242977 1
 */
Point::Point(vector<string> records) : point_id(id_generator) {
    double one_dimension;
    for (string record:records) {
        one_dimension = stringToDouble(record);
        coordinates.push_back(one_dimension);
    }

    cluster_id = static_cast<long>(coordinates.back());
//    cluster_id=static_cast<long>(id_generator);
    coordinates.pop_back();
    ++id_generator;
}

ostream &operator<<(ostream &ostream1, const Point &point) {
    ostream1 << "pid:" << point.point_id << " => ";
    for (double dim:point.coordinates) {
        ostream1 << dim << ",";
    }
    ostream1 << endl<<point.getCluster_id()<<endl;
    ostream1 << endl<<"dim:"<<point.getCoordinates().size()<<endl;


    return ostream1;
}


ostream &operator<<(ostream &ostream1, vector<double> const coordinates) {
    for (double dim:coordinates) {
        ostream1 << dim << ",";
    }
    ostream1 << endl;

    return ostream1;
}

double Point::stringToDouble(const string str) {
    istringstream double_str(str);
    double x;
    if (!(double_str >> x)) {
        cerr << "Error converting string to double" << endl;
        exit(-1);
    }

    return x;
}


int Point::getPoint_id() const {
    return point_id;
}



long Point::getCluster_id() const {
    return cluster_id;
}
