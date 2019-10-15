/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   yaml_cfg_tree.h
 * Author: alex
 *
 * Created on 15 октября 2019 г., 21:31
 */

#ifndef YAML_CFG_TREE_H
#define YAML_CFG_TREE_H

#ifdef __cplusplus
extern "C" {
#endif
    
    #define _GNU_SOURCE
    #include <stdio.h>
    #include <stdio.h>
    #include <yaml.h>
    #include <stdint.h>
    #include <assert.h>
#include "yaml_cfg_tree_pub.h"

    

    typedef uintptr_t yaml_val;

    typedef struct YML_NODE_s {
        char *tag;
        yaml_val data; //char* or YML_NODE*
        yaml_node_type_t type;
        //struct YML_NODE_s *head;
        struct YML_NODE_s *next; //next item same level


    } YML_NODE_s;


    
    void seqencing(yaml_parser_t *parser, char *pr, YML_NODE_s *R);
    void mapping(yaml_parser_t *parser, char *pr, YML_NODE_s *R);
    void byevent(yaml_parser_t *parser, YML_NODE_s *RR);
    void print_y(YML_NODE_s *R);
    void print_yy(YML_NODE_s *R);

#ifdef __cplusplus
}
#endif

#endif /* YAML_CFG_TREE_H */

