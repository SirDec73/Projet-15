#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define RED "\x1B[31m"
#define RESET "\x1B[0m"
#define GREEN "\x1b[32m"
#define YELLOW "\x1b[33m"
#define BLUE "\x1b[34m"
// Clear the buffer of scanf_s

void ClearBuffer()
{
    char c = getchar();
    ;
    while (c != '\n')
    {
        c = getchar();
    }
}

// Ask an int enter a minimum and a maximum
int AskIntInRange(const char* text, int Value, int Bmin, int Bmax)
{
    int iError = 0;

    // while the value is not an int or the value is out of range
    while (iError == 0)
    {
        printf(text);
        // Input is an int ?
        iError = scanf_s("%d", &Value);
        if (iError == 1)
        {
            // If in range
            if (Value <= Bmax && Value >= Bmin)
            {
                printf("\n");
                // reset buffer of scanf_s
                ClearBuffer();
                return Value;
            }
            // If out of range
            else
            {
                printf("ERREUR Hors bornes \nEntree entre %d et %d : \n", Bmin, Bmax);
                iError = 0;
            }
        }
        // reset buffer of scanf_s
        ClearBuffer();
    }
}
// Compare an input char with a list of possibility
int CompareWithArray(const char* Message, char* ExpectedAnswer, int SizeArray)
{
    int iError = 0;
    char Input;
    while (iError == 0)
    {
        printf(Message);
        // Verifier que l'entre est un char
        iError = scanf_s("%c", &Input, 1);
        Input = toupper(Input);
        for (int i = 0; i < SizeArray; i++)
        {
            if (Input == ExpectedAnswer[i])
            {
                // reset buffer of scanf_s
                ClearBuffer();
                return 1;
            }
        }
        iError = 0;
        // reset buffer of scanf_s
        return 0;
    }
}

// Struc of Tile
typedef struct Tile {
    int x;
    int y;
    int content;
    int hidden;
    int Flag;
} Tile;

// Struct of Hypothesis
typedef struct Suggestion {
    int nbHypothesis;
    int nbTilesRemain;
} Suggestion;

// Struct of the Grid
typedef struct Grid {
    int Width;
    int Height;
    int RemainingTiles;
    Tile* Tile;
    Suggestion* Suggestion;

} Grid;

/*
// TODO
Tile* GetTile(Grid* grid, int x, int y ) {

    return NULL;

}
*/
;
#define BOMB 9

// Initialisation default Tiles
void InitiateDefaultTiles(Grid* Grid, int SizeHeight, int SizeWidth)
{
    for (int i = 0; i < SizeHeight; i++) // For each y (Column) *
    {
        for (int j = 0; j < SizeWidth; j++) // For each x in a Column
        {
            Grid->Tile[i * SizeWidth + j].x = j;
            Grid->Tile[i * SizeWidth + j].y = i;
            Grid->Tile[i * SizeWidth + j].content = 0;
            Grid->Tile[i * SizeWidth + j].hidden = 1;
            Grid->Tile[i * SizeWidth + j].Flag = 0;
            Grid->Suggestion[i * SizeWidth + j].nbHypothesis = 0;
            Grid->Suggestion[i * SizeWidth + j].nbTilesRemain = 0;
        }
    }
}

// Display the game grid for the player
void DisplayPlayerGrid(int Height, int Width, const Grid* Grid)
{
    printf("Grid Player\n");
    for (int i = 0; i < Height; i++)
    {
        for (int j = 0; j < Width; j++)
        {

            if (Grid->Tile[i * Width + j].hidden == 0)
            {
                if (Grid->Tile[i * Width + j].content == BOMB)
                {
                    printf(RED "|%d|" RESET, Grid->Tile[i * Width + j].content);
                }
                else if (Grid->Tile[i * Width + j].content == 1)
                {
                    printf(BLUE "|%d|" RESET, Grid->Tile[i * Width + j].content);
                }
                else if (Grid->Tile[i * Width + j].content == 2)
                {
                    printf(GREEN "|%d|" RESET, Grid->Tile[i * Width + j].content);
                }
                else {
                    printf("|%d|", Grid->Tile[i * Width + j].content);
                }
            }
            else if (Grid->Tile[i * Width + j].Flag == 1)
            {
                printf("|P|");
            }
            else
            {
                printf("| |");
            }
        }
        printf("\n");
    }
}

// Display the game grid for the dev (with all the content)
void DisplayOpenedGrid(int Height, int Width, const Grid* Grid)
{
    //system("cls");
    printf("Opened Grid\n");
    for (int i = 0; i < Grid->Height; i++)
    {
        for (int j = 0; j < Grid->Width; j++)
        {
            if (Grid->Tile[i * Width + j].content == BOMB)
            {
                printf(RED "|%d|" RESET, Grid->Tile[i * Width + j].content);
            }
            else if (Grid->Tile[i * Width + j].content == 1)
            {
                printf(BLUE "|%d|" RESET, Grid->Tile[i * Width + j].content);
            }
            else if (Grid->Tile[i * Width + j].content == 2)
            {
                printf(GREEN "|%d|" RESET, Grid->Tile[i * Width + j].content);
            }
            else
            {
                printf("|%d|", Grid->Tile[i * Width + j].content);
            }
        }
        printf("\n");
    }
    printf("\n");
}

void DisplayHypoGrid(int Height, int Width, const Grid* Grid)
{
    //system("cls");
    printf("NbHypo :\n");
    for (int i = 0; i < Grid->Height; i++)
    {
        for (int j = 0; j < Grid->Width; j++)
        {
            printf("|%d|", Grid->Suggestion[i * Width + j].nbHypothesis);
        }
        printf("\n");
    }
    printf("\n");
    printf("NbTileRemHypo :\n");
    for (int i = 0; i < Grid->Height; i++)
    {
        for (int j = 0; j < Grid->Width; j++)
        {
            printf("|%d|", Grid->Suggestion[i * Width + j].nbTilesRemain);
        }
        printf("\n");
    }
    printf("\n");
}

// Function to check if the chosen tile is alr open/flagged
int AskChoices(int Flag, int ChoiceX, int ChoiceY, const Grid* Grid)
{

    const Tile* tile = &Grid->Tile[ChoiceY * Grid->Height + ChoiceX];
    if (tile->hidden == 0)
    {
        printf("The tile is already open\n");
        return 0;
    }
    else if (tile->Flag == 1 && Flag == 0)
    {
        printf("The tile is Flaged !\n");
        return 0;
    }
    return 1;
}
// Function to check if a mine is generated around the first chosen tile
int CheckAroundChosenTile(int ChoiceX, int ChoiceY, int MineY, int MineX)
{
    for (int dx = -1; dx <= 1; dx++)
    {
        for (int dy = -1; dy <= 1; dy++)
        {
            int newChoiceX = ChoiceX + dx;
            int newChoiceY = ChoiceY + dy;
            if (newChoiceX == MineX && newChoiceY == MineY)
            {
                return 1;
            }
        }
    }
    return 0;
}

// Function for assigning Mine in the Grid with all the condition (No mine around the first chosen tile + Preventing multiple mine on 1 tile)
int CreateMine(Grid* Grid, int SizeHeight, int SizeWidth, int ChoiceX, int ChoiceY, float PercentOfMine)
{

    int MineY = 0;
    int MineX = 0;
    int NbBomb = floor((SizeHeight * SizeWidth) * PercentOfMine);
    int* AllMines = (int*)malloc(SizeHeight * SizeWidth * sizeof(int));
    if (AllMines == NULL)
    {
        exit(1);
    }
    int CompteurTileDispo = SizeHeight * SizeWidth - 1;
    // Generate All Mines in a Grid
    for (int k = 0; k < SizeHeight; k++)
    {
        for (int j = 0; j < SizeWidth; j++)
        {
            AllMines[k * SizeWidth + j] = k * SizeWidth + j;
        }
    }
    //
    for (int i = 0; i < SizeHeight; i++)
    {
        MineX = SizeHeight - i - 1;
        for (int j = 0; j < SizeWidth; j++)
        {
            MineY = SizeWidth - j - 1;
            if (CheckAroundChosenTile(ChoiceX, ChoiceY, MineY, MineX))
            {
                int indexImpossiblePosition = MineX * SizeWidth + MineY;
                AllMines[indexImpossiblePosition] = AllMines[CompteurTileDispo];
                CompteurTileDispo -= 1;
            }
        }
    }
    int BoolStartGrid;
    // #TODO better algo
    for (int i = 0; i < NbBomb; i++)
    {
        int RandMine = rand() % CompteurTileDispo;
        int iTileIndex = AllMines[RandMine];
        Grid->Tile[iTileIndex].content = 9;
        MineY = floor(AllMines[RandMine] / SizeWidth);
        MineX = AllMines[RandMine] % SizeHeight;
        // Add the numbers Around mines
        for (int dx = -1; dx <= 1; dx++)
        {
            for (int dy = -1; dy <= 1; dy++)
            {
                int newX = MineX + dx;
                int newY = MineY + dy;
                if (newX >= 0 && newX < SizeWidth && newY >= 0 && newY < SizeHeight && Grid->Tile[newY * SizeHeight + newX].content != BOMB)
                {
                    Grid->Tile[newY * SizeHeight + newX].content++;
                }
            }
        }
        AllMines[RandMine] = AllMines[CompteurTileDispo - 1];

        CompteurTileDispo -= 1;
    }
    free(AllMines);
    return NbBomb;
}

// Function to reaveal all the free tiles around
void RevealTile(Grid* Grid, int x, int y) {
    if (x < 0 || x >= Grid->Width || y < 0 || y >= Grid->Height)
        return;
    if (Grid->Tile[y * Grid->Width + x].hidden == 0)
        return;
    Grid->Tile[y * Grid->Width + x].hidden = 0;
    Grid->RemainingTiles -= 1;
    if (Grid->Tile[y * Grid->Width + x].content == 0)
    {
        for (int dx = -1; dx <= 1; dx++)
        {
            for (int dy = -1; dy <= 1; dy++)
            {
                RevealTile(Grid, x + dx, y + dy);
            }
        }
    }
}

// Function to check Unflag/Flag a tile
void CheckPossibility(Grid* Grid, int Flag, int ChoiceX, int ChoiceY, int* NbFlag)
{
    const Tile* tile = &Grid->Tile[ChoiceY * Grid->Height + ChoiceX];
    if (tile->Flag == 1 && Flag == 1)
    {
        Grid->Tile[ChoiceY * Grid->Height + ChoiceX].Flag = 0;
        printf("The tile is now Unflaged !\n");
        *NbFlag -= 1;
    }
    else if (tile->Flag == 0 && Flag == 1)
    {
        Grid->Tile[ChoiceY * Grid->Height + ChoiceX].Flag = 1;
        printf("The tile is now Flaged !\n");
        *NbFlag += 1;
    }
    else if (tile->hidden == 1)
    {
        RevealTile(Grid, ChoiceX, ChoiceY);
    }
}

// Check If player lost
int ItsLose(const Grid* Grid, int ChoiceX, int ChoiceY)
{
    if (Grid->Tile[ChoiceY * Grid->Height + ChoiceX].content == BOMB)
    {
        DisplayOpenedGrid(Grid->Height, Grid->Width, Grid);
        printf("You Lose !\n");
        return 1;
    }
    return 0;
}

// Check If player Win
int ItsWin(const Grid* Grid, int NbBomb)
{
    if (NbBomb == Grid->RemainingTiles)
    {
        //system("cls");
        DisplayPlayerGrid(Grid->Height, Grid->Width, Grid);
        printf("You Win ! \n");
        return 1;
    }
    return 0;
}
// Count how many hidden tiles around a tile
int CheckAroundNbHidden(Grid* Grid, int ChoiceX, int ChoiceY) {
    int nbHidden = 0;
    for (int dx = -1; dx <= 1; dx++)
    {
        for (int dy = -1; dy <= 1; dy++)
        {
            int newChoiceX = ChoiceX + dx;
            int newChoiceY = ChoiceY + dy;
            if (newChoiceX >= 0 && newChoiceX < Grid->Width && newChoiceY >= 0 && newChoiceY < Grid->Height)
            {
                if (Grid->Tile[newChoiceY * Grid->Width + newChoiceX].hidden == 1) {
                    nbHidden++;
                }
            }
        }
    }
    return nbHidden;
}
// Count how many Flag around a tile
int CheckAroundNbFlag(Grid* Grid, int ChoiceX, int ChoiceY) {
    int nbFlag = 0;
    for (int dx = -1; dx <= 1; dx++)
    {
        for (int dy = -1; dy <= 1; dy++)
        {
            int newChoiceX = ChoiceX + dx;
            int newChoiceY = ChoiceY + dy;
            if (newChoiceX >= 0 && newChoiceX < Grid->Width && newChoiceY >= 0 && newChoiceY < Grid->Height)
            {
                if (Grid->Tile[newChoiceY * Grid->Width + newChoiceX].Flag == 1) {
                    nbFlag++;
                }
            }
        }
    }
    return nbFlag;
}

// Put a flag around a tile
int PutFlagAround(Grid* Grid, int ChoiceX, int ChoiceY, int* pFlag) {
    int haveFlag = 0;
    for (int dx = -1; dx <= 1; dx++)
    {
        for (int dy = -1; dy <= 1; dy++)
        {
            int newChoiceX = ChoiceX + dx;
            int newChoiceY = ChoiceY + dy;
            // Out of range ?
            if (newChoiceX >= 0 && newChoiceX < Grid->Width && newChoiceY >= 0 && newChoiceY < Grid->Height) {
                //If the tile is hidden and get no Flag
                if (Grid->Tile[newChoiceY * Grid->Width + newChoiceX].hidden == 1 && Grid->Tile[newChoiceY * Grid->Width + newChoiceX].Flag == 0) {
                    Grid->Tile[newChoiceY * Grid->Width + newChoiceX].Flag = 1;
                    pFlag += 1;
                    haveFlag = 1;
                }
            }
        }
    }
    return haveFlag;
}

// Reveal Tile around a tile
int RevealTileBot(Grid* Grid, int x, int y) {
    int haveReveal = 0;
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            int newX = x + dx;
            int newY = y + dy;
            // Out of range ?
            if (newX >= 0 && newX < Grid->Width && newY >= 0 && newY < Grid->Height) {
                // If not a Flag and hidden
                if (Grid->Tile[newY * Grid->Width + newX].Flag != 1 && Grid->Tile[newY * Grid->Width + newX].hidden == 1) {
                    // if the content == 0 -> make reveal a multitude of tile
                    if (Grid->Tile[newY * Grid->Width + newX].content == 0) {
                        RevealTile(Grid, newX, newY);
                    }
                    else {
                        Grid->Tile[newY * Grid->Width + newX].hidden = 0;
                        Grid->RemainingTiles -= 1;
                        haveReveal = 1;
                    }
                }
            }
        }
    }
    return haveReveal;
}
//Reveal tile with the hypothesis
void RevealHypothesisTile(Grid* Grid, int x, int y) {
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            int newX = x + dx;
            int newY = y + dy;
            // Out of range ?
            if (newX >= 0 && newX < Grid->Width && newY >= 0 && newY < Grid->Height) {
                // If no Flag and nbHypothesis+1 = nbTilesRemain
                if (!(Grid->Suggestion[newY * Grid->Width + newX].nbHypothesis + 1 == Grid->Suggestion[newY * Grid->Width + newX].nbTilesRemain) && Grid->Tile[newY * Grid->Width + newX].Flag == 0&&Grid->Tile[newY * Grid->Width + newX].hidden == 1) {
                    // if the content == 0 -> make reveal a multitude of tile
                    if (Grid->Tile[newY * Grid->Width + newX].content == 0) {
                        RevealTile(Grid, newX, newY);
                    }
                    else {
                        Grid->Tile[newY * Grid->Width + newX].hidden = 0;
                        Grid->RemainingTiles -= 1;
                    }
                }
            }
        }
    }
}

// Add Hypothesis with the content Nb and Tiles Remain Nb
void AddHypothesis(Grid* Grid, int x, int y) {
    // Nb Tiles Remain around a tile
    int nbHidden = CheckAroundNbHidden(Grid, x, y) - CheckAroundNbFlag(Grid, x, y);
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            int newX = x + dx;
            int newY = y + dy;
            // No Flag
            if (Grid->Tile[newY * Grid->Width + newX].Flag == 0) {
                // Out of range ?
                if (newX >= 0 && newX < Grid->Width && newY >= 0 && newY < Grid->Height) {
                    // Hidden , content >= nbHypothesis
                    if (Grid->Tile[newY * Grid->Width + newX].hidden == 1) {
                        //nbHypothesis < (content or =0) && nbTilesRemain > (nbHidden or =0)
                        if ((Grid->Suggestion[newY * Grid->Width + newX].nbHypothesis < Grid->Tile[y * Grid->Width + x].content) || (Grid->Suggestion[newY * Grid->Width + newX].nbHypothesis == 0) && (Grid->Suggestion[newY * Grid->Width + newX].nbTilesRemain > nbHidden || Grid->Suggestion[newY * Grid->Width + newX].nbTilesRemain == 0)) {
                            Grid->Suggestion[newY * Grid->Width + newX].nbHypothesis = Grid->Tile[y * Grid->Width + x].content - CheckAroundNbFlag(Grid, x, y);
                            Grid->Suggestion[newY * Grid->Width + newX].nbTilesRemain = nbHidden;
                        }
                    }
                }
            }
        }
    }
}
// Check hypothesis for reveal
void CheckRevealHypothesis(Grid* Grid, int x, int y) {
    //count how many superior hypothesis
    int overHyp = 0;

    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            int newX = x + dx;
            int newY = y + dy;
            // if Hidden and nbHypothesis + 1 == nbTilesRemain
            if (Grid->Suggestion[newY * Grid->Width + newX].nbHypothesis + 1 == Grid->Suggestion[newY * Grid->Width + newX].nbTilesRemain && Grid->Tile[newY * Grid->Width + newX].hidden == 1) {
                overHyp++;
            }
            //if superior hypothesis == 2 -> reveal with hypothesis
            if (overHyp == 2) {
                RevealHypothesisTile(Grid, x, y);
            }
        }
    }
}
// reset Hypothesis grid at 0
void ResetHypo(Grid* Grid) {
    for (int y = 0; y < Grid->Width; y++) {
        for (int x = 0; x < Grid->Height; x++) {
            Grid->Suggestion[y * Grid->Width + x].nbHypothesis = 0;
            Grid->Suggestion[y * Grid->Width + x].nbTilesRemain = 0;
        }
    }
}
void GameAI(Grid* Grid, int NbBomb, int NbFlag, int ChoiceX, int ChoiceY, int Flag) {
    //while (NbBomb < Grid->RemainingTiles)
    for (int time = 0; time < 5; time++)
    {
        // Display the game
        int haveReveal = 0;
        int haveFlag = 0;
        DisplayOpenedGrid(Grid->Height, Grid->Width, Grid);
        printf("Mine remaning : %d \n", NbBomb - NbFlag);
        DisplayPlayerGrid(Grid->Height, Grid->Width, Grid);
        DisplayHypoGrid(Grid->Height, Grid->Width, Grid);

        // Put easy flag
        for (int y = 0; y < Grid->Width; y++) {
            for (int x = 0; x < Grid->Height; x++) {
                // No Hidden
                if (Grid->Tile[y * Grid->Width + x].hidden == 0) {
                    // content > 0
                    if (Grid->Tile[y * Grid->Width + x].content > 0) {
                        // content == nb Hidden around
                        if (CheckAroundNbHidden(Grid, x, y) == Grid->Tile[y * Grid->Width + x].content && Grid->Tile[y * Grid->Width + x].Flag == 0) {
                            haveFlag = PutFlagAround(Grid, x, y, &NbFlag);
                        }
                    }
                }
            }
        }
        // reveal easy tile
        for (int y = 0; y < Grid->Width; y++) {
            for (int x = 0; x < Grid->Height; x++) {
                if (Grid->Tile[y * Grid->Width + x].content == CheckAroundNbFlag(Grid, x, y) && Grid->Tile[y * Grid->Width + x].content != 0 && Grid->Tile[y * Grid->Width + x].hidden == 0) {
                    haveReveal = RevealTileBot(Grid, x, y);
                }
            }
        }
        ResetHypo(Grid);
        // if not easy tile or flag
        if (!haveReveal && !haveFlag) {
            for (int y = 0; y < Grid->Width; y++) {
                for (int x = 0; x < Grid->Height; x++) {
                    // No hidden and content != nb Hidden around
                    if (Grid->Tile[y * Grid->Width + x].hidden == 0 && Grid->Tile[y * Grid->Width + x].content != CheckAroundNbFlag(Grid, x, y)) {
                        AddHypothesis(Grid, x, y);
                    }
                }
            }
            for (int y = 0; y < Grid->Width; y++) {
                for (int x = 0; x < Grid->Height; x++) {
                    // No hidden, 1 bomb remaining, content != nb Flag
                    if (Grid->Tile[y * Grid->Width + x].hidden == 0 && Grid->Tile[y * Grid->Width + x].content - CheckAroundNbFlag(Grid, x, y) == 1 && Grid->Tile[y * Grid->Width + x].content != CheckAroundNbFlag(Grid, x, y)) {
                        CheckRevealHypothesis(Grid, x, y);
                    }
                }
            }
        }
        // If Win : YOU WIN
        if (ItsWin(Grid, NbBomb))
        {
            break;
        }
    }
}


int Game(Grid* Grid, int NbBomb, int NbFlag, int ChoiceX, int ChoiceY, int Flag)
{
    while (NbBomb < Grid->RemainingTiles)
    {
        // Clear the Console
        system("cls");
        // Display the game
        DisplayOpenedGrid(Grid->Height, Grid->Width, Grid);
        printf("Mine remaning : %d \n", NbBomb - NbFlag);
        DisplayPlayerGrid(Grid->Height, Grid->Width, Grid);
        // Ask action (choice and position)
        do
        {
            Flag = AskIntInRange("Do you want to put or remove a Flag ? => 1 | If you want to reveal a Tile => 0 : ", Flag, 0, 1);
            ChoiceX = AskIntInRange("Choose X : ", ChoiceX, 1, Grid->Width) - 1;
            ChoiceY = AskIntInRange("Choose Y : ", ChoiceY, 1, Grid->Height) - 1;
        } while (AskChoices(Flag, ChoiceX, ChoiceY, Grid) == 0);
        printf("problème");
        // Check possibility
        CheckPossibility(Grid, Flag, ChoiceX, ChoiceY, &NbFlag);
        printf("problème");
        // If Lose : YOU LOSE
        if (Flag == 0) {
            if (ItsLose(Grid, ChoiceX, ChoiceY)) {
                break;
            }
        }
        printf("problème");
        // If Win : YOU WIN
        if (ItsWin(Grid, NbBomb))
        {
            break;
        }
    }
}

// Main
int main()
{
    while (1)
    {
        system("cls");
        char Yes[1] = { 'Y' };
        int ChoiceX = 0, ChoiceY = 0, Flag = 0, NbFlag = 0, SizeHeight = 0, SizeWidth = 0;

        // Define the percentage of mine in the Grid
        float PercentOfMine = 0.2;

        // Ask an Int for the scall of the Grid
        SizeWidth = AskIntInRange("Choose the Width of the Grid : ", SizeWidth, 1, 50);
        SizeHeight = AskIntInRange("Choose the Height of the Grid : ", SizeHeight, 1, 50);

        // Generate tile with the chosen Grid size
        Tile* AllTiles = (Tile*)malloc(sizeof(Tile) * SizeHeight * SizeWidth);
        if (AllTiles == NULL)
        {
            exit(1);
        }
        Suggestion* AllSuggestion = (Suggestion*)malloc(sizeof(Suggestion) * SizeHeight * SizeWidth);
        if (AllTiles == NULL)
        {
            exit(1);
        }
        srand(time(NULL));

        // Generate Grid
        Grid Grid;
        Grid.Height = SizeHeight;
        Grid.Width = SizeWidth;
        Grid.Tile = AllTiles;
        Grid.Suggestion = AllSuggestion;
        Grid.RemainingTiles = SizeHeight * SizeWidth;
        InitiateDefaultTiles(&Grid, SizeHeight, SizeWidth);

        // Grid for Dev
        DisplayOpenedGrid(Grid.Height, Grid.Width, &Grid);
        // Display
        DisplayPlayerGrid(Grid.Height, Grid.Width, &Grid);

        // Player or IA :
        int Player = 0;
        Player = AskIntInRange("Do you want to play or let the AI play for you : 0 | 1 : ", Player, 0, 1);

        if (!Player) { //Player
            // First move and Generate Mine
            ChoiceX = AskIntInRange("Choose X : ", ChoiceX, 1, Grid.Width) - 1;
            ChoiceY = AskIntInRange("Choose Y : ", ChoiceY, 1, Grid.Height) - 1;
            int NbBomb = CreateMine(&Grid, Grid.Height, Grid.Width, ChoiceX, ChoiceY, PercentOfMine);
            RevealTile(&Grid, ChoiceX, ChoiceY);

            // Game Function
            Game(&Grid, NbBomb, NbFlag, ChoiceX, ChoiceY, Flag);

            free(AllTiles);

            // ReStart
            if (!CompareWithArray("Do you want play again ?\n(YES --> 'y' // NO --> Other) : \n", Yes, 1))
            {
                printf("THE END");
                break;
            }
        }
        else { //IA

            // Unlock the tile in the middle for the first move
            ChoiceX = floor(Grid.Width / 2);
            ChoiceY = floor(Grid.Height / 2);
            int NbBomb = CreateMine(&Grid, Grid.Height, Grid.Width, ChoiceX, ChoiceY, PercentOfMine);
            RevealTile(&Grid, ChoiceX, ChoiceY);
            GameAI(&Grid, NbBomb, NbFlag, ChoiceX, ChoiceY, Flag);
            free(AllTiles);

            // ReStart
            if (!CompareWithArray("Do you want play again ?\n(YES --> 'y' // NO --> Other) : \n", Yes, 1))
            {
                printf("THE END");
                break;
            }

        }
    }
    return 0;
}


// Idée en tete / Chose a finir
//  LISIBILITE
//  Bot qui jous au jeux
//  possibilité de changer la difficulté ? 
//  Optimiser
//  mettre des couleurs



//Hypothèse dans grid
//Mettre pour chaque case des hypothèses avec le nb restant de bombe autour
//Mettre pour chaque case le nombre de case restante
//remplacer les hypothèse faible par les plus forte
//Si une tile possède 2 (hypothèses + 1 == remaining tile) autour alors reveal les autres 
