//
// Created by giannis on 26/05/16.
//

#ifndef THESIS_CALLSPATIALLIBRARY_H
#define THESIS_CALLSPATIALLIBRARY_H


#include <spatialindex/SpatialIndex.h>
//#include "/home/giannis/ClionProjects/spatialindex-src-1.8.5/src/rtree/RTree.h" //used for statistics methods
#include "Point.h"
#include <vector>
#include <memory>
#include <chrono>
#include <climits>

class CallSpatialLib {



    class MyVisitor : public SpatialIndex::IVisitor {

    private:
        uint32_t data_length;
        std::string cluster_id;


    public:
        const std::string &getCluster_id() const {
            return cluster_id;
        }

        MyVisitor(uint32_t const max_cluster_id) {
            data_length=std::to_string(max_cluster_id).length();
        }

        void visitNode(const SpatialIndex::INode &) {
//            std::cout << "visiting node" << std::endl;
            ++CallSpatialLib::height_traversed;
        }

        //does not enter here
        void visitData(std::vector<const SpatialIndex::IData *> &data) {
            std::cout << "visiting data:here" << data.size() << std::endl;

            for (auto t:data) {
                std::cout << "visiting data:loop:" << t->getIdentifier() << std::endl;
            }
            ++CallSpatialLib::height_traversed;
        }

        void visitData(const SpatialIndex::IData &d) {
            _indices.push_back(d.getIdentifier());
//            for (int i = 0; i < length; ++i) {
//                std::cout << "d:" << data[i] << std::endl;
//            }


            byte *bytearray = 0;
            //plus one for the a null terminated string
            char char_cluster_id[data_length+1];

            d.getData(data_length, &bytearray);
            std::copy(&bytearray[0],&bytearray[data_length],char_cluster_id);
            char_cluster_id[data_length]='\0';

            cluster_id=char_cluster_id;

            ++CallSpatialLib::height_traversed;
        }

        std::vector<size_t> &indices() { return _indices; }

        std::vector<double> getTimes();

    private:
        std::vector<size_t> _indices;
    };

private:
    /** The number of state dimensions for this RTree. */
    /** The properties of the RTree. */
    std::unique_ptr<Tools::PropertySet> properties_;
    /** A storage manager for the RTree. */
    std::unique_ptr<SpatialIndex::IStorageManager> storageManager_;
    /** The RTree itself. */
    std::unique_ptr<SpatialIndex::ISpatialIndex> tree_;
//    std::unique_ptr<SpatialIndex::RTree::RTree> tree_;
    uint32_t max_clusterid;

    std::string file_name;

    std::ifstream in_file_stream;
    long file_size;
    uint32_t num_of_points;
    std::set<uint32_t> random_indices;
    uint32_t measurements;
    std::vector<Point> random_points;


public:

    uint32_t getDimensionality();

    void findRandomIndices();

    CallSpatialLib(char *filename, uint32_t num_of_queries)
            : properties_(nullptr), storageManager_(nullptr), tree_(nullptr), max_clusterid(LONG_MIN), file_name(filename),in_file_stream(filename),num_of_points(0),measurements(num_of_queries)  {

        uint32_t dimensionality=getDimensionality();
        findRandomIndices();
        std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
        file_size= in.tellg();

        std::cout << "creating rtree now" << std::endl;
        // First we set up the properties for the RTree correctly.
        properties_.reset(new Tools::PropertySet());
        Tools::Variant var;

        // We use an R*-tree since this should be OK for general-purpose use.
        var.m_varType = Tools::VT_LONG;
        var.m_val.lVal = SpatialIndex::RTree::RV_RSTAR;
        properties_->setProperty("TreeVariant", var);

        // We set it to have the correct # of dimensions.
        var.m_varType = Tools::VT_ULONG;
        var.m_val.ulVal = dimensionality;
        properties_->setProperty("Dimension", var);

        // The IndexCapacity, LeafCapacity and FillFactor are chosen as defaults;
        // it may be useful to be able to tune them for performance.
        var.m_varType = Tools::VT_ULONG;
        var.m_val.ulVal = 100;
        properties_->setProperty("IndexCapacity", var);

        var.m_varType = Tools::VT_ULONG;
        var.m_val.ulVal = 100;
        properties_->setProperty("LeafCapacity", var);

        var.m_varType = Tools::VT_DOUBLE;
        var.m_val.dblVal = 0.7;
        properties_->setProperty("FillFactor", var);

        // Now we create a new StorageManager and a new tree.
        tree_.reset(nullptr);
        storageManager_.reset(
                SpatialIndex::StorageManager::returnMemoryStorageManager(*properties_));
//    tree_.reset(SpatialIndex::RTree::returnRTree(*storageManager_, *properties_));
        tree_.reset(SpatialIndex::RTree::returnRTree(*storageManager_, *properties_));
//    tree_.reset(new SpatialIndex::RTree::RTree(*storageManager_, *properties_));
        if (!tree_->isIndexValid()) {
            throw std::runtime_error("Failed to create valid indexRtree");
        }

    };


    void readPoints();

    void insert_point(std::unique_ptr<SpatialIndex::ISpatialIndex>::pointer index, double coords[],
                      unsigned int num_of_coordinates, int id, uint32_t cluster_id);

    uint32_t call_lib_spatial(Point  const &point);



    static int height_traversed;

    long getFileSize() const {
        return file_size;
    }

    std::vector<Point> getRandomPoints();
};

#endif //THESIS_CALLSPATIALLIBRARY_H
