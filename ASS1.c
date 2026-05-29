/* Program to allocate rectangular cutting segments on a steel coil.
Skeleton written by Alistair Moffat, ammoffat@unimelb.edu.au,
April 2026, with the intention that it be modified by students
to add functionality, as required by the assignment specification.
Student Authorship Declaration:
(1) I certify that except for the code provided in the initial
skeleton file the program contained in this submission is my
own work, except where explicitly noted below in connection with
the use of AI. I understand that work that has been developed
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

I developed my own complete solution to the problem, and then
used AI to help debug individual functions, find errors, and
provide other guidance in regard to structure and so on.

(4) The AI system(s) that I used are: Claude.

(5) I understand that submitting for assessment work developed
in collaboration with other people constitutes Academic Misconduct,
and may be penalized by mark deductions, or by other penalties
determined via the University of Melbourne Academic Honesty
Policy, as described at https://academicintegrity.unimelb.edu.au.
Signed by: Ziqiang Jiang
Dated: 30/04/2026
*/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#define MAXCOIL 99
#define MAXCOMPONENT 999
#define WIDTH_COIL 3.5
#define LENGTH_COIL 24
#define START_Y 0.0
#define DIMEN_FMT "(%4.1f,%4.1f)"
#define INT_FMT "%3d"

void int_swap(int *p1, int *p2);
void dbl_swap(double *p1, double *p2);
double internal_waste(double X[], double Y[],int j, int n);
void swap(double X[], double Y[], int n);
double start_x(double X[], int i);
int numcoil(double X[], int i);
void update(double X[], double* x, double* num);
double end_wastage(double X[], int j, int n);
void sort_array(double X[], int num_comp[], int n);
void stage_1(double X[], double Y[], int n);
void stage_2(double X[], double Y[], int n);
void stage_3(double X[], double Y[], int n);
void optimize_array(double X[], int num_comp[],int n);


int
main(int argc, char *argv[]) {

    int n=1; // n=buddy variabe to the parallel array

    double X[MAXCOMPONENT], Y[MAXCOMPONENT];

    X[0]=0; //intializing the zeroth component 
    Y[0]=0;

    for (int i=0; i<n; i++){
        if (scanf(&X, &Y)==2){
            n++;  // buddy variable n to keep track of how many entries 
        }
    }

    swap(X, Y, n); //rearrange the whole arrays 

    printf("\n-------\nStage 1\n-------\n");

    stage_1(X,Y,n);

    printf("\n-------\nStage 2\n-------\n");

    stage_2(X,Y,n);

    printf("\n-------\nStage 3\n-------\n");

    stage_3(X,Y,n);

    printf("\nProgramming is fun!");

    return EXIT_SUCCESS;

}

void stage_1(double X[], double Y[], int n){

    for (int i=1; i<n; i++){ 

        if (numcoil(X, i)>numcoil(X, i-1)){
        printf ("\n");
        }
        printf ("component " INT_FMT ", " DIMEN_FMT " starting ", i,X[i], Y[i]);
        printf ("" DIMEN_FMT " on coil " INT_FMT "\n", start_x(X,i), START_Y, numcoil(X,i));
    }
   
}

void stage_2(double X[], double Y[], int n){

    double total_wastage=0;

    for (int j=1; j<=numcoil(X,n-1); j++){
        printf("coil " INT_FMT ", internal wastage  %4.1f m^2", j, internal_waste(X,Y,j,n));
        total_wastage += internal_waste(X,Y,j,n);
        if (j != numcoil(X,n-1)){
            printf(", end wastage  %4.1f m^2\n", end_wastage(X,j,n));
            total_wastage += end_wastage(X,j,n);
        } else {
            break;
        }
    }      
    
    printf ("\noverall,     total wastage  %4.1f m^2\n", total_wastage);
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

void swap(double X[], double Y[], int n){
    for (int i=1; i<=n;i++){
    if (X[i]>Y[i] && X[i]<=WIDTH_COIL){ 
        dbl_swap(&X[i], &Y[i]);
        }
    }
}

double start_x(double X[],int i){
    double x_coord=0;
    for (int j=1; j<=i; j++){ 

        x_coord+=X[j-1];
        
        if (x_coord+X[j]>LENGTH_COIL){ // gives the end x value for the new iteration
                x_coord=0; 
            } 
         }
    return x_coord;
}

int numcoil(double X[], int i){
    int ncoil=1;

    for (int j=1; j<=i; j++){
        if  ((start_x(X, j-1)+X[j-1]+X[j])>LENGTH_COIL){
            ncoil++;
        }
    }
    return ncoil;
}

double internal_waste(double X[], double Y[], int j, int n){

    double waste_1=0; 
  
    for (int i=1; i<=n;i++){ 
        if (numcoil(X,i)==j){ //the condition gives the lower+upper bound of the ith coil 
            waste_1+=(WIDTH_COIL-Y[i])*X[i]; 
        }
    }

    return waste_1;
}

double end_wastage(double X[], int j, int n){
    double waste_2=0;

    for (int i=1; i<=n;i++){ 
        if (numcoil(X,i)==j){ 

            waste_2=(LENGTH_COIL-(start_x(X, i)+X[i]))*WIDTH_COIL;
        }
    }
    
    return waste_2;
}

void sort_array(double X[], int num_comp[],int n){ 
    for (int i=1; i<=n; i++){
        for (int j=i-1; j>=1&&X[j]<X[j+1]; j--){
            dbl_swap(&X[j],&X[j+1]);
            int_swap(&num_comp[j],&num_comp[j+1]);//need to update the array num_comp which tracks the component number
        }
    }
}

void optimize_array(double X[],int num_comp[], int n){ 
    double x_pos=0;
    
    for (int i=1; i<n; i++){
        
        if(x_pos+X[i]<=24){
            x_pos+=X[i];
        } else {
            int found=0; // found is like the guard that determines if new coil or search for one that fits
            for (int j=1+i; j<n; j++){
                if (x_pos+X[j]<=24){
                    dbl_swap(&X[j],&X[i]);
                    int_swap(&num_comp[j],&num_comp[i]);
                    x_pos+=X[i];
                    sort_array(X+i,num_comp+i,n-i); // would be (X+i+1,n-i-1) if my sort_array started with index 0
                    found=1;
                    break;
                }
            }
            if (!found){
                x_pos=0;
                i--;
            }
        }
    }
}



void
int_swap(int *p1, int *p2){
int tmp;
tmp = *p1;
*p1 = *p2;
*p2 = tmp;
}


void
dbl_swap(double *p1, double *p2){
double tmp;
tmp = *p1;
*p1 = *p2;
*p2 = tmp;
}
