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
#include <stdlib.h>

#include "yaml_cfg_tree_pub.h"

int main(int argc, char** argv) {
    setvbuf(stdout, (char*) NULL, _IONBF, 0);

    //аналогия с Judy. Массив это Pvoid_t (void*), а в функции передается &Pvoid_t (т.е. PPvoid_t)
    Yvoid_t Tree = YreadCfg(NULL, argv[1]);

    if (Tree == NULL) {
        printf("Read YAML failed\n");
        int line = 0;
        char *desc = NULL;
        char *err = NULL;

        int r = YlastError(&err, &desc, &line);
        printf("YlastError: r= %i, err=%s, desc=%s, line=%i\n", r, err, desc, line);
        return -1;
    }



    printf("\n\n\n");

    print_t(Tree);

    printf("\n\n\n");

    Yvoid_t tier_config = YmapNode(Tree, "tier_config");
    if (tier_config) {
        printf("Node 'tier_config' found\n");
        char *val = NULL;
        if (val = YmapVal(tier_config, "log_level")) {
            printf("'tier_config' -> 'log_level' = %s\n", val);
        } else
            printf("'tier_config' -> 'log_level' = [not found]\n");
    } else
        printf("tier_config NOT found\n");





    printf("\n");




    Yvoid_t tier_config_tx_config = YmapNode(Tree, "tier_config:tx_config");
    if (tier_config_tx_config) {
        printf("Node 'tier_config:tx_config' found\n");
        char *val = NULL;
        if (val = YmapVal(tier_config_tx_config, "MaxMemoryAllowedKBytes")) {
            printf("'tier_config:tx_config' -> 'MaxMemoryAllowedKBytes' = %s\n", val);
        } else
            printf("'tier_config:tx_config' -> 'MaxMemoryAllowedKBytes' = [not found]\n");

    } else
        printf("'tier_config:tx_config' NOT found\n");


    printf("\n");



    char * tier_config__ancillary_params = YmapVal(Tree, "tier_config:ancillary_params");
    if (tier_config__ancillary_params)
        printf("MapVAl 'tier_config:ancillary_params' = %s\n", tier_config__ancillary_params);
    else
        printf("MapVAl 'tier_config:ancillary_params' = [not found]\n");


    printf("\n");

    char * tier_config__tx_config__MaxMemoryAllowedKBytes = YmapVal(Tree, "tier_config:tx_config:MaxMemoryAllowedKBytes");
    if (tier_config__tx_config__MaxMemoryAllowedKBytes)
        printf("MapVAl 'tier_config:tx_config:MaxMemoryAllowedKBytes' = %s\n", tier_config__tx_config__MaxMemoryAllowedKBytes);
    else
        printf("MapVAl 'tier_config:tx_config:MaxMemoryAllowedKBytes' = [not found]\n");




    printf("\n");




    Yvoid_t tier_config_Rx_config = YmapNode(Tree, "tier_config:rx_config");
    if (tier_config_Rx_config) {
        printf("Map Node 'tier_config:rx_config' found\n");
        int char_list_num = 0;
        char **list = YseqVals(tier_config_Rx_config, "topics", &char_list_num);
        if (char_list_num) {
            printf("SEQ vals for 'topics':\n");
            for (int i = 0; i < char_list_num; i++) {
                printf("\t%s\n", list[i]);
            }
            free(list);
        } else
            printf("Seq Node list empty\n");

    } else
        printf("Map Node 'tier_config:rx_config' NOT found\n");


    
    
    
    
    
    {
    Yvoid_t tier_config_Rx_config = YseqNode(Tree, "tier_config:rx_config:topics");
    if (tier_config_Rx_config) {
        printf("Map Node 'tier_config:rx_config' found\n");
        int char_list_num = 0;
        char **list = YseqVals(tier_config_Rx_config, NULL, &char_list_num);
        if (char_list_num) {
            printf("SEQ vals for 'topics':\n");
            for (int i = 0; i < char_list_num; i++) {
                printf("\t%s\n", list[i]);
            }
            free(list);
        } else
            printf("Seq Node list empty\n");

    } else
        printf("Seq Node 'tier_config:rx_config:topics' NOT found\n");

    }
    
    
    
    

    printf("\n");





    int tree_list_num = 0;
    Yvoid_t *tlist = YseqList(Tree, "test_root:test_subroot", &tree_list_num);
    if (tlist) {
        printf("Seq list 'test_root:test_subroot' found\n");

        char *temp;
        for(int i=0;i<tree_list_num;i++){
        if (temp = YmapVal(tlist[i], "mapkey1"))
            printf("List N%i, 'mapkey1': %s\n", i, temp);
        if (temp = YmapVal(tlist[i], "mapkey2"))
            printf("List N%i, 'mapkey2': %s\n", i, temp);
        printf("\n");
        }
        
        

        free(tlist);
    }

    
    
    
    
    

    destroy_t(Tree);



    printf("\n\n\n");

    printf("\n\nFINITA FINITA\n\n");




}






