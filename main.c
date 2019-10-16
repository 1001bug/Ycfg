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
  Yvoid_t Tree=YreadCfg(NULL,argv[1]);
  
  
  

  printf("\n\n\n");
  
  print_t(Tree);
  
  printf("\n\n\n");
  
  
  destroy_t(Tree);
  
  printf("\n\n\n");
  
  /*
  

  printf("\n\nSearch for transmitter, then print reys\n");
  YML_NODE_s * T = yaml_get_node_by_name(R,"transmitter");
  if(T){
      printf("All types\n");
      print_keys(T);
      printf("type SEQ only\n");
      print_keys_type(T,YAML_SEQUENCE_NODE);
      printf("type SCALAR only\n");
      print_keys_type(T,YAML_SCALAR_NODE);
  }
  
   tab=0;
  printf("\n\nprint transmitter tree\n");
  if(T)
    print_y(T);
  
  
  
  
  printf("\n\nSearch for level_1_B after root, then print reys\n");
  YML_NODE_s * M = yaml_get_node_by_name(T,"level_1_LISTMAP");
  if(M){
      print_keys(M);
      print_keys_value(M);
  }
  
  
  
  
  printf("recivier YAML_MAPPING_NODE\n");
  YML_NODE_s * F = yaml_get_node_by_name_and_type(R,"recivier",YAML_MAPPING_NODE);
  if(F){
      print_keys(F);
      print_keys_value(F);
  }
  
  
  
  
  printf("Get tier_config node\n");
  YML_NODE_s * Tier = yaml_get_node_by_name_and_type(R,"tier_config",YAML_MAPPING_NODE);
  if(Tier){
      
      printf("Get OK, print seq 'test_seq22'\n");
      print_seq_items(yaml_get_node_by_name_and_type(Tier,"test_seq22",YAML_SEQUENCE_NODE));
      
  }
  
  */
  
  printf("\n\nFINITA FINITA\n\n");
  
  

  
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






