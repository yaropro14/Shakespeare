#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>


#define PRO_DEBU
#ifdef PRO_DEBUG
#define PRINTF printf
#else 
#define PRINTF
#endif

FILE * f_out;


int size_of_file(FILE * f);
char * file_read(FILE * f, int * file_length);
int num_of_str(char * str);
char ** make_strings(char * original_text, int * s_num);
int cmp_char_beginning(const void * str1, const void * str2);
int cmp_char_end(const void * str1, const void * str2);
void file_print(char ** a, int s_num, FILE * f_out);
void sort_alphabet(char ** onegin_str, int s_num, FILE * f_out);
void sort_rhyme(char ** onegin_str, int s_num, FILE * f_out);


int main()
{
	printf("Encyclopedia of English life \"Romeo and Juliet\"\n");
	
	FILE * f_in = fopen("Shakespeare.txt", "r");
	FILE * f_out = fopen("My_Shakespeare.txt", "w");
	
	if (f_in == NULL)
	{
		printf ("Error: file can't open");
		return -1;
	}
	
	
	int file_length = 0;
	char * text = file_read(f_in, &file_length);
	assert(text);
	
	char * copy_text = strdup(text);
	
	int s_num = 0;
	
	char ** text_str = make_strings(copy_text, &s_num);
	assert(text_str);
	
	sort_alphabet(text_str, s_num, f_out);
	sort_rhyme(text_str, s_num, f_out);
	
	fprintf(f_out, "Original Text\n\n");
	fprintf(f_out, "%s", text);
	
	free(text);
	free(copy_text);
	free(text_str);
	
	fclose(f_in);
	fclose(f_out);
	
	
	
	return 0;
}


void sort_alphabet(char ** text_str, int s_num, FILE * f_out)
{
	assert(text_str);
	qsort(text_str, s_num, sizeof(char*), cmp_char_beginning);
	fprintf(f_out, "\nEncyclopedia of English life \"Romeo and Juliet\" (version 2.0, revised)\n");
	file_print(text_str, s_num, f_out);
}


void sort_rhyme(char ** text_str, int s_num, FILE * f_out)
{
	assert(text_str);
	qsort(text_str, s_num, sizeof(char*), cmp_char_end);
	fprintf(f_out, "Encyclopedia of the English Life \"Romeo and Juliet\" (version 2.1, rhymed)\n");
	file_print(text_str, s_num, f_out);
}


int size_of_file(FILE * f)
{
	fseek(f, 0, SEEK_END);
	int file_length = ftell(f);
	rewind(f);
	if(file_length == -1){
		printf("Error in function size_of_file");
	}
	return file_length;
}


char * file_read(FILE * f, int * file_length)
{
	*file_length = size_of_file(f);
	
	char * str = (char *) calloc(*file_length, sizeof(char));
	assert(str);
	
	fread(str, *file_length, sizeof (*str), f);
	assert(str);
	

	return str;
}	


int num_of_str(char * str)
{
	int num = 1;
	char * p;
	char * t = str;
	p = strchr(t, '\n');
	while(p != NULL)
	{
		num ++;
		t = p + 1;
		p = strchr(t, '\n');
	}
	return num;
}


char ** make_strings(char * copy_text, int * s_num)
{
	assert(copy_text);
	* s_num = num_of_str(copy_text);
	
	char ** text_str = (char **) calloc(* s_num, sizeof(char*));
	
	char * p = copy_text;
	char * t = copy_text;
	
	p = strchr(t, '\n');
	//printf("%s", p);
	
	int i = 0;
	while(p != NULL)
	{
		*p = '\0';
		text_str[i] = t;
		i++;
		//PRINTF("i = %d, T = %s\n", i, onegin_str[i - 1]);
		t = p + 1;
		p = strchr(t, '\n');
		//printf("%s, %s\n", p, t);
	}
	
	text_str[i++] = t;
	//printf("i = %d, T = %s\n", i, onegin_str[i - 1]);
	return text_str;
}


int cmp_char_beginning(const void * str1, const void * str2)
{
	assert(str1);
    assert(str2);
    
    char * s1 = *(char **) str1;
    char * s2 = *(char **) str2;
    
    
    //PRINTF("s1 - %s s2 - %s\n", s1, s2);
    
    while (ispunct(*s1) || isspace(*s1)) 
		s1 = s1 + 1;
    while (ispunct(*s2) || isspace(*s2)) 
		s2 = s2 + 1;
    
    
    return strcmp(s1, s2);
}


int cmp_char_end(const void * str1, const void * str2)
{
	assert(str1);
    assert(str2);
    
    char * s1 = *(char **) str1;
    char * s2 = *(char **) str2;
    if (s1 == s2)
		return 0;
    
    int l1 = strlen(s1);
    int l2 = strlen(s2);
    
    int i = 0, j = 0;
    int max = l1 > l2 ? l1 : l2;
	while ((i < max) && (j < max))
	{
		if (ispunct(s1[l1 - i - 1]) || isspace(s1[l1 - i - 1]))
			i++;
		else if (ispunct(s2[l2 - j - 1]) || isspace(s2[l2 - j - 1]))
			j++;
			
		else {
			int dif = s1[l1 - i - 1] - s2[l2 - j - 1];
			if(dif != 0)
				return dif ;
			
			else{
				i++;
				j++;
			}
		}
	}
	return NAN;
}


void file_print(char ** text_str, int s_num, FILE * f_out)
{
	for(int i = 0; i < s_num; i++)
	{
		fprintf(f_out, "%s\n", text_str[i]);
	}
	
	fprintf(f_out, "\n");
}
