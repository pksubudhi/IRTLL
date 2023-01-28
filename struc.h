struct Coach;
struct Seat;

typedef struct Engine {
	int eno;				//Engine Number (A 6 digit number)
	int tno;				//Train Number
	char tname[50];			//Train Name	[eg. Vishakha Express, Amritsar-New Delhi Express]
	struct Coach *cptr;		//Pointer to hold address of first coach
	struct Engine *next;	//Pointer to hold address of next engine
}ENGINE;

typedef struct Coach {
	int cno;				//Coach Number (A 6 digit number same as eno of Engine)
	char cid[5];			//Coach ID (eg. S1, S2, A1, B1, SE1 etc)
	int tot_seat;			// Total Number of seats
	struct Seat *start;		//Starting addres of seat chain
	struct Coach *next;
}COACH;

typedef struct Seat {
	int sno;				//Seat numbers
	char sid[3];			//Seat ID [eg. LB, MB, SL, SU etc]
	int status;				//1-Reserved, 0-Available (Default)
	struct Seat *next;
}SEAT;
