#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <chrono>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <cctype>

using namespace std;

void showKernelInfo()
{
    int fd = open("/dev/sysmon", O_RDONLY);

    if (fd < 0)
    {
        cerr << "Could not open /dev/sysmon\n";
        return;
    }

    char buffer[512];

    int bytesRead = read(fd, buffer, sizeof(buffer) - 1);

    if (bytesRead > 0)
    {
        buffer[bytesRead] = '\0';
        cout << buffer;
    }

    close(fd);
}

void showCPU()
{
    ifstream file("/proc/stat");

    string cpu;
    long long user, nice, system, idle;
    long long iowait, irq, softirq, steal;

    file >> cpu >> user >> nice >> system >> idle
         >> iowait >> irq >> softirq >> steal;

    long long idle1 = idle + iowait;

    long long total1 =
        user + nice + system + idle +
        iowait + irq + softirq + steal;

    this_thread::sleep_for(chrono::milliseconds(500));

    file.clear();
    file.seekg(0);

    file >> cpu >> user >> nice >> system >> idle
         >> iowait >> irq >> softirq >> steal;

    long long idle2 = idle + iowait;

    long long total2 =
        user + nice + system + idle +
        iowait + irq + softirq + steal;

    long long totalDiff = total2 - total1;
    long long idleDiff = idle2 - idle1;

    double usage = 0.0;

    if (totalDiff > 0)
        usage = 100.0 * (1.0 - (double)idleDiff / totalDiff);

    cout << "CPU Usage     : " << usage << "%\n";
}

void showMemory()
{
    ifstream file("/proc/meminfo");

    string line;

    long long total = 0;
    long long available = 0;

    while (getline(file, line))
    {
        if (line.find("MemTotal:") == 0)
            sscanf(line.c_str(), "MemTotal: %lld", &total);

        if (line.find("MemAvailable:") == 0)
            sscanf(line.c_str(), "MemAvailable: %lld", &available);
    }

    long long used = total - available;

    cout << "Memory Total  : " << total / 1024 << " MB\n";
    cout << "Memory Used   : " << used / 1024 << " MB\n";
    cout << "Memory Free   : " << available / 1024 << " MB\n";
}

void showProcesses()
{
    DIR *directory = opendir("/proc");

    if (directory == nullptr)
    {
        cout << "Could not access /proc\n";
        return;
    }

    cout << "\n[ RUNNING PROCESSES ]\n";
    cout << "PID\tPROCESS\n";
    cout << "------------------------------\n";

    struct dirent *entry;
    int count = 0;

    while ((entry = readdir(directory)) != nullptr && count < 10)
    {
        string name = entry->d_name;

        bool isNumber = !name.empty();

        for (char c : name)
        {
            if (!isdigit(c))
            {
                isNumber = false;
                break;
            }
        }

        if (!isNumber)
            continue;

        string pid = name;
        string statusPath = "/proc/" + pid + "/comm";

        ifstream processFile(statusPath);

        string processName;

        if (processFile >> processName)
        {
            cout << pid << "\t" << processName << "\n";
            count++;
        }
    }

    closedir(directory);
}

int main()
{
    cout << "\n";
    cout << "============================================\n";
    cout << "          LINUX SYSTEM MONITOR\n";
    cout << "============================================\n\n";

    cout << "[ KERNEL INFORMATION ]\n";
    showKernelInfo();

    cout << "\n[ USER-SPACE MONITORING ]\n";
    showCPU();
    showMemory();

    showProcesses();

    cout << "\n============================================\n";

    return 0;
}
