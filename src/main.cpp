#include <napi.h>
#include <vector>
#include <string>
#include <iostream>
#include "Sudoko_Brute_Force/Sudoko_Brute_Force.h"

using namespace std;

vector<vector<int>> transformNapiObjectToArray(Napi::Object obj) {
	vector<vector<int>> vec(9);
	for (int i = 0; i < 9; i++) {
		vec[i] = vector<int>(9);
		for (int j = 0; j < 9; j++) {
			string index = to_string(i) + to_string(j);
			Napi::Value num = obj[index];
			vec[i][j] = num.As<Napi::Number>();
		}
	}
	return vec;
}
 
Napi::Object transformArrayToNapiObject(vector<vector<int>> vec, Napi::Env env) {
	Napi::Object obj = Napi::Object::New(env);
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			string index = to_string(i) + to_string(j);
			obj.Set(index, vec[i][j]);
		}
	}
	return obj;
}

Napi::Object solveSudoko(Napi::Object board, Napi::Env env) {
	vector<vector<int>> initialBoard = transformNapiObjectToArray(board);
	vector<vector<int>> solvedBoard = Sudoko_Brute_Force::solveSudoko(initialBoard);
	if (solvedBoard[0][0] == -1) {
		Napi::RangeError::New(env, "Impossible Board").ThrowAsJavaScriptException();
	}
	else {
		return transformArrayToNapiObject(solvedBoard, env);
	}
}

Napi::Object solveSudokoWrapped(const Napi::CallbackInfo& info) {
	Napi::Env env = info.Env();
	Napi::Object initialBoard = info[0].As<Napi::Object>();
	return solveSudoko(initialBoard, env);
}

Napi::Object init(Napi::Env env, Napi::Object exports) {
	exports.Set("solve", Napi::Function::New(env, solveSudokoWrapped));
	return exports;
}

NODE_API_MODULE(SolveSudoko, init)