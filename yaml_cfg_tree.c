/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include "yaml_cfg_tree.h"

#include<unistd.h>
#include <bits/string2.h>




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




/** Error type. */
static __thread yaml_error_type_t parcer_error = YAML_NO_ERROR;
/** Error description. */
#define ERR_DESCR_LEN 500
static __thread char parcer_problem[ERR_DESCR_LEN] = {0};
static __thread size_t parcer_problem_line = 0;


int tab = 0;

void p(int v) {
    for (int i = 0; i < v; i++)
        printf("  ");
}

//Inernal fun - remeber parser error

static void YError(yaml_error_type_t e, const char *problem, size_t line) {
    parcer_error = e;
    parcer_problem_line = line;
    memset(parcer_problem, 0, ERR_DESCR_LEN);
    strncpy(parcer_problem, problem, ERR_DESCR_LEN - 1);
    return;
}

/*
 * Public fun - get last parces error
 */
int YlastError(char **error, char **description, int *line) {

    *description = parcer_problem;
    *line = (int) parcer_problem_line;

    switch (parcer_error) {
            /** No error is produced. */
        case YAML_NO_ERROR: *error = "YAML_NO_ERROR";
            return (int) parcer_error;

            /** Cannot allocate or reallocate a block of memory. */
        case YAML_MEMORY_ERROR: *error = "YAML_MEMORY_ERROR";
            return (int) parcer_error;

            /** Cannot read or decode the input stream. */
        case YAML_READER_ERROR: *error = "YAML_READER_ERROR";
            return (int) parcer_error;
            /** Cannot scan the input stream. */
        case YAML_SCANNER_ERROR: *error = "YAML_SCANNER_ERROR";
            return (int) parcer_error;
            /** Cannot parse the input stream. */
        case YAML_PARSER_ERROR: *error = "YAML_PARSER_ERROR";
            return (int) parcer_error;
            /** Cannot compose a YAML document. */
        case YAML_COMPOSER_ERROR: *error = "YAML_COMPOSER_ERROR";
            return (int) parcer_error;

            /** Cannot write to the output stream. */
        case YAML_WRITER_ERROR: *error = "YAML_WRITER_ERROR";
            return (int) parcer_error;
            /** Cannot emit a YAML stream. */
        case YAML_EMITTER_ERROR: *error = "YAML_EMITTER_ERROR";
            return (int) parcer_error;
    }
    return 0;
}

//Inernal - parcing seqence block

int seqencing(yaml_parser_t *parser, YML_NODE_s **Proot) {
    yaml_event_t event;


    if (Proot == NULL)
        return -1;
    if (*Proot == NULL)
        *Proot = calloc(1, sizeof (YML_NODE_s));

    YML_NODE_s *root = *Proot;

    do {
        if (!yaml_parser_parse(parser, &event)) {
            //fprintf(stderr, "Parser error (%d): %s\n", parser->error, parser->problem);
            YError(parser->error, parser->problem, parser->problem_mark.line);
            yaml_event_delete(&event);
            return -1;
        }

        switch (event.type) {

            case YAML_MAPPING_START_EVENT:
            {

                root->type = YAML_MAPPING_NODE;
                //root->data=(yaml_val)calloc(1,sizeof(YML_NODE_s));

                int result = mapping(parser, (YML_NODE_s **) (&(root->data)));
                if (result != 0)
                    return result;

                YML_NODE_s *tmp = calloc(1, sizeof (YML_NODE_s));
                root->next = tmp;
                root = tmp;

                break;
            }

            case YAML_SEQUENCE_END_EVENT:
            {
                return 0;
            }


            case YAML_SCALAR_EVENT:


            {
                //SCALAR in SEQUENCE is just value (no key vlaue pairs)

                root->tag = NULL;
                root->type = YAML_SCALAR_NODE;
                root->data = (yaml_val) strdup((char*) event.data.scalar.value);

                YML_NODE_s *tmp = calloc(1, sizeof (YML_NODE_s));
                root->next = tmp;
                root = tmp;

                break;
            }
            
            case YAML_SEQUENCE_START_EVENT:
            {
                root->type = YAML_SEQUENCE_NODE;
                int result = seqencing(parser, (YML_NODE_s **) (&(root->data)));
                if (result != 0)
                    return result;

                YML_NODE_s *tmp = calloc(1, sizeof (YML_NODE_s));
                root->next = tmp;
                root = tmp;

                break;
            }
            
            
            
            
            
            case YAML_NO_EVENT:
            case YAML_ALIAS_EVENT:
            case YAML_STREAM_START_EVENT:
            case YAML_STREAM_END_EVENT:
            case YAML_DOCUMENT_START_EVENT:
            case YAML_DOCUMENT_END_EVENT:
//            case YAML_SEQUENCE_START_EVENT:
            case YAML_MAPPING_END_EVENT:
                break;
        }

        if (event.type != YAML_STREAM_END_EVENT)
            yaml_event_delete(&event);

    } while (event.type != YAML_STREAM_END_EVENT);

    yaml_event_delete(&event);

    return 0;

}

//inernal - parsing map block

static int mapping(yaml_parser_t *parser, YML_NODE_s **Proot) {
    yaml_event_t event;

    int kvp = 1;


    if (Proot == NULL)
        return -1;
    if (*Proot == NULL)
        *Proot = calloc(1, sizeof (YML_NODE_s));

    YML_NODE_s *root = *Proot;

    do {
        if (!yaml_parser_parse(parser, &event)) {
            //fprintf(stderr, "Parser error (%d): %s\n", parser->error, parser->problem);
            YError(parser->error, parser->problem, parser->problem_mark.line);
            yaml_event_delete(&event);
            return -1;
        }

        switch (event.type) {

            case YAML_MAPPING_START_EVENT:
            {

                kvp = 1;
                root->type = YAML_MAPPING_NODE;
                int result = mapping(parser, (YML_NODE_s **) (&(root->data)));
                if (result != 0)
                    return result;

                YML_NODE_s *tmp = calloc(1, sizeof (YML_NODE_s));
                root->next = tmp;
                root = tmp;

                break;
            }
            case YAML_MAPPING_END_EVENT:
            {
                return 0;
            }


            case YAML_SEQUENCE_START_EVENT:
            {
                kvp = 1;
                root->type = YAML_SEQUENCE_NODE;
                int result = seqencing(parser, (YML_NODE_s **) (&(root->data)));
                if (result != 0)
                    return result;

                YML_NODE_s *tmp = calloc(1, sizeof (YML_NODE_s));
                root->next = tmp;
                root = tmp;

                break;
            }


            case YAML_SCALAR_EVENT:
            {
                if (kvp) {
                    kvp = 0;
                    root->tag = strdup((char*) event.data.scalar.value);
                } else {
                    kvp = 1;

                    root->type = YAML_SCALAR_NODE;

                    char *val = (char*) event.data.scalar.value;
                    if (val != NULL && strlen(val) > 0)
                        root->data = (yaml_val) strdup(val);
                    else
                        root->data = (yaml_val) NULL;

                    YML_NODE_s *tmp = calloc(1, sizeof (YML_NODE_s));
                    root->next = tmp;
                    root = tmp;

                }

                break;
            }
            
            
            
            
            case YAML_NO_EVENT:
            case YAML_ALIAS_EVENT:
            case YAML_STREAM_START_EVENT:
            case YAML_STREAM_END_EVENT:
            case YAML_DOCUMENT_START_EVENT:
            case YAML_DOCUMENT_END_EVENT:
            case YAML_SEQUENCE_END_EVENT:
                break;
            
        }
        if (event.type != YAML_STREAM_END_EVENT)
            yaml_event_delete(&event);
    } while (event.type != YAML_STREAM_END_EVENT);

    yaml_event_delete(&event);

    return 0;

}

//inernal - eventbase yaml parsing

static int byevent(yaml_parser_t *parser, YML_NODE_s **root) {
    yaml_event_t event;


    do {
        if (!yaml_parser_parse(parser, &event)) {
            YError(parser->error, parser->problem, parser->problem_mark.line);
            yaml_event_delete(&event);
            return -1;
        }

        switch (event.type) {

            case YAML_SEQUENCE_START_EVENT:
            {
                int result = seqencing(parser, root);
                if (result != 0)
                    return result;
                break;
            }

            case YAML_MAPPING_START_EVENT:
            {
                int result = mapping(parser, root);
                if (result != 0)
                    return result;
                break;
            }

            case YAML_NO_EVENT:
            case YAML_ALIAS_EVENT:
            case YAML_SCALAR_EVENT:
            case YAML_STREAM_START_EVENT:
            case YAML_STREAM_END_EVENT:
            case YAML_DOCUMENT_START_EVENT:
            case YAML_SEQUENCE_END_EVENT:
            case YAML_DOCUMENT_END_EVENT:
            case YAML_MAPPING_END_EVENT:
                break;
        }//switch


        if (event.type != YAML_STREAM_END_EVENT)
            yaml_event_delete(&event);
    } while (event.type != YAML_STREAM_END_EVENT);
    yaml_event_delete(&event);




    return 0;
}

//public - free tree recur

void destroy_t(Yvoid_t Tree) {

    YML_NODE_s *R = (YML_NODE_s *) Tree;
    destroy_y(R);

}

//inernal  - free tree recur

static void destroy_y(YML_NODE_s *R) {

    for (YML_NODE_s * tmp = R; tmp;) {
        destroy_yy(tmp);
        YML_NODE_s *t = tmp->next;
        free(tmp);
        tmp = t;

    }
}
//inernal  - free tree recur

static void destroy_yy(YML_NODE_s *R) {

    //сам текущий блок высвобождается по выходу от сюда

    if (R->type == YAML_MAPPING_NODE) {

        destroy_y((YML_NODE_s *) R->data);

        if (R->tag)
            free(R->tag);

    } else if (R->type == YAML_SEQUENCE_NODE) {

        destroy_y((YML_NODE_s *) R->data);
        if (R->tag)
            free(R->tag);

        //free((void*)R->data);

    } else if (R->type == YAML_SCALAR_NODE) {

        if (R->tag)
            free(R->tag);

        if (R->data)
            free((void*) R->data);

    } else {
        if (R->tag)
            free(R->tag);

        if (R->data)
            free((void*) R->data);

    }

}
//public - print tree to stdout

void print_t(Yvoid_t Tree) {

    YML_NODE_s *R = (YML_NODE_s *) Tree;
    print_y(R);
}

//inernal - print tree to stdout recur

static void print_y(YML_NODE_s *R) {



    for (YML_NODE_s * tmp = R; tmp; tmp = tmp->next) {
        print_yy(tmp);

    }
    tab--;


}
//inernal - print tree to stdout recur

static void print_yy(YML_NODE_s *R) {

    if (R->type == YAML_MAPPING_NODE) {
        p(tab);
        printf("MAP TAG [%s]\n", R->tag ? R->tag : "[null]");
        tab++;
        print_y((YML_NODE_s *) R->data);
    }
    if (R->type == YAML_SEQUENCE_NODE) {
        p(tab);
        printf("SEQ [%s]\n", R->tag ? R->tag : "noname");
        tab++;
        print_y((YML_NODE_s *) R->data);
    }
    if (R->type == YAML_SCALAR_NODE) {
        p(tab);
        if (R->tag) {
            char *val = (char*) R->data;
            printf("KVP: %s = %s\n", R->tag, val ? val : "[null]");
        } else
            printf("ITM: %s\n", (char*) R->data);

    }

}

/*Internal fun. 
 * Find Node by path root:subTag:subSubTag
 * on success check last Node type
 * return poiner to YML_NODE_s or NULL
 */
static YML_NODE_s * YNode(YML_NODE_s * Tree, char * nodeTagPath, yaml_node_type_t lastNodeType) {

    if (nodeTagPath == NULL || strlen(nodeTagPath) == 0)
        return NULL;

    char * subTag = nodeTagPath;
    int delim_num = 0;


    while (*subTag) {
        if (*subTag == ':')
            delim_num += 1;
        subTag += 1;
    }

    subTag = nodeTagPath;

    YML_NODE_s * R = (YML_NODE_s *) Tree;

    for (;;) {

        //YML_NODE_s *RR = YNode(R,subkey,YAML_MAPPING_NODE);


        int subTag_len = strlen(subTag);
        char * subTag_first_delim = strchr(subTag, ':');
        int subTag_compare_len = subTag_first_delim ? subTag_first_delim - subTag : subTag_len;

        if (subTag_compare_len == 0)
            return (YML_NODE_s *) NULL;

        //search down
        YML_NODE_s * tmp = NULL;
        for (tmp = R; tmp != NULL; tmp = tmp->next) {
            if (
                    tmp->type != YAML_NO_NODE //avoid dereference tmp->data as YML_NODE_s *
                    && tmp->data //avoid dereference NULL      as YML_NODE_s *
                    && tmp->tag
                    && strlen(tmp->tag) == subTag_compare_len
                    && strncmp(tmp->tag, subTag, subTag_compare_len) == 0 //check name
                    )
                goto FOUND;
        }
        //reach end of current level, no tag found, escape!
        return NULL;

FOUND:
        //found right NODE
        R = (YML_NODE_s *) tmp;

        //any delimeters? Lets jump deepper, but is next NODE type good for that?
        if (subTag_first_delim && R->type != YAML_SCALAR_NODE) {
            //it is MAP or SEQ, lets jump to child NODE
            subTag = subTag_first_delim + 1;
            R = (YML_NODE_s *) R->data;
            continue; //next for iteration
        } else {
            //it is last subkey
            //check type and return
            if (R) {//useless.... check
                if (R->type == lastNodeType)
                    return R;
            }
            return NULL;
        }

    }//for ;;

    return (YML_NODE_s *) NULL;
}

/*
 * Public fun - find MAPPING Node by path
 */
Yvoid_t YmapNode(Yvoid_t Tree, char * nodeTagPath) {

    YML_NODE_s * R = YNode((YML_NODE_s *) Tree, nodeTagPath, YAML_MAPPING_NODE);
    if (R)
        return (Yvoid_t) (R->data);
    return (Yvoid_t) NULL;
}

/*
 * Public fun - find SEQUENCE Node by path
 */
Yvoid_t YseqNode(Yvoid_t Tree, char * nodeTagPath) {

    YML_NODE_s * R = YNode((YML_NODE_s *) Tree, nodeTagPath, YAML_SEQUENCE_NODE);
    if (R)
        return (Yvoid_t) (R->data);
    return (Yvoid_t) NULL;
}

/*
 * Public fun - get MAP value by path (root:subroot:key) => val
 */
char* YmapVal(Yvoid_t Tree, char *nodeTagPath) {
    YML_NODE_s * R = YNode((YML_NODE_s *) Tree, nodeTagPath, YAML_SCALAR_NODE);

    if (R && R->data) {
        return (char *) (R->data);
    }
    return NULL;
}

/*
 * Public fun - get list of SEQUENCE strings
 * Need to free()
 */
char** YseqVals(Yvoid_t Tree, char *nodeTagPath, int *N) {
    YML_NODE_s * R = NULL;

    //if nodeTagPath is empty, return current level seq list

    if (nodeTagPath != NULL && strlen(nodeTagPath) > 0) {
        //jump to seq node
        R = YNode((YML_NODE_s *) Tree, nodeTagPath, YAML_SEQUENCE_NODE);

        //found?
        if (R && R->data) {
            R = (YML_NODE_s *) R->data;
        } else return NULL;
    }        //Use current root
    else
        R = (YML_NODE_s *) Tree;



    //valid Root?
    if (R) {


        //count SEQ nodes
        int n = 0;
        for (YML_NODE_s *tmp = R; tmp != NULL; tmp = tmp->next)
            if (tmp->type == YAML_SCALAR_NODE)
                n += 1;
        if (n == 0) {
            *N = 0;
            return NULL;
        }

        //alloc
        int i = 0;
        char **LIST = calloc(n, sizeof (char *));

        //fillin
        for (YML_NODE_s *tmp = R; tmp != NULL; tmp = tmp->next)
            if (tmp->type == YAML_SCALAR_NODE)
                LIST[i++] = (char *) tmp->data;

        //return list
        *N = n;
        return LIST;

    }
    return NULL;
}

/*
 * Public fun - get list of SEQUENCE MAPS (return array of Trees)
 * Need to free()
 */
Yvoid_t* YseqList(Yvoid_t Tree, char *nodeTagPath, int *N) {
    //jump to seq node
    YML_NODE_s * R = YNode((YML_NODE_s *) Tree, nodeTagPath, YAML_SEQUENCE_NODE);

    //found?
    if (R && R->data) {
        R = (YML_NODE_s *) R->data;

        //count
        int n = 0;
        for (YML_NODE_s *tmp = R; tmp != NULL; tmp = tmp->next)
            if (tmp->type == YAML_MAPPING_NODE)
                n += 1;
        if (n == 0) {
            *N = 0;
            return NULL;
        }
        //alloc
        int i = 0;
        Yvoid_t*LIST = calloc(n, sizeof (Yvoid_t));

        //remember
        for (YML_NODE_s *tmp = R; tmp != NULL; tmp = tmp->next)
            if (tmp->type == YAML_MAPPING_NODE)
                LIST[i++] = (Yvoid_t) tmp->data;

        //return list
        *N = n;

        return LIST;
    }
    return NULL;
}

/******************************************************************************/

/*

void print_keys(YML_NODE_s * R) {
    int i = 0;
    for (YML_NODE_s * tmp = R; tmp != NULL && tmp->type != YAML_NO_NODE; tmp = tmp->next) {



        if (tmp->type == YAML_MAPPING_NODE || tmp->type == YAML_SCALAR_NODE)
            printf("key %02i: %s {nodetype %i}\n", i++, tmp->tag, tmp->type);
        else
            printf("key %02i: [%s] {nodetype %i}\n", i++, tmp->tag ? tmp->tag : "noname", tmp->type);

    }
}

void print_keys_type(YML_NODE_s * R, yaml_node_type_t t) {
    int i = 0;
    for (YML_NODE_s * tmp = R; tmp; tmp = tmp->next) {

        if (tmp->type == t)
            printf("key %02i: %s {nodetype %i}\n", i++, tmp->tag, tmp->type);

    }
}

void print_keys_value(YML_NODE_s * R) {
    int i = 0;
    for (YML_NODE_s * tmp = R; tmp != NULL && tmp->type != YAML_NO_NODE; tmp = tmp->next) {



        if (tmp->type == YAML_SCALAR_NODE)
            printf("key %02i: %s = %s\n", i++, tmp->tag, (char*) tmp->data);
        else
            printf("not kvp %02i: [%s]\n", i++, tmp->tag ? tmp->tag : "no__tag");

    }
}

void print_seq_items(YML_NODE_s * R) {
    int i = 0;
    for (YML_NODE_s * tmp = R; tmp; tmp = tmp->next) {



        if (tmp->type == YAML_SCALAR_NODE && tmp->tag == NULL)
            printf("ITM %02i: %s\n", i++, (char*) tmp->data);


    }
}

YML_NODE_s * yaml_get_node_by_name(YML_NODE_s * R, char *name) {

    for (YML_NODE_s * tmp = R; tmp; tmp = tmp->next) {
        if (tmp->type == YAML_MAPPING_NODE && tmp->tag && name && strcmp(tmp->tag, name) == 0)
            return (YML_NODE_s *) tmp->data;

    }
    return NULL;
}

YML_NODE_s * yaml_get_node_by_name_and_type(YML_NODE_s * R, char *name, yaml_node_type_t t) {

    for (YML_NODE_s * tmp = R; tmp; tmp = tmp->next) {
        if (tmp->type == t && tmp->tag && name && strcmp(tmp->tag, name) == 0)
            return (YML_NODE_s *) tmp->data;

    }
    return NULL;
}
 */
/******************************************************************************/

/*
 * Public fun - build config tree
 * if fd is NULL, not passed, open file by name, close on finish
 * if fd is not NULL, read, but do not close
 */
Yvoid_t YreadCfg(FILE *fd, char* file) {

    FILE *FD = NULL;
    yaml_parser_t parser;
    YML_NODE_s *R = NULL;

    int shoud_closw_fd = 0;
    if (fd == NULL) {
        errno = 0;
        FD = fopen(file, "r");
        if (FD == NULL) {
            //YError(YAML_READER_ERROR,"Open file for read failed",0);
            YError(YAML_READER_ERROR, strerror(errno), 0);
            return NULL;
        }
        shoud_closw_fd = 1;
    } else
        FD = fd;



    if (!yaml_parser_initialize(&parser)) {

        YError(parser.error, parser.problem, parser.problem_mark.line);
        return NULL;
    }
    yaml_parser_set_input_file(&parser, FD);

    //event base parsing
    int r = byevent(&parser, &R);

    // Cleanup
    yaml_parser_delete(&parser);

    //close only if opened by us
    if (shoud_closw_fd)
        fclose(FD);

    if (R == NULL || r < 0) {

        //cleanup tree
        destroy_y(R);
        return NULL;
    }

    return (Yvoid_t *) R;


}