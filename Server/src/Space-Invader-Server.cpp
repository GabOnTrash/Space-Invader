#include "../../Network/src/Server.cpp"
#include <thread>

#define HISTORY 100

std::vector<std::string> splitInTokens(const std::string& s)
{
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);

    while (tokenStream >> token)
        tokens.push_back(token);

    return tokens;
}

void cliThread(Server& srv)
{
    using CommandFunc = std::function<void(const std::vector<std::string>&)>;

    std::deque<std::vector<std::string>> history;
    std::unordered_map<std::string, CommandFunc> commands;

    commands["reboot"] = [&] (auto args) { srv.Stop(); srv.Start(); LOG_INFO_CONSOLE("Server rebooted!"); };
    commands["stop"] = [&] (auto args){ srv.Stop(); exit(0); };
    commands["plist"] = [&] (auto args) { srv.ListPlayers(); };
    commands["cleanup"] = [&] (auto args) { Logger::Get().CleanUp();  LOG_ERROR_CONSOLE("Log file was cleaned"); };
    commands["help"] = [&] (auto args)
    {
        LOG_INFO_CONSOLE("Available commands:");
        for (const auto& [name, _] : commands) LOG_INFO_CONSOLE("- " + name);
    };
    commands["kick"] = [&] (auto args)
    {
        uint32_t id;
        if (args.size() < 1)
        {
            LOG_WARN_CONSOLE("Usage: kick <ID>");
            return;
        }
        try { id = std::stoi(args[0]); }
        catch (std::exception& ex) { LOG_ERROR_CONSOLE("Not a valid ID"); return; }

        if (srv.KickPlayer(id))
            LOG_WARN_CONSOLE("Kicked player " + std::to_string(id));
        else
            LOG_ERROR_EVERYWHERE("Could not kick player with id #" + std::to_string(id));
    };
    commands["history"] = [&] (auto args)
    {
        size_t quantity = 0;

        switch (args.size())
        {
        case 0:
        {
            quantity = history.size();
            break;
        }
        case 1:
        {
            try { quantity = std::stoi(args[0]); }
            catch (std::exception& ex) { LOG_ERROR_CONSOLE("Not a valid quantity"); return; }
            break;
        }
        default:
            LOG_WARN_CONSOLE("Usage: history <quant/empty>");
            return;
        }

        LOG_DEBUG_CONSOLE("Commands history of last " + std::to_string(quantity) + " commands, most to least recent");
        for (size_t i = 0; i < quantity; i++)
        {
            std::ostringstream cmd;
            for (const auto& item : history[i])
                cmd << item << " ";

            LOG_DEBUG_CONSOLE(std::to_string(i + 1) + ". " + cmd.str());
        }
    };

    std::string line;
    while (true)
    {
        if (!std::getline(std::cin, line)) break;
        if (line.empty())
        {
            std::cout << "> " << std::flush;
            continue;
        }

        auto parts = splitInTokens(line);
        std::string cmdName = parts[0];                 // command

        if (commands.find(cmdName) != commands.end())
        {
            if (history.size() > HISTORY)
                history.pop_back();

            if (cmdName != "history" && cmdName != "help")
                history.emplace_front(parts);

            try
            {
                parts.erase(parts.begin());
                commands[cmdName](parts);
                line.clear();
            }  // function call w/ params
            catch (std::exception& ex) { LOG_ERROR_CONSOLE("Error while executing command: " + std::string(ex.what())); }
        }
        else
            LOG_WARN_CONSOLE("Unknown command: " + cmdName + ", type 'help' to find suggestions");
    }
}

int main(int argc, char** argv)
{
    Logger::Get().Init("Server_Log.txt");

    if (argc != 2)
    {
        LOG_WARN_CONSOLE("Usage: " + std::string(argv[0]) + " <port>");
        return 1;
    }

    try
    {
        std::stoi(argv[1]);
    }
    catch (std::exception& ex)
    {
        LOG_ERROR_CONSOLE("Couldn't convert input to port");
        return 1;
    }

    Server server_instance(std::stoi(argv[1]));
    server_instance.Start();

    std::thread cli(cliThread, std::ref(server_instance));
    cli.detach();

    while (true)
    {
        server_instance.Update(-1, false);
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}