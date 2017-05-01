//#include <syscall.h>
//#include <types.h>
//#include <lib.h>
//#include <kern/unistd.h>
//#include <uio.h>
//#include <curthread.h>
//#include <thread.h>
//#include <vnode.h>
//#include <vfs.h>
//#include <test.h>

//int sys_write(int fd, const void *buf, size_t size){
//    if (fd != STDOUT_FILENO)
//        return EBADF;
//
//    if (buf == NULL)
//        return EFAULT;
//
//    char* temp = kmalloc((nbytes+1)*sizeof(char));
//    int status = copyin(buf, temp, nbytes);
//
//    if (status != 0) {
//        kfree(temp);
//        return EFAULT;
//
//    } else {
//        temp[nbytes] = "\0";
//        kprintf("%s", temp);
//        kfree(temp);
//        return nbytes;
//    }
//}

//int sys_read(){
//
//}