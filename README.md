# Operating-System---Programming-Assignment-1

Use the following commands in terminal to run:

- gcc producer.c -pthread -lrt -o producer
- gcc consumer.c -pthread -lrt -o consumer
- ./producer & ./consumer & 

KNOWN ISSUES:
- Upon starting, producer will run only once before locking. This can be fixed by running the "./producer & ./consumer &" command again. Running it a third time will ensure all processes are closed. 
