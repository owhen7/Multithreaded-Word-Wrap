#include "wwheader.h"
#include "limits.h"

void enqueueDir(struct dirQueue* directory_queue, char* item)
{
    directory_queue->array[directory_queue->rear + 1] = *item;
    directory_queue->size = directory_queue->size + 1;
    //printf("%d enqueued to queue\n", item);
    return EXIT_SUCCESS;
}

// Function to remove an item from queue.
// It changes front and size
char dequeueDir(struct dirQueue* directory_queue)
{
    char item = directory_queue->array[directory_queue->front];
    directory_queue->front = (directory_queue->front + 1);
    directory_queue->size = directory_queue->size - 1;
    return item;
}

int isFull(struct fileQueue* file_queue)
{
    return (file_queue->size == file_queue->capacity);
}

// Queue is empty when size is 0
int isEmpty(struct fileQueue* file_queue)
{
    return (file_queue->size == 0);
}

// Function to add an item to the queue.
// It changes rear and size
void enqueueFile(struct fileQueue* file_queue, char* item)
{
    if (isFull(file_queue))
        return;
    file_queue->rear = (file_queue->rear + 1)
                  % file_queue->capacity;
    file_queue->array[file_queue->rear] = *item;
    file_queue->size = file_queue->size + 1;
   // printf("%d enqueued to queue\n", item);
}

// Function to remove an item from queue.
// It changes front and size
char dequeueFile(struct fileQueue* file_queue)
{
    if (isEmpty(file_queue))
        return NULL;
    char item = file_queue->array[file_queue->front];
    file_queue->front = (file_queue->front + 1) % file_queue->capacity;
    file_queue->size = file_queue->size - 1;
    return item;
}

//creating static memory array that will act as our memory location to dynamically allocate data
static char memorySpace1[MEMSIZE1];
static char memorySpace2[MEMSIZE2];

//a void pointer that acts as a head pointer of our queue
void* startPoint1 = (dirQueue*)(memorySpace1);
void* startPoint2 = (dirQueue*)(memorySpace2);

void *directoryWorker(void* initialPath)
{

    struct dirQueue *basePath = initialPath;
    dirQueue* directory_queue = startPoint1;
    fileQueue* file_queue = startPoint2;

    if(directory_queue->array == NULL){
         //struct dirQueue* directory_queue = (struct dirQueue*)malloc(sizeof(struct dirQueue));
        directory_queue->front = directory_queue->size = 0;

        // This is important, see the enqueue
        directory_queue->rear = 0;
        directory_queue->array = (char*)malloc(30000);
    }

    if(file_queue->array == NULL){
         //struct dirQueue* directory_queue = (struct dirQueue*)malloc(sizeof(struct dirQueue));
        unsigned capacity = 30000;
        file_queue->capacity = capacity;
        file_queue->front = file_queue->size = 0;

        // This is important, see the enqueue
        file_queue->rear = capacity - 1;
        file_queue->array = (char*)malloc(file_queue->capacity * sizeof(char));

    }

    if(isDirectory(basePath->array) == 0)   // If the file is not a folder, it should be a text file.
    {
         enqueueFile(file_queue, basePath->array);
         return EXIT_SUCCESS;
    }

    for(int i = basePath->start; i< basePath->stop; i++){
        char path[1000];
        struct dirent *dp;
        DIR *dir = opendir(directory_queue[i].array);

        if (!dir){
             return EXIT_FAILURE;
        }

        while ((dp = readdir(dir)) != NULL)
        {
            if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
            {
                printf("%s\n", dp->d_name);
                strcpy(path, directory_queue[i].array);
                strcat(path, "/");
                strcat(path, dp->d_name);

                    if(isDirectory(dp->d_name) == 0) //For some reason, isDirectory is producing the wrong output in cases I've tested. It thinks all files are not directories.
                        {
                            if((strstr(dp->d_name, "wrap.") == NULL)) //if the string doesn't contain "wrap." as prefix
                            {
                                if(!(*dp->d_name == '.')) //if the string doesn't contain "." as prefix
                                {
                                    enqueueFile(file_queue, path);
                                }
                            }

                        }
                    else
                        enqueueDir(directory_queue, path);
            }
        }

        closedir(dir);
    }


                    if(directory_queue->size <= readThreads){
                            //wait for active threads to finish
                            for(int i=1; i<numReadThreads;i++){
                                    pthread_join(readtids[i],NULL);
                                    numReadThreads--;
                            }

                            int threadsToCreate = directory_queue->size;
                            int readChunk = directory_queue->size / threadsToCreate;
                            for(int i=1;i<threadsToCreate;i++){
                                directory_queue[i].start = readChunk * i;
                                directory_queue[i].stop = readChunk * (i+1);
                               pthread_create(&readtids[numReadThreads],NULL,directoryWorker,&directory_queue[i]);
                               numReadThreads += 1;
                            }
                    }
                    else{
                            //wait for active threads to finish
                            for(int i=0; i<numReadThreads;i++){
                                    pthread_join(readtids[i],NULL);
                                    numReadThreads--;
                            }

                            int threadsToCreate = readThreads;
                            int readChunk = directory_queue->size / threadsToCreate;
                            for(int i=0;i<threadsToCreate;i++){
                                directory_queue[i].start = readChunk * i;
                                directory_queue[i].stop = readChunk * (i+1);
                                pthread_create(&readtids[numReadThreads],NULL,directoryWorker,&directory_queue[i]);
                                numReadThreads += 1;
                            }
                    }

    }
