#include <arpa/inet.h>
#include <atomic>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>
#include <unistd.h>
#include "IPC.h"

//const char *QNX_IP = "169.254.158.67";
//const char *WIN_IP = "169.254.106.144";

const char *QNX_IP = "192.168.10.1";
const char *WIN_IP = "192.168.10.2";

const char *LOCAL_IP = QNX_IP;
const int LOCAL_PORT = 5000;

const char *REMOTE_IP = WIN_IP;
const int REMOTE_PORT = 5001;

int sockfd;
std::atomic<bool> running(true);

sockaddr_in remoteAddr;

Shm_store shm_store("NEWshm1", 2000);

Shm_element<double> m_smeActualPosition("ACTUAL_POSITION", "/group_1/task/ethercat/servo/motor_5_den_net_e", 0, shm_store);
Shm_element<double> m_smeCommandPosition("COMMAND_POSITION", "/group_1/task/ethercat/servo/motor_5_den_net_e", 0, shm_store);
Shm_element<bool>   m_smeTiLimitSwitchPressed("TI_LIMIT_SWITCH_PRESSED", "/group_1/task/ethercat/servo/motor_5_den_net_e", 0, shm_store);
Shm_element<bool>   m_smeTiClutchButtonPressed("TI_CLUTCH_BUTTON_PRESSED", "/group_1/task/ethercat/servo/motor_5_den_net_e", 0, shm_store);

Shm_element<bool> m_smeShm_1_group_1("SHM_CH_1", "/group_1/task/interface/shm", 0, shm_store);
Shm_element<bool> m_smeShm_2_group_1("SHM_CH_2", "/group_1/task/interface/shm", 0, shm_store);
Shm_element<bool> m_smeShm_3_group_1("SHM_CH_3", "/group_1/task/interface/shm", 0, shm_store);
Shm_element<bool> m_smeShm_4_group_1("SHM_CH_4", "/group_1/task/interface/shm", 0, shm_store);
Shm_element<bool> m_smeShm_5_group_1("SHM_CH_5", "/group_1/task/interface/shm", 0, shm_store);
Shm_element<bool> m_smeShm_6_group_1("SHM_CH_6", "/group_1/task/interface/shm", 0, shm_store);
Shm_element<bool> m_smeShm_7_group_1("SHM_CH_7", "/group_1/task/interface/shm", 0, shm_store);
Shm_element<bool> m_smeShm_8_group_1("SHM_CH_8", "/group_1/task/interface/shm", 0, shm_store);
Shm_element<bool> m_smeShm_1_group_2("SHM_CH_1", "/group_2/task/interface/shm", 0, shm_store);
Shm_element<bool> m_smeShm_2_group_2("SHM_CH_2", "/group_2/task/interface/shm", 0, shm_store);
Shm_element<bool> m_smeShm_3_group_2("SHM_CH_3", "/group_2/task/interface/shm", 0, shm_store);
Shm_element<bool> m_smeShm_4_group_2("SHM_CH_4", "/group_2/task/interface/shm", 0, shm_store);
Shm_element<bool> m_smeShm_5_group_2("SHM_CH_5", "/group_2/task/interface/shm", 0, shm_store);
Shm_element<bool> m_smeShm_6_group_2("SHM_CH_6", "/group_2/task/interface/shm", 0, shm_store);
Shm_element<bool> m_smeShm_7_group_2("SHM_CH_7", "/group_2/task/interface/shm", 0, shm_store);
Shm_element<bool> m_smeShm_8_group_2("SHM_CH_8", "/group_2/task/interface/shm", 0, shm_store);

double m_actualPosition;
double m_commandPosition;
bool   m_tiLimitSwitchPressed;
bool   m_tiClutchButtonPressed;

bool m_Shm_1_group_1 = false;
bool m_Shm_2_group_1 = false;
bool m_Shm_3_group_1 = false;
bool m_Shm_4_group_1 = false;
bool m_Shm_5_group_1 = false;
bool m_Shm_6_group_1 = false;
bool m_Shm_7_group_1 = false;
bool m_Shm_8_group_1 = false;
bool m_Shm_1_group_2 = false;
bool m_Shm_2_group_2 = false;
bool m_Shm_3_group_2 = false;
bool m_Shm_4_group_2 = false;
bool m_Shm_5_group_2 = false;
bool m_Shm_6_group_2 = false;
bool m_Shm_7_group_2 = false;
bool m_Shm_8_group_2 = false;

void socketToShmFunction()
{
    char buffer[1024];

    while (running)
    {
    	usleep(1000000 / 25);

        sockaddr_in sender{};
        socklen_t len = sizeof(sender);

        int bytes = recvfrom(sockfd, buffer, sizeof(buffer) - 1, 0, (sockaddr *)&sender, &len);

        if (bytes < 0)
        {
			m_Shm_1_group_1 = 0;
			m_Shm_2_group_1 = 0;
			m_Shm_3_group_1 = 0;
			m_Shm_4_group_1 = 0;
			m_Shm_5_group_1 = 0;
			m_Shm_6_group_1 = 0;
			m_Shm_7_group_1 = 0;
			m_Shm_8_group_1 = 0;
			m_Shm_1_group_2 = 0;
			m_Shm_2_group_2 = 0;
			m_Shm_3_group_2 = 0;
			m_Shm_4_group_2 = 0;
			m_Shm_5_group_2 = 0;
			m_Shm_6_group_2 = 0;
			m_Shm_7_group_2 = 0;
			m_Shm_8_group_2 = 0;

			m_smeShm_1_group_1.set(m_Shm_1_group_1);
			m_smeShm_2_group_1.set(m_Shm_2_group_1);
			m_smeShm_3_group_1.set(m_Shm_3_group_1);
			m_smeShm_4_group_1.set(m_Shm_4_group_1);
			m_smeShm_5_group_1.set(m_Shm_5_group_1);
			m_smeShm_6_group_1.set(m_Shm_6_group_1);
			m_smeShm_7_group_1.set(m_Shm_7_group_1);
			m_smeShm_8_group_1.set(m_Shm_8_group_1);
			m_smeShm_1_group_2.set(m_Shm_1_group_2);
			m_smeShm_2_group_2.set(m_Shm_2_group_2);
			m_smeShm_3_group_2.set(m_Shm_3_group_2);
			m_smeShm_4_group_2.set(m_Shm_4_group_2);
			m_smeShm_5_group_2.set(m_Shm_5_group_2);
			m_smeShm_6_group_2.set(m_Shm_6_group_2);
			m_smeShm_7_group_2.set(m_Shm_7_group_2);
			m_smeShm_8_group_2.set(m_Shm_8_group_2);

        	continue;
        }

        buffer[bytes] = '\0';
        std::cout << "\nRX : " << buffer << std::endl;

        std::string new_msg = buffer;

		m_Shm_1_group_1 = 0;
		m_Shm_2_group_1 = 0;
		m_Shm_3_group_1 = 0;
		m_Shm_4_group_1 = 0;
		m_Shm_5_group_1 = 0;
		m_Shm_6_group_1 = 0;
		m_Shm_7_group_1 = 0;
		m_Shm_8_group_1 = 0;
		m_Shm_1_group_2 = 0;
		m_Shm_2_group_2 = 0;
		m_Shm_3_group_2 = 0;
		m_Shm_4_group_2 = 0;
		m_Shm_5_group_2 = 0;
		m_Shm_6_group_2 = 0;
		m_Shm_7_group_2 = 0;
		m_Shm_8_group_2 = 0;

		if(new_msg == "1 1") {m_Shm_1_group_1 = 1;}
		if(new_msg == "2 1") {m_Shm_2_group_1 = 1;}
		if(new_msg == "3 1") {m_Shm_3_group_1 = 1;}
		if(new_msg == "4 1") {m_Shm_4_group_1 = 1;}
		if(new_msg == "5 1") {m_Shm_5_group_1 = 1;}
		if(new_msg == "6 1") {m_Shm_6_group_1 = 1;}
		if(new_msg == "7 1") {m_Shm_7_group_1 = 1;}
		if(new_msg == "8 1") {m_Shm_8_group_1 = 1;}
		if(new_msg == "1 2") {m_Shm_1_group_2 = 1;}
		if(new_msg == "2 2") {m_Shm_2_group_2 = 1;}
		if(new_msg == "3 2") {m_Shm_3_group_2 = 1;}
		if(new_msg == "4 2") {m_Shm_4_group_2 = 1;}
		if(new_msg == "5 2") {m_Shm_5_group_2 = 1;}
		if(new_msg == "6 2") {m_Shm_6_group_2 = 1;}
		if(new_msg == "7 2") {m_Shm_7_group_2 = 1;}
		if(new_msg == "8 2") {m_Shm_8_group_2 = 1;}

		if(bytes > 10)
		{
			std::string text = new_msg;
			std::vector<std::string> tokens;

			size_t start = 0;
			size_t end = text.find(',');

			while (end != std::string::npos)
			{
				tokens.push_back(text.substr(start, end - start));
				start = end + 1;
				end = text.find(',', start);
			}
			tokens.push_back(text.substr(start));

			// for (const auto& str : tokens) std::cout << str << "\n";

			if(tokens.size() == 3)
			{
				int num = std::stoi(tokens[1]);
				if((num >= 0) && (num <= 1000))
				{
					double targetPosition = static_cast<double>(num) * 0.001 * 100.0;
					std::cout << "==" << targetPosition << std::endl;
					m_smeCommandPosition.set(targetPosition);
				}
			}
		}

		m_smeShm_1_group_1.set(m_Shm_1_group_1);
		m_smeShm_2_group_1.set(m_Shm_2_group_1);
		m_smeShm_3_group_1.set(m_Shm_3_group_1);
		m_smeShm_4_group_1.set(m_Shm_4_group_1);
		m_smeShm_5_group_1.set(m_Shm_5_group_1);
		m_smeShm_6_group_1.set(m_Shm_6_group_1);
		m_smeShm_7_group_1.set(m_Shm_7_group_1);
		m_smeShm_8_group_1.set(m_Shm_8_group_1);
		m_smeShm_1_group_2.set(m_Shm_1_group_2);
		m_smeShm_2_group_2.set(m_Shm_2_group_2);
		m_smeShm_3_group_2.set(m_Shm_3_group_2);
		m_smeShm_4_group_2.set(m_Shm_4_group_2);
		m_smeShm_5_group_2.set(m_Shm_5_group_2);
		m_smeShm_6_group_2.set(m_Shm_6_group_2);
		m_smeShm_7_group_2.set(m_Shm_7_group_2);
		m_smeShm_8_group_2.set(m_Shm_8_group_2);
    }
}

void shmToSocketFunction()
{
    while (running)
    {
    	usleep(1000000 / 25);

		m_smeActualPosition.get(m_actualPosition);
		m_smeTiLimitSwitchPressed.get(m_tiLimitSwitchPressed);
		m_smeTiClutchButtonPressed.get(m_tiClutchButtonPressed);

//		std::cout << "Posiation, LS, PB: " << m_actualPosition << ", " << m_tiLimitSwitchPressed << ", " << m_tiClutchButtonPressed << std::endl;

        std::string msg;

        // msg = "Posiation, LS, PB: " + std::to_string(m_actualPosition) + ", " + std::to_string(m_tiLimitSwitchPressed) + ", " + std::to_string(m_tiClutchButtonPressed);

        msg = std::to_string(m_actualPosition);

//        std::cout << "TX : ";
//        std::getline(std::cin, msg);

        if (msg == "exit")
        {
            running = false;
            break;
        }

        sendto(sockfd, msg.c_str(), msg.size(), 0, (sockaddr *)&remoteAddr, sizeof(remoteAddr));
    }
}

int main()
{
	std::cout << "IPC : TEST IPC QNX" << std::endl;

	shm_store.open("NEWshm1", 2000);

	m_smeActualPosition.access_element();
	m_smeCommandPosition.access_element();
	m_smeTiLimitSwitchPressed.access_element();
	m_smeTiClutchButtonPressed.access_element();

	m_smeShm_1_group_1.access_element();
	m_smeShm_2_group_1.access_element();
	m_smeShm_3_group_1.access_element();
	m_smeShm_4_group_1.access_element();
	m_smeShm_5_group_1.access_element();
	m_smeShm_6_group_1.access_element();
	m_smeShm_7_group_1.access_element();
	m_smeShm_8_group_1.access_element();
	m_smeShm_1_group_2.access_element();
	m_smeShm_2_group_2.access_element();
	m_smeShm_3_group_2.access_element();
	m_smeShm_4_group_2.access_element();
	m_smeShm_5_group_2.access_element();
	m_smeShm_6_group_2.access_element();
	m_smeShm_7_group_2.access_element();
	m_smeShm_8_group_2.access_element();

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    if (sockfd < 0)
    {
        perror("socket");
        return -1;
    }

    sockaddr_in localAddr{};

    localAddr.sin_family = AF_INET;
    localAddr.sin_port = htons(LOCAL_PORT);

    inet_pton(AF_INET, LOCAL_IP, &localAddr.sin_addr);
//    localAddr.sin_addr.s_addr = INADDR_ANY;

    char* sock = getenv("SOCK");

    if (sock)
        std::cout << "SOCK = " << sock << std::endl;
    else
        std::cout << "SOCK not set" << std::endl;

    if (bind(sockfd, (sockaddr *)&localAddr, sizeof(localAddr)) < 0)
    {
        perror("bind");
        return -1;
    }

    remoteAddr.sin_family = AF_INET;
    remoteAddr.sin_port = htons(REMOTE_PORT);

    inet_pton(AF_INET, REMOTE_IP, &remoteAddr.sin_addr);

    std::cout << "QNX UDP Ready" << std::endl;

    std::thread socketToShmThread(socketToShmFunction);
    std::thread shmToSocketThread(shmToSocketFunction);

    shmToSocketThread.join();

    running = false;

    close(sockfd);

    socketToShmThread.join();

    return 0;
}
