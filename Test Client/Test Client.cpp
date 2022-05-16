#include "base_client.h"

std::string server_ip = "13.67.161.91";
std::uint16_t server_port = 28019;


void wait_exit() {
    spdlog::info("Write anything to exit.");

    std::cin.get();
    WSACleanup();
}
int main()
{

    srand((unsigned)time(NULL) * 125);

    SetConsoleTitleA("YeetClient");

    spdlog::info("Initializing WSA.");
    WSAData data;
    WORD ver = MAKEWORD(2, 2);
    int wsResult = WSAStartup(ver, &data);


    base_client* client = new base_client();


    spdlog::info("Starting Queue Listener");
    std::thread m_clientqueuethread = std::thread([&]
        {
            client->queue_handler();
        });
    spdlog::info("Connecting to server ({}:{}).", server_ip, server_port);
    if (!client->start_connection(server_ip, server_port)) {
        spdlog::info("Failed to connect to server!");
        if (m_clientqueuethread.joinable())
            m_clientqueuethread.detach();

        wait_exit();
        return 0;
        
    }

    spdlog::info("Connected successfully");

    auto session = client->get_session();
    session->client->ping_sync.mtx.lock();
    while (1) {
        spdlog::info("{} | {}", session->client->ping_sync.get_elapsed_time(), session->client->ping_sync.get_current_ping());
        if (session->client->ping_sync.should_send_ping()) {
            spdlog::warn("Sending ping {}", session->client->ping_sync.get_current_ping()); 

            session->send_ping();
            session->client->ping_sync.on_update();
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    spdlog::info("received ping");


    wait_exit();

}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
