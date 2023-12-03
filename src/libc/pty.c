/*
 * Copyright (C) 2008 The Android Open Source Project
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <fcntl.h>
#include <pty.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#include <utmp.h>

int openpty(int* master, int* slave, char* name, const struct termios* t, const struct winsize* ws) {
  *master = getpt();
  if (*master == -1) {
    return -1;
  }

  if (grantpt(*master) == -1 || unlockpt(*master) == -1) {
    close(*master);
    return -1;
  }

  char buf[32];
  if (name == NULL) {
    name = buf;
  }

  if (ptsname_r(*master, name, sizeof(buf)) != 0) {
    close(*master);
    return -1;
  }

  *slave = open(name, O_RDWR|O_NOCTTY);
  if (*slave == -1) {
    close(*master);
    return -1;
  }

  if (t != NULL) {
    tcsetattr(*slave, TCSAFLUSH, t);
  }

  if (ws != NULL) {
    ioctl(*slave, TIOCSWINSZ, ws);
  }

  return 0;
}

int forkpty(int* master, char* name, const struct termios* t, const struct winsize* ws) {
  int slave;
  if (openpty(master, &slave, name, t, ws) == -1) {
    return -1;
  }

  pid_t pid = fork();
  if (pid == -1) {
    close(*master);
    close(slave);
    return -1;
  }

  if (pid == 0) {
    // Child.
    close(*master);
    if (login_tty(slave) == -1) {
      _exit(1);
    }
    return 0;
  }

  // Parent.
  close(slave);
  return pid;
}

int login_tty(int fd) {
  setsid();

  if (ioctl(fd, TIOCSCTTY, NULL) == -1) {
    return -1;
  }

  dup2(fd, STDIN_FILENO);
  dup2(fd, STDOUT_FILENO);
  dup2(fd, STDERR_FILENO);
  if (fd > STDERR_FILENO) {
    close(fd);
  }

  return 0;
}
