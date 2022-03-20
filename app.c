#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main()
{
    int i = 0;
    while(i == 0)
    {
        char input;
        printf("Which information do you want?\n");
        printf("Version(v),CPU(c),Memory(m),Time(t),All(a),Exit(e)\n");
        scanf("%c",&input);
        getchar();
        FILE *fp = fopen("/proc/my_info", "r");
        char szTest[1000][1000] = {0};
        int j = 0;
        if(NULL == fp)
        {
            printf("failed to open\n");
        }
        while(!feof(fp))
        {
            memset(szTest[j], 0, 1000);
            fgets(szTest[j], 999, fp);
            //printf("%s", szTest[j]);
            j++;
        }
        fclose(fp);

        if(input == 'a')
        {
            for (int i = 0; i<j; i++)
            {
                printf("%s",szTest[i]);
            }
        }
        else if(input == 'e')
        {
            i = 1;
        }
        else if(input == 'v')
        {
            printf("Version :%s",szTest[2]);
        }
        else if(input == 'c')
        {
            printf("\nCPU information :\n");
            for(int i = 5; i< j-16 ; i++)
            {
                printf("%s",szTest[i]);
            }

        }
        else if (input =='m')
        {
            printf("\nMemory information :\n");
            for(int i = j -15; i< j-4 ; i++)
            {
                printf("%s",szTest[i]);
            }

        }
        else if(input =='t')
        {
            printf("\nTime information :\n");
            for(int i = j -3; i< j; i++)
            {
                printf("%s",szTest[i]);
            }
        }
    }
}
