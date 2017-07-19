#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#ifdef WIN32
#define mkdir(x, y) mkdir(x)
#endif

const int defaultFileCount = 500000;
const int filesPerDirectory = 1000;

_Noreturn void usage()
{
    fprintf(stderr, "test [-nochdir] <create | open | stat | unlink> <path> [count]\n");
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
    argn++;

    int fileCount = defaultFileCount;
    if (argn < argc)
    {
        fileCount = atoi(argv[argn]);
        argn++;
    }

    if (mode == modeCreate)
    {
        if (mkdir(dirName, 0777) < 0)
        {
            perror("mkdir");
            return 1;
        }
    }

    char cwd[2048];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        perror("getcwd");
        return 1;
    }

    if (!nochdir && chdir(dirName) < 0)
    {
        perror("chdir");
        return 1;
    }

    for (int i = 0; i < fileCount; i++)
    {
        int di = i / filesPerDirectory;
        char dn[2048];
        char fn[2048];
        if (nochdir)
        {
            snprintf(dn, 2048, "%s/%d", dirName, di);
            snprintf(fn, 2048, "%s/%d/%d", dirName, di, i);
        }
        else
        {
            snprintf(dn, 2048, "%d", di);
            snprintf(fn, 2048, "%d/%d", di, i);
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

            if (i % filesPerDirectory == filesPerDirectory - 1 || i == fileCount - 1)
            {
                if (rmdir(dn) < 0)
                {
                    perror("rmdir");
                    return 1;
                }
            }
        }
        else
        {
            int oflag = 0;
            if (mode == modeCreate)
            {
                if (i % filesPerDirectory == 0)
                {
                    if (mkdir(dn, 0777) < 0)
                    {
                        perror("mkdir");
                        return 1;
                    }
                }
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
        if (chdir(cwd) < 0)
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
