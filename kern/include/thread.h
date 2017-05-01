#ifndef _THREAD_H_
#define _THREAD_H_

/*
 * Definition of a thread.
 */

/* Get machine-dependent stuff */
#include <machine/pcb.h>

/*
 * New added
 */
#define MAX_CHILD 100
#define MAX_THREADS 100
struct semaphore* lock;

struct addrspace;

struct thread {
	/**********************************************************/
	/* Private thread members - internal to the thread system */
	/**********************************************************/
	
	struct pcb t_pcb;
	char *t_name;
	const void *t_sleepaddr;
	char *t_stack;
	
	/**********************************************************/
	/* Public thread members - can be used by other code      */
	/**********************************************************/
	
	/*
	 * This is public because it isn't part of the thread system,
	 * and will need to be manipulated by the userprog and/or vm
	 * code.
	 */
	struct addrspace *t_vmspace;

	/*
	 * This is public because it isn't part of the thread system,
	 * and is manipulated by the virtual filesystem (VFS) code.
	 */
	struct vnode *t_cwd;
 
  //**************************************
   pid_t pid;
   struct thread* child[MAX_CHILD];
   struct thread* parent;
   struct semaphore *wait;
   //**************************************
};

/* Call once during startup to allocate data structures. */
struct thread *thread_bootstrap(void);

/* Call during panic to stop other threads in their tracks */
void thread_panic(void);

/* Call during shutdown to clean up (must be called by initial thread) */
void thread_shutdown(void);

/*
 * Make a new thread, which will start executing at "func".  The
 * "data" arguments (one pointer, one integer) are passed to the
 * function.  The current thread is used as a prototype for creating
 * the new one. If "ret" is non-null, the thread structure for the new
 * thread is handed back. (Note that using said thread structure from
 * the parent thread should be done only with caution, because in
 * general the child thread might exit at any time.) Returns an error
 * code.
 */
int thread_fork(const char *name, 
		void *data1, unsigned long data2, 
		void (*func)(void *, unsigned long),
		struct thread **ret);

/*
 * Cause the current thread to exit.
 * Interrupts need not be disabled.
 */
void thread_exit(void);

/*
 * Cause the current thread to yield to the next runnable thread, but
 * itself stay runnable.
 * Interrupts need not be disabled.
 */
void thread_yield(void);

/*
 * Cause the current thread to yield to the next runnable thread, and
 * go to sleep until wakeup() is called on the same address. The
 * address is treated as a key and is not interpreted or dereferenced.
 * Interrupts must be disabled.
 */
void thread_sleep(const void *addr);

/*
 * Cause all threads sleeping on the specified address to wake up.
 * Interrupts must be disabled.
 */
void thread_wakeup(const void *addr);

void thread_wakeup_one(const void *addr);
/*
 * Return nonzero if there are any threads sleeping on the specified
 * address. Meant only for diagnostic purposes.
 */
int thread_hassleepers(const void *addr);


/*
 * Private thread functions.
 */

/* Machine independent entry point for new threads. */
void mi_threadstart(void *data1, unsigned long data2, 
		    void (*func)(void *, unsigned long));

/* Machine dependent context switch. */
void md_switch(struct pcb *old, struct pcb *nu);


//**************************************
struct pnode{
  struct thread *thread;
  struct pnode *next;
  struct pnode *previous;
  int excode;
  int exited;  // boolean
  pid_t pid;
  pid_t parentpid;
  int waited;  // boolean
};

void thread_init(struct thread* thread);
void plist_init();
void insert_to_plist(struct thread* thread);
void delete_from_plist(int pid);
int get_exited(int pid);
int get_excode(int pid);
void set_excode(int pid,int exitcode);
void set_exited(int pid);
void set_waited(int pid);
void thread_detach_child(struct thread* thread);
int find_child_index_by_pid(pid_t pid,struct thread* thread);
struct thread* find_thread_by_pid(pid_t pid);
void set_parentpid(pid_t parent,pid_t current);
int check_parent_child_relationship(pid_t parent,pid_t current);
int check_waited_by_pid(pid_t pid);
//**************************************

#endif /* _THREAD_H_ */