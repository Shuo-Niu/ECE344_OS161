/*
 * Synchronization primitives.
 * See synch.h for specifications of the functions.
 */

#include <types.h>
#include <lib.h>
#include <synch.h>
#include <thread.h>
#include <curthread.h>
#include <machine/spl.h>

////////////////////////////////////////////////////////////
//
// Semaphore.

struct semaphore *
sem_create(const char *namearg, int initial_count)
{
	struct semaphore *sem;

	assert(initial_count >= 0);

	sem = kmalloc(sizeof(struct semaphore));
	if (sem == NULL) {
		return NULL;
	}

	sem->name = kstrdup(namearg);
	if (sem->name == NULL) {
		kfree(sem);
		return NULL;
	}

	sem->count = initial_count;
	return sem;
}

void
sem_destroy(struct semaphore *sem)
{
	int spl;
	assert(sem != NULL);

	spl = splhigh();
	assert(thread_hassleepers(sem)==0);
	splx(spl);

	/*
	 * Note: while someone could theoretically start sleeping on
	 * the semaphore after the above test but before we free it,
	 * if they're going to do that, they can just as easily wait
	 * a bit and start sleeping on the semaphore after it's been
	 * freed. Consequently, there's not a whole lot of point in 
	 * including the kfrees in the splhigh block, so we don't.
	 */

	kfree(sem->name);
	kfree(sem);
}

void 
P(struct semaphore *sem)
{
	int spl;
	assert(sem != NULL);

	/*
	 * May not block in an interrupt handler.
	 *
	 * For robustness, always check, even if we can actually
	 * complete the P without blocking.
	 */
	assert(in_interrupt==0);

	spl = splhigh();
	while (sem->count==0) {
		thread_sleep(sem);
	}
	assert(sem->count>0);
	sem->count--;
	splx(spl);
}

void
V(struct semaphore *sem)
{
	int spl;
	assert(sem != NULL);
	spl = splhigh();
	sem->count++;
	assert(sem->count>0);
	thread_wakeup(sem);
	splx(spl);
}

////////////////////////////////////////////////////////////
//
// Lock.

struct lock *
lock_create(const char *name)
{
	struct lock *lock;

	lock = kmalloc(sizeof(struct lock));
	if (lock == NULL) {
		return NULL;
	}

	lock->name = kstrdup(name);
	if (lock->name == NULL) {
		kfree(lock);
		return NULL;
	}
	
	// add stuff here as needed
	lock->holder = NULL;
	
	return lock;
}

void
lock_destroy(struct lock *lock)
{
	int spl;
	assert(lock != NULL);

	// add stuff here as needed
	spl = splhigh();
  //Ensure the lock is not being used
	assert(lock->holder == NULL);
	splx(spl);

	kfree(lock->name);
	kfree(lock);
}

void
lock_acquire(struct lock *lock)
{
  // Write this
	int spl;
  assert(lock != NULL);
  // Ensure there are no interrupts in action
  assert(in_interrupt==0);

  spl = splhigh();
  // Wait until the lock is released
  while (lock->holder != NULL) {
    thread_sleep(lock);
  }

  // Once the lock is released, assign it to the calling (current) thread
  if (lock->holder == NULL) {
    lock->holder = curthread;
  }
  splx(spl);
}

void
lock_release(struct lock *lock)
{
  // Write this
  int spl;
  assert(lock != NULL);

  spl = splhigh();

  // Release the lock only if the calling thread actually holds it
  if (lock_do_i_hold(lock) != 0) {
    lock->holder = NULL;
    thread_wakeup_one(lock);
  }

  splx(spl);
}

int
lock_do_i_hold(struct lock *lock)
{
	// Write this
	int spl;
	assert(lock != NULL);

	spl = splhigh();

	if(lock->holder == curthread){
		splx(spl);
		return 1;       // current thread holds the lock
	} else {
		splx(spl);
		return 0;       // current thread does not hold the lock
	}
}

////////////////////////////////////////////////////////////
//
// CV


struct cv *
cv_create(const char *name)
{
	struct cv *cv;

	cv = kmalloc(sizeof(struct cv));
	if (cv == NULL) {
		return NULL;
	}

	cv->name = kstrdup(name);
	if (cv->name==NULL) {
		kfree(cv);
		return NULL;
	}
	
	// add stuff here as needed
	
	return cv;
}

void
cv_destroy(struct cv *cv)
{
	assert(cv != NULL);

	// add stuff here as needed
	int spl;
	spl = splhigh();
	assert(thread_hassleepers(cv)==0);
	splx(spl);
	
	kfree(cv->name);
	kfree(cv);
}

void
cv_wait(struct cv *cv, struct lock *lock)
{
	// Write this
	int spl;
	spl = splhigh();

	assert(cv != NULL);
	assert(lock != NULL);

	// Unlocking the mutex the calling thread is holding and putting the thread to sleep,
	// i.e. blocking the calling thread (until some other thread signals it)
	lock_release(lock);
	thread_sleep(cv);

	// Once the thread is waken up (signalled by other thread),
	// the calling thread acquires the lock back
	lock_acquire(lock);

	splx(spl);

//	(void)cv;    // suppress warning until code gets written
//	(void)lock;  // suppress warning until code gets written
}

void
cv_signal(struct cv *cv, struct lock *lock)
{
	// Write this
	int spl;
	spl = splhigh();

	assert(cv != NULL);
	assert(lock != NULL);

	thread_wakeup_one(cv);
	lock_release(lock);

	splx(spl);

//	(void)cv;    // suppress warning until code gets written
//	(void)lock;  // suppress warning until code gets written
}

void
cv_broadcast(struct cv *cv, struct lock *lock)
{
	// Write this
	int spl;
	spl = splhigh();

	assert(cv != NULL);
	assert(lock != NULL);

	thread_wakeup(cv);
	lock_release(lock);

	splx(spl);

//	(void)cv;    // suppress warning until code gets written
//	(void)lock;  // suppress warning until code gets written
}
