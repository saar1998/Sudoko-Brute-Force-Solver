const validator = {}, internals = {};

validator.validateBoard = (board) => {
  internals.validateLength(board);
  internals.validateNumbers(board);
};

internals.validateLength = (arr) => {
  const lengthError = 'Provided array is of no proper length';
  if (arr.length != 9) {
    throw new Error(lengthError);
  }
  arr.forEach((subarray) => {
    if (subarray.length != 9) {
      throw new Error(lengthError);
    }
  });
};

internals.validateNumbers = (arr) => {
  for (let i = 0; i < 9; i++) {
    for (let j = 0; j < 9; j++) {
      if (arr[i][j] < 0 || arr[i][j] > 9) {
        throw new Error('One of the provided numbers is out of range');
      }
    }
  }
};

module.exports = validator;