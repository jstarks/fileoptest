#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

const int fileCount = 100000;

_Noreturn void usage()
{
    fprintf(stderr, "test [-nochdir] <create | open | stat | unlink> <path>\n");
    exit(100);
}

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        usage();
    }

    int nochdir = 0;
    int argn = 1;
    if (strcmp(argv[argn], "-nochdir") == 0)
    {
        argn++;
        nochdir = 1;
    }

    enum mode
    {
        modeCreate,
        modeOpen,
        modeStat,
        modeUnlink,
    } mode;

    if (strcmp(argv[argn], "create") == 0)
    {
        mode = modeCreate;
    }
    else if (strcmp(argv[argn], "open") == 0)
    {
        mode = modeOpen;
    }
    else if (strcmp(argv[argn], "stat") == 0)
    {
        mode = modeStat;
    }
    else if (strcmp(argv[argn], "unlink") == 0)
    {
        mode = modeUnlink;
    }
    else
    {
        usage();
    }

    argn++;
    const char *dirName = argv[argn];

    if (mode == modeCreate)
    {
        if (mkdir(dirName, 0777) < 0)
        {
            perror("mkdir");
            return 1;
        }
    }

    int cwd = open(".", 0);
    if (cwd < 0)
    {
        perror("open .");
        return 1;
    }

    if (!nochdir && chdir(dirName) < 0)
    {
        perror("chdir");
        return 1;
    }

    for (int i = 0; i < fileCount; i++)
    {
        char fn[2048];
        if (nochdir)
        {
            snprintf(fn, 2048, "%s/%d", dirName, i);
        }
        else
        {
            snprintf(fn, 2048, "%d", i);
        }

        if (mode == modeStat)
        {
            struct stat buf;
            if (stat(fn, &buf) < 0)
            {
                perror("stat");
                return 1;
            }
        }
        else if (mode == modeUnlink)
        {
            if (unlink(fn) < 0)
            {
                perror("unlink");
                return 1;
            }
        }
        else
        {
            int oflag = 0;
            if (mode == modeCreate)
            {
                oflag |= O_CREAT;
            }
            int fd = open(fn, oflag, 0666);
            if (fd < 0)
            {
                perror("open");
                return 1;
            }

            close(fd);
        }
    }

    if (mode == modeUnlink)
    {
        if (fchdir(cwd) < 0)
        {
            perror("fchdir");
            return 1;
        }

        if (rmdir(dirName) < 0)
        {
            perror("unlink");
            return 1;
        }
    }
}
