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

#include <math.h>


#ifdef __cplusplus
extern "C" {
#endif

    typedef void * Yvoid_t;
    typedef void ** PYvoid_t;
    
    void print_t(Yvoid_t *Tree);
    void destroy_t(Yvoid_t *Tree);
    
    Yvoid_t YreadCfg(FILE *fd, char* file);
    
    //зачем везде PYvoid_t????
    
    //поиск tag==nodeTag с перемещением по next (тот же уровень, низ)
    //возвкат ссылки из поля data (с проверкой типа!!)
    Yvoid_t YmapNode(PYvoid_t Tree, char * nodeTagPath); 
    Yvoid_t YseqNode(PYvoid_t Tree, char * nodeTagPath); 
    // nodeTag может быть именем, а может это путь?
    // tier_config:rx_config -> мапа или tier_config:rx_config:topics -> список
    
    //просто перемещение вниз по next (проверять что next != NULL и type != NO_TYPE
    //в этом спике могут быть и SEQ и MAP и SCA. как быть? перепрыгивать?
    Yvoid_t YmapNodeNext(PYvoid_t Tree);
    Yvoid_t YseqNodeNext(PYvoid_t Tree);
    //за map это особого смысла не имеет, а вот для списка да. когда он безимянный и содержит в себя мапу
    
    int isYmap(PYvoid_t Tree);
    int isYseq(PYvoid_t Tree);
    int isYsca(PYvoid_t Tree);
    
    //tag текущего блока
    char* YmapTag(PYvoid_t Tree);
    //data текущего блока (с проверкой что это SCALAR
    char* YmapVal(PYvoid_t Tree);
    
    char* YseqTag(PYvoid_t Tree); //имя текущего блока, если есть
    char** YseqTags(PYvoid_t Tree,int *n); //массив ссылок на строки (проверка что там SCALAR на не мапа!!)
    
    int YgetTags(PYvoid_t Tree,char *path_to_key, char **vals, int *n);
    int YgetVal(PYvoid_t Tree,char *path_to_key, char **val); //нужно ли возвращать ссылку на key?
    /*внутри нее может использоваться YmapNode()
     * пример
     char *val;
     if(YgetVal(Tr,"tier_config:log_level",&val)){
        printf("Val=%s",val);
        //Val=RMM_LOGLEV_EVENT
     }
     
    if(YgetVal(Tr,"tier_config:rx_config:MulticastInterface",&val)){
        printf("Val=%s",val);
        //Val=0.0.0.0
     }
    */
    
    
    



#ifdef __cplusplus
}
#endif

#endif /* YAML_CFG_TREE_PUB_H */

