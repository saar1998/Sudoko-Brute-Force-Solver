#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <set>
#include <algorithm>
#include <iterator>
#include "Sudoko_Brute_Force.h"

using namespace std;

struct coordinate {
	int x, y;
};

int insertCoordinateToArray(coordinate* arr, int x, int y, int insertionCounter) {
	arr[insertionCounter] = { x, y };
	return insertionCounter + 1;
}

bool operator == (const coordinate& a, const coordinate& b) {
	return a.x == b.x && a.y == b.y;
}

template <typename T>
bool checkIfListContainsItem(list<T> listToCheck, T item) {
	typename list<T>::iterator it;
	it = std::find(listToCheck.begin(), listToCheck.end(), item);
	return it != listToCheck.end();
}

coordinate* getNeighborRow(coordinate place, bool selfIncluded) {
	int neighborsNum = selfIncluded ? 9 : 8,
		insertionCounter = 0;
	coordinate* neighborRow = new coordinate[neighborsNum];

	for (int i = 0; i < 9; i++) {
		if (place.y != i || selfIncluded) {
			insertionCounter = insertCoordinateToArray(neighborRow, place.x, i, insertionCounter);
		}
	}
	return neighborRow;
}

coordinate* getNeighborRow(coordinate place) {
	return getNeighborRow(place, false);
}


coordinate* getNeighborColumn(coordinate place, bool selfIncluded) {
	int neighborsNum = selfIncluded ? 9 : 8,
		insertionCounter = 0;
	coordinate* neighborColumn = new coordinate[neighborsNum];

	for (int i = 0; i < 9; i++) {
		if (place.x != i || selfIncluded) {
			insertionCounter = insertCoordinateToArray(neighborColumn, i, place.y, insertionCounter);
		}
	}
	return neighborColumn;
}

coordinate* getNeighborColumn(coordinate place) {
	return getNeighborColumn(place, false);
}

coordinate* getNeighborSquare(coordinate place, bool selfIncluded) {
	int neighborsNum = selfIncluded ? 9 : 8,
		insertionCounter = 0,
		bottomX = (place.x / 3) * 3,
		bottomY = (place.y / 3) * 3;
	coordinate* neighborSquare = new coordinate[neighborsNum];

	for (int i = bottomX; i < bottomX + 3; i++) {
		for (int j = bottomY; j < bottomY + 3; j++) {
			if (i != place.x || j != place.y || selfIncluded) {
				insertionCounter = insertCoordinateToArray(neighborSquare, i, j, insertionCounter);
			}
		}
	}
	return neighborSquare;
}

coordinate* getNeighborSquare(coordinate place) {
	return getNeighborSquare(place, false);
}

coordinate* getFilteredNeighborSquare(coordinate place) {
	int insertionCounter = 0,
		bottomX = (place.x / 3) * 3,
		bottomY = (place.y / 3) * 3;
	coordinate* neighborSquare = new coordinate[4];

	for (int i = bottomX; i < bottomX + 3; i++) {
		for (int j = bottomY; j < bottomY + 3; j++) {
			if (i != place.x && j != place.y) {
				insertionCounter = insertCoordinateToArray(neighborSquare, i, j, insertionCounter);
			}
		}
	}
	return neighborSquare;
}

coordinate* getNeighbors(coordinate place, bool selfIncluded) {
	int neighborsNum = selfIncluded ? 21 : 20,
		i = 0;
	coordinate* neighbors = new coordinate[neighborsNum];
	coordinate* neighborRow = getNeighborRow(place, selfIncluded);
	coordinate* neighborColumn = getNeighborColumn(place);
	coordinate* neighborSquare = getFilteredNeighborSquare(place);
	for (; i < 4; i++) {
		neighbors[i] = neighborSquare[i];
	}
	for (; i < 12; i++) {
		neighbors[i] = neighborColumn[i - 4];
	}
	for (; i < neighborsNum; i++) {
		neighbors[i] = neighborRow[i - 12];
	}

	return neighbors;
}

coordinate* getNeighbors(coordinate place) {
	return getNeighbors(place, false);
}

class BoardDescription {
	private: 
		void removePossibilityFromCoordinate(coordinate cor, int possibility) {
			possibilitiesArray[cor.x][cor.y].remove(possibility);
		}

		void updateNeighborsOfChange(coordinate currentCoordinate) {
			int content = getCellContent(currentCoordinate);
			coordinate* neighbors = getNeighbors(currentCoordinate);
			for (int i = 0; i < 20; i++) {
				coordinate neighbor = neighbors[i];
				removePossibilityFromCoordinate(neighbor, content);
			}
		}

		void basicUpdatePossibilities(coordinate place) {
			coordinate* coordianates = getNeighbors(place);
			for (int i = 0; i < 20; i++) {
				removePossibilityFromCoordinate(place, getCellContent(coordianates[i]));
			}
		}

		void updatePossibilitieArrays() {
			for (int i = 0; i < 9; i++) {
				for (int j = 0; j < 9; j++) {
					coordinate currentCoordinate{ i, j };
					if (getCellContent(currentCoordinate) == 0) {
						basicUpdatePossibilities(currentCoordinate);
					}
				}
			}
		}

		bool updateBoardByPossibilties(bool lastUpdated) {
			bool updated = false;
			updatePossibilitieArrays();
			for (int i = 0; i < 9; i++) {
				for (int j = 0; j < 9; j++) {
					if (getCellContent(coordinate{ i, j }) == 0 && possibilitiesArray[i][j].size() == 1) {
						updated = true;
						insertToCoordinate(coordinate{ i, j }, possibilitiesArray[i][j].front());
					}
				}
			}
			if (updated) {
				return updateBoardByPossibilties(true);
			}
			return lastUpdated;
		}

		bool updateBoardByPossibilties() {
			return updateBoardByPossibilties(false);
		}

		list<coordinate>* getNumbersPossibleOccurences(coordinate* neighbors) {
			list<coordinate>* possibleOccurenceList = new list<coordinate>[9];
			for (int i = 0; i < 9; i++) {
				for (int j = 0; j < 9; j++) {
					if (getCellContent(neighbors[j]) == 0) {
						if (checkIfListContainsItem<int>(getPossibilitiesByCoordinate(neighbors[j]), i + 1)) {
							possibleOccurenceList[i].push_back(neighbors[j]);
						}
					}
				}
			}
			return possibleOccurenceList;
		}

		bool updateNeighborsByPossibleOccurenes(coordinate* neighbors) {
			bool updated = false;
			list<coordinate>* list = getNumbersPossibleOccurences(neighbors);
			for (int i = 0; i < 9; i++) {
				if (list[i].size() == 1) {
					coordinate toPlace = list[i].back();
					insertToCoordinate(toPlace, i + 1);
					updated = true;
				}
			}
			return updated;
		}

		coordinate* getNeighborsByType(coordinate cor, string type, bool selfIncluded) {
			if (type == "column") {
				return getNeighborColumn(cor, true);
			}
			if (type == "row") {
				return getNeighborRow(cor, true);
			}
			return getNeighborSquare(cor, true);
		}

		bool updateBoardBySpecificPossibleOccurences(string neighborsType, bool lastUpdated) {
			bool updated = false;
			coordinate* neighbors;
			if (neighborsType == "column") {
				for (int m = 0; m < 9; m++) {
					neighbors = getNeighborsByType(coordinate{ 0, m }, neighborsType, true);
					updated = updateNeighborsByPossibleOccurenes(neighbors) || updated;
				}
			}
			else if (neighborsType == "row") {
				for (int m = 0; m < 9; m++) {
					neighbors = getNeighborsByType(coordinate{ m, 0 }, neighborsType, true);
					updated = updateNeighborsByPossibleOccurenes(neighbors) || updated;
				}
			}
			else {
				for (int m = 0; m < 9; m++) {
					neighbors = getNeighborsByType(coordinate{ (m % 3) * 3 , (m / 3) * 3 }, neighborsType, true);
					updated = updateNeighborsByPossibleOccurenes(neighbors) || updated;
				}
			}
			if (updated) {
				return updateBoardBySpecificPossibleOccurences(neighborsType, true);
			}
			return lastUpdated;;
		}

		bool updateBoardByAllPossibleOccurences(bool lastUpdated) {
			updatePossibilitieArrays();
			bool updatedColumns = updateBoardBySpecificPossibleOccurences("column", false);
			bool updatedRows = updateBoardBySpecificPossibleOccurences("row", false);
			bool updatedSquares = updateBoardBySpecificPossibleOccurences("square", false);
			if (updatedColumns || updatedRows || updatedSquares) {
				return updateBoardByAllPossibleOccurences(true);
			}
			return lastUpdated;
		}

		bool updateBoardByAllPossibleOccurences() {
			return updateBoardByAllPossibleOccurences(false);
		}


	public: 
		list<int> possibilitiesArray[9][9];
		vector<vector<int>> board;

		BoardDescription(vector<vector<int>> currentBoard) {
			board = currentBoard;
			if (board[0].size() < 2) {
				return;
			}
			for (int i = 0; i < 9; i++) {
				for (int j = 0; j < 9; j++) {
					for (int m = 1; m <= 9; m++) {
						possibilitiesArray[i][j].insert(possibilitiesArray[i][j].end(), m);
					}
				}
			}
			updateBoard();
		}

		int getCellContent(coordinate place) {
			return board[place.x][place.y];
		}

		void updateBoard() {
			if (updateBoardByPossibilties() || updateBoardByAllPossibleOccurences()) {
				updateBoard();
			}
		}

		void insertToCoordinate(coordinate place, int num) {
			board[place.x][place.y] = num;
			possibilitiesArray[place.x][place.y].clear();
			updateNeighborsOfChange(place);
			updateBoard();
		}

		void printBoard() {
			for (int i = 0; i < 9; i++) {
				for (int j = 0; j < 9; j++) {
					if (board[i][j] != 0) {
						cout << board[i][j];
					}
					else {
						cout << ' ';
					}
					cout << '|';
				}
				cout << '\n';
			}
		}

		bool isBoardValid() {
			for (int i = 0; i < 9; i++) {
				for (int j = 0; j < 9; j++) {
					coordinate currentcor = coordinate{ i, j };
					if (getCellContent(currentcor) == 0 && getPossibilitiesByCoordinate(currentcor).size() == 0) {
						return false;
					}
				}
			}
			return true;
		}

		bool isBoardFull() {
			for (int i = 0; i < 9; i++) {
				for (int j = 0; j < 9; j++) {
					coordinate currentcor = coordinate{ i, j };
					if (getCellContent(currentcor) == 0) {
						return false;
					}
				}
			}
			return true;
		}

		coordinate getFirstEmptyCoordinate() {
			for (int i = 0; i < 9; i++) {
				for (int j = 0; j < 9; j++) {
					if (board[i][j] == 0) {
						return coordinate{ i, j };
					}
				}
			}
			return coordinate{ -1, -1 };
		}

		list<int> getPossibilitiesByCoordinate(coordinate c) {
			return possibilitiesArray[c.x][c.y];
		}
};

BoardDescription insertFirstPossibility(BoardDescription desc) {
	coordinate emptyCor = desc.getFirstEmptyCoordinate();
	if (emptyCor.x == -1) {
		desc.board[0][0] = -1;
		return desc;
	}
	list<int> possibilities = desc.getPossibilitiesByCoordinate(emptyCor);
	BoardDescription newDesc = desc;
	newDesc.insertToCoordinate(emptyCor, possibilities.front());
	return newDesc;
}

vector<vector<int>> invalidBoard = { { -1 } };
const BoardDescription invalidBoardDescription(invalidBoard);

BoardDescription eliminateFirstPossibility(BoardDescription desc) {
	coordinate emptyCor = desc.getFirstEmptyCoordinate();
	desc.possibilitiesArray[emptyCor.x][emptyCor.y].pop_front();
	if (desc.getPossibilitiesByCoordinate(emptyCor).size() == 0) {
		return invalidBoardDescription;
	}
	return desc;
}

BoardDescription solveBoard(BoardDescription currentDesc) {
	BoardDescription newDesc = insertFirstPossibility(currentDesc);
	if(newDesc.board[0][0] == -1) {
		return newDesc;
	}
	if (newDesc.isBoardFull()) {
		return newDesc;
	}
	if (newDesc.isBoardValid()) {
		BoardDescription possibleSolution = solveBoard(newDesc);
		if (possibleSolution.board[0][0] != -1) {
			return possibleSolution;
		}
		possibleSolution = eliminateFirstPossibility(currentDesc);
		if (possibleSolution.board[0][0] == -1) {
			return possibleSolution;
		}
		return solveBoard(possibleSolution);
	}
	BoardDescription possibleSolution = eliminateFirstPossibility(currentDesc);
	if (possibleSolution.board[0][0] == -1) {
		return possibleSolution;
	}
	return solveBoard(possibleSolution);
}

vector<vector<int>> Sudoko_Brute_Force::solveSudoko(vector<vector<int>> board) {
	BoardDescription desc(board);
	desc = solveBoard(desc);
	return desc.board;
}