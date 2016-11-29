void prints(char *s){
	while (*s){
		putc(*s);
		s++;
	}	

}

void printf(char *fmt, ...){
	
	char *cp = fmt;
	u16 *ip = (u16 *)&fmt + 1;
	u32 *up;
	while (*cp){
		if (*cp != '%'){
			putc(*cp);
			if (*cp=='\n')
				putc('\r');
			cp++;
			continue;
		}
		cp++;
		switch(*cp){
			case 'c' : putc(*ip); break;
}	

char *gets(char s[]){
	char current;
	int i = 0;
	while (current != '\r'){
		current = getc();
		putc(current);
		s[i] = current;
		i++;
	}
	s[i] = 0;
	putc('\n');
	putc('\r');
	return s;
}

