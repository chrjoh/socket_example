#pragma once


void exit_with_user_message(const char *msg, const char *detail);
void exit_with_system_message(const char *msg);
int64_t system_message(const char *msg, int64_t code);
