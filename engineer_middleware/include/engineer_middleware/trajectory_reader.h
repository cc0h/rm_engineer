//
// Created by cch on 24-7-22.
//

#pragma once

#include <iostream>
#include <fstream>
#include "yaml-cpp/yaml.h"

std::vector<std::string> joint_names;
std::vector<YAML::Node> trajectory_points;

void read_trajectory_from_yaml(const std::string& file_path)
{
  try
  {
    YAML::Node data = YAML::LoadFile(file_path);

    // Check if 'file_path' key exists and is a sequence
    if (data["file_path"] && data["file_path"].IsSequence())
    {
      // Skip the first entry which is the file path
      for (size_t i = 1; i < data["file_path"].size(); ++i)
      {
        YAML::Node entry = data["file_path"][i];
        if (entry.IsMap())
        {
          YAML::Node time_from_start = entry["time_from_start"];
          YAML::Node positions = entry["positions"];
          YAML::Node velocities = entry["velocities"];

          if (joint_names.empty() && positions.IsSequence())
          {
            for (size_t j = 0; j < positions.size(); ++j)
            {
              joint_names.push_back("joint" + std::to_string(j + 1));
            }
          }

          YAML::Node point;
          point["time_from_start"] = time_from_start;
          point["positions"] = positions;
          point["velocities"] = velocities;
          trajectory_points.push_back(point);
        }
        else
        {
          std::cout << "Skipping invalid entry." << std::endl;
        }
      }
    }
    else
    {
      std::cout << "No valid 'file_path' key found in YAML data." << std::endl;
    }
  }
  catch (const YAML::Exception& e)
  {
    std::cerr << "Error parsing YAML file: " << e.what() << std::endl;
  }
}
