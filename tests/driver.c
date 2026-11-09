#define _POSIX_C_SOURCE 200809L
#ifdef TEST_BONUS
# include "get_next_line_bonus.h"
#else
# include "get_next_line.h"
#endif
#include <limits.h>
#include <stdio.h>
#include <string.h>

static FILE *stream(const char *text)
{
	FILE *file = tmpfile();

	if (!file || fwrite(text, 1, strlen(text), file) != strlen(text))
		exit(2);
	rewind(file);
	return (file);
}

static int line_is(int fd, const char *expected)
{
	char *line = get_next_line(fd);
	int result;

	if (expected == NULL)
		result = (line == NULL);
	else
		result = (line != NULL && strcmp(line, expected) == 0);
	free(line);
	return (result);
}

int main(int argc, char **argv)
{
	FILE *first;
	FILE *second;
	int which;
	int fd;
	int ok = 1;
	char large[5005];

	if (argc != 2)
		return (2);
	which = atoi(argv[1]);
	if (which == 6)
		return (get_next_line(-1) != NULL);
	if (which == 7)
		return (get_next_line(INT_MAX) != NULL);
	if (which == 8)
	{
		first = stream("closed\n");
		fd = fileno(first);
		fclose(first);
		return (get_next_line(fd) != NULL);
	}
	if (which == 9)
	{
		first = stream("a1\na2");
		second = stream("b1\nb2\n");
		ok = line_is(fileno(first), "a1\n") && line_is(fileno(second), "b1\n");
		ok = line_is(fileno(first), "a2") && ok;
		ok = line_is(fileno(second), "b2\n") && ok;
		ok = line_is(fileno(first), NULL) && line_is(fileno(second), NULL) && ok;
		fclose(first);
		fclose(second);
		return (!ok);
	}
	if (which == 0)
		first = stream("");
	else if (which == 1)
		first = stream("\n");
	else if (which == 2)
		first = stream("last line");
	else if (which == 3)
		first = stream("one\n\nthree\n");
	else if (which == 4)
	{
		memset(large, 'x', 5000);
		memcpy(large + 5000, "\nend", 5);
		first = stream(large);
		large[5001] = '\0';
	}
	else
		first = stream("stdin\n");
	fd = fileno(first);
	if (which == 5)
	{
		if (dup2(fd, STDIN_FILENO) < 0)
			return (2);
		fd = STDIN_FILENO;
	}
	if (which == 1)
		ok = line_is(fd, "\n");
	if (which == 2)
		ok = line_is(fd, "last line");
	if (which == 3)
		ok = line_is(fd, "one\n") && line_is(fd, "\n") && line_is(fd, "three\n");
	if (which == 4)
		ok = line_is(fd, large) && line_is(fd, "end");
	if (which == 5)
		ok = line_is(fd, "stdin\n");
	ok = line_is(fd, NULL) && line_is(fd, NULL) && ok;
	fclose(first);
	return (!ok);
}
