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

int main(int argc, char **argv)
{
	char judgeInput[2][7] = {"show", "change"};
	uid_t ruid, euid, suid;
	int temp;
	int showInfo, changeInfo;
	showInfo = !strcmp(judgeInput[0], argv[1]);
	changeInfo = !strcmp(judgeInput[1], argv[1]);

	// 判断输入是否合法
	if (!((argc == 2 && showInfo) || (argc == 4 && changeInfo)))
	{
		printf("Error order!Please input again!\n");
		printf("For example:\n./lab2 show\n./lab2 change userName passWord\n");
		return -1;
	}

	if ((temp = getresuid(&ruid, &euid, &suid)) == -1)
		perror("getresuid failed!\n");
	else
		printf("ruid=%d euid=%d suid=%d\n", ruid, euid, suid);
	// 获得当前用户名
	struct passwd *cur;
	cur = getpwuid(getuid());

	if (showInfo)
	{
		struct spwd *p;
		FILE *fs;
		fs = fopen("aaa", "r");
		if (!ruid)
		{
			while ((p = fgetspent(fs)) != NULL)
			{
				printf("Login name:%s\nEncrypted password:%s\nLast change:%ld\nMin age:%ld\nMax age:%ld\nWarn:%ld\nInact:%ld\nExpire:%ld\nFlag:%ld\n\n", p->sp_namp, p->sp_pwdp, p->sp_lstchg, p->sp_min, p->sp_max, p->sp_warn, p->sp_inact, p->sp_expire, p->sp_flag);
			}
		}
		else
		{
			while ((p = fgetspent(fs)) != NULL)
			{
				if (!strcmp(cur->pw_name, p->sp_namp))
					printf("Login name:%s\nEncrypted password:%s\nLast change:%ld\nMin age:%ld\nMax age:%ld\nWarn:%ld\nInact:%ld\nExpire:%ld\nFlag:%ld\n\n", p->sp_namp, p->sp_pwdp, p->sp_lstchg, p->sp_min, p->sp_max, p->sp_warn, p->sp_inact, p->sp_expire, p->sp_flag);
			}
		}
		fclose(fs);
		return 0;
	}

	char newPassword[10];
	char *cryptedPassword;
	char userTochange[10];
	strcpy(userTochange, argv[2]);
	strcpy(newPassword, argv[3]);
	struct spwd store[100], *p;
	int total = 0;
	int i;
	FILE *fs;
	fs = fopen("aaa", "r");
	int flag = 0;
	// root 用户可以更改所有人的密码
	if (!ruid)
	{
		while ((p = fgetspent(fs)) != NULL)
		{
			store[total] = *p;
			store[total].sp_namp = malloc(strlen(p->sp_namp) + 1);
			store[total].sp_pwdp = malloc(strlen(p->sp_pwdp) + 1);
			strcpy(store[total].sp_namp, p->sp_namp);
			if (!strcmp(p->sp_namp, userTochange))
			{
				cryptedPassword = crypt(newPassword, "$6$FaUYKA8B$CUuItcF");
				strcpy(store[total].sp_pwdp, cryptedPassword);
				flag = 1;
			}
			else
			{
				strcpy(store[total].sp_pwdp, p->sp_pwdp);
			}
			total++;
		}
	}

	//普通用户只能更改自己的passwd
	else
	{
		if (!strcmp(cur->pw_name, userTochange))
		{
			while ((p = fgetspent(fs)) != NULL)
			{
				store[total] = *p;
				store[total].sp_namp = malloc(strlen(p->sp_namp) + 1);
				store[total].sp_pwdp = malloc(strlen(p->sp_pwdp) + 1);
				strcpy(store[total].sp_namp, p->sp_namp);
				if (!strcmp(p->sp_namp, userTochange))
				{
					cryptedPassword = crypt(newPassword, "$6$FaUYKA8B$CUuItcF");
					strcpy(store[total].sp_pwdp, cryptedPassword);
					flag = 1;
				}
				else
				{
					strcpy(store[total].sp_pwdp, p->sp_pwdp);
				}
				total++;
			}
		}
		// 要修改的不是当前用户则直接退出
	}

	fclose(fs);
	if (!flag)
	{
		printf("failed to change the passwd!\n");
		return -1;
	}
	//修改成功，写回
	{
		printf("change success!\n");
		fs = fopen("aaa", "w");
		for (i = 0; i < total; i++)
			putspent(&store[i], fs);
		fclose(fs);
	}
	return 0;
}