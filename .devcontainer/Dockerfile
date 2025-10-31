# Use a standard base image
FROM mcr.microsoft.com/devcontainers/base:jammy
    
# Install system dependencies for PlatformIO Core
RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
    && apt-get install -y \
    curl \
    cmake \
    python3 \
    python3-pip \
    python3-venv \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

# Install PlatformIO Core
RUN pip3 install platformio