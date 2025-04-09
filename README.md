# MobiEdgeSim WIFI

## Features
- The author has developed a custom application based on INET's `UdpBasicApp` to measure round-trip time (RTT) between mobile clients and edge servers.
- An `Orchestrator` module is introduced to collect runtime metrics and make centralized task placement decisions.
- A movable edge server (`mecHost`) is implemented using Veins mobility modules, enabling it to follow realistic movement patterns defined in the SUMO configuration.

## Dependencies

This project requires the following OMNeT++ frameworks:

| Library | Version | Description |
|--------|---------|-------------|
| [Simu5G](https://github.com/Unipisa/Simu5G) | `v1.3.0` | Provides 5G network infrastructure support.  |
| [Veins](https://github.com/sommer/veins) | `v5.3.1` | Supports vehicular mobility using SUMO.|
| [INET](https://github.com/inet-framework/inet) | `v4.5.4` | Offers WiFi (IEEE 802.11), TCP/IP stack, routing, and general network models.|

## Quick Start

### Simu5G Clone and Checkout

```bash
git clone https://github.com/Unipisa/Simu5G.git && \
cd Simu5G && \
git checkout v1.3.0
. setenv -f
make makefiles
make MODE=release -j$(nproc) all

### Veins Clone and Chekout
```bash
git clone https://github.com/sommer/veins.git && \
cd veins && \
git checkout veins-5.3.1
. setenv # set environment variables
./configure # configure veins
make -j$(nproc) # build veins


### Inet Clone and Chekout
```bash
git clone https://github.com/inet-framework/inet.git && \
cd inet && \
git checkout v4.5.4
. setenv # set the environment variables
make makefiles # generate makefile
make -j$(nproc) # build inet


