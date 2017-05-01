/*
 * catlock.c
 *
 * 30-1-2003 : GWA : Stub functions created for CS161 Asst1.
 *
 * NB: Please use LOCKS/CV'S to solve the cat syncronization problem in
 * this file.
 */


/*
 *
 * Includes
 *
 */

#include <types.h>
#include <lib.h>
#include <test.h>
#include <thread.h>
#include <synch.h>

/*
 *
 * Constants
 *
 */

/*
 * Number of food bowls.
 */

#define NFOODBOWLS 2

/*
 * Number of cats.
 */

#define NCATS 6

/*
 * Number of mice.
 */

#define NMICE 2

/*
 * Variables
 */

    static struct lock *first_bowl;
    static struct lock *second_bowl;
    static struct lock *cat_num_change_lock;
    static struct lock *mouse_num_change_lock;
    static struct cv *cat_cv;
    static struct cv *mouse_cv;
    int cat_eating_num = 0;
    int mouse_eating_num = 0;

/*
 *
 * Function Definitions
 *
 */

/* who should be "cat" or "mouse" */
static void
lock_eat(const char *who, int num, int bowl, int iteration)
{
        kprintf("%s: %d starts eating: bowl %d, iteration %d\n", who, num,
                bowl, iteration);
        clocksleep(1);
        kprintf("%s: %d ends eating: bowl %d, iteration %d\n", who, num,
                bowl, iteration);
}

/*
 * catlock()
 *
 * Arguments:
 *      void * unusedpointer: currently unused.
 *      unsigned long catnumber: holds the cat identifier from 0 to NCATS -
 *      1.
 *
 * Returns:
 *      nothing.
 *
 * Notes:
 *      Write and comment this function using locks/cv's.
 *
 */

static
void
catlock(void * unusedpointer,
        unsigned long catnumber)
{
        /*
         * Avoid unused variable warnings.
         */

         int ctr;
         for(ctr = 0; ctr < 4; ctr++) { // each cat of mouse eats four times
            if(first_bowl->holder == NULL) {    // if the first bowl is available, go for it
                lock_acquire(first_bowl);   // acquire first bowl's lock

                while(mouse_eating_num != 0) {  // if a mouse is eating (at the second bowl), wait until it leaves
                    cv_wait(cat_cv,first_bowl); // this cat waits in first bowl's queue
                }

                lock_acquire(cat_num_change_lock);  // acquire lock to change the value of 'cat_eating_num'
                cat_eating_num++;   // this cat is about to eat at the first bowl
                lock_release(cat_num_change_lock);  // release lock for changing the value of 'cat_eating_num'

                lock_eat("cat",catnumber,1,ctr);    // this cat is eating at the first bowl

                lock_acquire(cat_num_change_lock);  // acquire lock to change the value of 'cat_eating_num'
                cat_eating_num--;   // this cat finishes eating at the first bowl
                lock_release(cat_num_change_lock);  // release lock for changing the value of 'cat_eating_num'

                lock_release(first_bowl);   // release first bowl's lock, so others can acquire it to eat here

                if(cat_eating_num == 0) {   // after this cat finishing, if no cat is eating, wake up all mice waiting in the first bowl's queue
                    cv_broadcast(mouse_cv,first_bowl);
                }
            } else if(second_bowl->holder == NULL) {
                lock_acquire(second_bowl);

                while(mouse_eating_num != 0) {
                    cv_wait(cat_cv,second_bowl);
                }

                lock_acquire(cat_num_change_lock);
                cat_eating_num++;
                lock_release(cat_num_change_lock);

                lock_eat("cat",catnumber,2,ctr);

                lock_acquire(cat_num_change_lock);
                cat_eating_num--;
                lock_release(cat_num_change_lock);

                lock_release(second_bowl);

                if(cat_eating_num == 0) {
                    cv_broadcast(mouse_cv,second_bowl);
                }
            } else {
                ctr--;  // if both bowls are taken, the number of iteration doesn't increase
            }
         }
//        (void) unusedpointer;
//        (void) catnumber;
}


/*
 * mouselock()
 *
 * Arguments:
 *      void * unusedpointer: currently unused.
 *      unsigned long mousenumber: holds the mouse identifier from 0 to
 *              NMICE - 1.
 *
 * Returns:
 *      nothing.
 *
 * Notes:
 *      Write and comment this function using locks/cv's.
 *
 */

static
void
mouselock(void * unusedpointer,
          unsigned long mousenumber)
{
        /*
         * Avoid unused variable warnings.
         */

         int ctr;
         for(ctr = 0; ctr < 4; ctr++) {
            if(first_bowl->holder == NULL) {
                lock_acquire(first_bowl);

                while(cat_eating_num != 0) {
                    cv_wait(mouse_cv,first_bowl);
                }

                lock_acquire(mouse_num_change_lock);
                mouse_eating_num++;
                lock_release(mouse_num_change_lock);

                lock_eat("mouse",mousenumber,1,ctr);

                lock_acquire(mouse_num_change_lock);
                mouse_eating_num--;
                lock_release(mouse_num_change_lock);

                lock_release(first_bowl);

                if(mouse_eating_num == 0) {
                    cv_broadcast(cat_cv,first_bowl);
                }
            } else if(second_bowl->holder == NULL) {
                lock_acquire(second_bowl);

                while(cat_eating_num != 0) {
                    cv_wait(mouse_cv,second_bowl);
                }

                lock_acquire(mouse_num_change_lock);
                mouse_eating_num++;
                lock_release(mouse_num_change_lock);

                lock_eat("mouse",mousenumber,2,ctr);

                lock_acquire(mouse_num_change_lock);
                mouse_eating_num--;
                lock_release(mouse_num_change_lock);

                lock_release(second_bowl);

                if(mouse_eating_num == 0) {
                    cv_broadcast(cat_cv,second_bowl);
                }
            } else {
                ctr--;
            }
         }
//        (void) unusedpointer;
//        (void) mousenumber;
}


/*
 * catmouselock()
 *
 * Arguments:
 *      int nargs: unused.
 *      char ** args: unused.
 *
 * Returns:
 *      0 on success.
 *
 * Notes:
 *      Driver code to start up catlock() and mouselock() threads.  Change
 *      this code as necessary for your solution.
 */

int
catmouselock(int nargs,
             char ** args)
{
    int index, error;

        /*
         * Avoid unused variable warnings.
         */

        first_bowl = lock_create("first_bowl");
        second_bowl = lock_create("second_bowl");
        cat_num_change_lock = lock_create("cat_num_change");
        mouse_num_change_lock = lock_create("mouse_num_change");
        cat_cv = cv_create("cat_waiting_addr");
        mouse_cv = cv_create("mouse_waiting_addr");
//        (void) nargs;
//        (void) args;

        /*
         * Start NCATS catlock() threads.
         */

        for (index = 0; index < NCATS; index++) {

                error = thread_fork("catlock thread",
                                    NULL,
                                    index,
                                    catlock,
                                    NULL
                                    );

                /*
                 * panic() on error.
                 */

                if (error) {

                        panic("catlock: thread_fork failed: %s\n",
                              strerror(error)
                              );
                }
        }

        /*
         * Start NMICE mouselock() threads.
         */

        for (index = 0; index < NMICE; index++) {

                error = thread_fork("mouselock thread",
                                    NULL,
                                    index,
                                    mouselock,
                                    NULL
                                    );

                /*
                 * panic() on error.
                 */

                if (error) {

                        panic("mouselock: thread_fork failed: %s\n",
                              strerror(error)
                              );
                }
        }

        // THOUGHT OF DESTROYING USED LOCKS:
        // int counter
        // lock_create (counter_lock)

        // lock_acquire(counter_lock)
        // counter++
        // lock_release(counhter_lock)

        // if(counter == NMICE)
        //  destroy(mousenumchangelock)
        //  destroy(counter_lock);

        return 0;
}

/*
 * End of catlock.c
 */
