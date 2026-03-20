#include "../../Network/src/Server.cpp"

int main(int argc, char** argv)
{
    Logger::Get().Init("Server_Log.txt");

    if (argc == 2)
    {
        if (std::string(argv[1]) == "-clean")
            Logger::Get().CleanUp();
    }

    Server server_instance(56000);
    server_instance.Start();

    while (true)
    {
        server_instance.Update(-1, false);
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}