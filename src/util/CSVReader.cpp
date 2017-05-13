#include "glade/exception/GladeException.h"

#include "glade/util/CSVReader.h"
#include "glade/debug/log.h"
#include "glade/exception/GladeException.h"

using namespace std;

const char DELIMITER = ',';

void CSVReader::read(istream &input, vector<vector<string> > &output)
{
  if (!input.good()) {
    throw GladeException("CSVReader: input stream is broken");
  }
  
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

void CSVReader::read(vector<char> &input, vector<vector<string> > &output)
{
  char ch = 0, previousChar = 0, nextChar = 0;
  bool insideQuotedField = false;
  string field;
  vector<string> row;

  output.clear();

  for (int i = 0; i < input.size(); ++i) {
    ch = input[i];
    nextChar = input[i + 1];
    
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
            ++i;
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
