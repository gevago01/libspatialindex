#include <iostream>
#include "callSpatialLibrary.h"

#define MEASUREMENTS 600
#define CUT_MEASUREMENTS 100
#define GB 1000000000

using std::cout;
using std::vector;
using std::string;
using std::endl;

void calculate_statistics(vector<double> &times, float file_size) {

    //sort times
    std::sort(times.begin(), times.end());

    times.resize(MEASUREMENTS - CUT_MEASUREMENTS);

    /*estimated mean*/
    double sum = std::accumulate(times.cbegin(), times.cend(), 0.0);
    double mean = sum / times.size();

    vector<double> times_minus_mean;

    /*subtract mean from every vector element*/
    std::transform(times.cbegin(), times.cend(), std::back_inserter(times_minus_mean),
                   [&mean](double const x) { return x - mean; });

    double innerProduct = std::inner_product(times_minus_mean.cbegin(), times_minus_mean.cend(),
                                             times_minus_mean.cbegin(), 0.0);

    double std = std::sqrt(innerProduct / times_minus_mean.size());

    double std_error = std / std::sqrt((MEASUREMENTS - CUT_MEASUREMENTS));


    cout << "#Dataset size|Time|Standard error" << endl;
    cout << "bst:" << file_size / GB << "\t" << mean << "\t" << std_error << "\t" << endl;

}


int main(int argc, char **argv) {
    vector<double> times;
    std::chrono::high_resolution_clock::time_point t1;
    std::chrono::high_resolution_clock::time_point t2;
    vector<Point> random_points;


    /*create r-tree*/
    CallSpatialLib callSpatialLib(argv[argc - 1], MEASUREMENTS);

    callSpatialLib.readPoints();
//    callSpatialLib.printRtreeStatistics();

    random_points=callSpatialLib.getRandomPoints();

    for (Point  &point:random_points) {
        double slib_time = 0;
        long cluster_id;

        cout << "looking for point:" << point.getPoint_id() << " that belongs in cluster:" << point.getCluster_id()
             << endl;
        CallSpatialLib::MyVisitor visitor(callSpatialLib.getMax_clusterid());
        std::vector<double> const &point_coordinates = point.getCoordinates();
        ulong dimen = point_coordinates.size();
        double coordinates[dimen];


        std::copy(point_coordinates.begin(), point_coordinates.end(), coordinates);
        t1 = std::chrono::high_resolution_clock::now();
        cluster_id = callSpatialLib.call_lib_spatial(point, visitor, dimen, coordinates);
        t2 = std::chrono::high_resolution_clock::now();
        cout << "cluster_id:" << cluster_id << endl;
        if (cluster_id != point.getCluster_id()) {
            cout << "Wrong Cluster id retrieved" << endl;
            exit(14);
        }
        slib_time = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

        times.push_back(slib_time);


    }

//    callSpatialLib.printRtreeStatistics();

    calculate_statistics(times, (float) callSpatialLib.getFileSize());
    exit(111);

}

