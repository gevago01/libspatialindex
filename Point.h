//
// Created by giannis on 15/04/16.
//

#ifndef THESIS_POINT_H
#define THESIS_POINT_H

#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cmath>

class Point {

    //friend function that prints the object
    friend std::ostream &operator<<(std::ostream &, const Point &);

    friend std::ostream &operator<<(std::ostream &ostream1, std::vector<double> const coordinates);

    // friend class declaration is its own a forward declaration
    friend class Cluster;

private:
    /*holds coordinates for a number of dimensions
     its size is the number of dimensions*/
    std::vector<double> coordinates;

    /*denotes the cluster id (from the input
     * file) that the point belongs to*/
    long cluster_id;

    /*point id */
    int point_id;
    static int id_generator;

public:
    Point(std::vector<std::string> records);

    Point(std::vector<double> records);

    Point() : /*cluster_id(0), num_of_dimensions(0), */ point_id(id_generator) {
        ++id_generator;
    };

    /**
    * Define the copy constructor - compiler synthesized
    */
    Point(const Point &originalPoint) = default;
//    Point(const Point &originalPoint):coordinates(originalPoint.coordinates),cluster_id(originalPoint.cluster_id),point_id(originalPoint.point_id){} ;
    /**
     * Copy assignment operator - compiler synthesized
     */
    Point &operator=(const Point &right) = default;

    /**
     * Converts string to double
     */
    static double stringToDouble(std::string const str);

    Point &operator=(Point &&point) = default;

    Point(Point &&point) noexcept = default;

    int getPoint_id() const;

     const std::vector<double> getCoordinates() const{
            return coordinates;
        }

    long getCluster_id() const;

};

#endif //THESIS_POINT_H
