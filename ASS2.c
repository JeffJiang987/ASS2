/* Program to plan package deliveries to be done by drone

   Skeleton written by Alistair Moffat, ammoffat@unimelb.edu.au,
   May 2026, with the intention that it be modified by students
   to add functionality, as required by the assignment specification.

   Student Authorship Declaration:

   (1) I certify that except for the code provided in the initial
   skeleton file the program contained in this submission is my
   own work, except where explicitly noted below in connection with
   the use of AI.  I understand that work that has been developed
   by another student, or by me in collaboration with other students,
   or by non-students as a result of request, solicitation, or
   payment, may not be submitted for assessment in this subject.

   (2) I also certify that I have not provided a copy of this work
   in either softcopy or hardcopy or any other form to any other
   student, and nor will I do so until after the marks are released.
   I understand that providing my work to other students, regardless
   of my intention or any undertakings made to me by that other
   student, is also Academic Misconduct.

   (3) I declare that I have used generative AI tools in connection
   with the following aspects of the assignment (please select the
   option that applies best in your case and delete the other three
   choices):

   -- I developed my own complete solution to the problem, and then
   used AI to help debug individual functions, find errors, and
   provide other guidance in regard to structure and so on.


   (4) The AI system(s) that I used are:

   Claude 

   (5) I understand that submitting for assessment work developed
   in collaboration with other people constitutes Academic Misconduct,
   and may be penalized by mark deductions, or by other penalties
   determined via the University of Melbourne Academic Honesty
   Policy, as described at https://academicintegrity.unimelb.edu.au.

   Signed by: [Ziqiang Jiang, 1625190]
   Dated:     [23/05/2026]

*/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>

/**********************************************************************/

/* put all your constants here */
#define MAX_CARRY 5.8
#define WEIGHT_D_B 3.8
#define V_HORIZONTAL 4.2
#define BATTERY 100
#define FMT_DB "%4.1lf"
#define MAXPACKAGE 999
#define FMT_INT "%2d"
#define SWITCH_T 120
#define OTHER_T 90
#define BATTERY_C1 57.0
#define BATTERY_C2 170.0
#define FLIGHT_C1 6.7
#define FLIGHT_C2 5.6


/**********************************************************************/

/* put all your typedefs and structs here */
typedef struct {
   double X;
   double Y;
   double W;
} Package_t;

/**********************************************************************/

/* put all your function prototypes here */

void stage_1(Package_t package[], int n);
void stage_2(double X[], double Y[], int n);
void stage_3(double X[], double Y[], int n);
void print_stage(int x);

/**********************************************************************/

int
main(int argc, char *argv[]) {
	
	/* you have to write the body of the main function, but don't
	   make it too long, it should control the traffic flow and use
	   functions to do the actual work
	*/

   int n=0; // n=buddy variabe to the parallel array

   Package_t package [MAXPACKAGE];

   while (scanf("%lf&lf&lf", &package[n].X, &package[n].Y, &package[n].W)==3){
      n++;  // buddy variable n to keep track of how many entries 
   }


	/* all done, time for a nap */
	return EXIT_SUCCESS;
}

/**********************************************************************/

/* add all your functions here, one function per major (or minor!)
   task
*/

void print_stage(int x){
    printf("-------\n");
    printf("Stage %d", x);
    printf("-------\n");
}

double distance(Package_t* package){
    
    double dis= sqrt(package->X*package->X+package->Y*package->Y);

    return dis;
}

double battery(Package_t* package){

    double battery= (BATTERY_C1+distance(package))*(WEIGHT_D_B+package->W)/BATTERY_C2;

    return battery;

}
double flight(Package_t* package){

    double flight=FLIGHT_C1*(WEIGHT_D_B+package->W)+distance(package)/(V_HORIZONTAL)+FLIGHT_C2;

    return flight;
}
void stage_1(Package_t package[], int n){

    print_stage(1);

    printf ("num packages :" FMT_INT "", n-1);

    double t_weight=0;

    for (int i=0; i<n; i++){ 

        t_weight=t_weight+package[i].W;
    }
        
    printf ("package " FMT_INT ": x=" FMT_DB ", y=" FMT_DB ", kg=" FMT_DB " ", 1, package[0].X, package[0].Y, package[0].W);

    printf ("package " FMT_INT ": x=" FMT_DB ", y=" FMT_DB ", kg=" FMT_DB " ", n-1, package[n-1].X, package[n-1].Y, package[n-1].W);

    printf ("total weight :" FMT_DB "kg", t_weight);
}

void stage_2(Package_t package[], int n){


}   

void stage_3(double X[], double Y[], int n){

    int num_comp[MAXCOMPONENT];
    double new_Y[MAXCOMPONENT];
    double total_wastage=0;
    

    for (int i=1; i<n;i++){
        num_comp[i]=i;
    }
    
    sort_array(X,num_comp,n);
        
    optimize_array(X,num_comp,n);

    for (int i=1; i<n;i++){
        new_Y[i]= Y[num_comp[i]];
    }
    
   
    for (int i=1; i<n; i++){  //handle "stage 1" like structure 

        if (numcoil(X, i)>numcoil(X, i-1)){
        printf ("\n");
        }
        printf ("component " INT_FMT ", " DIMEN_FMT " starting ", num_comp[i],X[i], new_Y[i]);
        printf ("" DIMEN_FMT " on coil " INT_FMT "\n", start_x(X,i), START_Y, numcoil(X,i));
    }

    printf("\n");


    for (int j=1; j<=numcoil(X,n-1); j++){ //handle "stage 2" like structure 
        printf("coil " INT_FMT ", internal wastage  %4.1f m^2", j, internal_waste(X,new_Y,j,n));
        total_wastage += internal_waste(X,new_Y,j,n);
        if (j != numcoil(X,n-1)){
            printf(", end wastage  %4.1f m^2\n", end_wastage(X,j,n));
            total_wastage += end_wastage(X,j,n);
        } else {
            break;
        }
    }      
    
    printf ("\noverall,     total wastage  %4.1f m^2\n", total_wastage);

    printf("\n");

    printf("tadaa!");
    
}
