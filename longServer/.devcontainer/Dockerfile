#-------------------------------------------------------------------------------------------------------------
# Copyright (c) Microsoft Corporation. All rights reserved.
# Licensed under the MIT License. See https://go.microsoft.com/fwlink/?linkid=2090316 for license information.
#-------------------------------------------------------------------------------------------------------------

FROM fedora:latest

# Configure apt
RUN dnf -y update && dnf -y install clang clang-tools-extra git

# Install C++ tools
RUN dnf -y install cmake make cppcheck valgrind boost-devel fmt-devel mariadb-connector-c-devel range-v3-devel

# Clean up
RUN dnf autoremove -y

# Set the default shell to bash instead of sh
ENV SHELL /bin/bash
