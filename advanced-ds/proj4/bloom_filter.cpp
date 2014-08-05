// Zheng He (zhe001@citymail.cuny.edu)

#include <iostream>
#include <stdio.h>
#include <stdlib.h>


#define BLOCKSIZE 256

typedef char object_t;

#define MAXP 2000003

typedef struct l_node {  
    char *key;
    object_t *obj;
    struct l_node  *next; 
} list_node_t;

typedef struct htp_l_node { 
	int a; 
    struct htp_l_node *next; 
} htp_l_node_t; 

typedef struct { 
	int b;   
	int size; 
	struct htp_l_node *a_list;
} hf_param_t;

typedef struct { 
	char tables[8][250000];
    int (*hash_function)(char *, hf_param_t); 
    hf_param_t hf_param[8]; 
} bf_t;

// for bit manipulation and figuring out which bit to set in the char bit arrays
char mask[8] = { 0x1, 0x2, 0x4, 0x8, 0x10, 0x20, 0x40, 0x80 };

list_node_t *currentblock = NULL;
int size_left;
list_node_t *free_list = NULL;

list_node_t *get_node()
{ list_node_t *tmp;
    if( free_list != NULL )
    {  tmp = free_list;
        free_list = free_list -> next;
    }
    else
    {  if( currentblock == NULL || size_left == 0)
        {  currentblock = 
            (list_node_t *) malloc( BLOCKSIZE * sizeof(list_node_t) );
            size_left = BLOCKSIZE;
        }
        tmp = currentblock++;
        size_left -= 1;
    }
    return( tmp );
}

void return_node(list_node_t *node)
{  node->next = free_list;
    free_list = node;
}

int universalhashfunction(char *key, hf_param_t hfp)
{  int sum;
    htp_l_node_t *al;
    sum = hfp.b;
    al = hfp.a_list;
    while( *key != '\0' )
    {  if( al->next == NULL )
        {   al->next = (htp_l_node_t *) get_node();
            al->next->next = NULL;
            al->a = rand()%MAXP;
        }
        sum += ( abs((al->a)*((int) *key)))%MAXP;
        key += 1;
        al = al->next;
    }
    return( sum%hfp.size );
}

bf_t *create_bf() {
	bf_t *tmp;
	tmp = (bf_t *) malloc(sizeof(bf_t));
	for(int i = 0; i < 8; i++) {
		tmp->hf_param[i].b = rand()%MAXP;
		tmp->hf_param[i].size = 250000*8;
		tmp->hf_param[i].a_list = (htp_l_node_t *) get_node();
		tmp->hf_param[i].a_list->next = NULL;
		for(int j = 0; j < 250000; j++) {
			tmp->tables[i][j] = 0;
		}
	}
	tmp->hash_function = universalhashfunction;
	return tmp;
}

void insert_bf(bf_t *b, char *s) {
	for(int i = 0; i < 8; i++) {
		// get index
		int j = b->hash_function(s, b->hf_param[i]);
		// set equal to 1 after finding which bit at the index
		b->tables[i][j/8] = b->tables[i][j/8] | mask[j%8];
	}
}

int is_element(bf_t *b, char *q) {
	for(int i = 0; i < 8; i++) {
		int j = b->hash_function(q, b->hf_param[i]);
		// check if bit is 0
		if((b->tables[i][j/8] & mask[j%8]) == 0) return 0;
	}
	return 1;
}

void sample_string_A(char *s, long i)
{  s[0] = (char)(1 + (i%254));
    s[1] = (char)(1 + ((i/254)%254));
    s[2] = (char)(1 + (((i/254)/254)%254));
    s[3] = 'a';
    s[4] = 'b';
    s[5] = (char)(1 + ((i*(i-3)) %217));
    s[6] = (char)(1 + ((17*i+129)%233 ));
    s[7] = '\0';
}
void sample_string_B(char *s, long i)
{  s[0] = (char)(1 + (i%254));
    s[1] = (char)(1 + ((i/254)%254));
    s[2] = (char)(1 + (((i/254)/254)%254));
    s[3] = 'a';
    s[4] = (char)(1 + ((i*(i-3)) %217));
    s[5] = (char)(1 + ((17*i+129)%233 ));
    s[6] = '\0';
}
void sample_string_C(char *s, long i)
{  s[0] = (char)(1 + (i%254));
    s[1] = (char)(1 + ((i/254)%254));
    s[2] = 'a';
    s[3] = (char)(1 + ((i*(i-3)) %217));
    s[4] = (char)(1 + ((17*i+129)%233 ));
    s[5] = '\0';
}
void sample_string_D(char *s, long i)
{  s[0] = (char)(1 + (i%254));
    s[1] = (char)(1 + ((i/254)%254));
    s[2] = (char)(1 + (((i/254)/254)%254));
    s[3] = 'b';
    s[4] = 'b';
    s[5] = (char)(1 + ((i*(i-3)) %217));
    s[6] = (char)(1 + ((17*i+129)%233 ));
    s[7] = '\0';
}
void sample_string_E(char *s, long i)
{  s[0] = (char)(1 + (i%254));
    s[1] = (char)(1 + ((i/254)%254));
    s[2] = (char)(1 + (((i/254)/254)%254));
    s[3] = 'a';
    s[4] = (char)(2 + ((i*(i-3)) %217));
    s[5] = (char)(1 + ((17*i+129)%233 ));
    s[6] = '\0';
}

int main()
{  long i,j; 
    bf_t * bloom;
    bloom = create_bf();
    printf("Created Filter\n");
    for( i= 0; i< 1450000; i++ )
    {  char s[8];
        sample_string_A(s,i);
        insert_bf( bloom, s );
    }
    for( i= 0; i< 500000; i++ )
    {  char s[7];
        sample_string_B(s,i);
        insert_bf( bloom, s );
    }
    for( i= 0; i< 50000; i++ )
    {  char s[6];
        sample_string_C(s,i);
        insert_bf( bloom, s );
    }
    printf("inserted 2,000,000 strings of length 8,7,6.\n");

    for( i= 0; i< 1450000; i++ )
    {  char s[8];
        sample_string_A(s,i);
        if( is_element( bloom, s ) != 1 )
        {  printf("found negative error (1)\n"); exit(0); }
    }
    for( i= 0; i< 500000; i++ )
    {  char s[7];
        sample_string_B(s,i);
        if( is_element( bloom, s ) != 1 )
        {  printf("found negative error (2)\n"); exit(0); }
    }
    for( i= 0; i< 50000; i++ )
    {  char s[6];
        sample_string_C(s,i);
        if( is_element( bloom, s ) != 1 )
        {  printf("found negative error (3)\n"); exit(0); }
    }
    j = 0;
    for( i= 0; i< 500000; i++ )
    {  char s[8];
        sample_string_D(s,i);
        if( is_element( bloom, s ) != 0 )
            j+=1;
    }
    for( i= 0; i< 500000; i++ )
    {  char s[7];
        sample_string_E(s,i);
        if( is_element( bloom, s ) != 0 )
            j+=1;
    }
    printf("Found %d positive errors out of 1,000,000 tests.\n",j);
    printf("Positive error rate %f\%.\n", (float)j/10000.0);
	
} 
