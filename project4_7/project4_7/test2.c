void main(void)
{
    int cheon;
    int baek;
    int sip;
    int il;

    int i;

    i = 1;
   while( i != 0)
   {
       i = input();
        if( i < 10 ) 
        { 
            il = i;
            output(il);
        }
        else
        {
            if( i < 100 )
            {
                sip = i/10;
                output(sip);
                il = i-sip*10;
                output(il);
            }
            else
            {
                if( i < 1000 )
                {
                    baek = i/100;
                    output(baek);
                    sip = i/10 - baek*10;
                    output(sip);
                    il = i - (i/10)*10;
                    output(il);
                }
                else
                {
                    if(i < 10000)
                    {
                        cheon = i/1000;
                        output(cheon);
                        baek = i/100-cheon*10;
                        output(baek);
                        sip = i/10-(i/100)*10;
                        output(sip);
                        il = i - (i/10)*10;
                        output(il);
                    }
                }
            }
        }
    }
}
