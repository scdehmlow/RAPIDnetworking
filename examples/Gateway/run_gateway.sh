#!/bin/bash

stdbuf -oL ./RF24_gateway | ./send_to_database.sh winlink.from-il.com 22110

watch -n 1 60 
