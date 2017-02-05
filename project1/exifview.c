/*
Zhisheng Xu
zhx22@pitt.edu
People Soft: 3971307
Section: Monday Wednesday 1:00 p.m.
For the final print out, in order to increase the accuracy of the ratios, I extend its precision by one additional digit.
For example, 2.8 I will print it as 2.80 which is the same value. Just in case if this additional digit is needed.
If you have any questions, please feel free to email me. 
*/


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>


int main(int argc, char *argv[]) {
	unsigned char manufacturer[100];//used to store the name of manufacturer
	unsigned char model[100];//used to store the name of the model
	unsigned int exposure[2];//used to store the ratio of exposure time
	unsigned int Fstop[2];//used to store the F stop 
	unsigned int LensLength[2];//used to store the LensLength
	unsigned short ISO;//used to store the ISO speed
	unsigned char DataTaken[100];//used to store the data taken section
	unsigned int width;//the width of image
	unsigned int height;//the height of image
	
	
	unsigned short count = 0;//to store how many tags are there in the file
	unsigned short newCount = 0;//when we encounter 0x8769, we need another count to loop through
	unsigned int newOffset = 0;//the starting address after jumping
	unsigned int offset = 22;//offset used to control where the file pointer should be
	
	
	//structs used to store the tags
	struct EXIF
	{
		unsigned char exiftag[20];
	};
	struct TIFF
	{
		unsigned short tifftag;//the tag identifier
		unsigned short dataT;
		unsigned int howMany;
		unsigned int value;
	};
	
	
	struct EXIF tagf;
	struct TIFF tagt;

	
	FILE *f = fopen(argv[1],"rb+");//begin to scan the file
	
	if (f == NULL)//to check whether user has input a vaild file name or not
	{
		fprintf(stderr,"\nHey man, please check if the file exists or if it is in the same directory\n");
		exit(1);
	}
	
	
	fread((char *)&tagf, sizeof(tagf), 1, f);//read in the first 20 bytes which is the EXIF tag
	
	if (tagf.exiftag[0] != 255 || tagf.exiftag[1] != 216)//not the right file marker for jpeg file
	{
		fprintf(stderr, "\nHey man, are you sure it is a jpg or jpeg file? Try again.");
		exit(1);
	}
	else if (tagf.exiftag[2] != 255 || tagf.exiftag[3] != 225)//not APP1 marker
	{
		fprintf(stderr, "\nSorry we do not support APP0.");
		exit(1);
	}
	else if (tagf.exiftag[6] != 'E' || tagf.exiftag[7] != 'x' || tagf.exiftag[8] != 'i' || tagf.exiftag[9] != 'f')//the Exif string is not in the right place
	{
		fprintf(stderr, "\nTag was not found.");
		exit(1);
	}
	else if (tagf.exiftag[12] != 'I' || tagf.exiftag[13] != 'I')//not II ending
	{
		fprintf(stderr, "\nThe ending is not supported.");
		exit(1);
	}
	
	
	
	fseek(f, 20, SEEK_SET);//reset file pointer to read in how many tags are there in the file
	fread((char *)&count, 1, 2, f);
	
	
	
	for (int i = 0; i < count; i ++)//begin to loop through tags
	{
		fseek(f, offset, SEEK_SET);
		fread((char *)&tagt, 1, sizeof(tagt), f);//reading in the tag 
	
		
		if (tagt.tifftag == 271)//the 0x010F tag
		{
			fseek(f, 12+tagt.value, SEEK_SET);
			fread((char *)manufacturer, 1, tagt.howMany, f);
		}
		else if (tagt.tifftag == 272)//the 0x0110 tag
		{
			fseek(f, 12+tagt.value, SEEK_SET);
			fread((char *)model, 1, tagt.howMany, f);
		}
		else if (tagt.tifftag == 34665)//0x8769 tag, we need to jump to new address
		{
			newOffset = tagt.value + 12;
			fseek(f, newOffset, SEEK_SET);
			break;
		}
		
		offset += 12;
	}
	
	//first we need to read in new count
	fread((char *)&newCount, 1, 2, f);
	newOffset += 2;
	
	for (int i = 0; i < newCount; i ++)//begin to loop through the file once again
	{
		fseek(f, newOffset, SEEK_SET);
		fread((char *)&tagt, 1, sizeof(tagt), f);//reading in the tag 

		if (tagt.tifftag == 40962)//the 0xA002 tag
		{
			width = tagt.value;
		}
		else if(tagt.tifftag == 40963)//the 0xA003 tag
		{
			height = tagt.value;
		}
		else if (tagt.tifftag == 34855)//the 0x8827 tag
		{
			ISO = tagt.value;
		}
		else if (tagt.tifftag == 36867)//the 0x9003 tag
		{
			fseek(f, 12+tagt.value, SEEK_SET);
			fread((char *)DataTaken, 1, tagt.howMany, f);
		}
		else if (tagt.tifftag == 33434)//0x829a tag
		{
			fseek(f, 12+tagt.value, SEEK_SET);
			fread((char *)exposure, 1, 8, f);
		}
		else if (tagt.tifftag == 33437)// the 0x829d tag
		{
			fseek(f, 12+tagt.value, SEEK_SET);
			fread((char *)Fstop, 1, 8, f);
		}
		else if (tagt.tifftag == 37386)//the 0x920a tag
		{
			fseek(f, 12+tagt.value, SEEK_SET);
			fread((char *)LensLength, 1, 8, f);
		}
		
		
		newOffset += 12;
	}
	

	//begin to print everything out
	printf("\nManufacturer:    %s",manufacturer);
	printf("\nModel:           %s",model);
	printf("\nExposure Time:   %d/%d second",exposure[0],exposure[1]);
	printf("\nF-stop:          f/%.2f",(float)Fstop[0]/Fstop[1]);
	printf("\nISO:             ISO %d",ISO);
	printf("\nDate Taken:      %s",DataTaken);
	printf("\nFocal Length:    %.2f mm",(float)LensLength[0]/LensLength[1]);
	printf("\nWidth:           %d pixels",width);
	printf("\nHeight:          %d pixels",height);
	

	
	fclose(f);//close the stream
	
	
	
	
	
}//end of main