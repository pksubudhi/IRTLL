#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#include "struc.h"
#include "global.h"

ENGINE *addEngine(ENGINE *, int);
void addFirstCoach(ENGINE *, int, int, char *, int, int, char *);
void addCoach(ENGINE *, int, int, char *, int);

void dispAllEngine(ENGINE *);
void dispAllCoaches(ENGINE *, int);

BOOL isEngineFound (ENGINE *, int);
BOOL isCoachFound(ENGINE *, int, int, char *);


void closeAll(ENGINE *);
void delinkCoaches(COACH *);
void vacateSeats(SEAT *);

ENGINE *makeFromFile(ENGINE *head);
void parseString (char *str, int *cno, int *tno, int *cap, char *tname, char *cname, int *flg, int *eno);
void substring(char *src, char *tar, int sindex);
int main() {
	
	ENGINE *head = NULL;
	
	head = makeFromFile(head);
	
	printf("\nTRAINS: \n");
	dispAllEngine(head);
	printf(":END\n");
	
	closeAll(head);
	return 0;
}

//Garbage Collection

void closeAll(ENGINE *head) {
	ENGINE *temp = NULL, *prev = NULL;
	if(head != NULL) {
		if(head->next == NULL) {
			free(head);
		}
		else {
			while(head->next != NULL) {
				temp = head;
				while(temp->next != NULL) {
					temp = temp->next;
				}
				prev = head;
				while(prev->next != temp) {
					prev = prev->next;
				}
				delinkCoaches(temp->cptr);
				free(temp);
				prev->next = NULL;
			}
			free(head);
		}
	}
}
void delinkCoaches(COACH *head) {
	COACH *temp = NULL, *prev = NULL;
	if(head != NULL) {
		if(head->next == NULL) {
			free(head);
		}
		else {
			while(head->next != NULL) {
				temp = head;
				while(temp->next != NULL) {
					temp = temp->next;
				}
				prev = head;
				while(prev->next != temp) {
					prev = prev->next;
				}
				vacateSeats(temp->start);
				free(temp);
				prev->next = NULL;
			}
			free(head);
		}
	}
}
void vacateSeats(SEAT *head) {
	SEAT *temp = NULL, *prev = NULL;
	if(head != NULL) {
		if(head->next == NULL) {
			free(head);
		}
		else {
			while(head->next != NULL) {
				temp = head;
				while(temp->next != NULL) {
					temp = temp->next;
				}
				prev = head;
				while(prev->next != temp) {
					prev = prev->next;
				}
				free(temp);
				prev->next = NULL;
			}
			free(head);
		}
	}
}

ENGINE *makeFromFile(ENGINE *head) {
	int coachno=0, trainno=0, capacity=0, eno=0;
	char train_name[50], coach_name[5];
	int cflag=-1;	//0-if it is engine, 1-for first coach, 2- other coach
	FILE *fp;
	char buffer[1024];
	fp = fopen("trains.txt", "r");
	if(fp == NULL) {
		printf("Unable to open the source file!");
	}
	else {
		while (fgets(buffer, 1024, fp)) {
			//printf("%s", buffer);
			parseString(buffer, &coachno, &trainno, &capacity, train_name, coach_name, &cflag, &eno);
			//printf("%d %d %d %s %s %d %d\n",coachno, cflag, trainno, train_name, coach_name, capacity, eno);
			if (cflag == 0) {
				if(!isEngineFound(head,coachno)) {
					head = addEngine(head, coachno);
					//printf("Engine Created\n");
				}
				else {
					printf("Engine alread exists in the pool\n");
				}
			}
			if (cflag == 1) {
				if(!isCoachFound(head, eno, coachno, coach_name)) {
					addFirstCoach(head, eno, coachno, coach_name, capacity, trainno, train_name);
					//printf("First Coach %s added!\n", coach_name);
				}
				else {
					printf("Coach already added or creating conflict!\n");
				}
			}
			if (cflag == 2 ) {
				if(!isCoachFound(head, eno, coachno, coach_name)) {
					addCoach(head, eno, coachno, coach_name, capacity);
					//printf("Coach S2 added!\n");
				}
				else {
					printf("Coach already added or creating conflict!\n");
				}
			}
		}
	}
	fclose(fp);
	return head;
}
void parseString (char *str, int *cno, int *tno, int *cap, char *tname, char *cname, int *flg, int *eno) {
	char sword[30], tword[20], fword[20], xtra[10];
	sscanf(str,"%d %s %s %s %s", cno, sword, tword, fword, xtra);
	if(isdigit(sword[0])==0) {
		if(strcmp(xtra,"F") == 0) {
			*flg = 1;	
		}
		else {
			*flg = 2;
		}
		
		//*tname='\0';
		strcpy(cname, sword);
		*cap = atoi(tword);
		*eno = atoi(fword);
	}
	else {
		
		*flg = 0;
		*tno = atoi(sword);
		substring(str, tname, 13);
		*cname='\0';
	}
}
void substring(char *src, char *tar, int sindex) {
	while(src[sindex] !='\n') {
		*tar = src[sindex++];
		tar++;
	}
	*tar = '\0';
}
ENGINE *addEngine(ENGINE *head, int eno){
	ENGINE *temp=head;
	ENGINE *new_engine;
	new_engine = (ENGINE *) malloc(sizeof(ENGINE));
	new_engine->eno = eno;
	new_engine->tno = 0;
	strcpy(new_engine->tname, "UNDEFINED");
	new_engine->cptr = NULL;
	new_engine->next = NULL;
	
	if (head == NULL ) {
		head = new_engine;
		return head;
	}
	else {
		while(head->next != NULL) {
			head = head->next;
		}
		head->next = new_engine;
	}
	return temp;
}
void dispAllEngine(ENGINE *head) {
	while(head!= NULL) {
		printf("[%d - %s (%d)]-> ", head->tno, head->tname, head->eno);
		printf("\n\tCOACHES: ");
		dispAllCoaches(head, head->eno);
		printf(": END\n");
		head = head->next;
	}
}

void dispAllCoaches(ENGINE *head, int eno) {
	COACH *temp = head->cptr;
	while(temp != NULL) {
		printf("[ %s ] ", temp->cid);
		temp = temp->next;
	}
}
void addFirstCoach(ENGINE *head, int eno, int cno, char *cid, int tot_seats, int tno, char *tname) {
	COACH *new_coach = (COACH *) malloc(sizeof(COACH));
	strcpy(new_coach->cid, cid);
	new_coach->cno = cno;
	new_coach->tot_seat = tot_seats;
	new_coach->next = NULL;
	
	while(head->eno != eno){
		head = head->next;
	}
	head->tno = tno;
	strcpy(head->tname, tname);
	head->cptr = new_coach;
}
void addCoach(ENGINE *head, int eno, int cno, char *cid, int tot_seats) {
	COACH *temp = NULL;
	COACH *new_coach = (COACH *) malloc(sizeof(COACH));
	
	strcpy(new_coach->cid, cid);
	new_coach->cno = cno;
	new_coach->tot_seat = tot_seats;
	new_coach->next = NULL;
	
	while(head->eno != eno){
		head = head->next;
	}
	temp = head->cptr;
	while(temp->next != NULL) {
		temp = temp->next;
	}
	temp->next = new_coach;
}

BOOL isEngineFound (ENGINE *head, int eno) {
	while(head != NULL) {
		if (head->eno == eno) {
			return TRUE;
		}
		head = head->next;
	}
	return FALSE;
}
BOOL isCoachFound(ENGINE *head, int eno, int cno, char *cid) {
	while(head != NULL) {
		if (head->eno == eno) {
			COACH *temp = head->cptr;
			while(temp != NULL) {
				if ((temp->cno == cno) || (strcmp(temp->cid, cid) == 0)) {
					return TRUE;
				}
				temp = temp->next;
			}
			return FALSE;
		}
		head = head->next;
	}
	printf("Engine not found!\n");
	return TRUE;
}
