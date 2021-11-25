#-------------------------------------------------------------------------------------------------------------
# Copyright (c) Microsoft Corporation. All rights reserved.
# Licensed under the MIT License. See https://go.microsoft.com/fwlink/?linkid=2090316 for license information.
#-------------------------------------------------------------------------------------------------------------

FROM fedora:latest

# Configure apt
RUN dnf -y update && dnf -y install clang clang-tools-extra git cmake ninja-build && dnf clean all

# Set the default shell to bash instead of sh
ENV SHELL /bin/bash
