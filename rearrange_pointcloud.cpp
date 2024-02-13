// Copyright (C) 2024 Kiyoshi Irie
// MIT License
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <pcl/common/transforms.h>
#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>
#include <Eigen/Geometry>

int main(int argc, char *argv[]) {
    std::ifstream file(argv[1]);
    std::string line;

    pcl::PointCloud<pcl::PointXYZ>::Ptr merged_cloud(new pcl::PointCloud<pcl::PointXYZ>);

    int cnt = 0;
    while (std::getline(file, line)) {
        ++cnt;
        if (cnt % 10 !=0) continue;
        std::istringstream iss(line);
        std::string filename;
        float x, y, z, qx, qy, qz, qw, rx, ry, rz;

        auto tokenize = [&](const std::string& line) {
            std::string token;
            std::vector<std::string> tokens;
            while (iss >> token) {
                tokens.push_back(token);
            }
            return tokens;
        };

#if 1 // Quaternion
        iss >> filename >> x >> y >> z >> qx >> qy >> qz >> qw;
        Eigen::Quaterniond quaternion(qw, qx, qy, qz);
        Eigen::Translation3d translation(x, y, z);
        Eigen::Affine3d transform = translation * quaternion;
#else
        iss >> filename >> x >> y >> z >> rx >> ry >> rz;
        //std::cout << x << " " << y << " " << z << " " << qx << " " << qy << " " << qz << " " << qw << std::endl;
        Eigen::Vector3d rotation_vector(rx, ry, rz);
        double angle = rotation_vector.norm();
        rotation_vector.normalize();
        Eigen::Affine3d transform = Eigen::Translation3d(x, y, z) *
                                    Eigen::AngleAxisd(angle, rotation_vector);
#endif

        pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);
        if (pcl::io::loadPCDFile<pcl::PointXYZ>(filename, *cloud) == -1) {
            PCL_ERROR("Couldn't read file %s \n", filename.c_str());
            continue;
        }

        pcl::transformPointCloud(*cloud, *cloud, transform);
        *merged_cloud += *cloud;
    }

    pcl::io::savePCDFileASCII("merged_cloud.pcd", *merged_cloud);
    return 0;
}

