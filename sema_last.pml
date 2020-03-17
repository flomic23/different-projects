#define CountW	3
#define CountR	3

typedef Semaphore{
	byte V;
	chan L = [CountW + CountR] of {pid};
}

Semaphore readResourceAccess;
Semaphore readCountAccess;
Semaphore writeResourceAccess;

byte readCount = 0;
byte writeCount = 0;

bool wantW[CountW];
bool critW[CountW];

inline init_sem(S, value) {
	S.V = value;
}

inline wait(S) {
	atomic {
		if
		::	S.V > 0 -> S.V--;
		::	else -> 
			S.L ! _pid;
			!(S.L ?? [eval(_pid)]);
		fi
	}
}

inline signal (S) {
	atomic {
		if
		::	len(S.L) == 0 -> S.V++;
		::	else -> S.L ? _;
		fi
	}
}

proctype reader() {
	do
	:: true -> 

		wait(readResourceAccess);
		wait(readCountAccess);
		readCount++;
		if 
		:: readCount == 1 -> 
			wait(writeResourceAccess);
		:: else -> skip;
		fi
		signal(readCountAccess);
		signal(readResourceAccess);

		/*-----------------reading---------------------*/
		printf( "Data read by reader%u\n",_pid);


		wait(readCountAccess);
		readCount--;
		if 
		:: readCount == 0 ->
			signal(writeResourceAccess);
		:: else -> skip;
		fi
		signal(readCountAccess);
	od
}

proctype writer (byte i) {
	do 
	::true ->
		wantW[i] = true;
		wait(readResourceAccess);
		wait(writeResourceAccess);

		wantW[i] = false;
		critW[i] = true;
		writeCount++;
		printf( "Data written by writer%u\n",_pid);
		/*------------------writing-----------*/
		writeCount--;
		critW[i] = false;

		signal(writeResourceAccess);
		signal(readResourceAccess);
	od
}

init{
	atomic{
		init_sem(readResourceAccess, 1);
		init_sem(writeResourceAccess, 1);
		init_sem(readCountAccess, 1);

		int i = 0;
		do
		:: 	i < CountR -> 
				run reader();
				i++;
		:: i >= CountR ->
				i = 0;
				break;
		od;
		do
		:: i < CountW-> 
				run writer(i);
				i++
		:: i >= CountW ->
				break;
		od
	}
}

ltl mutex 
{
	[]((readCount > 0 -> writeCount == 0) && 
	  (writeCount == 1 -> readCount == 0)) &&
	[](writeCount <= 1)
}

ltl starv 
{
	[]((wantW[0]) -> <>(critW[0])) &&
	[]((wantW[1]) -> <>(critW[1])) &&
	[]((wantW[2]) -> <>(critW[2]))
}
