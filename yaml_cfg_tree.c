/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include "yaml_cfg_tree.h"
#include<unistd.h>

int map_level = 0;
int tab = 0;

void p(int v) {
    for (int i = 0; i < v; i++)
        printf("  ");
}

int seqencing(yaml_parser_t *parser, YML_NODE_s **Proot) {
    yaml_event_t event;
    int kvp = 1;

    if (Proot == NULL)
        return -1;
    if (*Proot == NULL)
        *Proot = calloc(1, sizeof (YML_NODE_s));

    YML_NODE_s *root = *Proot;

    do {
        if (!yaml_parser_parse(parser, &event)) {
            fprintf(stderr, "Parser error (%d): %s\n", parser->error, parser->problem);
            yaml_event_delete(&event);
            return -1;
        }

        switch (event.type) {
            case YAML_NO_EVENT: puts("No event!");
                break;
                /* Stream start/end */

            case YAML_MAPPING_START_EVENT:
            {
                kvp = 1;
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
                //сиквенс - это список, там нет ключа и значения

                root->tag = NULL;
                root->type = YAML_SCALAR_NODE;
                root->data = (yaml_val) strdup((char*) event.data.scalar.value);

                YML_NODE_s *tmp = calloc(1, sizeof (YML_NODE_s));
                root->next = tmp;
                root = tmp;

                break;
            }
        }

        if (event.type != YAML_STREAM_END_EVENT)
            yaml_event_delete(&event);

    } while (event.type != YAML_STREAM_END_EVENT);

    yaml_event_delete(&event);

    return 0;

}

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
            fprintf(stderr, "Parser error (%d): %s\n", parser->error, parser->problem);
            yaml_event_delete(&event);
            return -1;
        }

        switch (event.type) {

            case YAML_MAPPING_START_EVENT:
            {

                kvp = 1;
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
            case YAML_MAPPING_END_EVENT:
            {
                return 0;
            }


            case YAML_SEQUENCE_START_EVENT:
            {
                kvp = 1;
                root->type = YAML_SEQUENCE_NODE;
                //root->data=(yaml_val)calloc(1,sizeof(YML_NODE_s));
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
        }
        if (event.type != YAML_STREAM_END_EVENT)
            yaml_event_delete(&event);
    } while (event.type != YAML_STREAM_END_EVENT);

    yaml_event_delete(&event);

    return 0;

}

static int byevent(yaml_parser_t *parser, YML_NODE_s **root) {
    yaml_event_t event;


    do {
        if (!yaml_parser_parse(parser, &event)) {
            fprintf(stderr, "Parser error (%d): %s\n", parser->error, parser->problem);
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
            case YAML_DOCUMENT_END_EVENT:
                break;
        }//switch


        if (event.type != YAML_STREAM_END_EVENT)
            yaml_event_delete(&event);
    } while (event.type != YAML_STREAM_END_EVENT);
    yaml_event_delete(&event);
    /* END new code */



    return 0;
}

void destroy_t(Yvoid_t *Tree) {

    YML_NODE_s *R = (YML_NODE_s *) Tree;
    destroy_y(R);

}

static void destroy_y(YML_NODE_s *R) {

    for (YML_NODE_s * tmp = R; tmp;) {
        destroy_yy(tmp);
        YML_NODE_s *t = tmp->next;
        free(tmp);
        tmp = t;

    }
}

static void destroy_yy(YML_NODE_s *R) {

    //сам текущий блок высвобождается по выходу от сюда
    
    if (R->type == YAML_MAPPING_NODE) {

        destroy_y((YML_NODE_s *) R->data);

        if (R->tag)
            free(R->tag);

    }
    else if (R->type == YAML_SEQUENCE_NODE) {

        destroy_y((YML_NODE_s *) R->data);
        if (R->tag)
            free(R->tag);

        //free((void*)R->data);

    }
    else if (R->type == YAML_SCALAR_NODE) {

        if (R->tag)
            free(R->tag);

        if (R->data)
            free((void*) R->data);

    }
    else{
        if (R->tag)
            free(R->tag);

        if (R->data)
            free((void*) R->data);
        
    }

}

void print_t(Yvoid_t *Tree) {

    YML_NODE_s *R = (YML_NODE_s *) Tree;
    print_y(R);
}

static void print_y(YML_NODE_s *R) {



    for (YML_NODE_s * tmp = R; tmp; tmp = tmp->next) {
        print_yy(tmp);

    }
    tab--;


}

static void print_yy(YML_NODE_s *R) {

    if (R->type == YAML_MAPPING_NODE) {
        p(tab);
        printf("MAT TAG [%s]\n", R->tag ? R->tag : "[null]");
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

Yvoid_t YreadCfg(FILE *fh, char* file) {

    yaml_parser_t parser;

    YML_NODE_s *R = NULL;
    int shoud_closw_fd = 0;
    if (fh == NULL) {
        fh = fopen(file, "r");
        if (fh == NULL) {
            fprintf(stderr, "Failed to open file!\n");
            return NULL;
        }
        shoud_closw_fd = 1;
    }






    /* Initialize parser */
    if (!yaml_parser_initialize(&parser)) {
        fprintf(stderr, "Failed to initialize yaml parser!\n");
        return NULL;
    }
    yaml_parser_set_input_file(&parser, fh);

    //event base parsing
    int r = byevent(&parser, &R);

    // Cleanup
    yaml_parser_delete(&parser);

    //close only if opened by us
    if (shoud_closw_fd)
        fclose(fh);

    if (R == NULL || r < 0) {
        fprintf(stderr, "Failed to parse yaml!\n");
        //cleanup tree 'R'!!!!!
        return NULL;
    }

    return (Yvoid_t *) R;


}