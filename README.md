# libspatialindex example
A simple example of the library that loads points into an R-Tree. Since the example was adapted from an existing project,  it contains some unnecessary staff such as using a custom-made class "Point.h" and assuming that each point belongs to a cluster. However, these assumptions do not affect the clarity of the example. 

## The application does the following:
1. Reads the points from a file and inserts them directly into the R-Tree - no intermediate structure is used as this can easily fill up your memory when dealing with big data. I have used **valgrind massif** to verify that memory usage is equal to the size of the input
2. It stores a cluster id (as a sequence of bytes) along with each point 
3. It selects 600 random points and uses them as query points 
4. It queries the R-Tree and estimates the average time for retrieving a point. It also estimates the error of the measurements 

## Notes:
1. The application was built in a way that it does not use more memory than the data itself 
2. Requires cmake version 3.6
3. Assumes libspatialindex is properly installed (see [here how to install](https://github.com/libspatialindex/libspatialindex/wiki/1.-Getting-Started) )
4. A **sample file** is provided

## How to run on Linux
```bash
mkdir build
cd build
cmake ..
make
./SpatialIndexRtree sample.txt
```
