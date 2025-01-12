#include <stdio.h>
#include <string.h>

int nums[1001];
// 2: poped
// 1: on stack
// 0: not stacked

int main(void)
{
    int m, n, k;
    scanf("%d%d%d", &m, &n, &k);
    for(int k_i = 0; k_i < k; k_i++)
    {
        //printf("%d---\n", k_i);
        // first element
        int max, cur;
        scanf("%d", &max);
        if(max > m) // stack overflow
        {
            printf("No\n");
            for(int i = 0; i <= m; i++)
                nums[i] = 0;
            while(getchar()!='\n');
            continue;
        }
        nums[max] = 2; cur = max - 1;
        for(int i = 0; i < max; i++)
            nums[i] = 1;
        // from 0 to max - 1 now on stack

        int valid = 1;
        for(int n_i = 2; n_i <= n; n_i++)
        {
            int input;
            scanf("%d", &input);
            //printf("[Get] %d\n", input);
            if(input > max){
                // on stack more element
                memset(nums + max + 1, 1, (input - max) * sizeof(int));
                nums[input] = 2;
                //printf("cur = %d ", cur);
                cur += input - max;//push
                //printf("cur now is %d\n", cur);
                if(cur > m) // stack overflow
                {
                    printf("No\n");
                    valid = 0;
                    break;
                }
                cur--;//pop
                max = input;
            }
            else if(input == max){
                printf("No\n");
                valid = 0;
                break;
                // max is already poped
            }
            else{
                if(nums[input] == 2 || nums[input] == 0)
                {
                    //printf("current stack:");
                    //for(int i = 1; i <= n; i++) printf("%d ", nums[i]);
                    //putchar('\n');
                    printf("No\n"); // not on stack
                    valid = 0;
                    break;
                }
                else
                {
                    int flag = 1;
                    for(int a_i = input + 1; a_i < max; a_i++)
                    {
                        if(nums[a_i] != 2)
                            flag = 0;
                    }
                    if(flag)
                    {
                        cur--;
                        nums[input] = 2;
                    }
                    else{
                        printf("No\n");
                        valid = 0;
                        break;
                    }
                }
            }
        }
        if(valid)
            printf("YES\n");
        else{
            while(getchar() != '\n');
        }
        // clean record
        for(int i = 0; i <= m; i++)
            nums[i] = 0;
    }
    return 0;
}