#include "includes/pipex.h"

int	main(int argc, char **argv, char **env)
{
	int		fd;
	char		*cmd[] = {"/bin/echo", "hello", (void *)0};

	fd = open("file2", O_RDWR | O_CREAT | O_TRUNC);
	printf("fd=%d\n", fd);
	if (dup2(fd, STDOUT_FILENO) == -1)
		write(1, "dup2 failed\n", 12); //close free etc
	close(fd);
	execve("/bin/echo", cmd, env);
	return (0);
}