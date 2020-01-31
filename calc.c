 /* calc.c - Multithreaded calculator */

#include "calc.h"

pthread_t adderThread;
pthread_t degrouperThread;
pthread_t multiplierThread;
pthread_t readerThread;
pthread_t sentinelThread;

char buffer[BUF_SIZE];
int num_ops;
int add=1,multip=1,group=1;
sem_t lock; 
/* Utiltity functions provided for your convenience */

/* int2string converts an integer into a string and writes it in the
   passed char array s, which should be of reasonable size (e.g., 20
   characters).  */
char *int2string(int i, char *s)
{
    sprintf(s, "%d", i);
    return s;
}

/* string2int just calls atoi() */
int string2int(const char *s)
{
    return atoi(s);
}

/* isNumeric just calls isdigit() */
int isNumeric(char c)
{
    return isdigit(c);
}

/* End utility functions */


void printErrorAndExit(char *msg)
{
    msg = msg ? msg : "An unspecified error occured!";
    fprintf(stderr, "%s\n", msg);
    exit(EXIT_FAILURE);
}

int timeToFinish()
{
    /* be careful: timeToFinish() also accesses buffer */
    return buffer[0] == '.';
}

/* Looks for an addition symbol "+" surrounded by two numbers, e.g. "5+6"
   and, if found, adds the two numbers and replaces the addition subexpression 
   with the result ("(5+6)*8" becomes "(11)*8")--remember, you don't have
   to worry about associativity! */
void *adder(void *arg)
{
    
    int bufferlen,operlength;
    int value1, value2,sum;
    int startOffset, remainderOffset;
    int i;
    
    
    //return NULL; /* remove this line */

    while (1) {
	startOffset = remainderOffset = -1;
	value1 = value2 = -1;
    sum=0;
	sem_wait(&lock); 
	if (timeToFinish()) {
		sem_post(&lock);
	    return NULL;
	}

	/* storing this prevents having to recalculate it in the loop */
	 char S[BUF_SIZE];
	 bufferlen = strlen(buffer);
     
	for (i = 0; i < bufferlen; i++)
   {
// Look for a '+' in the expression.
    //pthread_mutex_lock(&lock); 
	
	if(buffer[i] == '+')
      { 
	   
	//    printf("before :check in adder: %s\n",check);
	//     printf("before: buffer in adder: %s\n",buffer);

	   //printf("check in adder:%s\n",check);
        for(startOffset=i-1;;startOffset--)
        {
            if(startOffset>=0&&isNumeric(buffer[startOffset]))
			{
			continue;}
            
            else
             break;
        }
        if(startOffset==i-1)
        {
	   sem_post(&lock);
		continue;}
        
        for(remainderOffset=i+1;;remainderOffset++)
        {
            if(remainderOffset<bufferlen&&isNumeric(buffer[remainderOffset]))
            {
			continue;}
            else
             break;
        }
        if(remainderOffset==i+1)
        {
		sem_post(&lock);
		continue;
		}
        //printf("buffer in adder :%s\n",buffer);
        startOffset++;
        remainderOffset--;
		//printf("startoffset in adder :%d\n",startOffset);
        strncpy(S,&buffer[startOffset],i-startOffset);
        S[i-startOffset] = '\0';
		 
		//printf("S in left:%s\n",S);
        value1=string2int(S);
       //printf("value1 in adder :%d\n",value1);
        strncpy(S,&buffer[i+1],remainderOffset - i);
        S[remainderOffset-i] = '\0';
		
         value2=string2int(S);
		//printf("value2 in adder :%d\n",value2);
         sum=value1+value2;

         int2string(sum,S);
         operlength=strlen(S);

        strncpy(&buffer[startOffset], S, operlength);
		//printf("buffer in adder:%s\n",buffer);
        // strcpy(&buffer[operlength+startOffset], &buffer[remainderOffset+1]);
		 
		 int m=remainderOffset+1-startOffset-operlength;
		 for(int k=0;k<m;k++)
		 {
			 for(int z=0;z<bufferlen;z++)
			 buffer[startOffset+operlength+z]=buffer[startOffset+operlength+z+1];
		 }
		//printf("buffer in adder:%s\n",buffer);
        bufferlen= bufferlen - (remainderOffset - startOffset + 1) + operlength;
        buffer[bufferlen]='\0';
		num_ops+=1;
	// 	printf("after :check in adder: %s\n",check);
	//    printf("after: buffer in adder: %s\n",buffer);
	   
      }
	//printf("buffer in adder:%s\n",buffer);
	//printf("check in adder:%s\n",check);
	//int result=strcmp(check,buffer);
	//printf("result in adder:%d\n",result);
	
    
  } 
  if(strlen(S)==0&&bufferlen>0)
	add=3;
    sem_post(&lock);
	sched_yield();
	usleep(100);
   
    }
  
 }
 
/* Looks for a multiplication symbol "*" surrounded by two numbers, e.g.
   "5*6" and, if found, multiplies the two numbers and replaces the
   mulitplication subexpression with the result ("1+(5*6)+8" becomes
   "1+(30)+8"). */
void *multiplier(void *arg)
{
    
    int bufferlen,operlength;
    int value1, value2;
    int startOffset, remainderOffset;
    int i;
	int mul=1;
    //return NULL; /* remove this line */
 
    while (1) {
	startOffset = remainderOffset = -1;
	value1 = value2 = -1;
    
  sem_wait(&lock); 
	if (timeToFinish()) {
	sem_post(&lock);
	    return NULL;
	}

	/* storing this prevents having to recalculate it in the loop */
	char S[BUF_SIZE];
	bufferlen = strlen(buffer);
    
	 
  for (i = 0; i < bufferlen; i++)
   { 
// Look for a '*' in the expression.
   
    if(buffer[i] == '*')
      {  
		 
	    // printf("before :check in mul: %s\n",check);
	    // printf("before: buffer in mul: %s\n",buffer);
        for(startOffset=i-1;;--startOffset)
        {
            if(startOffset>=0&&isNumeric(buffer[startOffset]))
            { 
				continue;}
            else
             break;
        }
        if(startOffset==i-1)
        {
			sem_post(&lock);continue;}
        
        for(remainderOffset=i+1;;remainderOffset++)
        {
            if(remainderOffset<bufferlen&&isNumeric(buffer[remainderOffset]))
            { continue;}
            else
             break;
        }
        if(remainderOffset==i+1)
        {
		sem_post(&lock);
		continue;}
        //printf("buffer in multi:%s\n",buffer);
        startOffset++;
        remainderOffset--;
        strncpy(S,&buffer[startOffset],i-startOffset);
        S[i-startOffset] = '\0';
		 
        value1=string2int(S);
        //printf("value1 in mul :%d\n",value1);
        strncpy(S,&buffer[i+1],remainderOffset - i);
        S[remainderOffset-i] = '\0';
         value2=string2int(S);
		 //printf("value2 in mul :%d\n",value1);
         mul=value1*value2;

         int2string(mul,S);
         operlength=strlen(S);

        strncpy(&buffer[startOffset], S, operlength);
        int m=remainderOffset+1-startOffset-operlength;
		 for(int k=0;k<m;k++)
		 {
			 for(int z=0;z<bufferlen;z++)
			 buffer[startOffset+operlength+z]=buffer[startOffset+operlength+z+1];
		 }
        bufferlen= bufferlen - (remainderOffset - startOffset + 1) + operlength;
        buffer[bufferlen]='\0';
		 num_ops+=1;
		 
		 //printf("buffer in multi:%s\n",buffer); 
		//  printf("after :check in mul: %s\n",check);
	    // printf("after: buffer in mul: %s\n",buffer);
	 
      }
	
     
    }
   if(strlen(S)==0&&bufferlen>0)
	multip=3;
	sem_post(&lock);
	// something missing?
	 
     sched_yield();
	 usleep(100);
      
    
	 } 
	
}


/* Looks for a number immediately surrounded by parentheses [e.g.
   "(56)"] in the buffer and, if found, removes the parentheses leaving
   only the surrounded number. */
void *degrouper(void *arg)
{
	 
    int bufferlen;
    int i;
     
    //return NULL; /* remove this line */

    while (1) {
    sem_wait(&lock);
	if (timeToFinish()) {
	    sem_post(&lock);
		return NULL;
	}
   char S[BUF_SIZE];
	/* storing this prevents having to recalculate it in the loop */
	bufferlen = strlen(buffer);
	for (i = 0; i < bufferlen; i++) {
		
		if(buffer[i]=='('&&isNumeric(buffer[i+1]))
		{
			 
			//  printf("before :check in group: %s\n",check);
	        // printf("before: buffer in group: %s\n",buffer);
			 
			int index=i+1;
			while(index<bufferlen)
			{
			 if(isNumeric(buffer[index]))
			 index=index+1;
			 else break;
			}
             
			if(buffer[index]==')')
		{
		  
		 for(int k=i;k<bufferlen-2;k++)
		 {
             buffer[k]=buffer[k+1];
             
         }
		 
		 for(int k=index-1;k<bufferlen-3;k++)
		 buffer[k]=buffer[k+1];
		  
		 bufferlen-=2;
		 buffer[bufferlen-1]=';';
		 buffer[bufferlen]='\0';
         strcpy(S,buffer);
		 num_ops+=1;
		 
		}
    //     printf("after :check in group: %s\n",check);
	//    printf("after: buffer in group: %s\n",buffer);
    
          
       } 
	   
     
	   
	    // check for '(' followed by a naked number followed by ')'
	    // remove ')' by shifting the tail end of the expression
	    // remove '(' by shifting the beginning of the expression
	 }
     if(strlen(S)==0&&bufferlen>0)
	    group=3;
	 sem_post(&lock);
    sched_yield();
    usleep(100);

	// something missing?
 }  
	 
    

}


/* sentinel waits for a number followed by a ; (e.g. "453;") to appear
   at the beginning of the buffer, indicating that the current
   expression has been fully reduced by the other threads and can now be
   output.  It then "dequeues" that expression (and trailing ;) so work can
   proceed on the next (if available). */
void *sentinel(void *arg)
{
	 
    char numberBuffer[20];
    int bufferlen;
    int i;
     
     
     
    while (1) {
		 
  sem_wait(&lock);
	if (timeToFinish()) {
	    return NULL;
	}

	/* storing this prevents having to recalculate it in the loop */
	bufferlen = strlen(buffer);
    if(add==3&&multip==3&&group==3)
    {
	 fprintf(stdout,"No progress can be made\n");
     return EXIT_FAILURE;
    }
	for (i = 0; i < bufferlen; i++) {
		
	    if (buffer[i] == ';') {
		if (i == 0) {
			sem_post(&lock); 
		    printErrorAndExit("Sentinel found empty expression!");
		} else {
		    /* null terminate the string */
		    numberBuffer[i] = '\0';
		    /* print out the number we've found */
		    fprintf(stdout, "%s\n", numberBuffer);
		    /* shift the remainder of the string to the left */
		    strcpy(buffer, &buffer[i + 1]);
			sem_post(&lock);
		    break;
		}
	    } else if (!isNumeric(buffer[i])) {
			sem_post(&lock);
		break;
	    } else { 
		numberBuffer[i] = buffer[i];
		
	    }
		
     
	 }
	 

	// something missing? 
	 
	 sem_post(&lock);
	 sched_yield();
	 usleep(100);
     }

    }
  


/* reader reads in lines of input from stdin and writes them to the
   buffer */
void *reader(void *arg)
{
 
	 
    while (1) {
	char tBuffer[100];
	int currentlen;
	int newlen;
	int free;

	fgets(tBuffer, sizeof(tBuffer), stdin);
	 

	/* Sychronization bugs in remainder of function need to be fixed */
    
	newlen = strlen(tBuffer);
	currentlen = strlen(buffer);

	/* if tBuffer comes back with a newline from fgets, remove it */
	if (tBuffer[newlen - 1] == '\n') {
	    /* shift null terminator left */
	    tBuffer[newlen - 1] = tBuffer[newlen];
	    newlen--;
	}

	/* -1 for null terminator, -1 for ; separator */
	free = sizeof(buffer) - currentlen - 2;

	while (free < newlen) {
		sem_wait(&lock);
		currentlen=strlen(buffer);
		free = sizeof(buffer) - currentlen - 2;
		sem_post(&lock);
		sched_yield();
		// spinwaiting
	}

	/* we can add another expression now */
	strcat(buffer, tBuffer);
	strcat(buffer, ";");
	sem_post(&lock);
   
	/* Stop when user enters '.' */
	if (tBuffer[0] == '.') {
	    return NULL;
	}
	 

 }

	
}


/* Where it all begins */
int smp3_main(int argc, char **argv)
{
    void *arg = 0;		/* dummy value */
	 
     sem_init(&lock, 0,1);
    //printErrorAndExit("Failed trying to create mutex");
     


    /* let's create our threads */
    if (pthread_create(&multiplierThread, NULL, multiplier, arg)
	|| pthread_create(&adderThread, NULL, adder, arg)
	|| pthread_create(&degrouperThread, NULL, degrouper, arg)
	|| pthread_create(&sentinelThread, NULL, sentinel, arg)
	|| pthread_create(&readerThread, NULL, reader, arg)) {
	printErrorAndExit("Failed trying to create threads");
    }

    /* you need to join one of these threads... but which one? */
   
    pthread_detach(multiplierThread);
    pthread_detach(adderThread);
    pthread_detach(degrouperThread);
    pthread_detach(readerThread);
    pthread_join(sentinelThread,NULL);
    /* everything is finished, print out the number of operations performed */
    fprintf(stdout, "Performed a total of %d operations\n", num_ops);
	//sem_destroy(&lock); 
    return EXIT_SUCCESS;
}



