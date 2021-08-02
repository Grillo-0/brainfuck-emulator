#include <stdio.h>
#include <stdlib.h>

typedef struct cell{
    struct cell *next;
    struct cell *back;
    unsigned char value;
}cell;


cell *newCell(cell *back);
void printError(int err, cell **head);
void freeCells(cell **head);
int interpret(FILE *inputFile, cell **head);
int validateInput(FILE *inputFile);

int main(int argc, char *argv[]){
    FILE *inputFile;
    if(argc == 1){
        inputFile = stdin; 
    }
    else{
        inputFile = fopen(argv[1], "rb");
    }
    if(inputFile == NULL) printError(1,NULL);

    if(!validateInput(inputFile))
        printError(4,NULL);

    cell *head = newCell(NULL);    
    interpret(inputFile, &head);
    freeCells(&head);
    fclose(inputFile);
}

cell *newCell(cell *back){
    cell *head = (cell*) malloc(sizeof(cell) * 1);
    head->value = 0;
    head->next = NULL;
    head->back = back;
    return head;
}

void moveFoward(cell **head){
    //printf("moving foward\n");
    if((*head)->next == NULL){
        cell *next = newCell(*head);
        (*head)->next = next;
    }
    *head = (*head)->next;
}

void moveBack(cell **head){
    //printf("moving back\n");
    if((*head)->back == NULL){
        printError(3,head);
    }
    *head = (*head)->back;
}

void freeCells(cell **head){
    while((*head)->next != NULL) *head = (*head)->next;
    while((*head)->back != NULL){
        *head = (*head)->back;
        free((*head)->next);
    }
    free((*head));
}

void printCells(cell **head){
    cell *temp = *head;
    while(temp->back != NULL) temp = temp->back;
    while(temp != NULL){
        printf("| %c(%d) ",temp->value, temp->value);
        temp = temp->next;
    }
    printf("|\n");
}

void printError(int err, cell **head){
    switch(err){ 
        case 1:
            printf("\nCannot locate file\n");
            break;
        case 2:
            printf("\nInvalid char\n");
            break;
        case 3:
            printf("\nIndex out of range\n");
            break;
        case 4:
            printf("\ninvalide code\n");
    }
    freeCells(head);
    exit(-1);
}

int validateInput(FILE *inputFile){
    char currChar;
    int closedSum = 0;
    while(fread(&currChar,sizeof(char), 1, inputFile) != 0){
        if(currChar == '['){
            closedSum++;
        }
        if(currChar == ']'){
            closedSum--;
        }

        if(closedSum < 0){
            return 0;
        }
    }
    fseek(inputFile,0,0);
    return closedSum == 0? 1: 0;
}

int interpret(FILE *inputFile, cell **head){
    char currChar;
    long int temp;
    while(fread(&currChar,sizeof(char), 1, inputFile) != 0){
        //printCells(head);
        switch(currChar){
            case '<':
                moveBack(head);
                break;
            case '>':
                moveFoward(head);
                break;
            case '+':
                (*head)->value++;
                //printf("incrementing value\n");
                break;
            case '-':
                (*head)->value--;
                //printf("decrementing value\n");
                break;
            case '.':
                printf("%c",(*head)->value); 
                break;
            case ',':
                scanf("%c",&(*head)->value);
                break;
            case '[':
                //printf("entered loop\n");
                temp = ftell(inputFile)-1;
                if(interpret(inputFile, head) != 0)
                    fseek(inputFile,temp,0);
                break;
            case ']':
                //printf("end of loop\n");
                //printf("end value: %d\n", (*head)->value);
                return (*head)->value;
                break;
            case '\n':
            case ' ':
                continue;
            default:
                printError(2,head);
        }
    }
}
