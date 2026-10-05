# Linux System Monitor with Custom Kernel Module

## 1. Project Overview

Linux System Monitor is a Linux-based system monitoring project developed using C++ and a custom Linux kernel module.

The project demonstrates communication between user space and kernel space through a custom character device named /dev/sysmon.

The application also uses the Linux /proc virtual filesystem to collect CPU, memory, and process information.

The project has two main components:

1. sysmon.c - Custom Linux kernel module
2. monitor.cpp - C++ user-space monitoring application

The kernel module is compiled into sysmon.ko and creates the device /dev/sysmon.

The application displays:

1. Number of online CPUs
2. System uptime
3. Memory information
4. CPU utilization
5. Memory utilization
6. Process names


## 2. Problem Statement

Normal user-space applications cannot directly access many low-level kernel resources.

This project demonstrates how a C++ user-space application can communicate with the Linux kernel through a custom kernel module and character device.

It also demonstrates the use of the /proc virtual filesystem for obtaining runtime CPU, memory, and process information.


## 3. Objectives

The main objectives of the project are:

1. Understand Linux user space and kernel space.
2. Develop and load a Linux kernel module.
3. Create a character device.
4. Establish user-space and kernel-space communication.
5. Use Linux kernel APIs to obtain system information.
6. Understand the /proc virtual filesystem.
7. Monitor CPU and memory usage.
8. Display basic process information using C++.


## 4. Features

The system monitor provides:

1. Number of online CPUs
2. System uptime
3. Total and available memory
4. CPU utilization
5. Memory utilization
6. Running process names

The project obtains information from:

1. Custom kernel module through /dev/sysmon
2. /proc/stat
3. /proc/meminfo
4. /proc/<PID>/comm


## 5. System Architecture

The project consists of two main execution environments:

1. Kernel Space
2. User Space

The main communication flow is:

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

Additional information is obtained from:

/proc/stat
    |
    v
CPU Statistics

/proc/meminfo
    |
    v
Memory Information

/proc/<PID>/comm
    |
    v
Process Names

The complete architecture is:

LINUX SYSTEM

KERNEL SPACE                         USER SPACE

sysmon.c                             monitor.cpp
    |                                     |
    v                                     |
sysmon.ko                                |
    |                                     |
    v                                     |
/dev/sysmon <-----------------------------+
    |
    v
Linux Kernel
    |
CPU / Memory / Uptime

Additional Data:

/proc/stat
/proc/meminfo
/proc/<PID>/comm

All collected information is finally processed by monitor.cpp and displayed as the final system-monitoring output.


## 6. Project Structure

The project contains the following files:

1. sysmon.c - Source code of the Linux kernel module.
2. monitor.cpp - C++ system monitoring application.
3. Makefile - Used to build the kernel module.
4. README.md - Project documentation.
5. .gitignore - Prevents generated files from being committed.


## 7. Technologies Used

1. Linux
2. Ubuntu
3. C
4. C++
5. Linux Kernel Modules
6. Character Devices
7. /proc Virtual Filesystem
8. GNU Make
9. g++
10. Git and GitHub
11. VirtualBox


## 8. How the Project Works

### 8.1 Kernel Module

sysmon.c is compiled into sysmon.ko.

The kernel module performs the following operations:

1. Registers a character device.
2. Creates /dev/sysmon.
3. Implements open(), read(), and release() operations.
4. Collects CPU, memory, and uptime information.
5. Provides the information to the user-space application.
6. Cleans up resources when the module is removed.

Important kernel APIs used:

1. num_online_cpus() - Returns the number of online CPUs.
2. ktime_get_boottime_seconds() - Provides system uptime.
3. si_meminfo() - Provides system memory information.


### 8.2 User-Space Application

monitor.cpp communicates with /dev/sysmon.

It also reads:

/proc/stat

to calculate CPU utilization.

It reads:

/proc/meminfo

to calculate memory utilization.

It reads:

/proc/<PID>/comm

to obtain process names.

The collected information is combined and displayed as the final system-monitoring output.


## 9. Build and Run

Navigate to the project directory:

cd ~/sysmon

Build the kernel module:

make

Compile the C++ application:

g++ monitor.cpp -o monitor

Load the kernel module:

sudo insmod ./sysmon.ko

Verify the module:

lsmod | grep sysmon

Verify the character device:

ls -l /dev/sysmon

Run the application:

./monitor

After the demonstration, remove the kernel module:

sudo rmmod sysmon


## 10. Verification

The following commands can be used to verify different parts of the project.

### 10.1 Check Kernel Module

lsmod | grep sysmon

### 10.2 Check Character Device

ls -l /dev/sysmon

### 10.3 Check CPU Information

cat /proc/stat | head -n 1

### 10.4 Check Memory Information

cat /proc/meminfo | head

### 10.5 Check Running Processes

ls /proc | grep '^[0-9]' | head

### 10.6 Check Kernel Messages

dmesg | tail

### 10.7 Remove Kernel Module

sudo rmmod sysmon


## 11. Advantages

1. Demonstrates user-space and kernel-space communication.
2. Demonstrates Linux character-device concepts.
3. Uses a custom Linux kernel module.
4. Uses Linux kernel APIs.
5. Uses the /proc virtual filesystem.
6. Combines kernel-level and user-space information.
7. Demonstrates C++ system programming.


## 12. Future Enhancements

The project can be extended with:

1. Real-time continuous monitoring.
2. CPU usage per process.
3. Memory usage per process.
4. Disk usage monitoring.
5. Network statistics.
6. Process sorting and filtering.
7. Graphical user interface.
8. System resource alerts.


## 13. Learning Outcomes

This project provided practical understanding of:

1. Linux operating system concepts.
2. User space and kernel space.
3. Linux kernel modules.
4. Character devices.
5. file_operations.
6. Linux kernel APIs.
7. /proc virtual filesystem.
8. CPU and memory monitoring.
9. Process identification.
10. C++ system programming.
11. GNU Make.
12. Linux command-line tools.
13. Git and GitHub.


## 14. GitHub Repository

GitHub Repository:

https://github.com/Subhakant2004/linux-system-monitor


## 15. Author

Subhakant Biswal

B.Tech - Computer Science and Engineering

Linux System Programming Project
