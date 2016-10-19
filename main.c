#include <stdlib.h>
#include <stdio.h>

void fatal_error();
void print_usage();


////////////
//main
//////////

int main(int argc, char *argv[])
{
	FILE *fp;
	
	char tmp,newline;
	char dummy[2];
	int rows,cols,depth,i,j;
	int color=1,temp;
	unsigned char r,g,b;
	unsigned char *Y;	
	
	////////////////
	// image read 
	////////////////
	
	if(argc!=3)
	{
		print_usage();
	}
		
	if ((fp = fopen(argv[1],"rb"))==NULL)
	{
		printf("Error accessing %s\n",argv[1]);
		exit(0);
	}
		
	for(i=0;argv[1][i]!='\0';i++);
	
	if(i>4)
	{
		argv[1][i-4] = '\0';
	}
	
	if((dummy[0]=fgetc(fp))!='P')
		fatal_error();
	
	if((dummy[1]=fgetc(fp))=='6')
		color=3; // P5: grayscale binary; P6: color binary
	
	newline  = fgetc(fp);
	
	// get rid of those damn comments!
	if ((tmp=fgetc(fp))=='#')
	{
		while ((tmp=fgetc(fp))!=newline); // tolerate crap
	}
	else
	{
		ungetc((int)tmp,fp); // wow! no crap!
	}
	
	fscanf(fp,"%d %d\n%d",&cols,&rows,&depth);
	fgetc(fp); // the damn '\n' after depth!
	
	/////////////
	// mallocs //
	/////////////
	
	Y = (unsigned char *)malloc(rows*cols*sizeof(unsigned char));
	
	for(i=0;i<rows;i++)
	{
		for(j=0;j<cols;j++)
		{
			r = fgetc(fp);
			g = fgetc(fp);
			b = fgetc(fp);
			
			temp = (int)((0.299*r) + (0.587*g) + (0.114*b));
			if(temp<0)
				temp = 0;
			else if(temp>255)
				temp = 255;
			Y[(i*cols)+j] = temp;
		}
	}
	
	fclose(fp);

	fp = fopen(argv[2],"wb");
	if(!fp)
	{
		printf("Error accessing %s,argv[2]");
		exit(0);
	}
	dummy[1] = 53;//48+5=53
	fprintf(fp,"%c%c\n%d %d\n%d\n",dummy[0],dummy[1],cols,rows,depth);
	fwrite(Y,sizeof(unsigned char),rows*cols,fp);
	fclose(fp);
	
	printf("Conversion complete.");
	
	return(0);
}

void fatal_error()
{
	printf("Fatal error: Image starts not with P\n");
	exit(0);
}

void print_usage()
{
	printf("ppmtoyuv --help\t:\tDisplay this message.\n");
	printf("Usage: ppmtopgm <inputfile> <outputfile>\n");
	exit(0);
}
