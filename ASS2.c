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
#define MAX_BATTERY 100
#define FMT_DB "%4.1lf"
#define MAXPACKAGE 999
#define FMT_INT "%2d"
#define SWITCH_T 120
#define OTHER_T 90
#define BATTERY_C1 57.0
#define BATTERY_C2 170.0
#define FLIGHT_C1 6.7
#define FLIGHT_C2 5.6
#define V_VAN 22.0
#define THROW_DIS 5
#define ORIGIN_X 0.0
#define ORIGIN_Y 0.0
/**********************************************************************/

/* put all your typedefs and structs here */
typedef struct {
   double X;
   double Y;
   double W;
   double distance;
   double battery_out;
   double battery_in;
   double flight_out;
   double flight_in;
   int delivered;
} Package_t;

/**********************************************************************/

/* put all your function prototypes here */

double distance(Package_t* package,double X_base, double Y_base);
double battery_out(Package_t* package);
double battery_in(Package_t* package);
double flight_out(Package_t* package);
double flight_in(Package_t* package);
double battery_cost(Package_t* package);
void stage_1(Package_t package[], int n);
void stage_2(Package_t package[], int n);
void stage_3(Package_t package[], int n);
void print_stage(int x);

/**********************************************************************/

int
main(int argc, char *argv[]) {

	/* you have to write the body of the main function, but don't
	   make it too long, it should control the traffic flow and use
	   functions to do the actual work
	*/ 
    int n=0;

    Package_t package [MAXPACKAGE];

    while (scanf("%lf%lf%lf", &package[n].X, &package[n].Y, &package[n].W)==3){
      n++;  // buddy variable n to keep track of how many entries 
    }

    compute_struct(package, ORIGIN_X, ORIGIN_Y, n);

    stage_1(package,n);
    stage_2(package,n);

	/* all done, time for a nap */
	return EXIT_SUCCESS;
}

void compute_struct (Package_t package[],double X_base, double Y_base,int n){

    for (int i=0; i<n;i++){ // populate the struct 
        package[i].distance = distance(&package[i], X_base, Y_base);
        package[i].battery_out= battery_out(&package[i]);
        package[i].battery_in= battery_in(&package[i]);
        package[i].flight_out= flight_out(&package[i]);
        package[i].flight_in=flight_in(&package[i]);
        package[i].delivered=0; //initialize the tracking of whether package being delivered
    }
}

/**********************************************************************/

/* add all your functions here, one function per major (or minor!)
   task
*/

void print_stage(int x){
    printf("-------\n");
    printf("Stage %d\n", x);
    printf("-------\n");
}

double distance(Package_t* package, double X_base, double Y_base){

    double dis= sqrt(pow(package->X- X_base,2)+pow(package->Y-Y_base,2));

    return dis;
}

double battery_out(Package_t* package){

    double battery= (BATTERY_C1+package->distance)*(WEIGHT_D_B+package->W)/BATTERY_C2;

    return battery;

}

double battery_in(Package_t* package){

    double battery= (BATTERY_C1+package->distance)*(WEIGHT_D_B+0)/BATTERY_C2;

    return battery;

}
double flight_out(Package_t* package){

    double flight=FLIGHT_C1*(WEIGHT_D_B+package->W)+package->distance/(V_HORIZONTAL)+FLIGHT_C2;

    return flight;
}

double flight_in(Package_t* package){

    double flight=FLIGHT_C1*(WEIGHT_D_B+0)+package->distance/(V_HORIZONTAL)+FLIGHT_C2;

    return flight;
}

double battery_cost(Package_t* package){

    double sum=package->battery_in+package->battery_out;

    return sum;

}

int pick_max (Package_t package[], int n, double battery_remain){
    int max=-1;  // -1 means no appropriate package found 

    for (int i=0; i<n; i++){
        if (package[i].delivered==0){
            if (battery_cost(&package[i])<=battery_remain||package[i].distance<THROW_DIS){ 
                if (max==-1){
                    max=i;
                }else if (battery_cost(&package[i])>=battery_cost(&package[max])) {
                    max=i;
                }
            }

        }

    }

    return max;
}

void stage_1(Package_t package[], int n){

    print_stage(1);

    printf ("num packages :" FMT_INT "\n", n-1);

    double t_weight=0;

    for (int i=0; i<n; i++){ 

        t_weight=t_weight+package[i].W;
    }
        
    printf ("package " FMT_INT ": x=" FMT_DB ", y=" FMT_DB ", kg=" FMT_DB " \n", 1, package[0].X, package[0].Y, package[0].W);

    printf ("package " FMT_INT ": x=" FMT_DB ", y=" FMT_DB ", kg=" FMT_DB " \n", n-1, package[n-1].X, package[n-1].Y, package[n-1].W);

    printf ("total weight :" FMT_DB "kg \n", t_weight);
}


//int num_delivered(Package_t package[], int n){
   // int sum=0;

   // for (int i=0; i<n; i++){
   //     sum= sum+ package[i].delivered;
   // }
   // return sum;
//}

void stage_2(Package_t package[], int n){

    print_stage(2);

    int num_delivered=0, battery_change, p_num;
    double battery_remain=MAX_BATTERY, total_time=0;

    while (num_delivered<n){
        battery_change=0;

        p_num= pick_max(package,n, battery_remain); // pick the package number to be delivered in this iteration
        
        if (p_num==-1){
            battery_remain=MAX_BATTERY;
            p_num= pick_max(package,n, battery_remain);
            battery_change=1;
        }
        

        printf("package " FMT_INT "\n", p_num+1);

        if (battery_change==1){

            total_time =total_time+SWITCH_T;

            printf("change battery :" FMT_INT " sec, total " FMT_DB " sec, battery is " FMT_DB "% \n", SWITCH_T, total_time, battery_remain);
        }

        total_time= total_time+OTHER_T;
        printf("load drone : " FMT_INT "sec, total " FMT_DB " sec, battery is " FMT_DB " \n", OTHER_T, total_time,battery_remain );
        total_time= total_time+package[p_num].flight_out;
        battery_remain=battery_remain-package[p_num].battery_out;
        printf("drone out  : " FMT_DB "sec, total " FMT_DB " sec, battery is " FMT_DB " \n", package[p_num].flight_out, total_time, battery_remain );
        total_time= total_time+package[p_num].flight_in;
        battery_remain=battery_remain-package[p_num].battery_in;
        printf("drone return : " FMT_DB "sec, total " FMT_DB " sec, battery is " FMT_DB " \n", package[p_num].flight_in, total_time, battery_remain );



        package[p_num].delivered=1;

        num_delivered++;

    }

}   
double simulate_time(Package_t package[], int n){

    int num_delivered=0, battery_change, p_num;
    double battery_remain=MAX_BATTERY, total_time=0;

    while (num_delivered<n){
        battery_change=0;

        p_num= pick_max(package,n, battery_remain); // pick the package number to be delivered in this iteration
        
        if (p_num==-1){
            battery_remain=MAX_BATTERY;
            p_num= pick_max(package,n, battery_remain);
            battery_change=1;
        }

        if (battery_change==1){

            total_time =total_time+SWITCH_T;

        }

        total_time= total_time+OTHER_T+package[p_num].flight_out+package[p_num].flight_in;

        battery_remain=battery_remain-package[p_num].battery_out-package[p_num].battery_in;

        package[p_num].delivered=1;

        num_delivered++;
        
    }
    return total_time;
}   

double van_time(Package_t* package){
    double dis= fabs(package->X)+ fabs(package->Y);
    double time= dis/V_VAN;
    return time;
}

void throwout(Package_t temp[], int n){

    for (int i=0; i<n; i++){
        if (temp[i].distance<=THROW_DIS){
            temp[i].delivered=1;
        }
    }
}

int pick_start(Package_t temp[], int n, double best_time){
    int best=-1;
    double simulated_time;
    for (int i=0; i<n; i++){
        compute_struct(&temp,temp[i].X,temp[i].Y,n); //repopulating the struct temp with the ith starting point
        throwout(temp,n);
        simulated_time = simulate_time (temp, n)+van_time(&temp)*2;

        if (simulated_time<best_time) {
            best=i;
            best_time=simulated_time;
        }
    return best;
    }
}

void make_temp(Package_t package[],Package_t temp[], int n){

    for (int i=0; i<n; i++){
        temp[i].X=package[i].X;
        temp[i].Y=package[i].Y;
        temp[i].W=package[i].W;  
    }
}

void stage_3(Package_t package[], int n){
    print_stage(3);

    double best_time=simulate_time (package, n);
    
    Package_t temp [MAXPACKAGE];

    make_temp(temp, n);

    int start = pick_start(temp,n, best_time);

    if (start==-1){
        printf("deliver the packages from the warehouse");
        return;
    } else {
        compute_struct(&temp,temp[start].X,temp[start].Y,n)
    }

    while (num_delivered<n){

        battery_change=0;

        p_num= pick_max(temp,n, battery_remain); // pick the package number to be delivered in this iteration

        if (<THROW_DIS){

        }
        
        if (p_num==-1){
            battery_remain=MAX_BATTERY;
            p_num= pick_max(temp,n, battery_remain);
            battery_change=1;
        }

        if 
        

        printf("package " FMT_INT "\n", p_num+1);

        if (battery_change==1){

            total_time =total_time+SWITCH_T;

            printf("change battery :" FMT_INT " sec, total " FMT_DB " sec, battery is " FMT_DB "% \n", SWITCH_T, total_time, battery_remain);
        }

        total_time= total_time+OTHER_T;
        printf("load drone : " FMT_INT "sec, total " FMT_DB " sec, battery is " FMT_DB " \n", OTHER_T, total_time,battery_remain );
        total_time= total_time+package[p_num].flight_out;
        battery_remain=battery_remain-package[p_num].battery_out;
        printf("drone out  : " FMT_DB "sec, total " FMT_DB " sec, battery is " FMT_DB " \n", package[p_num].flight_out, total_time, battery_remain );
        total_time= total_time+package[p_num].flight_in;
        battery_remain=battery_remain-package[p_num].battery_in;
        printf("drone return : " FMT_DB "sec, total " FMT_DB " sec, battery is " FMT_DB " \n", package[p_num].flight_in, total_time, battery_remain );

        package[p_num].delivered=1;

        num_delivered++;

    }



}
