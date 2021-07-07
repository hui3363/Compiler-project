void printfun(int array[],int size)
{
    int i;
    i=0;
    while(i < size)
    {
        output(array[i]);
        i = i + 1;
    }
}
void swap(int array[], int i, int j)
{
    int temp;

    temp = array[i];
    array[i] = array[j];
    array[j] = temp;
}
void bubble(int array[])
{
    int i;
    int j;
    int temp;
    int brray[2];    
    
    brray[0]=100;
    brray[1]=100;
    
    printfun(brray,2);
    i=0;
    while( i < 5 )
    {
        j=0;
        while( j < 4 )
        {
            if(array[j] > array[j+1])
            {
                swap(array,j,j+1);
            }
            j = j + 1;
        }
        i = i + 1;
    }
    printfun(array,5);
}

void main(void)
{

    int i;
    int a;
    int b;
    int flag;
    int array[5];
    i = 0;

    while( i < 5 )
    {
        array[i] = input();
        i = i + 1;
    }
    printfun(array,5);
    a = input();
    b = input();
    flag=0;
    while( flag == 0 )
    { 
        swap(array, a, b);

        printfun(array,5);

        bubble(array);

        a = input();
        b = input();
        if(a > 4) flag=1;
        if(a < 0) flag=1;
        if(b > 4) flag=1;
        if(b < 0) flag=1;

    }
}
