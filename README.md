# Operating-System---Programming-Assignment-1

Use the following commands in terminal to run:

- gcc producer.c -pthread -lrt -o producer
- gcc consumer.c -pthread -lrt -o consumer
- ./producer & ./consumer & 

KNOWN ISSUES:
- Occasionally, a process will not print a "Done" pthread line in the terminal. This can be fixed by using CTRL+C as the process has already finished.
- Upon starting, producer will run only once before locking. This can be fixed by running the "./producer & ./consumer &" command again.
