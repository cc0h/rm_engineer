//
// Created by ljq on 2022/4/28.
//

#pragma once

#include <moveit/planning_scene_interface/planning_scene_interface.h>

namespace engineer_middleware {

class PlanningScence
{
public:
		PlanningScence(const XmlRpc::XmlRpcValue& scence)
		{
			for(int i = 0; i < scence.size();i++)
			{
				moveit_msgs::CollisionObject collision_object;
				collision_object.header.frame_id = std::string(scence[i]["frame_id"]);
				collision_object.id = std::string(scence[i]["id"]);
				object_ids[i] = std::string(scence[i]["id"]);

				shape_msgs::SolidPrimitive primitives;
				primitives = Primitive(std::string(scence[i]["primitive"]));

				geometry_msgs::Pose pose;
				pose = Pose(std::string(scence[i]["psoe"]));

				collision_object.primitives.push_back(primitives);
				collision_object.primitive_poses.push_back(pose);

				collision_objects.push_back(collision_object);
			}
		};

		shape_msgs::SolidPrimitive Primitive(const XmlRpc::XmlRpcValue& primitives)
		{
			shape_msgs::SolidPrimitive primitive;
			if(std::string(primitives["type"]) == "box")
			{
				primitive.type = primitive.BOX;
				primitive.dimensions.resize(3);
				primitive.dimensions[primitive.BOX_X] = primitives["dimension"][0];
				primitive.dimensions[primitive.BOX_Y] = primitives["dimension"][1];
				primitive.dimensions[primitive.BOX_Z] = primitives["dimension"][2];
			}

			return primitive;
		};

		geometry_msgs::Pose Pose(const XmlRpc::XmlRpcValue& poses)
		{
			geometry_msgs::Pose pose;
			pose.orientation.x = poses["orientation"][0];
			pose.orientation.y = poses["orientation"][1];
			pose.orientation.z = poses["orientation"][2];
			pose.orientation.w = poses["orientation"][3];

			pose.position.x = poses["position"][0];
			pose.position.y = poses["position"][1];
			pose.position.z = poses["position"][2];

			return pose;
		}

		void Add()
		{
			for(int i = 0; i < collision_objects.size();i++)
			{
				collision_objects[i].operation = collision_objects[i].ADD;
			}
			planning_scene_interface.addCollisionObjects(collision_objects);

		}
		void Delete()
		{
			planning_scene_interface.removeCollisionObjects(object_ids);
		}

		std::vector<std::string> object_ids;
		moveit::planning_interface::PlanningSceneInterface planning_scene_interface;
		std::vector<moveit_msgs::CollisionObject> collision_objects;

};

}