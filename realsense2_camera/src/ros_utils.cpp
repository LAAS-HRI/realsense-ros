#include <ros_utils.h>
#include <algorithm>
#include <map>
#include <cctype>

namespace realsense2_camera
{
template <typename K, typename V>
std::ostream& operator<<(std::ostream& os, const std::map<K, V>& m)
{
    os << '{';
    for (const auto& kv : m)
    {
        os << " {" << kv.first << ": " << kv.second << '}';
    }
    os << " }";
    return os;
}

/**
 * Same as ros::names::isValidCharInName, but re-implemented here because it's not exposed.
 */
bool isValidCharInName(char c)
{
    return std::isalnum(c) || c == '/' || c == '_';
}

/**
 * ROS Graph Resource names don't allow spaces and hyphens (see http://wiki.ros.org/Names),
 * so we replace them here with underscores.
 */

std::string create_graph_resource_name(const std::string &original_name)
{
    std::string fixed_name = original_name;
    std::transform(fixed_name.begin(), fixed_name.end(), fixed_name.begin(),
                    [](unsigned char c) { return std::tolower(c); });
    std::replace_if(fixed_name.begin(), fixed_name.end(), [](const char c) { return !(realsense2_camera::isValidCharInName(c)); },
                    '_');
    return fixed_name;
}

const rmw_qos_profile_t qos_string_to_qos(std::string str)
{
#ifndef DASHING
    if (str == "UNKNOWN")
        return rmw_qos_profile_unknown;
#endif
    if (str == "SYSTEM_DEFAULT")
        return rmw_qos_profile_system_default;
    if (str == "PARAMETER_EVENTS")
        return rmw_qos_profile_parameter_events;
    if (str == "SERVICES_DEFAULT")
        return rmw_qos_profile_services_default;
    if (str == "PARAMETERS")
        return rmw_qos_profile_parameters;
    if (str == "DEFAULT")
        return rmw_qos_profile_default;
    if (str == "SENSOR_DATA")
        return rmw_qos_profile_sensor_data;
    throw std::runtime_error("Unknown QoS string " + str);
}

rs2_stream rs2_string_to_stream(std::string str)
{
    if (str == "RS2_STREAM_ANY")
        return RS2_STREAM_ANY;
    if (str == "RS2_STREAM_COLOR")
        return RS2_STREAM_COLOR;
    if (str == "RS2_STREAM_INFRARED")
        return RS2_STREAM_INFRARED;
    if (str == "RS2_STREAM_FISHEYE")
        return RS2_STREAM_FISHEYE;
    throw std::runtime_error("Unknown stream string " + str);
}

stream_index_pair rs2_string_to_sip(const std::string& str)
{
    if (str == "color")
        return realsense2_camera::COLOR;
    if (str == "depth")
        return DEPTH;
    if (str == "infra")
        return INFRA0;
    if (str == "infra1")
        return INFRA1;
    if (str == "infra2")
        return INFRA2;
    if (str == "fisheye")
        return FISHEYE;
    if (str == "fisheye1")
        return FISHEYE1;
    if (str == "fisheye2")
        return FISHEYE2;
    if (str == "gyro")
        return GYRO;
    if (str == "accel")
        return ACCEL;
    if (str == "pose")
        return POSE;
    std::stringstream ss;
    ss << "Unknown parameter name in" << __FILE__ << ":" << __LINE__;
    throw std::runtime_error(ss.str());
}

}
