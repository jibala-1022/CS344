CS344
LAB_08
READERS-WRITERS PROBLEM

Execute repository.c before executing other programs.
It goes to sleep.
When all the programs are done executing, exit repository.c
Open 2 terminals to observe parallel execution of programs.
Semaphores are initialised and removed in repository.c


F. Execute writer.c in one terminal. It writes the file contents and waits befores exitting write mode. Execute another writer.c in another terminal. We observe that it waits for the previous process to release the lock. When we exit the previous process and release the lock, the other writer acquires lock and enters write mode.

G. Execute writer.c in one terminal. It writes the file contents and waits befores exitting write mode. Execute reader.c in another terminal. We observe that it waits for the previous process to release the lock. When we exit the previous process and release the lock, the reader acquires lock and enters read mode.

H. Execute reader.c in one terminal. It reads the file contents and waits befores exitting read mode. Execute reader.c in another terminal. We observe that it can also enter read mode and read the file contents in parallel.
