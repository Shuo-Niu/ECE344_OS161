/*
 * stoplight.c
 *
 * 31-1-2003 : GWA : Stub functions created for CS161 Asst1.
 *
 * NB: You can use any synchronization primitives available to solve
 * the stoplight problem in this file.
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
 * Number of cars created.
 */

#define NCARS 20

/*
 * Variables
 */

 static struct semaphore *nw;
 static struct semaphore *ne;
 static struct semaphore *sw;
 static struct semaphore *se;
 static struct semaphore *intersection;

/*
 *
 * Function Definitions
 *
 */

static const char *directions[] = { "N", "E", "S", "W" };

static const char *msgs[] = {
        "approaching:",
        "region1:    ",
        "region2:    ",
        "region3:    ",
        "leaving:    "
};

/* use these constants for the first parameter of message */
enum { APPROACHING, REGION1, REGION2, REGION3, LEAVING };

static void
message(int msg_nr, int carnumber, int cardirection, int destdirection)
{
        kprintf("%s car = %2d, direction = %s, destination = %s\n",
                msgs[msg_nr], carnumber,
                directions[cardirection], directions[destdirection]);
}

/*
 * gostraight()
 *
 * Arguments:
 *      unsigned long cardirection: the direction from which the car
 *              approaches the intersection.
 *      unsigned long carnumber: the car id number for printing purposes.
 *
 * Returns:
 *      nothing.
 *
 * Notes:
 *      This function should implement passing straight through the
 *      intersection from any direction.
 *      Write and comment this function.
 */

static
void
gostraight(unsigned long cardirection,
           unsigned long carnumber)
{
        /*
         * Avoid unused variable warnings.
         */

        if(cardirection == 0) { //N
            P(intersection);    // maximum 3 cars in the intersection simultaneously, wait for my turn
            message(0,carnumber,cardirection,(cardirection + 2) % 4);   // be in the intersection and block others if more than 3 cars
            P(nw);  // maximum 1 car in region 1, wait for my turn
            message(1,carnumber,cardirection,(cardirection + 2) % 4);   // be in region 1 and block others
            P(sw);  // maximum 1 car in region 2, wait for my turn
            message(2,carnumber,cardirection,(cardirection + 2) % 4);   // be in region 2 and block others
            V(nw);  // give region 1 to others
            message(4,carnumber,cardirection,(cardirection + 2) % 4);   // leave the intersection
            V(sw);  // give region 2 to others
            V(intersection);
        } else if (cardirection == 1) {  //E
            P(intersection);
            message(0,carnumber,cardirection,(cardirection + 2) % 4);
            P(ne);
            message(1,carnumber,cardirection,(cardirection + 2) % 4);
            P(nw);
            message(2,carnumber,cardirection,(cardirection + 2) % 4);
            V(ne);
            message(4,carnumber,cardirection,(cardirection + 2) % 4);
            V(nw);
            V(intersection);
        } else if (cardirection == 2) {  //S
            P(intersection);
            message(0,carnumber,cardirection,(cardirection + 2) % 4);
            P(se);
            message(1,carnumber,cardirection,(cardirection + 2) % 4);
            P(ne);
            message(2,carnumber,cardirection,(cardirection + 2) % 4);
            V(se);
            message(4,carnumber,cardirection,(cardirection + 2) % 4);
            V(ne);
            V(intersection);
        } else {    //W
            P(intersection);
            message(0,carnumber,cardirection,(cardirection + 2) % 4);
            P(sw);
            message(1,carnumber,cardirection,(cardirection + 2) % 4);
            P(se);
            message(2,carnumber,cardirection,(cardirection + 2) % 4);
            V(sw);
            message(4,carnumber,cardirection,(cardirection + 2) % 4);
            V(se);
            V(intersection);
        }
//        (void) cardirection;
//        (void) carnumber;
}


/*
 * turnleft()
 *
 * Arguments:
 *      unsigned long cardirection: the direction from which the car
 *              approaches the intersection.
 *      unsigned long carnumber: the car id number for printing purposes.
 *
 * Returns:
 *      nothing.
 *
 * Notes:
 *      This function should implement making a left turn through the
 *      intersection from any direction.
 *      Write and comment this function.
 */

static
void
turnleft(unsigned long cardirection,
         unsigned long carnumber)
{
        /*
         * Avoid unused variable warnings.
         */

         if(cardirection == 0) { //N
            P(intersection);
            message(0,carnumber,cardirection,(cardirection + 1) % 4);
            P(nw);
            message(1,carnumber,cardirection,(cardirection + 1) % 4);
            P(sw);
            message(2,carnumber,cardirection,(cardirection + 1) % 4);
            V(nw);
            P(se);
            message(3,carnumber,cardirection,(cardirection + 1) % 4);
            V(sw);
            message(4,carnumber,cardirection,(cardirection + 1) % 4);
            V(se);
            V(intersection);
        } else if (cardirection == 1) {  //E
            P(intersection);
            message(0,carnumber,cardirection,(cardirection + 1) % 4);
            P(ne);
            message(1,carnumber,cardirection,(cardirection + 1) % 4);
            P(nw);
            message(2,carnumber,cardirection,(cardirection + 1) % 4);
            V(ne);
            P(sw);
            message(3,carnumber,cardirection,(cardirection + 1) % 4);
            V(nw);
            message(4,carnumber,cardirection,(cardirection + 1) % 4);
            V(sw);
            V(intersection);
        } else if (cardirection == 2) {  //S
            P(intersection);
            message(0,carnumber,cardirection,(cardirection + 1) % 4);
            P(se);
            message(1,carnumber,cardirection,(cardirection + 1) % 4);
            P(ne);
            message(2,carnumber,cardirection,(cardirection + 1) % 4);
            V(se);
            P(nw);
            message(3,carnumber,cardirection,(cardirection + 1) % 4);
            V(ne);
            message(4,carnumber,cardirection,(cardirection + 1) % 4);
            V(nw);
            V(intersection);
        } else {    //W
            P(intersection);
            message(0,carnumber,cardirection,(cardirection + 1) % 4);
            P(sw);
            message(1,carnumber,cardirection,(cardirection + 1) % 4);
            P(se);
            message(2,carnumber,cardirection,(cardirection + 1) % 4);
            V(sw);
            P(ne);
            message(3,carnumber,cardirection,(cardirection + 1) % 4);
            V(se);
            message(4,carnumber,cardirection,(cardirection + 1) % 4);
            V(ne);
            V(intersection);
        }
//        (void) cardirection;
//        (void) carnumber;
}


/*
 * turnright()
 *
 * Arguments:
 *      unsigned long cardirection: the direction from which the car
 *              approaches the intersection.
 *      unsigned long carnumber: the car id number for printing purposes.
 *
 * Returns:
 *      nothing.
 *
 * Notes:
 *      This function should implement making a right turn through the
 *      intersection from any direction.
 *      Write and comment this function.
 */

static
void
turnright(unsigned long cardirection,
          unsigned long carnumber)
{
        /*
         * Avoid unused variable warnings.
         */

         if(cardirection == 0) { //N
            P(intersection);
            message(0,carnumber,cardirection,(cardirection + 3) % 4);
            P(nw);
            message(1,carnumber,cardirection,(cardirection + 3) % 4);
            message(4,carnumber,cardirection,(cardirection + 3) % 4);
            V(nw);
            V(intersection);
        } else if (cardirection == 1) {  //E
            P(intersection);
            message(0,carnumber,cardirection,(cardirection + 3) % 4);
            P(ne);
            message(1,carnumber,cardirection,(cardirection + 3) % 4);
            message(4,carnumber,cardirection,(cardirection + 3) % 4);
            V(ne);
            V(intersection);
        } else if (cardirection == 2) {  //S
            P(intersection);
            message(0,carnumber,cardirection,(cardirection + 3) % 4);
            P(se);
            message(1,carnumber,cardirection,(cardirection + 3) % 4);
            message(4,carnumber,cardirection,(cardirection + 3) % 4);
            V(se);
            V(intersection);
        } else {    //W
            P(intersection);
            message(0,carnumber,cardirection,(cardirection + 3) % 4);
            P(sw);
            message(1,carnumber,cardirection,(cardirection + 3) % 4);
            message(4,carnumber,cardirection,(cardirection + 3) % 4);
            V(sw);
            V(intersection);
        }
//        (void) cardirection;
//        (void) carnumber;
}


/*
 * approachintersection()
 *
 * Arguments:
 *      void * unusedpointer: currently unused.
 *      unsigned long carnumber: holds car id number.
 *
 * Returns:
 *      nothing.
 *
 * Notes:
 *      Change this function as necessary to implement your solution. These
 *      threads are created by createcars().  Each one must choose a direction
 *      randomly, approach the intersection, choose a turn randomly, and then
 *      complete that turn.  The code to choose a direction randomly is
 *      provided, the rest is left to you to implement.  Making a turn
 *      or going straight should be done by calling one of the functions
 *      above.
 */

static
void
approachintersection(void * unusedpointer,
                     unsigned long carnumber)
{
        int carDirection;
        int carDriving;

        /*
         * Avoid unused variable and function warnings.
         */


//        (void) unusedpointer;
//        (void) carnumber;
//	(void) gostraight;
//	(void) turnleft;
//	(void) turnright;

        /*
         * cardirection is set randomly.
         */

        carDirection = random() % 4;
        carDriving = random() % 3;
        if (carDriving == 0) {
            gostraight(carDirection,carnumber);
        } else if (carDriving == 1){
            turnleft(carDirection,carnumber);
        } else {
            turnright(carDirection,carnumber);
        }
}


/*
 * createcars()
 *
 * Arguments:
 *      int nargs: unused.
 *      char ** args: unused.
 *
 * Returns:
 *      0 on success.
 *
 * Notes:
 *      Driver code to start up the approachintersection() threads.  You are
 *      free to modiy this code as necessary for your solution.
 */

int
createcars(int nargs,
           char ** args)
{
        int index, error;

        /*
         * Avoid unused variable warnings.
         */

         nw = sem_create("NW",1);   // maximum 1 car in each region simultaneously
         ne = sem_create("NE",1);
         sw = sem_create("SW",1);
         se = sem_create("SE",1);
         intersection = sem_create("Intersection",3);   // maximum 3 cars in the intersection simultaneously
//        (void) nargs;
//        (void) args;

        /*
         * Start NCARS approachintersection() threads.
         */

        for (index = 0; index < NCARS; index++) {

                error = thread_fork("approachintersection thread",
                                    NULL,
                                    index,
                                    approachintersection,
                                    NULL
                                    );

                /*
                 * panic() on error.
                 */

                if (error) {

                        panic("approachintersection: thread_fork failed: %s\n",
                              strerror(error)
                              );
                }
        }

        return 0;
}
