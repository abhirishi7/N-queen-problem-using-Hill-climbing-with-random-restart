/*
 * QueenMinConflictCopy.cpp
 * Program to implement Min conflict to solve n-queen problem
 *  Created on: 23-Oct-2017
 *      Author: Abhishek Tripathi
 *  Student ID: 801010002
 *
 */

#include<iostream>
#include<conio.h>
#include<time.h>

using namespace std;

typedef struct stateNode { // structure definition of a state node
	int** state; // matrix representation of a node's state.
	stateNode* bestNeighbour; // the best neighbour of this particular state.
	int* queenIndexes; // array of queen positions in each column.
	int conflicts; // total number of queens in conflict with other queens.
	int heuristicCost; // heuristic value of this node's state.
} stateNode;

/*
 * Min conflict class contains information about
 * the board size, in other words total number of queens
 * to solve this problem for. It stores the current state of the board
 * configuration with all queens occupying a particular
 * position in each column.
 */
class MinConflict {

private:
	int boardSize; // board size, or N size
	stateNode* currentState; // current state of board configuration.
	int totalRestarts; // total number of restarts made to get to solution.
	int totalStateChanges; // total number of state changes or steps.
	int totalConflicts; // total number of conflicts in current configuration.

public:
	MinConflict(int n) {
		boardSize = n;
		totalRestarts = 0;
		totalStateChanges = 0;
	}
	void setInitialState();
	void printSolution(stateNode*);
	void startMinConflictSearch();
	stateNode* randomQueenPlacement();
	int getMostConflictingQueenIndex(stateNode*);
	stateNode* moveQueenToLeastConflict(int);
	int getTotalConflicts(int, stateNode*);
};

/*
 * Function moves the queen in the column indexed by the argument passed
 * to the position with the least conflicting state.
 */
stateNode* MinConflict::moveQueenToLeastConflict(int queenCol) {
	int totalMinConflictsPositions = 0;
	long currMinConflicts = LONG_MAX;
	stateNode** leastConflictingNodes = new stateNode*[boardSize];
	stateNode* neighbour;
	for (int i = 0; i < boardSize; i++) {
		neighbour = new stateNode();
		int** tempState = new int*[boardSize];
		int* tempQueenIndexes = new int[boardSize];
		for (int k = 0; k < boardSize; k++) {
			int* temp = new int[boardSize];
			memcpy(temp, currentState->state[k], boardSize * sizeof(int));
			tempState[k] = &temp[0];
		}
		memcpy(tempQueenIndexes, currentState->queenIndexes,
				boardSize * sizeof(int));

		int queenIndex = currentState->queenIndexes[queenCol];
		tempState[queenIndex][queenCol] = 0;
		tempState[i][queenCol] = 1;
		tempQueenIndexes[queenCol] = i;
		neighbour->state = tempState;
		neighbour->queenIndexes = tempQueenIndexes;
		int totalConflicts = getTotalConflicts(queenCol, neighbour);
		if (totalConflicts < currMinConflicts) {
			totalMinConflictsPositions = 0;
			currMinConflicts = totalConflicts;
			leastConflictingNodes[totalMinConflictsPositions] = neighbour;
			totalMinConflictsPositions++;
		} else if (totalConflicts == currMinConflicts) {
			leastConflictingNodes[totalMinConflictsPositions] = neighbour;
			totalMinConflictsPositions++;
		}
	}
	return leastConflictingNodes[rand() % totalMinConflictsPositions];
}

/*
 * Returns the total number of queens the current queen is in conflict with.
 */
int MinConflict::getTotalConflicts(int queenCol, stateNode* node) {
	int totalConflicts = 0;
	int i = node->queenIndexes[queenCol];
	int j = queenCol;
	while (i > 0 && j < (boardSize - 1)) { // diagonal up right
		i--;
		j++;
		if (node->state[i][j] == 1)
			totalConflicts++;
	}
	i = node->queenIndexes[queenCol];
	j = queenCol;
	while (i < (boardSize - 1) && j < (boardSize - 1)) { // diagonal down right
		i++;
		j++;
		if (node->state[i][j] == 1)
			totalConflicts++;
	}
	i = node->queenIndexes[queenCol];
	j = queenCol;
	while (j < (boardSize - 1)) { // right
		j++;
		if (node->state[i][j] == 1)
			totalConflicts++;
	}
	i = node->queenIndexes[queenCol];
	j = queenCol;
	while (j > 0) { // left
		j--;
		if (node->state[i][j] == 1)
			totalConflicts++;
	}
	i = node->queenIndexes[queenCol];
	j = queenCol;
	while (i < (boardSize - 1) && j > 0) { // diagonal down left
		i++;
		j--;
		if (node->state[i][j] == 1)
			totalConflicts++;
	}
	i = node->queenIndexes[queenCol];
	j = queenCol;
	while (i > 0 && j > 0) { // diagonal up left
		i--;
		j--;
		if (node->state[i][j] == 1)
			totalConflicts++;
	}
	return totalConflicts;
}

/*
 * Functions returns the column index of the queen that is in
 * conflict with most of the other queens.
 */
int MinConflict::getMostConflictingQueenIndex(stateNode* node) {

	int totalMaxConflictQueens = 0;
	int* conflictingQueens = new int[boardSize];
	int maxConflictIndex;
	int currentMaxConflict = 0;
	for (int col = 0; col < boardSize; col++) {
		int totalConflicts = getTotalConflicts(col, node);
		if (totalConflicts > currentMaxConflict) {
			this->totalConflicts = totalConflicts;
			totalMaxConflictQueens = 0;
			currentMaxConflict = totalConflicts;
			conflictingQueens[totalMaxConflictQueens] = col;
			totalMaxConflictQueens++;
		} else if (totalConflicts == currentMaxConflict) {
			this->totalConflicts = totalConflicts;
			conflictingQueens[totalMaxConflictQueens] = col;
			totalMaxConflictQueens++;
		}
	}
	maxConflictIndex = conflictingQueens[rand() % totalMaxConflictQueens];
	return maxConflictIndex;
}

/*
 * Function to print solution using the state node passed as an argument.
 */
void MinConflict::printSolution(stateNode* solution) {
	if (solution == NULL) {
		cout << "No solution found !!!";
		return;
	}
	cout << endl;
	if (boardSize <= 1000)
		for (int i = 0; i < boardSize; i++) {
			for (int j = 0; j < boardSize; j++) {
				if (solution->state[i][j] > 1)
					solution->state[i][j] = 0;
				cout << solution->state[i][j] << " ";
			}
			cout << endl;
		}
	else
		cout << "Solution found" << endl;

	cout << endl << "Total restarts made : " << totalRestarts << endl;
	cout << "Total state changes : " << totalStateChanges << endl;
}

/*
 * Function implementing the Min conflict Search algorithm with random restart to avoid
 * getting stuck.
 */
void MinConflict::startMinConflictSearch() {
	int totalMoves = 0;
	while (true) {
		if (totalConflicts == 0 ) {
			printSolution(currentState);
			break;
		} else {
			if (totalMoves == (boardSize * 3)) {
				setInitialState(); // random restart
				totalRestarts++;
				totalMoves = 0;
				continue;
			}
			int queenColIndex = getMostConflictingQueenIndex(currentState);
			currentState = moveQueenToLeastConflict(queenColIndex);
			totalStateChanges++;
			totalMoves++;
		}
	}
}

/*
 * Function to set the initial state by randomly placing each queen
 * in a particular column with no two queens occupying the same column.
 */
void MinConflict::setInitialState() {
	currentState = randomQueenPlacement();
	totalConflicts = 1;
}

/*
 * Function to randomly place each queen in a particular column
 * with no two queens in the same column.
 */
stateNode* MinConflict::randomQueenPlacement() {
	stateNode* node = new stateNode();
	int* queenIndexes = new int[boardSize];
	int** state = new int*[boardSize];
	for (int i = 0; i < boardSize; i++) {
		int* row = new int[boardSize];
		int randomIndex = rand() % boardSize;
		queenIndexes[i] = randomIndex;
		for (int j = 0; j < boardSize; j++) {
			row[j] = 0;
		}
		state[i] = &row[0];
	}
	for (int i = 0; i < boardSize; i++) {
		state[queenIndexes[i]][i] = 1;
	}
	node->state = state;
	node->queenIndexes = queenIndexes;
	return node;
}

/*
 * Driver function.
 */
int main() {
	int boardSize;
	cout << "Enter total number of queens(N) :  (Between - 1 to 100)" << endl;
	cin >> boardSize;
	clock_t start = clock();
	if (boardSize < 1) {
		cout << "Input too small !!!" << endl;
	} else if (boardSize > 100) {
		cout << "Input too large !!!" << endl;
	} else if (boardSize == 1) {
		cout << "1" << endl;
	} else if (boardSize <= 3) {
		cout << "No solution found !!!" << endl;
	} else {
		MinConflict minConflict(boardSize);
		minConflict.setInitialState();
		minConflict.startMinConflictSearch();
	}
	clock_t finish = clock();
	cout << "Total running time  : " << (float(finish - start) / CLOCKS_PER_SEC)
			<< " secs";
	getch();
	return 0;
}

