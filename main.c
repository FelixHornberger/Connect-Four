#include <stdio.h>
#include <stdlib.h>

#define ANSI_COLOR_RED      "\x1b[31m"
#define ANSI_COLOR_YELLOW   "\x1b[33m"
#define ANSI_COLOR_GREEN    "\x1b[32m"
#define ANSI_COLOR_RESET    "\x1b[0m"

// Mit dieser Methode werden die Zellen "erschaffen";
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

// Methode um das Spielfeld zu resetten
void cleanArray(int array[7][6]) {
    for(int i = 0; i < 7;i++) {
        for ( int j = 0; j < 6; j++) {
            array[i][j] = 0;
        }
    }
}

// Methode um das Spielfeld zu zeichnen
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
        if( x == -1 || x == 7) {
            break;
        }
        counter++;
        x += direction;
    }
    return counter;
}

// Diagonale Überprüfungsmethode
int diagonalCheck(int array[7][6], int directionX, int directionY, int startAtX, int startAtY, int player){
    int x = startAtX, y = startAtY;
    int counter=0;
    while (array[x][y] == player){
        if ( y == 6 || y == -1 || x == 7 || x == -1) {
            break;
        }
        counter++;
        x += directionX;
        y += directionY;
    }
    return counter;
}


// Hier werden alle Überprüfungen durchgeführt
int checkIfSomeOneHasWon(int array[7][6], int stonePlacedAtX, int stonePlacedAtY, int player){
    // Check von Stein aus nach Unten
    if(verticalCheck(array,1,stonePlacedAtX,stonePlacedAtY,player)  == 4) {
        return 1;
    }
    // Check links und rechts vom Stein <- X ->
    if(horizontalCheck(array,1,stonePlacedAtX,stonePlacedAtY,player) + horizontalCheck(array,-1,stonePlacedAtX-1,stonePlacedAtY,player) >= 4 ){
        return 1;
    }
    // Check von Stein nach Links unten und von Stein nach rechts oben;
    if(diagonalCheck(array,-1,-1,stonePlacedAtX,stonePlacedAtY,player) + diagonalCheck(array,1,1,stonePlacedAtX+1,stonePlacedAtY+1,player) >= 4){
        return 1;
    }
    // Check von Stein nach Links oben und von Stein nach rechts unten;
    if(diagonalCheck(array,-1,1,stonePlacedAtX,stonePlacedAtY,player) + diagonalCheck(array,1,-1,stonePlacedAtX+1,stonePlacedAtY-1,player) >= 4){
        return 1;
    }

    return 0;
}

// Methode um zu überprüfen ob die Spalte voll ist und wenn nicht wird dort der Steinpalziert, gibt die Y Pos des Steins zurück
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

// Methode um den Spieler zu tauschen
int changePlayer(int player) {
    switch (player) {
        case 1:
            return  2;
        case 2:
            return  1;
        default:
            return -1; // Default ist ein error;
    }
}

// Hier befindet sich die eigentliche spiel Logik
// Zweite varibale Dient dazu um definieren ob das Spiel gegen Echten Spieler oder PC gespielt wird (0 Human, 1 CPU)
void startGame(int array[7][6], int cpu) {
    cleanArray(array); // Hier mit wird sicher gestellt dass das Array leer ist
    drawField(array);
    int round = 0;
    int player = 1; // player1 = 1, player2 = 2; // Da Spieler 1 immer anfängt, wird es mit dieser Nr intialisiert
    int placeStoneAt;
    int y;
    while (round != 42) {
        printf("Bitte gib eine Reihe ein in Welcher dein Stein geworfen werden soll: \n");
        char charToCheck[1];
        scanf(" %s", &charToCheck[0]);
        placeStoneAt = charToCheck[0] - 49;
        y = checkIfColumIsFullAndPlace(array, placeStoneAt, player);
        while (y == -1) {
            printf("Bitte gib eine gueltige Reihe(1-7) an die nicht voll ist \n");
            scanf(" %s", &charToCheck[0]);
            placeStoneAt = charToCheck[0]- 49;
            y = checkIfColumIsFullAndPlace(array, placeStoneAt, player);
        }
        if (round >= 6) {
            if (checkIfSomeOneHasWon(array, placeStoneAt, y, player)) {
                drawField(array); // Normale Zeichen Methode vom Feld
                printf( ANSI_COLOR_GREEN "Player %i has won after %i rounds\n" ANSI_COLOR_RESET, player, round);
                break;
            }
        }
        // Der Zug vom Bot wird parallel zum Spielezug gemacht
        // Der Code zum Spieler Unterscheidet sich jeglich darin das der Bot die Zahl solange Random erzeugt, wie das Feld leer ist
        if (cpu == 1) {
            placeStoneAt = rand() % 7;
            y = checkIfColumIsFullAndPlace(array, placeStoneAt, 2);
            while (y == -1) {
                placeStoneAt = rand() % 7;
                y = checkIfColumIsFullAndPlace(array, placeStoneAt, 2);
            }
            printf("Coumpter hat in Spalte %i den Stein geworfen\n", y);
            if (round >= 6) {
                if (checkIfSomeOneHasWon(array, placeStoneAt, y, 2)) {
                    drawField(array);
                    printf(ANSI_COLOR_RED "Computer has won after %i rounds\n" ANSI_COLOR_RESET, round);
                    break;
                }
            }
            round++;
        }
        drawField(array);
        if (cpu == 0) {
            player = changePlayer(player);
        }
        round++;
    }
    printf("Unentschieden, kein Spieler konnte das Spiel fuer sich entscheiden!\n");
}

// Hier werden die Auswahlmöglichkeiten für den Spieler niedergeschrieben;
void menueText() {
    printf("Willkommen zu Vier gewinnt, waehle ein Spielmodus aus \n");
    printf("1) 2 Spieler\n");
    printf("2) Gegen den Computer\n");
    printf("3) Exit!\n");
}

// Hier wird überprüft ob die Eingabe Legitim ist zur Leserlichkeit getrennt :D // Wer liest schon Quellcode?
int menueEingabe() {
    menueText();
    char charToCheck[1];
    scanf(" %s", &charToCheck[0]);
    int eingabe = charToCheck[0] - 48;
    while (eingabe < 1 || eingabe > 3) {
        printf("Bitte geben sie einnen Spielmodus ein (1 | 2) oder 3, wenn sie das Spiel verlassen wollen\n");
        scanf(" %s", &charToCheck[0]);
        eingabe = charToCheck[0] - 48;
    }
    return eingabe;
}

// menu Methode hier wird überprüft welchen Spielmods der Spieler auswählt
void menue() {
    int loop = 0;
    int array[7][6];
    while (loop != -1) {
        switch (menueEingabe()) {
            case 1:
                startGame(array, 0);
                break;
            case 2:
                startGame(array, 1);
                break;
            case 3:
                loop = -1;
                break;
        }
    printf("\n");
    }
}

int main() {
    menue();
    return 0;
}
