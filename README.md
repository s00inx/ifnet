# ifnet - interface netlink

Simple CLI tool for configuring network in Linux on pure C using Netlink sockets and no external libs (only posix api and linux headers). This project was created for deep dive into how utilities like `ip` and `tc` (from `iproute2`) work with Linux kernel on low level using netlink.  

## What I learned
This pet project was a massive leap for my understanding linux internals, here are challenges i faced:
#### Working with Netlink
I learned how to work with the Netlink subsystem to handle low-level communication between kernel space and user space. I gained practical experience setting up raw `AF_NETLINK` sockets with the `NETLINK_ROUTE` protocol and subscribing to multicast Netlink groups (`RTMGRP_LINK`, `RTMGRP_IPV4_IFADDR`) for asynchronous event tracking.

#### Working with kernel binary protocols (uapi)
I learned how to construct and parse raw binary network packets for the kernel from scratch. I gained practical experience with memory alignment, structuring nested kernel headers (`nlmsghdr`, `ifinfomsg`, `tcmsg`), and dynamically packing attributes using raw kernel macros (like `RTA_DATA`, `RTA_NEXT`, `NLMSG_DATA`, etc)

#### Reverse-Engineering Linux kernel internals (`sch_tbf.c`)
The most challenging part was implementing traffic shaping algorithm. I analyzed how TBF (Token Bucket Filter) works. 
I discovered that kernel does not accept speed in bytes/sec, it uses precalculated table (`rtab`) instead, i dissected kernel time scaling mechanism (PSCHED_SHIFT bit shift from `include/net/pkt_sched.h`), so i implemented pure nanosecond-based time calculation, that actually mirrors `sch_tbf.c`, avoiding integer truncation like in userspace utilities (like `tc`).

## Sources 
`man 7 netlink`, `man 7 rtnetlink`, `man 3 socket`, `man 3 netdevice`, and official `iproute2` source code

## Core Features

* `show` — parses `RTM_GETLINK` and `RTM_GETADDR` responses and display available interfaces and their ip addresses.
* `listen` — enters loop to capture kernel broadcast events.
* `set <interface> up/down` — toggles the state of a network interface using `RTM_NEWLINK`.
* `set <interface> mtu <value>` — updates the MTU via nested netlink attributes with boundary validation.
* `set <interface> speed <bytes/sec>` — installs TBF qdisc, automatically adjusting hardware `burst` bucket sizes and generating nanosecond-accurate `rtab` tokens.
* `del <interface> qdisc` — safely tears down the TBF queue discipline, restoring the interface to its unrestricted line speed.

## Quick start 
For install you should have gcc and linux header files on your device.
```bash 
git clone https://github.com/s00inx/ifnet
cd ifnet
make
```
There is some usage examples: 
```bash
# View active interfaces and addresses
./ifnet show 

# Listen to real-time network events
./ifnet listen

# Configuring interfaces (requires sudo)
sudo ./ifnet set lo mtu 1280
sudo ./ifnet set lo speed 262144
sudo ./ifnet del lo qdisc
```