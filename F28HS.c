#include <stdio.h>
#include <stdlib.h>
#include <string.h>



struct node {

	char comment[100];
	struct node * next;

};

struct PPM {

	char P3[3];
	struct node * root;
	int width; 
	int height; 
	int maxi;
	struct pixels * pixel; 

};

struct pixels { 

	int r;
	int g;
	int b;
};

struct PPM * getPPM(FILE * fd) {
	char P3[3];
	char com[100];
	int counter = 0;
	int r,g,b;
	int  width, height, maxi, i;
	char c;
	int n = 0;
	struct node * conductor;
	struct PPM * PPM1;

	PPM1 = (struct PPM*)malloc(sizeof(struct PPM));
	PPM1->root = malloc(sizeof(struct node));
	PPM1->root->next = 0;
	strcpy(PPM1->root->comment,"comments");

	fscanf(fd," %s",&P3);

	if (P3[0] != 'P' || P3[1] != '3'){
		
		printf("Invalid Image Format\n");
		exit(1);
		
	}

	c = fgetc(fd);
	conductor = PPM1->root;
	c = fgetc(fd);
	
	if (c != '#'){
		
		ungetc(c,fd);
		
	}
	
	while(c == '#'){
		
		while(c != '\n'){
			com[n++] = c;
			c = fgetc(fd);
			
		}
		
		conductor->next = malloc(sizeof(struct node));
		conductor = conductor->next;
		conductor->next = 0;
		
		strcpy(conductor->comment,com);
		strcpy(com,"");
		
		n = 0;
		counter = counter + 1;
		c = fgetc(fd);
		
	}
	
	if (counter != 0){
		ungetc(c, fd);
	}

	fscanf(fd, " %d %d",&width, &height);	
	fscanf(fd, " %d", &maxi);

	strcpy(PPM1->P3, P3);
	PPM1->width = width;
	PPM1->height = height;
	PPM1->maxi = maxi;
	PPM1->pixel = (struct pixels*)malloc((PPM1->height * PPM1->width) * sizeof(struct pixels));

	for(i = 0; i < (PPM1->height*PPM1->width); i++){
		
			fscanf(fd," %d %d %d ",&r,&g,&b);
			PPM1->pixel[i].r = r;
			PPM1->pixel[i].g = g;
			PPM1->pixel[i].b = b; 
			
	}

	return  PPM1;
	
}

showPPM(struct PPM * img){
	int i;
	struct node * conductor;
	
	printf("%s\n%d %d\n%d\n",img->P3,img->width,img->height,img->maxi);
	
	for(i=0; i < (img->height * img-> width); i++){
		
			printf(" %d %d %d  ",img->pixel[i].r,img->pixel[i].g,img->pixel[i].b);
			printf("\n");
			
	}
	
	conductor = img->root;
	conductor = conductor->next;
	
	if ( conductor != 0) {
		
		while ( conductor->next != 0){
			
			printf("%s\n",conductor->comment);
			conductor = conductor->next;
		}
		printf("%s\n", conductor->comment);
		
	}

}


struct PPM * encode(char * text, struct PPM * img){
	int i,j,start;
	int counter = 0;
	time_t t;
	srand((unsigned) time(&t));

	start = rand() % (img->height*img->width/ 2);

	for(i = start; i < (img->height*img->width); i++){
		
			if (counter < strlen(text)){
				
				img->pixel[i].r = text[counter];
				counter = counter + 1;
				
			}else{
				
				return img;
				
			}
		
	}	
	return img;
}

char * decode(struct PPM * img1, struct PPM * img2){
	int i;
	int counter = 0;
	int spacecounter = 1;
	int pixel1,pixel2;
	char * message;
	
	message = (char *)malloc(1 * sizeof(char *));
	
	for(i = 0; i < (img1->height*img1->width); i++){
		
			pixel1 = img1->pixel[i].r;
			pixel2 = img2->pixel[i].r;
			
			if(pixel1 != pixel2){
					
				message = realloc(message,spacecounter * sizeof(char *));
				message[counter] = pixel2;
				counter = counter + 1;
				spacecounter = spacecounter + 1;
				
			}
		
	}
	
	return message;
}

void writePPM(struct PPM * img, FILE * fout){
	int i;
	struct node * conductor;

	fprintf(fout,"%s\n",img->P3);

	conductor = img->root;
	conductor = conductor->next;
	
	if ( conductor != 0) {
		
		while ( conductor->next != 0){
			
			fprintf(fout,"%s\n",conductor->comment);
			conductor = conductor->next;
			
		}
		
		fprintf(fout,"%s\n", conductor->comment);
		
	}

	fprintf(fout,"%d %d\n%d\n",img->width,img->height,img->maxi);

	for(i = 0; i < (img->height*img->width); i++){
		
			fprintf(fout," %d %d %d  ",img->pixel[i].r,img->pixel[i].g,img->pixel[i].b);
		
		fprintf(fout,"\n");
		
	}
	
}

main (int argc, char ** argv){
	FILE * fin;
	FILE * fout;
	
    struct PPM * file1;
	struct PPM * file2;
	
	char message[200];
	char * option;
	int limit;

	 
	option = (char *)malloc(sizeof(char));
	option = argv[1];
	
	if (option[0] == 'e'){
		
		fin = fopen(argv[2], "r");
    	file1 = getPPM(fin);
		
		limit = (file1->height * file1->width) / 2;

		printf("Enter a message to be encoded:\n");
		fgets(message, sizeof(message), stdin);
		
		if (strlen(message) > limit){
			
			printf("Message too long to be encoded.\n");
			exit(0);
			
		} else {
			
			file1 = encode(message,file1);
    			
			fout = fopen(argv[3], "w");
			writePPM(file1, fout);
			fclose(fin);
			fclose(fout);
			//showPPM(file1);
			printf("Done!\n");
			
		}
		
	} else if(option[0] == 'd'){
		
		fin = fopen(argv[2], "r");
		fout = fopen(argv[3], "r");
		file1 = getPPM(fin);
		file2 = getPPM(fout);
		printf("Decoded message:\n=> %s",decode(file1,file2));
		fclose(fin);
		fclose(fout);
		
	}else {
		
		printf("no");
		
	}
	
    	exit(0);
}



