By Owen Wexler (osw9) and Huzaif Mansuri (htm23)


We did not finish our code in time, but we hope that enough of it is present to demonstrate our progress and show our intended solution.
Nevertheless, here would be proposed tests to determine whether our program passed certain specifications of the project.

1. Our program must not open many directories at the same time via normal recursion. It should instead open one directory at a time, make note of the file paths of any subdirectories it encounters, and then put all the files paths into a queue. Then, it should open and close these subdirectories afterwards so that only N amount are open at a time, where N is the amount of threads running, rather than infinitely recursively many.
The amount of open directories allowed appears to be 64,000 on linux, so it would not be feasible to test the limits of this by inventing a test case with a lot of nested directories to try to reach the limit. Instead, we could implement print/sleep statements to verify that the code is not being wrapped at the same time that the directories are being traversed. One is happening before the other.


2. We have however re-created out wordwrap function to include -rM,N (recursive multithreading arguments). Our multithreading approach was to create a thread then have the directoryWorker work to sort out the directories traversed into the directory_queue, and enqueue the files traversed into the file_queue. Then, we could have other set of N threads work on getting files from file_queue to wrap them.
