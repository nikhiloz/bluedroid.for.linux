# Multi-stage Docker build for bluedroid.for.linux

# Stage 1: Builder
FROM ubuntu:22.04 AS builder

LABEL maintainer="nikhiloz"
LABEL description="Bluedroid Bluetooth Stack for Linux - Build Environment"

ENV DEBIAN_FRONTEND=noninteractive

# Install build dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    libpthread-stubs0-dev \
    libasound2-dev \
    pkg-config \
    valgrind \
    cppcheck \
    && rm -rf /var/lib/apt/lists/*

# Set working directory
WORKDIR /app

# Copy source code
COPY . .

# Build
RUN mkdir build && cd build && \
    cmake -DCMAKE_BUILD_TYPE=Release .. && \
    make -j$(nproc)

# Stage 2: Runtime
FROM ubuntu:22.04

LABEL description="Bluedroid Bluetooth Stack Runtime"

ENV DEBIAN_FRONTEND=noninteractive

# Install runtime dependencies only
RUN apt-get update && apt-get install -y \
    libasound2 \
    libpthread-stubs0 \
    bluez \
    bluez-tools \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

# Copy built artifacts from builder
COPY --from=builder /app/build/bin /app/bin
COPY --from=builder /app/build/lib /app/lib
COPY --from=builder /app/docs /app/docs
COPY --from=builder /app/README.md /app/README.md
COPY --from=builder /app/LICENSE /app/LICENSE

# Set library path
ENV LD_LIBRARY_PATH=/app/lib:$LD_LIBRARY_PATH
ENV PATH=/app/bin:$PATH

# Bluetooth device access
VOLUME ["/dev/ttyUSB0"]

# Health check
HEALTHCHECK --interval=30s --timeout=10s --start-period=5s --retries=3 \
    CMD test -d /app/bin || exit 1

# Default command
CMD ["/bin/bash"]
