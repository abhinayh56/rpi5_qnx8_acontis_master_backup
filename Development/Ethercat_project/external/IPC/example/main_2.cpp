#include <iostream>
#include <unistd.h>
#include "IPC.h"
#include "Msg.h"

int main()
{
    std::cout << "IPC : TEST 2" << std::endl;

    Shm_store shm_store("MyShm", 4096);
    shm_store.open();

    int32_t encoder_count_1 = 124;
    int32_t encoder_count_2 = 180;
    int32_t encoder_count_1_other = 446;
    int32_t encoder_count_2_other = 456;
    Pose my_pose;
    my_pose.seq = 0;
    my_pose.time_stamp_ms = 0;
    my_pose.translation.x = 0;
    my_pose.translation.y = 0;
    my_pose.translation.z = 0;
    my_pose.orientation.q0 = 1;
    my_pose.orientation.q1 = 0;
    my_pose.orientation.q2 = 0;
    my_pose.orientation.q3 = 0;

    Shm_element<int32_t> shm_element_encoder_count_1("ENC_1", "/path_arm_1", encoder_count_1, &shm_store);
    Shm_element<int32_t> shm_element_encoder_count_2("ENC_2", "/path_arm_1", encoder_count_2, &shm_store);
    Shm_element<int32_t> shm_element_encoder_count_1_other("ENC_1", "/path_arm_2", encoder_count_1_other, &shm_store);
    Shm_element<int32_t> shm_element_encoder_count_2_other("ENC_2", "/path_arm_2", encoder_count_2_other, &shm_store);
    Shm_element<Pose> shm_element_my_pose("POSE_EE", "/path_arm_7", my_pose, &shm_store);

    shm_element_encoder_count_1.access_element();
    shm_element_encoder_count_2.access_element();
    shm_element_encoder_count_1_other.access_element();
    shm_element_encoder_count_2_other.access_element();
    shm_element_my_pose.access_element();
    
    uint64_t last = 100;

    while (1)
    {
        shm_element_encoder_count_1.get(encoder_count_1);
        shm_element_encoder_count_2.get(encoder_count_2);
        shm_element_encoder_count_1_other.get(encoder_count_1_other);
        shm_element_encoder_count_2_other.get(encoder_count_2_other);
        shm_element_my_pose.get(my_pose);

        if (my_pose.seq != last)
        {
            std::cout << "---" << std::endl;
            my_pose.disp();
            last = my_pose.seq;
        }

        usleep(1);
    }

    return 0;
}
