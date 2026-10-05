Linux System Monitor with Custom Kernel Module
A Linux system-monitoring project built using C++ system programming and a custom Linux kernel module.
The project demonstrates communication between user space and kernel space through a custom character device, while also using the Linux /proc virtual filesystem to collect additional runtime information.

1. Project Overview
The project consists of two main components:
·	Kernel-space component:sysmon.c, compiled into sysmon.ko
·	User-space component:monitor.cpp, compiled into the monitor executable
The kernel module creates a character device:
/dev/sysmon

The C++ application reads kernel-provided information through this device and also reads the /proc filesystem for CPU, memory, and process information.
Information displayed
·	Number of online CPUs
·	Kernel/system uptime
·	Total memory
·	Free memory
·	CPU utilization
·	Memory utilization
·	Running process names

2. Problem Statement
Linux provides system information through kernel interfaces and virtual filesystems, but accessing low-level kernel information directly from a normal user-space program is restricted.
This project demonstrates how a user-space C++ application can communicate with a custom kernel module through a character device and combine that information with data available through /proc.

3. Objectives
The main objectives are:
1.	Understand the difference between user space and kernel space.
2.	Develop a basic Linux kernel module.
3.	Register and create a character device.
4.	Establish user-space ↔ kernel-space communication through /dev/sysmon.
5.	Use Linux kernel APIs to obtain system information.
6.	Read and interpret data from the /proc filesystem.
7.	Calculate CPU and memory utilization in a C++ application.
8.	Understand basic Linux system programming concepts.

4. System Architecture
                         LINUX SYSTEM
                              |
             +----------------+----------------+
             |                                 |
        KERNEL SPACE                       USER SPACE
             |                                 |
       +-----v-----+                     +-----v------+
       | sysmon.ko |                     | monitor.cpp|
       |  Kernel   |                     |    C++     |
       |  Module   |                     | Application|
       +-----+-----+                     +-----+------+
             |                                 |
             v                                 v
       +-------------+                    +-----------+
       | /dev/sysmon |                    |   /proc   |
       | Character   |                    |  Virtual  |
       |   Device    |                    | Filesystem|
       +------+------+                    +-----+-----+
              |                                 |
              |                          +------+------+
              |                          |             |
              |                         CPU         Memory/
              |                         stats       Processes
              |                          |             |
              +-------------+------------+-------------+
                            |
                            v
                     Final Monitor Output

Main communication path
monitor.cpp
     |
     v
/dev/sysmon
     |
     v
sysmon.ko
     |
     v
Linux Kernel

Additional information source
/proc/stat       -> CPU statistics
/proc/meminfo    -> Memory information
/proc/<PID>/comm -> Process names


5. Project Structure
linux-system-monitor/
│
├── sysmon.c
├── monitor.cpp
├── Makefile
├── README.md
└── .gitignore

File descriptions
File	Purpose
sysmon.c	Custom Linux kernel module
monitor.cpp	C++ user-space monitoring application
Makefile	Builds the kernel module
README.md	Project documentation
.gitignore	Prevents generated build files from being committed


6. Technologies Used
·	Linux / Ubuntu
·	C
·	C++
·	Linux Kernel Module
·	Character Device
·	Linux /proc filesystem
·	g++
·	GNU Make
·	Git / GitHub
·	VirtualBox for the Linux development environment

7. Kernel Module — sysmon.c
sysmon.c is the kernel-space component of the project.
The module:
1.	Registers a character-device number.
2.	Initializes a cdev structure.
3.	Registers the character device with the kernel.
4.	Creates a device class.
5.	Creates /dev/sysmon.
6.	Implements basic file operations.
7.	Reads system information using Linux kernel APIs.
8.	Copies the generated information to user space.
9.	Cleans up all registered resources when unloaded.
Main file operations
The module implements:
open()
read()
release()

These operations are connected through the Linux file_operations structure.

8. Kernel Information Collected
The kernel module uses Linux kernel APIs to obtain system information.
Online CPUs
num_online_cpus()

Returns the number of CPUs currently online.
System uptime
ktime_get_boottime_seconds()

Provides the system uptime in seconds.
Memory information
si_meminfo()

Provides information about system memory.

9. Character Device — /dev/sysmon
A character device provides a way for a user-space program to communicate with a kernel component.
In this project:
User Space
    |
monitor.cpp
    |
    v
/dev/sysmon
    |
    v
sysmon.ko
    |
    v
Kernel

The C++ application opens /dev/sysmon and reads the information exposed by the kernel module.
This is the main user-space ↔ kernel-space communication demonstrated by the project.

10. /proc Filesystem
Linux provides the /proc filesystem as a virtual filesystem containing runtime information about the system.
This project uses three areas of /proc.
/proc/stat
Used to obtain CPU time counters.
The application takes two samples separated by a short interval and uses the difference between the samples to estimate CPU utilization.
/proc/stat
    |
    +-- CPU counters
          |
          v
    CPU utilization

/proc/meminfo
Used to obtain memory information such as:
·	MemTotal
·	MemAvailable
Memory usage is estimated from these values.
/proc/<PID>/comm
Numeric directories under /proc represent process IDs.
For example:
/proc/1/
/proc/100/
/proc/250/

The comm file inside a process directory contains the process name.
The application scans numeric /proc directories and displays process names.

11. How the Application Works
When the user runs:
./monitor

the C++ application performs two main tasks.
Task 1 — Communicate with the kernel module
monitor.cpp
     |
     | open/read
     v
/dev/sysmon
     |
     v
sysmon.ko
     |
     v
CPU / Memory / Uptime

Task 2 — Read /proc
monitor.cpp
     |
     +--> /proc/stat
     |       |
     |       +--> CPU utilization
     |
     +--> /proc/meminfo
     |       |
     |       +--> Memory utilization
     |
     +--> /proc/<PID>/comm
             |
             +--> Process names

The application combines these values and prints the final system-monitor output.

12. Build Instructions
Open a terminal in the project directory:
cd ~/sysmon

Build the kernel module
make

This generates:
sysmon.ko

Compile the C++ application
g++ monitor.cpp -o monitor

This generates:
monitor


13. Running the Project
Step 1 — Load the kernel module
sudo insmod ./sysmon.ko

Step 2 — Verify the module
lsmod | grep sysmon

Step 3 — Verify the device
ls -l /dev/sysmon

Step 4 — Run the monitor
./monitor

The application displays the collected system information.

14. Example Execution Flow
$ make
        |
        v
   sysmon.ko

$ g++ monitor.cpp -o monitor
        |
        v
     monitor

$ sudo insmod ./sysmon.ko
        |
        v
   Kernel module loaded

$ ls -l /dev/sysmon
        |
        v
   Character device exists

$ ./monitor
        |
        +--> /dev/sysmon
        |       |
        |       +--> Kernel information
        |
        +--> /proc/stat
        |       |
        |       +--> CPU usage
        |
        +--> /proc/meminfo
        |       |
        |       +--> Memory usage
        |
        +--> /proc/<PID>/comm
                |
                +--> Processes

        |
        v
   Final system-monitor output


15. Verification Commands
Check loaded module
lsmod | grep sysmon

Check device
ls -l /dev/sysmon

Check kernel messages
dmesg | tail

Check CPU statistics
cat /proc/stat | head -n 1

Check memory information
cat /proc/meminfo | head

Check processes
ls /proc | grep '^[0-9]' | head

Remove the module
sudo rmmod sysmon

After removal, the custom kernel module is no longer loaded.

16. Important Linux Concepts Demonstrated
User Space
The area where normal applications execute.
In this project:
monitor.cpp

runs in user space.
Kernel Space
The privileged area where the Linux kernel and kernel modules execute.
In this project:
sysmon.ko

runs in kernel space.
Kernel Module
A loadable piece of code that extends kernel functionality without rebuilding the entire kernel.
Character Device
A device interface that allows data to be transferred between user-space applications and kernel code.
/proc
A virtual filesystem that exposes runtime information maintained by the Linux kernel.

17. Advantages
·	Demonstrates real user-space and kernel-space interaction.
·	Uses a custom character device instead of only reading /proc.
·	Combines kernel-level and user-space system information.
·	Uses C++ for Linux system programming.
·	Provides a simple command-line monitoring interface.
·	Helps understand basic Linux kernel module development.

18. Limitations
·	The project is a basic educational system monitor.
·	Process information is limited to the process names displayed by the application.
·	CPU utilization is calculated from sampled /proc/stat values rather than being provided directly by the kernel module.
·	The interface is command-line based.
·	It is intended for Linux systems and requires compatible kernel headers to build the module.

19. Future Enhancements
Possible future improvements include:
·	Continuous real-time monitoring.
·	CPU usage per process.
·	Memory usage per process.
·	Disk and network statistics.
·	Better process filtering and sorting.
·	A graphical user interface.
·	More kernel-exposed metrics through the character device.
·	Logging system statistics to a file.

20. Learning Outcomes
Through this project, the following concepts were practiced:
·	Linux command-line environment
·	C++ system programming
·	Linux kernel modules
·	Character devices
·	file_operations
·	User-space ↔ kernel-space communication
·	Kernel APIs
·	/proc filesystem
·	CPU and memory monitoring
·	Process identification
