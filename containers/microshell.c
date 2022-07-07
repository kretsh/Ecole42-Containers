/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   microshell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarchiba < aarchiba@student.21-school.r    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/16 17:49:26 by aarchiba          #+#    #+#             */
/*   Updated: 2022/02/18 22:44:41 by aarchiba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int	ft_strlen(char *str)
{
	int	i = 0;

	while (str[i])
		i++;
	return (i);
}

void	err_fatal(void)
{
	write(2, "error: fatal\n", 13);
	exit(1);
}

void	make_cd(char **line)
{
	int	i = 0;

	while (line[i])
		i++;
	if (i != 2)
		write(2, "error: cd: bad arguments\n", 25);
	else
	{
		if (chdir(line[1]))
		{
			write(2,"error: cd: cannot change directory to ", 38);
			write(2, line[1], ft_strlen(line[1]));
			write(2, "\n", 1);
		}
	}
}

int	main(int argc, char **argv, char **envp)
{
	int		i = 1;
	int		len = 0;
	char	**line;
	int		j;
	int		f_cd = 0;
	int		f_pipe = 0;
	int		opend = 0;
	int		fd[2];
	int		fds[2];
	int		status = 0;
	pid_t	pid;
	while (argc > 1 && i < argc)
	{
		if (!strcmp(argv[i], ";") || !strcmp(argv[i], "|"))
		{
			i++;
			continue ;
		}
		len = 0;
		while (argv[i + len] && strcmp(argv[i + len], ";") && strcmp(argv[i + len], "|"))
			len++;           
		line = malloc(sizeof(char *) * (len + 1));
		if (!line)
			err_fatal();
		line[len] = NULL;
		j = 0;
		while (j < len)
		{
			line[j] = argv[i];
			if (!strcmp(argv[i], "cd"))
				f_cd = 1;
			i++;
			j++;
		}
		if (argv[i] && !strcmp(argv[i], "|"))
			f_pipe = 1;
		else
			f_pipe = 0;
		if ((argv[i] && !strcmp(argv[i], "cd")) || f_cd)
			make_cd(line);
		if (opend)
		{
			fds[0] = dup(0);
			dup2(fd[0], 0);
			close(fd[0]);
		}
		if (f_pipe)
		{
			if (pipe(fd))
				err_fatal();
			fds[1] = dup(1);
			dup2(fd[1], 1);
			close(fd[1]);
		}
		pid = fork();
		if (pid < 0)
			err_fatal();
		else if (pid == 0)
		{
			if (!f_cd)
			{
				if (execve(line[0], line, envp))
				{
					write(2, "error: cannot execute ", 22);
					write(2, line[0], ft_strlen(line[0]));
					write (2, "\n", 1);
					exit(1);
				}
			}
			exit(0);
		}
		else
			waitpid(pid, &status, 0);
		if (opend)
		{
			dup2(fds[0], 0);
			close(fds[0]);
			opend = 0;
		}
		if (f_pipe)
		{
			dup2(fds[1], 1);
			close(fds[1]);
			opend = 1;
		}
		f_cd = 0;
		free(line);
	}
}
