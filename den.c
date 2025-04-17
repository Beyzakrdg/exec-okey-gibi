#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void	execute_command(char *cmd, char **args, char **env)
{
	pid_t	pid;
	int		status;
	
	pid = fork(); // Çocuk süreç oluştur
	if (pid < 0)
	{
		perror("fork error");
		exit(1);
	}
	if (pid == 0) // Çocuk süreç
	{
		if (execve(cmd, args, env) == -1)
		{
			perror("execve error");
			exit(1);
		}
	}
	else // Parent süreç
		waitpid(pid, &status, 0);
}

int main(int argc, char **argv, char **env)
{
	char *cmd = "/bin/ls"; // ls'nin tam yolu
	char *args[] = {cmd, "-l", NULL};

	execute_command(cmd, args, env); // Komutu çalıştır
	return (0);
}
