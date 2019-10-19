/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   yaml_cfg_tree_pub.h
 * Author: alex
 *
 * Created on 15 октября 2019 г., 22:56
 */

#ifndef YAML_CFG_TREE_PUB_H
#define YAML_CFG_TREE_PUB_H




#ifdef __cplusplus
extern "C" {
#endif

    typedef void * Yvoid_t;
    //typedef void ** PYvoid_t;
    
    void print_t(Yvoid_t Tree);
    void destroy_t(Yvoid_t Tree);
    
    
    //Public - load yaml from file
    Yvoid_t YreadCfg(FILE *fd, char* file);
    
    //Public fun - find MAPPING Node by path
    Yvoid_t YmapNode(Yvoid_t Tree, char * nodeTagPath); 

    //Public fun - find SEQUENCE Node by path
    Yvoid_t YseqNode(Yvoid_t Tree, char * nodeTagPath); 

    //Public fun - get MAP value by path (root:subroot:key) => val
    char* YmapVal(Yvoid_t Tree, char *nodeTagPath);

    //Public fun - get list of SEQUENCE strings (free() required!)
    char** YseqVals(Yvoid_t Tree, char *nodeTagPath, int *N);

    //Public fun - get list of SEQUENCE MAPS
    Yvoid_t* YseqList(Yvoid_t Tree, char *nodeTagPath, int *N);

    //Public fun - get last error
    int YlastError(char **error, char **description, int *line);
    
    
    
    



#ifdef __cplusplus
}
#endif

#endif /* YAML_CFG_TREE_PUB_H */

