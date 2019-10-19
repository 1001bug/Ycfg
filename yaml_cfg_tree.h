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
    #include <stdlib.h>
    #include<errno.h>
    #include "yaml_cfg_tree_pub.h"

    

    typedef uintptr_t yaml_val;

    typedef struct YML_NODE_s {
        char *tag; //scalar key, map tag, seq tag
        yaml_val data; //scalar value (char*) or next child node (YML_NODE*)
        yaml_node_type_t type; //describe what `data` is (scalar value or link to map/seq node)
        struct YML_NODE_s *next; //next node same level
    } YML_NODE_s;


    //recursion parsing
    static int mapping(yaml_parser_t *parser, YML_NODE_s **root);
    static int seqencing(yaml_parser_t *parser, YML_NODE_s **root);
    static int byevent(yaml_parser_t *parser, YML_NODE_s **root);
    
    //recur print tree to stdout
    static void print_y(YML_NODE_s *R);
    static void print_yy(YML_NODE_s *R);
    
    //recur free tree
    static void destroy_y(YML_NODE_s *R);
    static void destroy_yy(YML_NODE_s *R);
    
    //remember error reason
    static void YError(yaml_error_type_t e, const char *problem, size_t line);

    //traverce tree to certain node
    static YML_NODE_s * YNode(YML_NODE_s * Tree, char * nodeTagPath, yaml_node_type_t lastNodeType);

#ifdef __cplusplus
}
#endif

#endif /* YAML_CFG_TREE_H */

