#include "identify.hpp"

int main(int argc, char** argv)
{
    ros::init(argc, argv, "people_identify");
    ros::NodeHandle node;

    identify task1;
    task1.task_start(node);
    return 0;
}