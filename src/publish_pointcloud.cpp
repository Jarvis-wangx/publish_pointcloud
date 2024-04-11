/**
 *
 * 函数功能:读取pcl点云文件并发布到topic上去
 *
 *
 * maker: crp
 * data: 2016-6-8
 */

#include <iostream>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <sstream>
#include <vector>

#include <ros/ros.h>
#include <pcl/point_cloud.h>
#include <pcl_conversions/pcl_conversions.h>
#include <sensor_msgs/PointCloud2.h>
#include <pcl/io/pcd_io.h>

using namespace std;

int main(int argc, char **argv)
{

	std::string topic1, topic2, path1, path2, frame_id;
	int hz = 3;

    //初始化 ROS 节点 
	ros::init(argc, argv, "publish_pointcloud");
	// 创建 ROS 节点句柄
	ros::NodeHandle nh;


	// 从 ROS 参数服务器中获取参数
	nh.param<std::string>("path1", path1, "/home/stark/learn10_8/catkin_ws/src/publish_pointcloud/data/office5.pcd");
	nh.param<std::string>("path2", path2, "/home/stark/learn10_8/catkin_ws/src/publish_pointcloud/data/radiation_results.pcd");
	nh.param<std::string>("frame_id", frame_id, "camera");
	nh.param<std::string>("topic1", topic1, "/pointcloud/output1");
	nh.param<std::string>("topic2", topic2, "/pointcloud/output2");

	nh.param<int>("hz", hz, 5);


	// 创建 ROS 发布者（Publisher）
	ros::Publisher pcl_pub1 = nh.advertise<sensor_msgs::PointCloud2>(topic1, 10);//创建一个发布器，用于发布点云数据到指定的 ROS 话题
	ros::Publisher pcl_pub2 = nh.advertise<sensor_msgs::PointCloud2>(topic2, 10);//创建一个发布器，用于发布点云数据到指定的 ROS 话题


	pcl::PointCloud<pcl::PointXYZRGB> cloud1;
	pcl::PointCloud<pcl::PointXYZI> cloud2;
	sensor_msgs::PointCloud2 output1, output2;
	pcl::io::loadPCDFile(path1, cloud1);
	pcl::io::loadPCDFile(path2, cloud2);
	pcl::toROSMsg(cloud1, output1); // 转换成ROS下的数据类型 最终通过topic发布
	pcl::toROSMsg(cloud2, output2); // 转换成ROS下的数据类型 最终通过topic发布

	// 设置消息头部信息  设置消息的时间戳和坐标系。
	output1.header.stamp = ros::Time::now();
	output1.header.frame_id = frame_id;
	output2.header.stamp = ros::Time::now();
	output2.header.frame_id = frame_id;

	cout << "path1 = " << path1 << endl;
	cout << "path2 = " << path2 << endl;
	cout << "frame_id = " << frame_id << endl;
	cout << "topic1 = " << topic1 << endl;
	cout << "topic2 = " << topic2 << endl;
	cout << "hz = " << hz << endl;


	// 进入主循环
	ros::Rate loop_rate(hz);

	// 创建一个 ROS 频率对象，用于控制循环的执行频率。在循环中发布点云消息、处理 ROS 回调以及控制循环频率
	while (ros::ok())
	{
		pcl_pub1.publish(output1);
		pcl_pub2.publish(output2);
		ros::spinOnce();
		loop_rate.sleep();
	}

	
	return 0;
}
