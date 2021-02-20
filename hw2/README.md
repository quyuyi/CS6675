# HW2 OpenDHT

## Asynchronous
all operations are put into a queue to be processed by the dht working thread, so if you want to check the result, you have to wait for the operations to complete.