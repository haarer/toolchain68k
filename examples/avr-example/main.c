//#include <stdio.h>
unsigned long int long_var = 0x12345678;
char* string_ptr = "hallo\0";
int main(int argc, char *argv[])
{
    int i=0;
    while(1)
    {
        i++;
        long_var += i;
        //printf("Hello world\n");
    }
    //return 0;
}
