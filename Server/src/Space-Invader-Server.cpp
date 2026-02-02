#include "../../Network/src/Server.cpp"

int main()
{
    Server server_instance(54000);
    server_instance.Start();
    
    while (true)
    {
        server_instance.Update(-1, true);
    }
}