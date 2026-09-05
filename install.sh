#!/usr/bin/env bash

# Exit immediately if a command exits with a non-zero status
set -e

# Ensure the script is run as root or with sudo privileges
if [ "$EUID" -ne 0 ]; then
  echo "Please run as root or with sudo."
  exit 1
fi

echo "Updating package lists..."
apt-get update -y

echo "Installing essential OS development tools and dependencies..."
apt-get install -y \
  build-essential \
  nasm \
  xorriso \
  grub-pc-bin \
  grub-common \
  mtools \
  genisoimage \
  qemu-system-x86 \
  gdb \
  git \
  make \
  gcc \
  libc6-dev-i386

echo "Verifying installation versions..."
nasm -v
xorriso -version
qemu-system-x86_64 --version
gcc --version

echo "OS development environment installation completed successfully!"
