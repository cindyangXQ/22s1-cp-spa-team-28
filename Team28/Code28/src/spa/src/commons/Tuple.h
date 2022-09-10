#include <vector>
/*
* Encapsulates a single row in a table.
*/
class Tuple{
  std::vector<Value> values;

  pair<Tuple> splitTuple(std::vector<int>); // return a pair of tuples, first tuple contains the values corresponding to the indices given in argument, second tuple contains the remaining values.
  int hash();

  static combineSubTuple(Tuple tuple1, Tuple tuple2, Tuple tuple3); // tuple1 is the common sub-tuple
}