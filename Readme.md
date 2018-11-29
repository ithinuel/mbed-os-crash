# Additional information

From debugging sessions it seams that (at least) one of the TCPSocket gets allocation overlaps (at least) one of the reception buffer allocation generating the Fault we see.
- Could this be due to the allocator not being thread safe ?
- Or a some buffer overflow hidden by the static allocation ?
