const Expect = require('chai').expect;

const Solve = require('../index').solve;

const data = require('./testData');

describe('main tests', () => {
  it('should return solution when provided with vaild board', () => {
    const solution = Solve(data.goodBoard);
    Expect(solution).to.eql(data.goodBoardSolution);
  });

  it('should throw an error when provided with unsolveable board', () => {
    Expect(Solve.bind(this, data.unsolveableBoard)).to.throw('Impossible Board');
  });

  it('should throw an error when provided with board with invalid rows number ', () => {
    Expect(Solve.bind(this, data.boardWithInvalidRowsNumber)).to.throw('Provided array is of no proper length');
  });

  it('should throw an error when provided with board with invalid columns number ', () => {
    Expect(Solve.bind(this, data.boardWithInvalidColumnsNumber)).to.throw('Provided array is of no proper length');
  });

  it('should throw an error when provided with board with invalid number ', () => {
    Expect(Solve.bind(this, data.boardWithInvalidNumber)).to.throw('One of the provided numbers is out of range');
  });
});