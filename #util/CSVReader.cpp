#include "../exception/GladeException.h"

#include "CSVReader.h"

using namespace std;

const char DELIMITER = ',';

void CSVReader::read(istream &input, vector<vector<string> > &output)
{
  char ch = 0, previousChar = 0, nextChar = 0;
  bool insideQuotedField = false;
  string field;
  vector<string> row;

  output.clear();

  input >> noskipws;

  while (input >> ch, !input.eof()) {
    nextChar = input.peek();
    
    switch (ch) {
      case DELIMITER:
        if (insideQuotedField) {
          field.append(1, ch);
        } else {
          row.push_back(field);
          field.clear();
        }
        
        break;
      case '\n':
        row.push_back(field);
        field.clear();
        output.push_back(row);
        row.clear();
        break;
      case '\r':
        // skip
        break;
      case '"':
        if (previousChar == DELIMITER || previousChar == 0) {
          insideQuotedField = true;
        } else if (insideQuotedField) {
          if (nextChar == '"') {
            field.append(1, ch);
            input.get();
          } else {
            if (nextChar == DELIMITER || nextChar == EOF || nextChar == '\n') {
              insideQuotedField = false;
            } else {
              throw GladeException("Corrupted CSV");
            }
          }
        } else {
          field.append(1, ch);
        }
        
        break;
      default:
        field.append(1, ch);
        break;
    }
    
    previousChar = ch;
  }

  if (!field.empty()) {
    row.push_back(field);
  }

  if (!row.empty()) {
    output.push_back(row);
  }
}