#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define NUM_ALL 12000
#define VECTOR_SIZE 602
#define MAX_STRING_LENGTH 1000
#define NUM_SKIP 50

int main()
{
//read form two files
    FILE *fpin,*fpcompare,*fpcompare2;
    fpin = fopen("vectors.txt","r");
    fpcompare = fopen("vectors1.txt","r"); 
    fpcompare2 = fopen("vectors2.txt","r");
//compare
    int num_to_count = 0 ,b = 0,cnt=0;
    double calc_delta = 0.0;
    double calc_error_one = 0.0;
    double calc_mdis_one = 0.0, calc_mdis = 0.0;
    double calc_cdis_one = 0.0, calc_cdis = 0.0;
    double calc_cos_one = 0.0, calc_cos = 0.0;
    double calc_mae_one = 0.0, calc_mae = 0.0;
    double calc_edis_one = 0.0, calc_edis = 0.0;
    double err_x = 0.0, err_y = 0.0, err_xy = 0.0, err_elu = 0.0;
    double from_a , from_b , result;
    char format[20];
    char *word1 = malloc(sizeof(char) * MAX_STRING_LENGTH + 1);
    char *word2 = malloc(sizeof(char) * MAX_STRING_LENGTH + 1);

    sprintf(format,"%%%d",MAX_STRING_LENGTH);
    for(num_to_count = 0;num_to_count<NUM_ALL;num_to_count++)
    {
        fscanf(fpin,"%s",word1);
        fscanf(fpcompare,"%s",word2);
        if(strcmp(word1,word2)!=0)
        {
            printf("%s_%s\n",word1,word2);
            char c=getchar();
            c =getchar();
        }
        //else printf("%s\n",word1 );
        //assume the word has done nothing to the result
        calc_error_one = 0.0;
        calc_mdis_one = 0.0;
        calc_cdis_one = 0.0;
        calc_cos_one = 0.0;
        err_x = 0.0;
        err_y = 0.0;
        err_xy = 0.0;
        calc_mae_one = 0.0;
        calc_edis_one = 0.0;
            for( b = 0; b < VECTOR_SIZE ; b++)
            {

                fscanf(fpin,"%lf",&from_a); 
                fscanf(fpcompare,"%lf",&from_b);
                if(num_to_count%3==0&&num_to_count>NUM_SKIP){
                    calc_error_one += (from_a-from_b)*(from_a-from_b);
                    calc_mdis_one += abs((from_a-from_b)*(from_a-from_b)*(from_a-from_b));
                   // calc_cdis_one += abs((from_a-from_b)/(abs(from_a)+abs(from_b)));
                    err_x += from_a*from_a;
                    err_y += from_b*from_b;
                    err_xy += abs(from_a*from_b);
                    calc_mae_one += abs(from_a-from_b);
                    calc_edis_one += (from_a-from_b)*(from_a-from_b);
                }
            }
            if(num_to_count % 3 == 0 && num_to_count > NUM_SKIP){
                calc_delta += calc_error_one/VECTOR_SIZE;
                calc_mdis += pow(calc_mdis_one,1.0/3);
                //calc_cdis += calc_cdis_one;
                calc_cos += err_xy/(sqrt(err_x)*sqrt(err_y));
                calc_mae += calc_mae_one/VECTOR_SIZE;
                calc_edis += sqrt(calc_edis_one);
                cnt++;
            }
    }
    printf("the MSE Incremental is %.8lf\n",calc_delta/cnt);
    printf("the M-DIS Incremental is %.8lf\n",calc_mdis/cnt);
   // printf("the C-DIS Incremental is %.4lf\n",calc_cdis/NUM_ALL);
    printf("the COS Incremental is %.8lf\n",calc_cos/cnt);
    printf("the MAE Incremental is %.8lf\n",calc_mae/cnt);
    printf("the D-DIS Incremental is %.8lf\n",calc_edis/cnt);
    fclose(fpcompare);
    fpcompare = fopen("vectors1.txt","r"); 
    cnt = 0;
    calc_delta = 0.0;
    calc_mdis = 0.0;
    calc_cdis = 0.0;
    calc_cos = 0.0;
    calc_mae = 0.0;
    calc_edis = 0.0;
    for(num_to_count = 0;num_to_count<NUM_ALL;num_to_count++)
    {
    fscanf(fpcompare,"%s",word1);
    fscanf(fpcompare2,"%s",word2);
    if(strcmp(word1,word2)!=0)
    {
        printf("%s_%s\n",word1,word2);
        char c=getchar();
        c =getchar();
    }
    //else printf("%s\n",word1 );
    //assume the word has done nothing to the result
     calc_error_one = 0.0;
    calc_mdis_one = 0.0;
    calc_cdis_one = 0.0;
    calc_cos_one = 0.0;
    err_x = 0.0;
    err_y = 0.0;
    err_xy = 0.0;
    calc_mae_one = 0.0;
    calc_edis_one = 0.0;
        for( b = 0; b < VECTOR_SIZE ; b++)
            {

                fscanf(fpcompare,"%lf",&from_a); 
                fscanf(fpcompare2,"%lf",&from_b);
                if(num_to_count % 3 == 0 && num_to_count > NUM_SKIP){
                    calc_error_one += (from_a-from_b)*(from_a-from_b);
                    calc_mdis_one += abs((from_a-from_b)*(from_a-from_b)*(from_a-from_b));
                   // calc_cdis_one += abs((from_a-from_b)/(abs(from_a)+abs(from_b)));
                    err_x += from_a*from_a;
                    err_y += from_b*from_b;
                    err_xy += abs(from_a*from_b);
                    calc_mae_one += abs(from_a-from_b);
                    calc_edis_one += (from_a-from_b)*(from_a-from_b);
                }
            }
            if(num_to_count % 3 == 0 && num_to_count > NUM_SKIP){
                calc_delta += calc_error_one/VECTOR_SIZE;
                calc_mdis += pow(calc_mdis_one,1.0/3);
                //calc_cdis += calc_cdis_one;
                calc_cos += err_xy/(sqrt(err_x)*sqrt(err_y));
                calc_mae += calc_mae_one/VECTOR_SIZE;
                calc_edis += sqrt(calc_edis_one);
                cnt++;
            }
    }
    printf("the MSE Global is %.8lf\n",calc_delta/cnt);
    printf("the M-DIS Global is %.8lf\n",calc_mdis/cnt);
   // printf("the C-DIS Incremental is %.4lf\n",calc_cdis/NUM_ALL);
    printf("the COS Global is %.8lf\n",calc_cos/cnt);
    printf("the MAE Global is %.8lf\n",calc_mae/cnt);
    printf("the D-DIS Global is %.8lf\n",calc_edis/cnt);
    fclose(fpin);
    fclose(fpcompare);
    fclose(fpcompare2);
    return 0;
}
