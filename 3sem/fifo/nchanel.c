#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#define BUFSIZE 1024
#define NAMEFILE_MAX 30

#define PRINT_HELP                      do{\
                                            printf("Wrong arguments\n");\
                                            printf("Usage:\n"\
                                            "\"%s <SRCFILE>\" for sender mode\n"\
                                            "\"%s\" for receiver mode\n", argv[0], argv[0]);\
                                        }while(0);
                        
const char *fifo="./fifo";
const char *list="./list";

char* getstrpid(pid_t, char);
void terminate(char*, int);

int main(int argc, char *argv[]){
    char err_str[50];
    char emp_str[] = "";
    int errnum;
    
    switch(argc){
        case 2: {   //sender
            
            #define SET_ERR(pref, file)     {\
                                                errnum = errno;                         \
                                                strcpy(err_str, argv[0]);               \
                                                strcat(err_str, " (sender): ");         \
                                                strcat(err_str, pref);                  \
                                                strcat(err_str, file);                  \
                                                strcat(err_str, ": ");                  \
                                                write(rcvmesfd, "E", 1);                \
                                                close(rdlfd);                           \
                                                close(rcvchfd);                         \
                                                close(rcvmesfd);                        \
                                                terminate(err_str, errnum);             \
                                            }                                           
            int buf[BUFSIZE]; 
            pid_t pid;
            int rdlfd;
            int rcvchfd;
            int rcvmesfd;
            int srcfilefd;
            int bytesnum;
            
            if ((rdlfd = open(list, O_RDONLY)) < 0) 
                SET_ERR("Cannot open file ", list);
            if (read(rdlfd, (void*)&pid, sizeof(pid_t)) < 0)
                SET_ERR("Cannot read from file ", list);
            if ((rcvchfd = open(getstrpid(pid, 'C'), O_WRONLY)) < 0)
                SET_ERR("Cannot open file ", getstrpid(pid, 'C'));
            if ((rcvmesfd = open(getstrpid(pid, 'M'), O_WRONLY)) < 0)
                SET_ERR("Cannot open file ", getstrpid(pid, 'M'));
            if ((srcfilefd = open(argv[1], O_RDONLY)) < 0)
                SET_ERR("Cannot open file ", argv[1]);
            if (write(rcvmesfd, "O", 1) < 0)    //OK
                SET_ERR("Cannot write into file ", getstrpid(pid, 'M'));
            
            while((bytesnum = read(srcfilefd, buf, BUFSIZE)) > 0){
                if (bytesnum < 0)
                    SET_ERR("Cannot read from file ", argv[1]);
                if (write(rcvchfd, buf, bytesnum) < 0)
                    SET_ERR("Cannot write into file ", getstrpid(pid, 'C'));
            }

            if (write(rcvmesfd, "C", 1) < 0)    //Completed
                SET_ERR("Cannot write into file ", getstrpid(pid, 'M'));
            close(rdlfd);
            close(rcvchfd);
            close(rcvmesfd);
            exit(EXIT_SUCCESS);
            
            break;
        }
        
        case 1: {   //receiver
            
            #undef SET_ERR
            #define SET_ERR(pref, file)         {                                           \
                                                    errnum = errno;                         \
                                                    strcpy(err_str, argv[0]);               \
                                                    strcat(err_str, " (receiver): ");       \
                                                    strcat(err_str, pref);                  \
                                                    strcat(err_str, file);                  \
                                                    strcat(err_str, ": ");                  \
                                                    close(rcvchfd);                         \
                                                    close(rcvmesfd);                        \
                                                    unlink((char*)chanfifo);                \
                                                    unlink((char*)mesfifo);                 \
                                                    terminate(err_str, errnum);             \
                                                }     
            
            int buf[BUFSIZE];
            pid_t pid;
            int wrlfd;
            int rcvchfd;
            int rcvmesfd;
            char chanfifo[NAMEFILE_MAX];
            char mesfifo[NAMEFILE_MAX];
            char mes;
            int bytesnum;
            
            pid = getpid();
            strcpy((char*)chanfifo, getstrpid(pid, 'C'));
            strcpy((char*)mesfifo, getstrpid(pid, 'M'));
            if (mkfifo((char*)chanfifo, 0666 | O_CREAT | O_EXCL) < 0)
                SET_ERR("Cannot create file ", chanfifo);
            if (mkfifo((char*)mesfifo, 0666 | O_CREAT | O_EXCL) < 0)
                SET_ERR("Cannot create file ", mesfifo);
            if ((wrlfd = open(list, O_WRONLY)) < 0)
                SET_ERR("Cannot open file ", list);
            if (write(wrlfd, (void*)&pid, sizeof(pid_t)) < 0)
                SET_ERR("Cannot write into file ", list);
            if ((rcvchfd = open((char*)chanfifo, O_RDONLY)) < 0)
                SET_ERR("Cannot open file ", chanfifo);
            if ((rcvmesfd = open((char*)mesfifo, O_RDONLY)) < 0)
                SET_ERR("Cannot open file ", mesfifo);
            
            if (read(rcvmesfd, (void*)&mes, 1) < 0)
                SET_ERR("Cannot read from file ", mesfifo);
            if (mes == 'O')     //OK
                while((bytesnum = read(rcvchfd, buf, BUFSIZE)) > 0){
                    if (bytesnum < 0)
                        SET_ERR("Cannot read from file ", chanfifo);
                    if (write(STDOUT_FILENO, buf, bytesnum) < 0)
                        SET_ERR("Cannot write into stdout ", emp_str);
                }
            else if (mes == 'E')    //Error
                {
                    errno = EIO;
                    SET_ERR("", emp_str);
                }
                    
            if (read(rcvmesfd, (void*)&mes, 1) < 0)
                SET_ERR("Cannot read from file ", mesfifo);
            if(mes == 'C'){     //Completed
                close(rcvchfd);
                close(rcvmesfd);
                unlink((char*)chanfifo);
                unlink((char*)mesfifo);
                
                exit(EXIT_SUCCESS);
            }
            else if (mes == 'E')    //Error
                SET_ERR("Error while reading from file ", chanfifo);
            /*errno = EIO;
            SET_ERR("", emp_str);*/
                
            break;
        }
        
        default:
            PRINT_HELP;
            exit(EXIT_FAILURE);
    }
    return 0;
}

char* getstrpid(pid_t pid, char mode){
    char pid_str[10];
    char *str = calloc(NAMEFILE_MAX, sizeof(char));
    
    sprintf(pid_str, "%d", pid);
    strcpy(str, "./tmp/");
    strcat(str, pid_str);
    str[strlen(str)] = mode;
    return str;
}

void terminate(char *errstr, int errnum){
    fprintf(stderr, "%s", errstr);
    printf("%s\n", strerror(errnum));
    exit(EXIT_FAILURE);
}
