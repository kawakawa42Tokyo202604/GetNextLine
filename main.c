#ifdef GNL_BONUS
# include "get_next_line_bonus.h"
#else
# include "get_next_line.h"
#endif
#include <fcntl.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    int fd;
    char *line;
    int status;

    if (argc > 2)
        return (fprintf(stderr, "Usage: %s [file]\n", argv[0]), 1);
    fd = STDIN_FILENO;
    if (argc == 2)
        fd = open(argv[1], O_RDONLY);
    if (fd < 0)
        return (perror(argv[1]), 1);
    status = 0;
    while ((line = get_next_line(fd)) != NULL)
    {
        if (fputs(line, stdout) == EOF)
            status = 1;
        free(line);
        if (status != 0)
            break;
    }
    if (argc == 2 && close(fd) < 0)
        status = 1;
    if (fflush(stdout) == EOF)
        status = 1;
    return (status);
}
