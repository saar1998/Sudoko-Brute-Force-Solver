const SolveSudoko = require('./src/build/Release/SudokoSolver.node'),
  Validator = require('./src/boardValidation');

let solver = {};

solver.solve = (arr) => {
  Validator.validateBoard(arr);
  const initialObject = internals.convertArrayToObject(arr);
  const solvedObject = SolveSudoko.solve(initialObject);
  return internals.convertObjectToArray(solvedObject);
}

let internals = {};

internals.convertArrayToObject = (arr) => {
  let obj = {};
  for (let i = 0; i < 9; i++) {
    for (let j = 0; j < 9; j++) {
      obj[`${i}${j}`] = arr[i][j];
    }
  }
  return obj;
};

internals.convertObjectToArray = (obj) => {
  let arr = [[], [], [], [], [], [], [], [], []];
  for (let i = 0; i < 9; i++) {
    for (let j = 0; j < 9; j++) {
      arr[i].push(obj[`${i}${j}`]);
    }
  }
  return arr;
}

module.exports = solver;