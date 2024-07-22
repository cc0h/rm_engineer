#include <ros/ros.h>
#include <actionlib/client/simple_action_client.h>
#include <control_msgs/FollowJointTrajectoryAction.h>
#include <trajectory_msgs/JointTrajectoryPoint.h>
#include <string>
#include <vector>
#include <map>

#pragma once

class TrajectoryClient
{
public:
  TrajectoryClient(std::string server_name, bool print_feedback = false)
    : client(server_name, true), print_feedback(print_feedback)
  {
    client.waitForServer();
  }

  void feedbackCallback(const control_msgs::FollowJointTrajectoryFeedbackConstPtr& feedback)
  {
    client_feedback = *feedback;
    if (print_feedback)
    {
      ROS_INFO_STREAM("Feedback: " << client_feedback);
    }
  }

  control_msgs::FollowJointTrajectoryResultConstPtr sendGoal(const control_msgs::FollowJointTrajectoryGoal& goal)
  {
    client.sendGoal(goal,
                    actionlib::SimpleActionClient<control_msgs::FollowJointTrajectoryAction>::SimpleDoneCallback(),
                    actionlib::SimpleActionClient<control_msgs::FollowJointTrajectoryAction>::SimpleActiveCallback(),
                    boost::bind(&TrajectoryClient::feedbackCallback, this, _1));
    client.waitForResult();
    control_msgs::FollowJointTrajectoryResultConstPtr result = client.getResult();
    ROS_INFO_STREAM("Result: " << *result);
    return result;
  }

private:
  actionlib::SimpleActionClient<control_msgs::FollowJointTrajectoryAction> client;
  control_msgs::FollowJointTrajectoryFeedback client_feedback;
  bool print_feedback;
};

control_msgs::FollowJointTrajectoryGoal
createGoal(const std::vector<std::string>& joint_names,
           const std::vector<std::map<std::string, std::vector<double>>>& trajectory_points)
{
  control_msgs::FollowJointTrajectoryGoal goal;
  goal.trajectory.joint_names = joint_names;

  for (const auto& point : trajectory_points)
  {
    trajectory_msgs::JointTrajectoryPoint traj_point;
    traj_point.positions = point.at("positions");
    if (point.find("velocities") != point.end())
    {
      traj_point.velocities = point.at("velocities");
    }
    traj_point.time_from_start = ros::Duration(point.at("time_from_start")[0]);
    goal.trajectory.points.push_back(traj_point);
  }

  return goal;
}
