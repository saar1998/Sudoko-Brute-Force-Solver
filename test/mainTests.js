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
});