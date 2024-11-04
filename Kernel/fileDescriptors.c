#include "../include/fileDescriptors.h"
#include "../include/memoryManager.h"
#include "../include/videoDriver.h"


FD fileDescriptors[MAX_FDS];
void dummy3(){};
void initFileDescriptors()
{
   for (int i = 3; i < MAX_FDS; i++) {
        fileDescriptors[i].isOpen = 0;
    }

    fileDescriptors[0].isOpen = 1;
    fileDescriptors[0].mode = RW;
    fileDescriptors[0].resource = mallocMM(sizeof(Stream));
    fileDescriptors[0].resource->readPos = 0;
    fileDescriptors[0].resource->writePos = 0;
    fileDescriptors[0].resource->readSem = semCreateBy(0,2);
    fileDescriptors[0].resource->writeSem = semCreateBy(BUFFER_SIZE,2);
  

    fileDescriptors[1].isOpen = 1;
    fileDescriptors[1].mode = W;
    fileDescriptors[1].resource = NULL;

    fileDescriptors[2].isOpen = 1;
    fileDescriptors[2].mode = W;
    fileDescriptors[2].resource = NULL;
}


int writeToFD(int fd, char *buf, uint64_t count,uint64_t hexColor){
    int written = 0;
    if (fd < 0 || fd >= MAX_FDS || !fileDescriptors[fd].isOpen || fileDescriptors[fd].mode == R)
        return -1;
        
    if (fd == STDOUT || fd == STDERR){
        dummy3();  
        int color = (fd == STDERR)? 0x00FF0000 : hexColor;
        char toPrint[2] = {0, 0};
        for (written = 0; written < count; written++)
        {
            toPrint[0] = buf[written];
            vdPrint(toPrint, color);
        }
        return written;
    }
        
    Stream *stream = fileDescriptors[fd].resource;
   
    for (written = 0; written < count; written++)
    {
        semWait(stream->writeSem);
        stream->buffer[stream->writePos] = buf[written];
        stream->writePos = (stream->writePos + 1) % BUFFER_SIZE;
        semPost(stream->readSem);
    }
    return written;
}


int readFromFD(int fd, char *buf, uint64_t count){
    int sizeRead = 0;
    if (fd < 0 || fd >= MAX_FDS || !fileDescriptors[fd].isOpen || fileDescriptors[fd].mode == W)
        return -1;
    unsigned char lastRead = '\0';
    Stream *stream = fileDescriptors[fd].resource;
    while (sizeRead != count)
    {
        semWait(stream->readSem);
        lastRead = stream->buffer[stream->readPos];
        stream->readPos = (stream->readPos + 1) % BUFFER_SIZE;
        semPost(stream->writeSem);
        buf[sizeRead++] = lastRead;
    }
    return sizeRead;
}

int getAvailableFD(){
    for (int i = 3; i < MAX_FDS; i++)
    {
        if (!fileDescriptors[i].isOpen)
        {
            fileDescriptors[i].isOpen = 1;
            return i;
        }
    }
    return -1;
}

int createPipe(int fds[2]){
    int fd1 = getAvailableFD(), fd2 = getAvailableFD();
    if (fd1 == -1 || fd2 == -1)
    return -1;
    Stream *stream = mallocMM(sizeof(Stream));
    fileDescriptors[fd1].mode = R;
    fileDescriptors[fd1].resource = stream;
    fileDescriptors[fd1].resource->readPos = 0;
    fileDescriptors[fd1].resource->readSem = semOpen(NULL,0);
    fileDescriptors[fd2].mode = W;
    fileDescriptors[fd2].resource = stream;
    fileDescriptors[fd2].resource->writePos = 0;
    fileDescriptors[fd2].resource->writeSem = semOpen(NULL,BUFFER_SIZE);
    fds[0] = fd1;
    fds[1] = fd2;
    return 0;
}

