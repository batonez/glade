#include <fstream>
#include <vector>

#include "../log/log.h"
#include "../util/CSVReader.h"

int main()
{
  ifstream input("input.txt");
  vector<vector<string> > output;

  CSVReader::read(input, output);

  vector<vector<string> >::iterator rowi = output.begin();

  while (rowi != output.end()) {
    vector<string>::iterator coli = rowi->begin();

    while (coli != rowi->end()) {
      log("%s", coli->c_str());
      ++coli;
    }

    log("==");
    ++rowi;
  }
  
  return 0;
}