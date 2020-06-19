#include <iostream>
#include <string>
#include <vector>
#include <climits>
using namespace std;

typedef vector<bool> vb;
typedef vector<char> vc;
typedef vector<vb> vvb;
typedef vector<vc> vvc;
typedef pair<int, int> pii;
typedef vector<pii> vpii;
typedef pair<pii, int> ppiii;
typedef vector<ppiii> vppiii;

vvc field;
int limit;

bool fieldFilled(){
  // This function is used to check to see if the board is filled or not.
  for(int i = 0; i < 6; i++){
    for(int j = 0; j < 7; j++){
      if(field[i][j] == '_'){
        return false;
      }
    }
  }
  return true;
}

pii chains(char color){
  /*
    This function is used to find the number of chains of 2 and 3 a specific color has at the time.
    This function uses DFS and is the base of our heuristic.
   */
  vvb visited; visited.resize(6, vb (7, false));
  int lentwo = 0, lenthree = 0;
  for(int i = 0; i < 6; i++){
    for(int j = 0; j < 7; j++){
      if(field[i][j] == color && !visited[i][j]){
        vppiii aStack;
        visited[i][j] = true;
        aStack.push_back(make_pair(make_pair(i, j), 1));
        while(aStack.size() != 0){
          ppiii bP;
          bP = aStack[aStack.size() - 1]; aStack.pop_back();
          int curri = bP.first.first, currj = bP.first.second, counter = bP.second;
          visited[curri][currj] = true;
          if(counter == 2) lentwo++;
          if(counter == 3) lentwo--; lenthree++;
          int values[4] = {1, -1, 0, 0};
          for(int x = 0; x < 4; x++){
            for(int y = 0; y < 4; y++){
              int tempi = curri + values[x], tempj = currj + values[y];
              pii aP (tempi, tempj);
              if(0 <= tempi && tempi <= 5 && 0 <= tempj && tempj <= 6 && field[tempi][tempj] == color && !visited[tempi][tempj]) aStack.push_back(make_pair(aP, curri + 1));
            }
          }
        }
      }
    }
  }
  return make_pair(lentwo, lenthree);
}


int rowWinner(){
  // Checks the rows to see if there is a winner.
  for(int i = 0; i < 6; i++){
    for(int j = 0; j < 4; j++){
      if(vc (field[i].begin() + j, field[i].begin() + j + 4) == vc (4, 'R') || vc (field[i].begin() + j, field[i].begin() + j + 4) == vc (4, 'Y')) return (field[i][j] == 'R') ? 1000 : -1000;
    }
  }
  return 0;
}

int columnWinner(){
  // Checks the columns to see if there is a winner.
  for(int j = 0; j < 7; j++){
    vc zeroV, oneV, twoV;
    for(int i = 0; i < 4; i++){
      zeroV.push_back(field[i][j]);
      oneV.push_back(field[i + 1][j]);
      twoV.push_back(field[i + 2][j]);
    }
    if(zeroV == vc (4, 'R') || zeroV == vc (4, 'Y')) return (zeroV[0] == 'R') ? 1000 : -1000;
    if(oneV == vc (4, 'R') || oneV == vc (4, 'Y')) return (oneV[0] == 'R') ? 1000 : -1000;
    if(twoV == vc (4, 'R') || twoV == vc (4, 'Y')) return (twoV[0] == 'R') ? 1000 : -1000;
  }
  return 0;
}

int diagonalWinner(){
  // Checks the diagonals to see if there is a winner.
  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 4; j++){
      vc L2R, R2L;
      L2R.push_back(field[i][j]); L2R.push_back(field[i + 1][j + 1]); L2R.push_back(field[i + 2][j + 2]); L2R.push_back(field[i + 3][j + 3]);
      R2L.push_back(field[i][6 - j]); R2L.push_back(field[i + 1][5 - j]); R2L.push_back(field[i + 2][4 - j]); R2L.push_back(field[i + 3][3 - j]);
      if(L2R == vc (4, 'R') || L2R == vc (4, 'Y')) return (L2R[0] == 'R') ? 1000 : -1000;
      if(R2L == vc (4, 'R') || R2L == vc (4, 'Y')) return (R2L[0] == 'R') ? 1000 : -1000;
    }
  }
  return 0;
}

int wins(){
  // Uses the three winner functions to see if there is a winner.
  int row = rowWinner();
  if(row != 0) return row;
  int col = columnWinner();
  if (col != 0) return col;
  int dig = diagonalWinner();
  if (dig != 0) return dig;
  return 0;
}

void printField(){
  // Display the board to the user (called after every move).
  cout << "\nThe field is now:\n\n";
  for(int i = 0; i < field.size(); i++){
    for(int j = 0; j < field[0].size(); j++){
      cout << field[i][j] << ' ';
    }
    cout << "\n";
  }
  cout << '\n';
}

void mark_column(int col, int player){
  // Places a chip in the specified column with the player's (user or AI) color.
  for(int i = 5; i >= 0; i--){
    if(field[i][col] == '_') {field[i][col] = (player == 1000) ? 'R' : 'Y'; break;}
  }
  // printField(); I had this piece of code here to display the process that the AI undergoes
}

void unmark_column(int col){
  // Remove a chip from the top of a specified column. Crucial to our minimax.
  for(int i = 0; i < 6; i++){
    if(field[i][col] != '_'){ field[i][col] = '_'; break;}
  }
}

void playerChoice(){
  // Get the user's move (basically the column where he places his chip) and call the mark function.
  while(true){
    cout << "Make your move:\n  Column: ";
    int col;
    cin >> col; col--;
    if(field[0][col] == '_'){
      mark_column(col, -1000);
      break;
    }
    cout << "\nSorry, but that's a taken position. Please try again.\n\n";
  }
}

int chaining(){
  /*
  Calls the previous chains function to get the players' "scores" / chance of winning.
  Returns the AI's chance of winning.
  */
  pii redChains = chains('R');
  pii yellowChains = chains('Y');
  int red_value = (redChains.first * 4) + (redChains.second * 9);
  int yellow_value = (yellowChains.first * 4) + (yellowChains.second * 9);
  return red_value - yellow_value;
}

int minimax(int depth, bool maximizing){
  /*
   The bread and butter of our program.
   Tries and foresees every possible outcome for "depth" turns.
   Then, it returns the best option.
   */
  int result = wins();
  if(result != 0 || fieldFilled() || depth == limit){
    if(result != 0) return result;
    if(fieldFilled()) return result;
    if(depth == limit){ int chance = chaining(); return chance;}
  }
  if(maximizing){
    int best = INT_MIN;
    for(int j = 0; j < 7; j++){
      if(field[0][j] == '_'){
        mark_column(j, 1000);
        int score = minimax(depth + 1, false);
        unmark_column(j);
        best = max(score, best);
      }
    }
    return best;
  }
  else{
    int best = INT_MAX;
    for(int j = 0; j < 7; j++){
      if(field[0][j] == '_'){
        mark_column(j, -1000);
        int score = minimax(depth + 1, true);
        unmark_column(j);
        best = min(score, best);
      }
    }
    return best;
  }
}


void computerChoice(){
  // Calls minimax to get the best move.
  int bestMove;
    int best = INT_MIN;
    for(int j = 0; j < 7; j++){
      if(field[0][j] == '_'){
        mark_column(j, 1000);
        int score = minimax(0, false);
        unmark_column(j);
        if(score > best){
          best = score;
          bestMove = j;
        }
      }
    }
  mark_column(bestMove, 1000);
}


int main(){
  field.resize(6, vc(7, '_')); // Create the board
  cout << "\nWelcome to Connect Four with AI! I, the computer, will be playing you in Connect Four. I am Red (R), and you are Yellow (Y). Made by Lance Bae '23.\n\nDifficulty? (1-4) "; // Welcome the user
  cin >> limit; 
  while(limit < 1 || limit > 4){
    cout << "\nPlease enter a difficulty level 1-4\n";
    cin >> limit;
  }
  char answer; cout << "Would you like to go first? (y/n) "; cin >> answer;
  if(answer == 'n') computerChoice();
  printField();
  int winner = 0;
  while(true){
    playerChoice();
    printField();
    if(wins()){ winner = -1000; break;}
    if(fieldFilled()) break;
    cout << "Please wait..." << '\n';
    computerChoice();
    printField();
    if(wins()){ winner = 1000; break;}
    if(fieldFilled()) break;
  }
  if(winner == 1000) cout << "The computer won!\n";
  else if(winner == -1000) cout << "You won!\n";
  else if(winner == 0) cout << "Tie!\n";
}
