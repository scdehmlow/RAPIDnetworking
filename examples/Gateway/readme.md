To get a properly formatted master node which connects to the database, first navigate to this directory. Next, make the makefile simply typing "make" into the command line then entering. Next, change permissions of the .sh files with the following commands "chmod +x send_to_database.sh" and "chmod +x run_gateway.sh". To run the gateway, simply use the command "sudo ./run_gateway.sh".

If you find that after a while of changing nodes in and out of the mesh, your messages suddenly stop being received, try removing the file dhcplist.txt under this directory.
