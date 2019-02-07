/* 
 * File:   main.c
 * Author: vasiliev
 *
 * Created on August 1, 2014, 6:06 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>
#include <pwd.h>
#include <android/log.h>

#define LOG_TAG "su"
#define VERSION "0.1.2"

void print_info(int argc, char **argv);

int main(int argc, char **argv)
{
    // print_info(argc, argv);

    if(setgid(0) || setuid(0)) {
        __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "su: error %d (%s)", -errno, strerror(errno));
        /* UNIX exit codes are forced into the range 0-255 unsigned. Negative error code became 255. */
        return -errno;
    }

    /* User specified command for exec. */
    if (argc == 2) {
        if (execlp(argv[1], argv[1], NULL) < 0) {
            __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, 
                "su: exec failed for %s, error: %s", argv[1], strerror(errno));
            return -errno;
        }
    } else if (argc > 2) {
        /* Fix 'su -c command' format. */
        int effective_index = 1;
        if (strcmp(argv[effective_index], "-c") == 0) {
            effective_index++;
        }
        /* Copy the rest of the args from main. Args array must ends with null pointer. */
        char *exec_args[argc - (effective_index - 1)];
        memset(exec_args, 0, sizeof(exec_args));
        memcpy(exec_args, &argv[effective_index], sizeof(exec_args));
        if (execvp(argv[effective_index], exec_args) < 0) {
            __android_log_print(ANDROID_LOG_ERROR, LOG_TAG,
                "su: exec failed for %s, error: %s", argv[effective_index], strerror(errno));
            return -errno;
        }
    }

    /* Default exec shell. */
    execlp("/system/bin/sh", "sh", NULL);
    __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "su: exec failed");
    return 0;
}

void print_info(int argc, char **argv)
{
    __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "su: version %s", VERSION);
    char pname[256] = { 0 };
    char cmdpath[64] = { 0 };
    int pid;

    pid = getppid();
    sprintf(cmdpath, "/proc/%d/cmdline", pid);
    FILE *f = fopen(cmdpath, "r");
    fread(pname, 1, sizeof(pname), f);
    fclose(f);

    __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "su: exec pname: %s, ppid: %d", pname, pid);
    __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "su: arguments >");
    int i = 1;
    for (i = 0; i < argc; i++) {
        __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "    - %s", argv[i]);
    }
}