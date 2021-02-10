/*
 * KinfuServer.h
 *
 *  Created on: Jun 3, 2015
 *      Author: mklingen
 */

#ifndef YAK_OFFLINE_FUSION_SERVER_H_
#define YAK_OFFLINE_FUSION_SERVER_H_

#include "yak/kfusion/tsdf_container.h"
#include "yak/kfusion/kinfu.hpp"

//#include <pcl/point_types.h>  // For the 'getCloud' interface - TODO: Should this return a "native" point3d type
//#include <pcl/point_cloud.h>  // and we provide a PCL conversion seperately?

namespace yak
{
/**
 * @brief The OfflineFusionServer class
 *
 * @note Absolutely nothing in here is thread safe
 */
class FusionServer
{
public:
  FusionServer(const kfusion::KinFuParams& params, const Eigen::Affine3f& world_to_volume);

  /** @brief fuse() integrates a depth image into TSDF */
  bool fuse(const cv::Mat& depth_data, const Eigen::Affine3f& world_to_camera);

  /** @brief overload of fuse() that allows the extraction of computed ICP transforms */
  bool fuse(const cv::Mat& depth_data, const Eigen::Affine3f& world_to_camera, Eigen::Affine3f& icp_movement);

  bool reset();

  bool resetWithNewParams(const kfusion::KinFuParams& params);

  //  void getCloud(pcl::PointCloud<pcl::PointXYZ>& cloud) const;

  yak::TSDFContainer downloadTSDF();

  void display();

  void display(const Eigen::Affine3f& pose);

private:
  bool step(const Eigen::Affine3f& current_pose, const Eigen::Affine3f& last_pose, const cv::Mat& depth);

  /**
   * @brief Helper function for display() members; Downloads the active view from GPU and shows it in debug window
   */
  void downloadAndDisplayView();

  kfusion::KinFu::Ptr kinfu_;
  kfusion::cuda::Image viewDevice_;
  kfusion::cuda::Depth depthDevice_;

  Eigen::Affine3f volume_to_world_;
  Eigen::Affine3f last_camera_pose_;

public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

} /* namespace yak */
#endif /* YAK_OFFLINE_FUSION_SERVER_H_ */
