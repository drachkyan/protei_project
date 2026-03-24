#!/bin/bash

{ 
  for i in {1..50}; do echo "add_vector"; sleep 0.1; echo "int"; sleep 0.1; echo "16"; sleep 0.1; echo "0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5"; done
  for i in {1..50}; do echo "send_vector"; sleep 0.1; done
} | ./myApp_client -i 0.0.0.0 -p 8082