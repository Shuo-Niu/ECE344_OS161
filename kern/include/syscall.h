#ifndef _SYSCALL_H_
#define _SYSCALL_H_

/*
 * Prototypes for IN-KERNEL entry points for system call implementations.
 */

int sys_reboot(int code);
int sys_write(int fd, const void *buf, size_t nbytes, int *retval);
int sys_read(int fd, void *buf, size_t buflen, int *retval);
int sys_fork(struct trapframe *tf, int *retval);
int sys_getpid(int *retval);
int sys_waitpid(int pid, int *status, int options, int *retval);
int sys__exit(int exitcode);
int sys_execv(const char *program, char **args, int* retval);
int sys___time(userptr_t secs, userptr_t nsecs, int *retval);
int sys_sbrk(intptr_t amount, int *retval);

#endif /* _SYSCALL_H_ */
