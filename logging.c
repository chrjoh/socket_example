#include <stdio.h>
#include <stdlib.h>

void exit_with_user_message(const char *msg, const char *detail)
{
  fputs(msg, stderr);
  fputs(": ", stderr);
  fputs(detail, stderr);
  fputc('\n', stderr);
  exit(1);
}

void exit_with_system_message(const char *msg)
{
  perror(msg);
  exit(1);
}

int64_t system_message(const char *msg, int64_t code)
{
  perror(msg);
  return code;
}
