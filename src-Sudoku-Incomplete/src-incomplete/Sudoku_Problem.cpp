/********************************************************************************
 * Autor
 *    Carlos Segura Gonzalez
 * ******************************************************************************/

#include "Sudoku_Problem.h"

int Sudoku_Problem::getPenalizations(const vector<int> *data){
	int penalizations = 0;
	//Row penalizations
	for (int i = 0; i < 9; i++){
		int usedValues[10];
		int maxPenalty[10];
		memset(usedValues, 0, 10 * sizeof(int));
		memset(maxPenalty, 0, 10 * sizeof(int));

		for (unsigned int j = 0; j < fixedDataRow[i].size(); j++){
			usedValues[fixedDataRow[i][j]] = 1;
			maxPenalty[fixedDataRow[i][j]] = 1;
		}
		for (unsigned int j = 0; j < dataRowToGen[i].size(); j++){
			usedValues[data[dataRowToGen[i][j].first][dataRowToGen[i][j].second]]++;
			if (usedValues[data[dataRowToGen[i][j].first][dataRowToGen[i][j].second]] != 1){
				penalizations++;
				if (maxPenalty[data[dataRowToGen[i][j].first][dataRowToGen[i][j].second]] == 1){
					if (useExtraPenalty){
						penalizations += 100;
					}
				}
			}
		}
	}

	//Column penalizations
	for (int c = 0; c < 9; c++){
		int usedValues[10];
		int maxPenalty[10];
		memset(usedValues, 0, 10 * sizeof(int));
		memset(maxPenalty, 0, 10 * sizeof(int));

		for (unsigned int i = 0; i < fixedDataColumn[c].size(); i++){
			usedValues[fixedDataColumn[c][i]] = 1;
			maxPenalty[fixedDataColumn[c][i]] = 1;
		}
		for (unsigned int i = 0; i < dataColumnToGen[c].size(); i++){
			usedValues[data[dataColumnToGen[c][i].first][dataColumnToGen[c][i].second]]++;
			if (usedValues[data[dataColumnToGen[c][i].first][dataColumnToGen[c][i].second]] != 1){
				penalizations++;
				if (maxPenalty[data[dataColumnToGen[c][i].first][dataColumnToGen[c][i].second]] == 1){
					if (useExtraPenalty){
						penalizations += 100;
					}
				}
			}
		}
	}

	//Block penalizations
	for (int c = 0; c < 9; c++){
		int usedValues[10];
		int maxPenalty[10];
		memset(usedValues, 0, 10 * sizeof(int));
		memset(maxPenalty, 0, 10 * sizeof(int));

		for (unsigned int i = 0; i < fixedDataBlock[c].size(); i++){
			usedValues[fixedDataBlock[c][i]] = 1;
			maxPenalty[fixedDataBlock[c][i]] = 1;
		}
		for (unsigned int i = 0; i < dataBlockToGen[c].size(); i++){
			usedValues[data[dataBlockToGen[c][i].first][dataBlockToGen[c][i].second]]++;
			if (usedValues[data[dataBlockToGen[c][i].first][dataBlockToGen[c][i].second]] != 1){
				penalizations++;
				if (maxPenalty[data[dataBlockToGen[c][i].first][dataBlockToGen[c][i].second]] == 1){
					if (useExtraPenalty){
						penalizations += 100;
					}
				}
			}
		}
	}

	return penalizations;
}

void Sudoku_Problem::initiallize(const string &extraPenaltyStr, const string &instanceFile){
	if (extraPenaltyStr == "Yes"){
		useExtraPenalty = true;
	} else if (extraPenaltyStr == "No"){
		useExtraPenalty = false;
	} else {
		cerr << "Error. ExtraPenalty tiene un valor incorrecto: " << extraPenaltyStr << endl;
		exit(-1);
	}
	ifstream file(instanceFile.c_str());
	if (!file.is_open()){
		cerr << "Error. La instancia: " << instanceFile << " no se pudo abrir" << endl;
		exit(-1);
	}

	set<int> dataBlockUsed[9];
	int usedInBlock[9];
	memset(usedInBlock, 0, sizeof(usedInBlock));

	for (int r = 0; r < 9; r++){
		int ommited = 0;
		for (int c = 0; c < 9; c++){
			int data;
			file >> data;
			int block = (r / 3) * 3 + (c / 3);
			if (data != 0){
				ommited++;
				fixedDataColumn[c].push_back(data);
				fixedDataBlock[block].push_back(data);
				fixedDataRow[r].push_back(data);

				dataRowAll[r].push_back(pair<int, int>(-1, data));

				dataBlockUsed[block].insert(data);
			} else {
				pair<int, int> position;
				position = pair<int, int> (block, usedInBlock[block]);
				usedInBlock[block]++;
				dataColumnToGen[c].push_back(position);
				dataBlockToGen[block].push_back(position);
				dataRowToGen[r].push_back(position);
				dataRowAll[r].push_back(position);
			}
		}
	}

	//Calculate unused in each block
	for (int i = 0; i < 9; i++){
		for (int j = 1; j <= 9; j++){
			if (dataBlockUsed[i].count(j) == 0){
				unusedBlock[i].push_back(j);
			}
		}
	}
}

void Sudoku_Problem::print(ostream &out, const vector<int> *data) const{
	for (int r = 0; r < 9; r++){
		for (int c = 0; c < 9; c++){
			if (dataRowAll[r][c].first != -1){
				out << data[dataRowAll[r][c].first][dataRowAll[r][c].second] << " ";
			} 
		}
	}
}


vector<int> Sudoku_Problem::getDimensionChoices(int i){
	return unusedBlock[i];
}

int Sudoku_Problem::getDimensionChoicesSize(int i){
	return unusedBlock[i].size();
}
