# Arduino Examples
These examples only send exactly the number of bytes that they need so there will be no more trailing zeros ready by the gateway device. This update also allows all of the senor nodes to use the same header files for the nanopb encoding functions. This also means that they all use the same proto file. The universal proto file is called RAPID.

There are now only two example files to work from. Both examples send data every 60 seconds but the relay example doesn't use a sleep function so that it can run mesh.update() continually.
