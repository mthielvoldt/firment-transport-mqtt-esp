#!/bin/bash

cmake -B test/build -S test && \
  cmake --build test/build --target fmt_transport_test && \
  echo "Running fmt_transport_test" && test/build/fmt_transport_test