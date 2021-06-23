#include <stdio.h>

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_RESET   "\x1b[0m"

//Methode um die Werte des Arrays auszulesen
void printArray(int array[7][6]) {
    printf("| 1 | 2 | 3 | 4 | 5 | 6 | 7 |\n");
    for(int i = 0; i < 6;i++) {
        for ( int j = 0; j < 7; j++) {
            if(j == 0) {
                printf("|");
            }
            printf(" %i |", array[j][i]);
        }
        printf("\n");
    }
    printf("Print aufruf beendet;\n");
}

void drawCell(int array[7][6], int x, int y) {
    switch (array[x][y]) {
        case 0: //Empty
            printf("   |");
            break;
        case 1: //Player 1
            printf(ANSI_COLOR_RED " O " ANSI_COLOR_RESET "|" );
            break;
        case 2: // Player 2
            printf( ANSI_COLOR_YELLOW " X " ANSI_COLOR_RESET "|" );
            break;
    }
}

// Methode um das Feld in Hübsch auszulesen
void drawArray(int array[7][6]) {
    printf("| 1 | 2 | 3 | 4 | 5 | 6 | 7 |\n");
    for(int i = 0; i < 6;i++) {
        for ( int j = 0; j < 7; j++) {
            if(j == 0) {
                printf("|");
            }
            drawCell(array, j, i);
        }
        printf("\n");
    }
    printf("Print aufruf beendet;\n");
}

// Methode um das Spielfeld zu resetten
void cleanArray(int array[7][6]) {
    for(int i = 0; i < 7;i++) {
        for ( int j = 0; j < 6; j++) {
            array[i][j] = 0;
        }
    }
}

void drawField(int array[7][6]){
    printf("| 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 |\n");
    int counter = 1;
    for(int i = 0; i < 12;i++) {
        for ( int j = 0; j < 7; j++) {
            if(i % 2 == 0){
                if(j == 0) {
                    printf("-----");
                }
                printf("----");
            } else {
                if(j == 0) {
                    printf("| %i |", counter);
                    counter++;
                }
                drawCell(array, j , i/2);
            }
        }
        printf("\n");
    }
}

// Hier wird gezählt wie viele Steine, mit den Plazierten Stein eingerechnet, vom Spieler sind
int verticalCheck(int array[7][6], int direction, int startAtX, int startAtY, int player){
    int x = startAtX, y = startAtY;
    int counter=0;
    while (array[x][y] == player){
        counter++;
        y += direction;
    }
    return counter;
}

// Hier wird gezählt wie viel Steine, mit dem plazierten stein eingerechnet, vom Spieler sind
// Da diese Methode 2 mal aufgerufen wird für links und rechts müsste man entweder unten beim Vergleich
// -1 rechnen oder man verschiebt beim aufruf in die entsprechenden Wert um links oder Rechts
int horizontalCheck(int array[7][6], int direction, int startAtX, int startAtY, int player){
    int x = startAtX, y = startAtY;
    int counter=0;
    while (array[x][y] == player){
        counter++;
        x += direction;
    }
    return counter;
}

int diagonalCheck(int array[7][6], int directionX, int directionY, int startAtX, int startAtY, int player){
    int x = startAtX, y = startAtY;
    int counter=0;
    while (array[x][y] == player){
        counter++;
        x += directionX;
        y += directionY;
    }
    return counter;
}

int checkIfSomeOneHasWon(int array[7][6], int stonePlacedAtX, int stonePlacedAtY, int player){

    if(verticalCheck(array,1,stonePlacedAtX,stonePlacedAtY,player)  == 4) {
        return 1;
    }

    if(horizontalCheck(array,1,stonePlacedAtX,stonePlacedAtY,player) + horizontalCheck(array,-1,stonePlacedAtX-1,stonePlacedAtY,player) >= 4 ){
        return 1;
    }

    // Check von Stein nach Links unten und von Stein nach rechts oben;
    if(diagonalCheck(array,-1,-1,stonePlacedAtX,stonePlacedAtY,player) + diagonalCheck(array,1,1,stonePlacedAtX+1,stonePlacedAtY+1,player) >= 4){
        return 1;
    }
    if(diagonalCheck(array,-1,1,stonePlacedAtX,stonePlacedAtY,player) + diagonalCheck(array,1,-1,stonePlacedAtX+1,stonePlacedAtY-1,player) >= 4){
        return 1;
    }

    return 0;
}

int checkIfColumIsFullAndPlace(int array[7][6], int colum, int player) {
    if(colum >= 0 && colum <= 7) {
        for(int i=5; i >= 0; i--) {
            if (array[colum][i] == 0) {
                array[colum][i] = player;
                return i;
            }
        }
    }
    return -1;
}

int changePlayer(int player) {
    switch (player) {
        case 1:
            return  2;
        case 2:
            return  1;
    }
}

int getY(int array[7][6], int x){
    for (int i =0; i<=5;i++){
        if(array[x][i] != 0){
            return i;
        }
    }
}

void startGame(int array[7][6]) {
    int round = 0;
    int player = 1; // player1 = 1, player2 = 2;
    int placeStoneAt = 0;
    int y=-1;
    while (round != 43) {
        printf("Bitte gib eine Reihe ein in Welcher dein Stein geworfen werden soll: \n");
        scanf(" %i", &placeStoneAt);
        y = checkIfColumIsFullAndPlace(array,placeStoneAt-1,player);
        while (y==-1){
            printf("Bitte gib eine gueltige Reihe(1-7) an die nicht voll ist \n");
            scanf(" %i", &placeStoneAt);
        }
        drawField(array);
        //drawArray(array);
        //printArray(array);
        printf("X: %i, Y: %i\n",placeStoneAt-1, y);
        if(round >= 6){
            if(checkIfSomeOneHasWon(array, placeStoneAt-1, y, player)){
                printf("Player %i has won after %i rounds", player, round);
                break;
            }
        }
        player = changePlayer(player);
        round++;
    }
    if (round == 43) {
        printf("Unentschieden");
    }
}

int menueText() {
    printf("Willkommen zu Vier gewinnt, waehle ein Spielmodus aus \n");
    printf("1) 2 Spieler\n");
    printf("2) Gegen den Computer\n");
    printf("3) Exit!\n");
}

int menueEingabe() {
    menueText();
    int eingabe = 0;
    scanf("%i", &eingabe);
    while (eingabe < 1 || eingabe > 3) {
        printf("Bitte geben sie einnen Spielmodus ein (1 | 2) oder 3, wenn sie das Spiel verlassen wollen\n");
        scanf("%i", &eingabe);
    }
    return eingabe;
}

int menue() {
    int loop = 0;
    int array[7][6];
    while (loop != -1) {
        cleanArray(array);
        switch (menueEingabe()) {
            case 1:
                startGame(array);
                break;
            case 2:
                startGame(array);
                break;
            case 3:
                loop = -1;
        }
        printf("\n");
    }
    return 0;
}

int main() {
    menue();
    return 0;
}
