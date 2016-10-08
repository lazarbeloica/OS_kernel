Definitions of classes of the Kernel are in the h folder. 
A user should include thread.h if he/she wishes to create a thread, Thread control interface is given i the thread.h header. 
In similar way user is able to use semaphores (include semaphore.h) and events (include event.h)

Test files for running the Kernel are in the folder marked as test.
Compile these files with the ones from h and src folder.
If you wish to write your own program which uses implemented functions create a 
userMain function which will be called by the Os_kernel in similar way a standard C/C++ program calls the main function.
