/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: alex
 *
 * Created on 10 октября 2019 г., 22:10
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdint.h>
#include "yaml_cfg_tree_pub.h"


int main(int argc, char** argv)
{
  setvbuf(stdout, (char*)NULL, _IONBF, 0);
  
  //аналогия с Judy. Массив это Pvoid_t (void*), а в функции передается &Pvoid_t (т.е. PPvoid_t)
  //Yvoid_t Tree=YreadCfg(argv[1]);

//uintptr_t i=NAN;
double i=NAN;
printf("i=%f \n",i);
printf("i is nan? %i\n",isnan(i));
uintptr_t a = NAN;
printf("a %f\n",a);
printf("? %i\n",isnan((double)a));
  
}


/*
 * 
 */
/*
typedef struct YML_KVP_s{
    char *key;
    char *val;
    struct YML_KVP_s *next;
}YML_KVP_s;

typedef struct YML_SEQ_s{
    char *val;
    struct YML_SEQ_s *next;
}YML_SEQ_s;

typedef union {
    YML_SEQ_s *SeQ;
    YML_KVP_s *KvP;
    char *val;
} yaml_val;
*/






