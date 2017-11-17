/*
 * HillClimb.cpp
 *
 *  Created on: 19-Oct-2017
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
	int heuristicCost; // heuristic value of this node's state.
	stateNode* bestNeighbour; // the best neighbour of this particular state.
	int* queenIndexes; // array of queen positions in each column.
} stateNode;

/*
 * Hill climb class contains information about
 * the board size, in other words total number of queens
 * to solve this problem for. It stores the current state of the board
 * configuration with all queens occupying a particular
 * position in each column.
 */
class HillClimb {

private:
	int boardSize; // board size, or N size
	stateNode* currentState; // current state of board configuration.
	int totalRestarts; // total number of restarts made to get to solution.
	int totalStateChanges; // total number of state changes or steps.

public:
	HillClimb(int n) {
		boardSize = n;
		totalRestarts = 0;
		totalStateChanges = 0;
	}
	void setInitialState();
	void printSolution(stateNode*);
	int getHeuristicCost(stateNode*);
	stateNode* generateBestNeighbour(stateNode*);
	void startHillClimbSearch();
	void randomRestart();
	stateNode* randomQueenPlacement();
};

/*
 * Function to randomly place queen to different locations within each column.
 * Called after no better neighbour has been found of a particular state.
 */
void HillClimb::randomRestart() {
	currentState = randomQueenPlacement();
	currentState->heuristicCost = getHeuristicCost(currentState);
}

/*
 * Function to print solution using the state node passed as an argument.
 */
void HillClimb::printSolution(stateNode* solution) {
	if (solution == NULL) {
		cout << "No solution found !!!";
		return;
	}
	cout << endl;
	for (int i = 0; i < boardSize; i++) {
		for (int j = 0; j < boardSize; j++) {
			cout << solution->state[i][j] << " ";
		}
		cout << endl;
	}
	cout<<endl<<"Total restarts made : "<<totalRestarts<<endl;
	cout<<"Total state changes : "<<totalStateChanges<<endl;
}

/*
 * Function implementing the Hill Climb Search algorithm with random restart to avoid
 * getting stuck in local maxima.
 */
void HillClimb::startHillClimbSearch() {
	while (true) {
		if (currentState->heuristicCost == 0) {
			printSolution(currentState);
			break;
		} else {
			currentState->bestNeighbour = generateBestNeighbour(currentState);
			totalStateChanges++;
			if (currentState->bestNeighbour != NULL)
				currentState = currentState->bestNeighbour;
			else {
				randomRestart();
				totalRestarts++;
			}
		}
	}
}

/*
 * Function to generate best neighbour of the given node passed as an argument.
 * If no best neighbour is found, the function returns NULL.
 */
stateNode* HillClimb::generateBestNeighbour(stateNode* node) {

	int currHeuristic = node->heuristicCost;
	stateNode* bestNeighbour = NULL;
	for (int i = 0; i < boardSize; i++) { //// for each column.
		for (int j = 0; j < boardSize; j++) { //// for each row of a particular column.
			if (j == node->queenIndexes[i]) //// skip if the queen is in the same position as the original node.
				continue;
			stateNode* neighbour = new stateNode();
			int** tempState = new int*[boardSize];
			int* tempQueenIndexes = new int[boardSize];
			for (int k = 0; k < boardSize; k++) {
				int* temp = new int[boardSize];
				memcpy(temp, node->state[k], boardSize * sizeof(int));
				tempState[k] = &temp[0];
			}
			memcpy(tempQueenIndexes, node->queenIndexes,
					boardSize * sizeof(int));
			tempState[node->queenIndexes[i]][i] = 0;
			tempState[j][i] = 1;
			tempQueenIndexes[i] = j;
			neighbour->state = tempState;
			neighbour->queenIndexes = tempQueenIndexes;
			neighbour->heuristicCost = getHeuristicCost(neighbour);
			if (neighbour->heuristicCost < currHeuristic) {
				bestNeighbour = neighbour;
				currHeuristic = neighbour->heuristicCost;
			}
		}
	}
	return bestNeighbour;
}

/*
 * Function to compute the heuristic cost of the current state of the node
 * passed as an argument. Heuristic cost is calculated based on the total
 * number of queens in conflicting position.
 */
int HillClimb::getHeuristicCost(stateNode* node) {
	int cost = 0;
	for (int k = 0; k < boardSize - 1; k++) {
		int i = node->queenIndexes[k];
		int j = k;
		while (i > 0 && j < (boardSize - 1)) { // diagonal up
			i--;
			j++;
			if (node->state[i][j] == 1)
				cost++;
		}
		i = node->queenIndexes[k];
		j = k;
		while (i < (boardSize - 1) && j < (boardSize - 1)) { // diagonal down
			i++;
			j++;
			if (node->state[i][j] == 1)
				cost++;
		}
		i = node->queenIndexes[k];
		j = k;
		while (j < (boardSize - 1)) { // right
			j++;
			if (node->state[i][j] == 1)
				cost++;
		}
	}
	return cost;
}

/*
 * Function to set the initial state by randomly placing each queen
 * in a particular column with no two queens occupying the same column.
 */
void HillClimb::setInitialState() {
	currentState = randomQueenPlacement();
	currentState->heuristicCost = getHeuristicCost(currentState);
}

/*
 * Function to randomly place each queen in a particular column
 * with no two queens in the same column.
 */
stateNode* HillClimb::randomQueenPlacement() {
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
	cout << "Enter total number of queens(N) :  (Between - 1 to 40)" << endl;
	cout << "Note : It takes around 20-25 seconds to generate solution for N~40"
			<< endl;
	cin >> boardSize;
	clock_t start = clock();
	if (boardSize < 1) {
		cout << "Input too small !!!" << endl;
	} else if (boardSize > 40) {
		cout << "Input too large !!!" << endl;
	} else if (boardSize == 1) {
		cout << "1" << endl;
	} else if (boardSize <= 3) {
		cout << "No solution found !!!" << endl;
	} else {
		HillClimb hillClimb(boardSize);
		hillClimb.setInitialState();
		hillClimb.startHillClimbSearch();
	}
	clock_t finish = clock();
	cout << "Total running time  : "
			<< (float(finish - start) / CLOCKS_PER_SEC) << " secs";
	getch();
	return 0;
}

