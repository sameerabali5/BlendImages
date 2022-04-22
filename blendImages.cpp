#include <iostream>
#include <string>
using namespace std;
#include <fstream>
using std::ofstream;

typedef unsigned short WORD; 
typedef unsigned int DWORD; 
typedef unsigned int LONG; 
struct tagBITMAPFILEHEADER 
{ 
    WORD bfType;  //specifies the file type 
    DWORD bfSize;  //specifies the size in bytes of the bitmap file 
    WORD bfReserved1;  //reserved; must be 0 
    WORD bfReserved2;  //reserved; must be 0 
    DWORD bfOffBits;  //species the offset in bytes from the bitmapfileheader to the bitmap bits 
}; 
struct tagBITMAPINFOHEADER 
{ 
    DWORD biSize;  //specifies the number of bytes required by the struct 
    LONG biWidth;  //specifies width in pixels 
    LONG biHeight;  //species height in pixels 
    WORD biPlanes; //specifies the number of color planes, must be 1 
    WORD biBitCount; //specifies the number of bit per pixel 
    DWORD biCompression;//spcifies the type of compression 
    DWORD biSizeImage;  //size of image in bytes 
    LONG biXPelsPerMeter;  //number of pixels per meter in x axis 
    LONG biYPelsPerMeter;  //number of pixels per meter in y axis 
    DWORD biClrUsed;  //number of colors used by th ebitmap 
    DWORD biClrImportant;  //number of colors that are important 
}; 
typedef unsigned char BYTE;

unsigned char get_color(unsigned char *data, int width, int x, int y, int color)
{
    int width2 = width * 3;
    if(width2 % 4 != 0)
    {
        width2 = width2 + 4 - (width2 % 4);
    }
    width2 = width2 * y;
    return data[x*3 + width2 + color];
}

unsigned char get_red(unsigned char *imagedata, float x, float y, int imageWidth, int imageHeight)
{
    int x2 = (int)x + 1;
    int x1 = (int)x;
    int y2 = (int)y + 1;
    int y1= (int)y;

    float dx = x - x1;
    float dy = y - y1;;
    BYTE red_left_upper = get_color(imagedata, imageWidth, x1, y2, 2);
    BYTE red_right_upper = get_color(imagedata, imageWidth, x2, y2, 2);
    BYTE red_left_lower = get_color(imagedata, imageWidth, x1, y1, 2);
    BYTE red_right_lower = get_color(imagedata, imageWidth, x2, y1, 2);

    BYTE red_left = red_left_upper * (1 - dy) + red_left_lower * dy;
    BYTE red_right = red_right_upper * (1 - dy) + red_right_lower * dy;
    BYTE red_result = red_left * (1 - dx) + red_right * dx;

    return red_result;
}

unsigned char get_green(unsigned char *imagedata, float x, float y, int imageWidth, int imageHeight)
{
    int x2 = (int)x + 1;
    int x1 = (int)x;
    int y2 = (int)y + 1;
    int y1= (int)y;

    float dx = x - x1;
    float dy = y - y1;

    BYTE green_left_upper = get_color(imagedata, imageWidth, x1, y2, 1);
    BYTE green_right_upper = get_color(imagedata, imageWidth, x2, y2, 1);
    BYTE green_left_lower = get_color(imagedata, imageWidth, x1, y1, 1);
    BYTE green_right_lower = get_color(imagedata, imageWidth, x2, y1, 1);

    BYTE green_left = green_left_upper * (1 - dy) + green_left_lower * dy;
    BYTE green_right = green_right_upper * (1 - dy) + green_right_lower * dy;
    BYTE green_result = green_left * (1 - dx) + green_right * dx;

    return green_result;
}


unsigned char get_blue(unsigned char *imagedata, float x, float y, int imageWidth, int imageHeight)
{
    int x2 = (int)x + 1;
    int x1 = (int)x;
    int y2 = (int)y + 1;
    int y1= (int)y;

    float dx = x - x1;
    float dy = y - y1;

    BYTE blue_left_upper = get_color(imagedata, imageWidth, x1, y2, 0);
    BYTE blue_right_upper = get_color(imagedata, imageWidth, x2, y2, 0);
    BYTE blue_left_lower = get_color(imagedata, imageWidth, x1, y1, 0);
    BYTE blue_right_lower = get_color(imagedata, imageWidth, x2, y1, 0);

    BYTE blue_left = blue_left_upper * (1 - dy) + blue_left_lower * dy;
    BYTE blue_right = blue_right_upper * (1 - dy) + blue_right_lower * dy;
    BYTE blue_result = blue_left * (1 - dx) + blue_right * dx;

    return blue_result;
}

 void read_write_file(char *input1, char *input2, float ratioInput, char *input3)
{
    //File 1
    tagBITMAPFILEHEADER fh;
    tagBITMAPINFOHEADER fih;
    FILE *file1 = fopen(input1, "rb");
    fread(&fh.bfType, 2, 1, file1);
    fread(&fh.bfSize, sizeof(int), 1, file1);
    fread(&fh.bfReserved1, sizeof(short), 1, file1);
    fread(&fh.bfReserved2, sizeof(short), 1, file1);
    fread(&fh.bfOffBits, sizeof(int), 1, file1);
    fread(&fih, sizeof(fih), 1, file1);
    BYTE *data1 = new BYTE[fih.biSizeImage];
    fread(data1, fih.biSizeImage, 1, file1);
    fclose(file1);
    
    //File 2
    tagBITMAPFILEHEADER fh2;
    tagBITMAPINFOHEADER fih2;
    FILE *file2 = fopen(input2, "rb");
    fread(&fh2.bfType, 2, 1, file2);
    fread(&fh2.bfSize, sizeof(int), 1, file2);
    fread(&fh2.bfReserved1, sizeof(short), 1, file2);
    fread(&fh2.bfReserved2, sizeof(short), 1, file2);
    fread(&fh2.bfOffBits, sizeof(int), 1, file2);
    fread(&fih2, sizeof(fih2), 1, file2);  
    BYTE *data2 = new BYTE[fih2.biSizeImage];
    fread(data2, fih2.biSizeImage, 1, file2);
    fclose(file2);

    BYTE *data3 = new BYTE[fih.biSizeImage];
    if(fih.biWidth > fih2.biWidth)
    {
        for(int x = 0; x < fih.biWidth; x++)
        {
            for(int y = 0; y < fih.biHeight; y++)
            {
                int width = fih.biWidth * 3;
                if(width % 4 != 0)
                {
                    width = width + 4 - (width % 4);
                }
                width = width * y;
                float x_small = x * ((float)fih2.biWidth/(float)fih.biWidth);
                float y_small = y * ((float)fih2.biHeight/(float)fih.biHeight);

                BYTE r, g, b;
                b = get_color(data1, fih.biWidth, x, y, 0);
                g = get_color(data1, fih.biWidth, x, y, 1);
                r = get_color(data1, fih.biWidth, x, y, 2);

                BYTE r1, g1, b1;
                b1 = get_blue(data2, x_small, y_small, fih2.biWidth, fih2.biHeight);
                g1 = get_green(data2, x_small, y_small, fih2.biWidth, fih2.biHeight);
                r1 = get_red(data2, x_small, y_small, fih2.biWidth, fih2.biHeight);

                BYTE b2 = b*ratioInput + b1*(1 - ratioInput);
                BYTE g2 = g*ratioInput + g1*(1 - ratioInput);
                BYTE r2 = r*ratioInput + r1*(1 - ratioInput);

                data3[x*3 + width + 0] = b2;
                data3[x*3 + width + 1] = g2;
                data3[x*3 + width + 2] = r2;
            }
        }
    } 
    else if (fih2.biWidth > fih.biWidth)
    {
        data3 = new BYTE[fih2.biSizeImage];
        for(int x = 0; x < fih2.biWidth; x++)
        {
            for(int y = 0; y < fih2.biHeight; y++)
            {
                int width = fih2.biWidth * 3;
                if(width % 4 != 0)
                {
                    width = width + 4 - (width % 4);
                }
                width = width * y;
                float x_small = x * ((float)fih.biWidth/(float)fih2.biWidth);
                float y_small = y * ((float)fih.biHeight/(float)fih2.biHeight);
                BYTE r, g, b;
                b = get_color(data2, fih2.biWidth, x, y, 0);
                g = get_color(data2, fih2.biWidth, x, y, 1);
                r = get_color(data2, fih2.biWidth, x, y, 2);

                BYTE r1, g1, b1;
                b1 = get_blue(data1, x_small, y_small, fih.biWidth, fih.biHeight);
                g1 = get_green(data1, x_small, y_small, fih.biWidth, fih.biHeight);
                r1 = get_red(data1, x_small, y_small, fih.biWidth, fih.biHeight);

                BYTE b2 = b*ratioInput + b1*(1 - ratioInput);
                BYTE g2 = g*ratioInput + g1*(1 - ratioInput);
                BYTE r2 = r*ratioInput + r1*(1 - ratioInput);

                data3[x*3 + width + 0] = b2;
                data3[x*3 + width + 1] = g2;
                data3[x*3 + width + 2] = r2;
            }
        }
    } else 
    {
        for(int x = 0; x < fih.biWidth; x++)
        {
            for(int y = 0; y < fih.biHeight; y++)
            {
                BYTE r, g, b;
                r = get_color(data1, fih.biWidth, x, y, 2);
                g = get_color(data1, fih.biWidth, x, y, 1);
                b = get_color(data1, fih.biWidth, x, y, 0);

                BYTE r1, g1,b1;
                r1 = get_color(data2, fih.biWidth, x, y, 2);
                g1 = get_color(data2, fih.biWidth, x, y, 1);
                b1 = get_color(data2, fih.biWidth, x, y, 0);

                BYTE b2 = b*ratioInput + b1*(1 - ratioInput);
                BYTE g2 = g*ratioInput + g1*(1 - ratioInput);
                BYTE r2 = r*ratioInput + r1*(1 - ratioInput);

                data3[x*3 + 3*fih.biWidth*y + 0] = b2;
                data3[x*3 + 3*fih.biWidth*y + 1] = g2;
                data3[x*3 + 3*fih.biWidth*y + 2] = r2;
            }
        }
    }

    //FILE 3: writing to the file
    FILE *file3 = fopen(input3, "wb");
    if(fih2.biWidth > fih.biWidth)
    {
        fwrite(&fh2.bfType, 2, 1, file3);
        fwrite(&fh2.bfSize, sizeof(int), 1, file3);
        fwrite(&fh2.bfReserved1, sizeof(short), 1, file3);
        fwrite(&fh2.bfReserved2, sizeof(short), 1, file3);
        fwrite(&fh2.bfOffBits, sizeof(int), 1, file3);
        fwrite(&fih2, sizeof(fih2), 1, file3);
        fwrite(data3, fih2.biSizeImage, 1, file3);
        
    }
    else
    {
        fwrite(&fh.bfType, 2, 1, file3);
        fwrite(&fh.bfSize, sizeof(int), 1, file3);
        fwrite(&fh.bfReserved1, sizeof(short), 1, file3);
        fwrite(&fh.bfReserved2, sizeof(short), 1, file3);
        fwrite(&fh.bfOffBits, sizeof(int), 1, file3);
        fwrite(&fih, sizeof(fih), 1, file3);
        fwrite(data3, fih.biSizeImage, 1, file3);
    }
    fclose(file3);
    delete[] data3;
}



int main(int argc, char *argv[])
{
    char* file1;
    char* file2;
    string ratio = argv[3];
    char* file3;
    file1 = argv[1];
    file2 = argv[2];
    float ratio1 = std::stof(ratio);
    file3 = argv[4];
    if(argc > 5)
    {
        cout << ">>>\nERROR: Too many parameters!\n";
        cout << "Try this format: [programname] [imagefile1] [imagefile2] [ratio] [outputfile]\n>>>\n";
    }
    else if(argc < 5)
    {
        cout << ">>>\nERROR: Not enough parameters!\n";
        cout << "Try this format: [programname] [imagefile1] [imagefile2] [ratio] [outputfile]\n>>>\n";
    }
    else if(argc == 5)
    {
        string check1 = argv[1];
        string check2 = argv[2];
        string check3 = argv[4];
        string bmp = ".bmp";
        if(check1.find(bmp) == std::string::npos || check2.find(bmp) == std::string::npos || check3.find(bmp) == std::string::npos)
        {
            cout << ">>>\nERROR: Those files do not exist!\nUse .bmp files only!\n>>>\n";
        }
        else {
            read_write_file(argv[1], argv[2], atof(argv[3]), argv[4]);
            //read_write_file("lion.bmp", "wolf.bmp", 0.5, "myresult.bmp");
        }
    }
    return 0;   
}
