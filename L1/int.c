
/*************************************************************************
  usp  1   2   3   4   5   6   7   8   9  10   11   12    13  14  15  16
----------------------------------------------------------------------------
 |uds|ues|udi|usi|ubp|udx|ucx|ubx|uax|upc|ucs|uflag|retPC| a | b | c | d |
----------------------------------------------------------------------------
***************************************************************************/

/****************** syscall handler in C ***************************/
int kcinth()
{
   int a,b,c,d, r;

//==> WRITE CODE TO GET get syscall parameters a,b,c,d from ustack 

//d c b a
//get_word(running->uss, running->usp + 2bytes * 13)
a=get_word(running->uss, running->usp + 2 * 13);
b=get_word(running->uss, running->usp + 2 * 14);
c=get_word(running->uss, running->usp + 2 * 15);
d=get_word(running->uss, running->usp + 2 * 16);

   switch(a){
       case 0 : r = kgetpid();        break;
       case 1 : r = ksp();            break;
       case 2 : r = kchname(b);       break;
       case 3 : r = kkfork();         break;
       case 4 : r = ktswitch();       break;
       case 5 : r = kkwait(b);        break;
       case 6 : r = kkexit(b);        break;

	case 7: r = kmode();		break;
	case 8: r= exec(b);		break;
	case 9: r = vfork();		break;
	case 90: r = getc();		break;
	case 91: color = kgetpid()+11;
		r = putc(b);		break;

       case 99: kkexit(b);            break;
       default: printf("invalid syscall # : %d\n", a); 
   }

//==> WRITE CODE to let r be the return value to Umode
put_word(r, running->uss, running->usp + 2 * 8);
}

//============= WRITE C CODE FOR syscall functions ======================

int kgetpid()
{
    //WRITE YOUR C code
return running->pid;

}
