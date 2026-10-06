/*
 * main.c
 *Hold vector navigation and operation functions
 * Nick Erdmann
 * 9/29/2026
 * version 1
 * make, ./main
 */

 #include <stdio.h>
 #include <string.h>

 #include "vector_calc.h"


 //Add method       : void add_vectors(Vector a, Vector b)
 void add_vectors(Token a, Token b){
//find given vectors a and b
Vector alpha = find_vector(a);
Vector beta = find_vector(b);
//save answer in temp

Vector temp;

//one vector is lenght two
if ((alpha.z == null) || (beta.z == null)){
    
    if (alpha.z == null){
    //alpha is length two
    temp.x = alpha.x + beta.x;
    temp.y = alpha.y + beta.y;
    temp.z = beta.z;


    }else{
    //beta is length two

    }

}
//both vectors are length 3
else{
    temp.x = 
    temp.y = 
    temp.z = 
}

//have position: save in location, print



//no posistion: save in ans, print



 }




 //Subtract method  : void sub_vectors()
//find given vectors a and b
//save answer in temp
//have position: save in location, print
//no posistion: save in ans, print






 //multiply method  : void mult_vectors()
//find given vectors a and b
//save answer in temp
//have position: save in location, print
//no posistion: save in ans, print





//dot method        : void dot_vector()
//find given vectors a and b
//save answer in temp
//have position: save in location, print
//no posistion: save in ans, print




 //cross pro method : void cross_vectors()
//find given vectors a and b
//save answer in temp
//have position: save in location, print
//no posistion: save in ans, print





 //clear            : void clear_array()
//iterate through array setting all vals to null 




 //list             : void list_array()
//iterate through array printing values, exc ans







 //find vect        : Vector find_vector()
//interate through array until name matches
//return match




 //find answer vect : Vector find_answer_vector()
//find ans_vector name, name found: return vector pos, name not found space available: create new vecor in available pos return vector pos




//print vector      : void print_vector()



