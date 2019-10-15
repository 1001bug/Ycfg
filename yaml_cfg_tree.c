/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include "yaml_cfg_tree.h"


int map_level=0;
int tab=0;


void p(int v){
    for(int i=0;i<v;i++)
        printf("  ");
}


void seqencing(yaml_parser_t *parser, char *root_node, YML_NODE_s *RR){
    
    
    YML_NODE_s *R=RR;
    yaml_event_t  event;
    int seq_num=0;
    int inseq_num=0;
    char *new_node=root_node?strdup(root_node):NULL;
    int kvp=1;
    int first=0;
    //printf("***** ENTER MAPPING FUN *****\n");
    do {
    if (!yaml_parser_parse(parser, &event)) {
       printf("Parser error %d\n", parser->error);
       exit(EXIT_FAILURE);
    }

    switch(event.type)
    { 
    case YAML_NO_EVENT: puts("No event!"); break;
    /* Stream start/end */

        case YAML_MAPPING_START_EVENT: {
            p(tab++); 
            kvp=1; 
            printf("Mapping inside seq => for [[ %s ]]  {%3i}\n",new_node==NULL?"ROOT":new_node,seq_num++);
            R->type=YAML_MAPPING_NODE;
            R->data=(yaml_val)calloc(1,sizeof(YML_NODE_s));
            mapping(parser,new_node,(YML_NODE_s *)R->data); 
            
            YML_NODE_s *tmp=calloc(1,sizeof(YML_NODE_s));
            R->next=tmp;
            R=tmp;
            
            break;
        }
        
        case YAML_SEQUENCE_END_EVENT: {
            p(--tab);
            printf("End Sequence. [[ %s ]] FINITA\n",root_node==NULL?"NULL":root_node); 
            return;
        }

    
    case YAML_SCALAR_EVENT:  
            
            
    { 
        //сиквенс - это список, там нет ключа и значения
        
            p(tab);
            printf("{%s} {%02i} [%s]\n",root_node==NULL?"ROOT":root_node,inseq_num++,  event.data.scalar.length==0?"NULL":(char*)event.data.scalar.value);
            kvp=!kvp;
            new_node=strdup((char*)event.data.scalar.value);
            
            R->tag=NULL;
            R->type=YAML_SCALAR_NODE;
            R->data=(yaml_val)strdup((char*)event.data.scalar.value);
            
            
            YML_NODE_s *tmp=calloc(1,sizeof(YML_NODE_s));
            R->next=tmp;
            R=tmp;
            
            //R->next=calloc(1,sizeof(YML_NODE_s));
            //R=R->next;
                
        
        
         
            break;
        }
    }
    if(event.type != YAML_STREAM_END_EVENT)
      yaml_event_delete(&event);
  } while(event.type != YAML_STREAM_END_EVENT);
  yaml_event_delete(&event);
  
  
  
}

void mapping(yaml_parser_t *parser, char *root_node, YML_NODE_s *RR){
    yaml_event_t  event;
    YML_NODE_s *R;
    R=RR;
    map_level+=1;
    char *new_node=root_node?strdup(root_node):NULL;
    int kvp=1;
    int first=0;
    //printf("***** ENTER MAPPING FUN *****\n");
    do {
    if (!yaml_parser_parse(parser, &event)) {
       printf("Parser error %d\n", parser->error);
       exit(EXIT_FAILURE);
    }
    
    switch(event.type)
    { 
        
    case YAML_NO_EVENT: puts("No event!"); break;
    /* Stream start/end */

        case YAML_MAPPING_START_EVENT: {
            p(tab++); 
            kvp=1; 
            printf("Mapping => for [[ %s ]]\n",new_node==NULL?"ROOT":new_node); 
            R->type=YAML_MAPPING_NODE;
            R->data=(yaml_val)calloc(1,sizeof(YML_NODE_s));
            mapping(parser,new_node,(YML_NODE_s *)R->data); 
            
            YML_NODE_s *tmp=calloc(1,sizeof(YML_NODE_s));
            R->next=tmp;
            R=tmp;
            
            break;
        }
        case YAML_MAPPING_END_EVENT: {
            p(--tab);
            printf("End Mapping. [[ %s ]] FINITA\n",root_node==NULL?"NULL":root_node); 
            map_level-=1;
            return;
        }

        
        case YAML_SEQUENCE_START_EVENT: {
            p(tab++);
            kvp=1;
            printf("Sequence => for [[ %s ]]\n",new_node==NULL?"ROOT":new_node); 
            R->type=YAML_SEQUENCE_NODE;
            R->data=(yaml_val)calloc(1,sizeof(YML_NODE_s));
            seqencing(parser,new_node,(YML_NODE_s *)R->data);
            
            YML_NODE_s *tmp=calloc(1,sizeof(YML_NODE_s));
            R->next=tmp;
            R=tmp;
            
            break;
        }
    
    
    case YAML_SCALAR_EVENT:  
            
            
    { 
        if(kvp){
            p(tab);
            printf("{%s} [%s] = ",root_node==NULL?"ROOT":root_node,event.data.scalar.length==0?"NULL":(char*)event.data.scalar.value);
            kvp=!kvp;
            new_node=strdup((char*)event.data.scalar.value);
            
            
            
            R->tag=strdup((char*)event.data.scalar.value);
                
        }
        else{
            printf("[%s]\n",event.data.scalar.length==0?"NULL":(char*)event.data.scalar.value);
            kvp=!kvp;
            
            
            R->type=YAML_SCALAR_NODE;
            R->data=(yaml_val)strdup((char*)event.data.scalar.value);
            
            YML_NODE_s *tmp=calloc(1,sizeof(YML_NODE_s));
            R->next=tmp;
            R=tmp;
            
            
            
        }
         
            break;
        }
    }
    if(event.type != YAML_STREAM_END_EVENT)
      yaml_event_delete(&event);
  } while(event.type != YAML_STREAM_END_EVENT);
  yaml_event_delete(&event);
  
  
  
}

void byevent(yaml_parser_t *parser, YML_NODE_s *RR){
    yaml_event_t  event;
    YML_NODE_s *R=RR;
    int M=0;
    int S=0;
    
    /* START new code */
  do {
    if (!yaml_parser_parse(parser, &event)) {
       printf("Parser error %d\n", parser->error);
       exit(EXIT_FAILURE);
    }

    switch(event.type)
    { 
    case YAML_NO_EVENT: puts("No event!"); break;
    /* Stream start/end */
    case YAML_STREAM_START_EVENT:p(tab++); puts("STREAM START"); break;
    case YAML_STREAM_END_EVENT: p(--tab);  puts("STREAM END");   break;
    /* Block delimeters */
    case YAML_DOCUMENT_START_EVENT: p(tab++);puts("Document =>");break;
    case YAML_DOCUMENT_END_EVENT: p(--tab);   puts("End Document"); break;
    case YAML_SEQUENCE_START_EVENT: p(tab++); puts("Sequence => FIRST");seqencing(parser,NULL,R); break;
    
    
    case YAML_MAPPING_START_EVENT: {
        p(tab++);
        puts("Mapping => FIRST");
        mapping(parser,NULL,R); 
        break;
    }
    //case YAML_MAPPING_END_EVENT: p(--tab);    puts("End Mapping"); break;
    /* Data */
    case YAML_ALIAS_EVENT:  p(tab); printf("Got alias (anchor %s)\n", event.data.alias.anchor); break;
        case YAML_SCALAR_EVENT:  
            
            
        {
            p(tab);printf("!!! [%s]\n",event.data.scalar.length==0?"NULL":(char*)event.data.scalar.value);
            //else if(A) {p(tab);printf("Got scalar [%s]=",event.data.scalar.value);} else printf("[%s]\n", event.data.scalar.value);A=!A; break;
            break;
        }
    }
    if(event.type != YAML_STREAM_END_EVENT)
      yaml_event_delete(&event);
  } while(event.type != YAML_STREAM_END_EVENT);
  yaml_event_delete(&event);
  /* END new code */

  
  
  return;
}




void print_y(YML_NODE_s *R){
    
    
    for(YML_NODE_s * tmp=R;tmp;tmp=tmp->next){
        print_yy(tmp);
        
    }
    tab--;
    
    
}

void print_yy(YML_NODE_s *R){
    
    if(R->type==YAML_MAPPING_NODE){
        p(tab);
        printf("MAT TAG %s\n",R->tag);
        tab++;
        print_y((YML_NODE_s *)R->data);
    }
    if(R->type==YAML_SEQUENCE_NODE){
        p(tab);
        printf("SEQ [%s]\n",R->tag?R->tag:"noname");
        tab++;
        print_y((YML_NODE_s *)R->data);
    }
    if(R->type==YAML_SCALAR_NODE){
        p(tab);
        if(R->tag)
            printf("KVP: %s = %s\n",R->tag,(char*)R->data);
        else
            printf("ITM: %s\n",(char*)R->data);
        
    }
    
}


void print_keys(YML_NODE_s * R){
    int i=0;
    for(YML_NODE_s * tmp=R;tmp!=NULL&&tmp->type!=YAML_NO_NODE;tmp=tmp->next){
        
        
        
        if(tmp->type==YAML_MAPPING_NODE || tmp->type==YAML_SCALAR_NODE)
            printf("key %02i: %s {nodetype %i}\n",i++,tmp->tag,tmp->type);
        else
            printf("key %02i: [%s] {nodetype %i}\n",i++,tmp->tag?tmp->tag:"noname",tmp->type);
        
    }
}

void print_keys_type(YML_NODE_s * R, yaml_node_type_t t){
    int i=0;
    for(YML_NODE_s * tmp=R;tmp;tmp=tmp->next){

        if(tmp->type == t)
            printf("key %02i: %s {nodetype %i}\n",i++,tmp->tag,tmp->type);
        
    }
}

void print_keys_value(YML_NODE_s * R){
    int i=0;
    for(YML_NODE_s * tmp=R;tmp!=NULL&&tmp->type!=YAML_NO_NODE;tmp=tmp->next){
        
        
        
        if(tmp->type==YAML_SCALAR_NODE)
            printf("key %02i: %s = %s\n",i++,tmp->tag,(char*)tmp->data);
        else
            printf("not kvp %02i: [%s]\n",i++,tmp->tag?tmp->tag:"no__tag");
        
    }
}

void print_seq_items(YML_NODE_s * R){
    int i=0;
    for(YML_NODE_s * tmp=R;tmp;tmp=tmp->next){
        
        
        
        if(tmp->type==YAML_SCALAR_NODE && tmp->tag == NULL)
            printf("ITM %02i: %s\n",i++,(char*)tmp->data);
        
        
    }
}

YML_NODE_s * yaml_get_node_by_name(YML_NODE_s * R, char *name){
    
    for(YML_NODE_s * tmp=R;tmp;tmp=tmp->next){
        if(tmp->type==YAML_MAPPING_NODE &&  tmp->tag && name &&  strcmp(tmp->tag,name)==0   )
            return (YML_NODE_s *)tmp->data;
        
    }
 return NULL;   
}

YML_NODE_s * yaml_get_node_by_name_and_type(YML_NODE_s * R, char *name, yaml_node_type_t t){
    
    for(YML_NODE_s * tmp=R;tmp;tmp=tmp->next){
        if(tmp->type==t &&  tmp->tag && name &&  strcmp(tmp->tag,name)==0   )
            return (YML_NODE_s *)tmp->data;
        
    }
 return NULL;   
}



         










Yvoid_t YreadCfg(char* file)
{
  static __thread char *p;
  
  YML_NODE_s ROOT, *R;
  memset(&ROOT,0,sizeof(ROOT));
  R=&ROOT;
  
  
    
    

  
    
  FILE *fh = fopen(file, "r");
  yaml_parser_t parser;
  yaml_token_t  token;   /* new variable */

  /* Initialize parser */
  if(!yaml_parser_initialize(&parser))
    fprintf(stderr,"Failed to initialize parser!\n" );
  if(fh == NULL)
    fprintf(stderr,"Failed to open file!\n");

  /* Set input file */
  yaml_parser_set_input_file(&parser, fh);

  //bytoken(&parser);
  byevent(&parser,R);
  

  /* Cleanup */
  yaml_parser_delete(&parser);
  fclose(fh);
  
  tab=0;
  printf("\n\n\n");
  
  print_y(R);
  
  printf("\n\n\n");
  
  
  print_keys((YML_NODE_s *)R->data);
  

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
  
  
  
  printf("\n\nFINITA FINITA\n\n");
  
  
  
  
  
  
  
  
  
  
  
  
  return 0;
  
}