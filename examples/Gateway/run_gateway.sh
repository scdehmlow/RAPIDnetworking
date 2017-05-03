#!/bin/bash

stdbuf -oL ./RF24Mesh_Example_Master | ./send_to_database.sh winlink.from-il.com 22110

