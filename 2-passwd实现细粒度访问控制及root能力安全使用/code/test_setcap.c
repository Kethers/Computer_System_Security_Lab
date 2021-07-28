#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <shadow.h>
#include <sys/types.h>
#include <pwd.h>
#include <crypt.h>
#include <stdlib.h>

//cmd like: test_setcap root root
int main(int argc, char **argv)
{
    struct passwd *user, *group;
    user = getpwnam(argv[1]);
    group = getpwnam(argv[2]);
    if (!chown(argv[0], user->pw_uid, group->pw_gid))
        printf("Succeed to change uid and gid!\n");
    else
        printf("Failed to change uid and gid!\n");

    return 0;
}