#!/bin/bash
server=$1
port=$2

#server="tesla.agnd.net"
#port=22110

while IFS= read -r line; do
        echo "$line" | nc $server $port
done
