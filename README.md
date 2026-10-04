# Linux System Monitor with Custom Kernel Module

## 1. Project Overview

This project is a Linux-based system monitoring application developed using C++ and a custom Linux kernel module.

The system collects information from both kernel space and user space and displays:

- Number of online CPUs
- Kernel uptime
- Total and free memory
- CPU utilization
- Memory utilization
- Running processes

The kernel information is exposed to the user-space application through a custom character device `/dev/sysmon`.

---

## 2. Project Objective

The main objective of this project is to understand the interaction between:

- Linux Kernel
- Kernel Modules
- Character Devices
- User-Space Applications
- Linux `/proc` filesystem
- C++ System Programming

---

## 3. System Architecture

```text
                    LINUX SYSTEM
                         |
              +----------+----------+
              |                     |
         Kernel Space           User Space
              |                     |
        sysmon.ko              monitor.cpp
              |                     |
       Character Device        /proc filesystem
              |                     |
         /dev/sysmon          CPU / Memory /
              |               Process information
              +----------+----------+
                         |
                    Final Output
