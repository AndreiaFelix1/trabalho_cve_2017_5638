#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "$0")/.."
make clean all test
echo
echo "Benchmark local:"
./build/benchmark
