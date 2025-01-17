/* \author Aaron Brown */
// Quiz on implementing simple RANSAC line fitting

#include "../../render/render.h"
#include "../../processPointClouds.h"
// using templates for processPointClouds so also include .cpp to help linker
#include "../../processPointClouds.cpp"
#include <unordered_set>
#include <math.h>

pcl::PointCloud<pcl::PointXYZ>::Ptr CreateData()
{
	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>());
  	// Add inliers
  	float scatter = 0.6;
  	for(int i = -5; i < 5; i++)
  	{
  		double rx = 2*(((double) rand() / (RAND_MAX))-0.5);
  		double ry = 2*(((double) rand() / (RAND_MAX))-0.5);
  		pcl::PointXYZ point;
  		point.x = i+scatter*rx;
  		point.y = i+scatter*ry;
  		point.z = 0;

  		cloud->points.push_back(point);
  	}
  	// Add outliers
  	int numOutliers = 10;
  	while(numOutliers--)
  	{
  		double rx = 2*(((double) rand() / (RAND_MAX))-0.5);
  		double ry = 2*(((double) rand() / (RAND_MAX))-0.5);
  		pcl::PointXYZ point;
  		point.x = 5*rx;
  		point.y = 5*ry;
  		point.z = 0;

  		cloud->points.push_back(point);

  	}
  	cloud->width = cloud->points.size();
  	cloud->height = 1;

  	return cloud;

}

pcl::PointCloud<pcl::PointXYZ>::Ptr CreateData3D()
{
	ProcessPointClouds<pcl::PointXYZ> pointProcessor;
	return pointProcessor.loadPcd("../../../sensors/data/pcd/simpleHighway.pcd");
}


pcl::visualization::PCLVisualizer::Ptr initScene()
{
	pcl::visualization::PCLVisualizer::Ptr viewer(new pcl::visualization::PCLVisualizer ("2D Viewer"));
	viewer->setBackgroundColor (0, 0, 0);
  	viewer->initCameraParameters();
  	viewer->setCameraPosition(0, 0, 15, 0, 1, 0);
  	viewer->addCoordinateSystem (1.0);
  	return viewer;
}

std::unordered_set<int> Ransac(pcl::PointCloud<pcl::PointXYZ>::Ptr cloud, int maxIterations, float distanceTol)
{
	std::unordered_set<int> inliersResult;
	std::srand(std::time(NULL));
	// TODO: Fill in this function
	// For max iterations 
	while (maxIterations--) {
		// Randomly sample subset and fit line
		std::unordered_set<int> inliers;
		while (inliers.size() < 3)
			inliers.insert(rand()%(cloud->points.size()));
		auto intr = inliers.begin();
		pcl::PointXYZ p1 = cloud->points[*intr];
		intr++;
		pcl::PointXYZ p2 = cloud->points[*intr];
		intr++;
		pcl::PointXYZ p3 = cloud->points[*intr];

		// Measure distance between every point and fitted line
		for (size_t j = 0; j < cloud->points.size(); ++j) {
			if (inliers.count(j) > 0) continue;
			// If distance is smaller than threshold count it as inlier
			float a = p1.y-p2.y;
			float b = p2.x-p1.x;
			float c = p2.x-p1.x;
			float d = p1.x*p2.y-p2.x*p1.y;

			float distance = fabs(a * cloud->points[j].x + b * cloud->points[j].y + c * cloud->points[j].z + d) / sqrt(pow(a, 2) + pow(b, 2) + pow(c, 2));
			if (distance <= distanceTol) inliers.insert(j);

		}

		if (inliers.size() > inliersResult.size()) inliersResult = inliers;
	}

	// Return indicies of inliers from fitted line with most inliers
	return inliersResult;
}

std::unordered_set<int> RansacPlane(pcl::PointCloud<pcl::PointXYZ>::Ptr cloud, int maxIterations, float distanceTol)
{
	std::unordered_set<int> inliersResult;
	std::srand(std::time(NULL));
	// TODO: Fill in this function
	// For max iterations 
	while (maxIterations--) {
		// Randomly sample subset and fit line
		std::unordered_set<int> inliers;
		while (inliers.size() < 3)
			inliers.insert(rand()%(cloud->points.size()));
		auto intr = inliers.begin();
		pcl::PointXYZ p1 = cloud->points[*intr];
		intr++;
		pcl::PointXYZ p2 = cloud->points[*intr];
		intr++;
		pcl::PointXYZ p3 = cloud->points[*intr];

		// Measure distance between every point and fitted line
		for (size_t j = 0; j < cloud->points.size(); ++j) {
			if (inliers.count(j) > 0) continue;
			// If distance is smaller than threshold count it as inlier
			pcl::PointXYZ v1; 
			v1.x = p2.x - p1.x;
			v1.y = p2.y - p1.y;
			v1.z = p2.z - p1.z;
			pcl::PointXYZ v2;
			v2.x = p3.x - p1.x;
			v2.y = p3.y - p1.y;
			v2.z = p3.z - p1.z;
			pcl::PointXYZ n; 
			n.x = (p2.y-p1.y)*(p3.z-p1.z)-(p2.z-p1.z)*(p3.y-p1.y);
			n.y = (p2.z-p1.z)*(p3.x-p1.x)-(p2.x-p1.x)*(p3.z-p1.z);
			n.z = (p2.x-p1.x)*(p3.y-p1.y)-(p2.y-p1.y)*(p3.x-p1.x);
			float d = -((n.x*p1.x)+(n.y*p1.y)+(n.z*p1.z));

			float distance = fabs(n.x * cloud->points[j].x + n.y * cloud->points[j].y + n.z * cloud->points[j].z + d) / sqrt(pow(n.x, 2) + pow(n.y, 2) + pow(n.z, 2));
			if (distance <= distanceTol) inliers.insert(j);

		}

		if (inliers.size() > inliersResult.size()) inliersResult = inliers;
	}

	// Return indicies of inliers from fitted line with most inliers
	return inliersResult;
}

int main ()
{

	// Create viewer
	pcl::visualization::PCLVisualizer::Ptr viewer = initScene();

	// Create data
	// pcl::PointCloud<pcl::PointXYZ>::Ptr cloud = CreateData();
	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud = CreateData3D();
	

	// TODO: Change the max iteration and distance tolerance arguments for Ransac function
	std::unordered_set<int> inliers = RansacPlane(cloud, 10, 0.5);

	pcl::PointCloud<pcl::PointXYZ>::Ptr  cloudInliers(new pcl::PointCloud<pcl::PointXYZ>());
	pcl::PointCloud<pcl::PointXYZ>::Ptr cloudOutliers(new pcl::PointCloud<pcl::PointXYZ>());

	for(int index = 0; index < cloud->points.size(); index++)
	{
		pcl::PointXYZ point = cloud->points[index];
		if(inliers.count(index))
			cloudInliers->points.push_back(point);
		else
			cloudOutliers->points.push_back(point);
	}


	// Render 2D point cloud with inliers and outliers
	if(inliers.size())
	{
		renderPointCloud(viewer,cloudInliers,"inliers",Color(0,1,0));
  		renderPointCloud(viewer,cloudOutliers,"outliers",Color(1,0,0));
	}
  	else
  	{
  		renderPointCloud(viewer,cloud,"data");
  	}
	
  	while (!viewer->wasStopped ())
  	{
  	  viewer->spinOnce ();
  	}
  	
}
