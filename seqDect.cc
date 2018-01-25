#include <iostream>
#include <stdio.h>
#include <string>

const int MAX_BITS = 4;

enum EStates
{
  s0,
  s1,
  s2,
  s3
};

struct SStateInfo
{
  EStates state;
  EStates nextState_0;
  EStates nextState_1;
  std::string remembers;

  SStateInfo()
  {
    state = nextState_0 = nextState_1 = s0;
    remembers = "";
  }
};

class CSequenceDetecor
{
public:
  CSequenceDetecor(const std::string &sequence);
  ~CSequenceDetecor();
  void printTruthTable();
  int detectSequence();

private:
  bool startsWith( std::string const &subSeq );
  int getStateValue(std::string pattern);
  void constructStateTable();

  std::string       _sequence;
  SStateInfo        _stateInfo[MAX_BITS];
  bool              _detectedWithZero;
};

CSequenceDetecor::CSequenceDetecor(const std::string &sequence)
{
  _sequence = sequence;

  _stateInfo[0].state = s0;

  _stateInfo[1].state = s1;
  _stateInfo[1].remembers = _sequence[0];

  _stateInfo[2].state = s2;
  _stateInfo[2].remembers = _sequence.substr(0, 2);

  _stateInfo[3].state = s3;
  _stateInfo[3].remembers = _sequence.substr(0, 3);

  _detectedWithZero = false;

  constructStateTable();
}

CSequenceDetecor::~CSequenceDetecor()
{

}

void CSequenceDetecor::constructStateTable()
{
  std::string inputZero = "0";
  std::string inputOne = "1";
  std::string pattern;
  int stateValue;

  pattern = _stateInfo[3].remembers + inputZero;
  if( _sequence == pattern )
  {
    _detectedWithZero = true;
  }

  for(int i = 0; i < MAX_BITS; i++ )
  {
    pattern = _stateInfo[i].remembers + inputZero;
    if( _detectedWithZero && i == 3 )
    {
      pattern.erase(0, 1);
    }
    stateValue = getStateValue(pattern);
    switch ( stateValue )
    {
      case 1:
      {
        _stateInfo[i].nextState_0 = s3;
        break;
      }
      case 2:
      {
        _stateInfo[i].nextState_0 = s2;
        break;
      }
      case 3:
      {
        _stateInfo[i].nextState_0 = s1;
        break;
      }
      case 4:
      default:
      {
        _stateInfo[i].nextState_0 = s0;
        break;
      }
    }

    pattern = _stateInfo[i].remembers + inputOne;
    if( !_detectedWithZero && i == 3 )
    {
      pattern.erase(0, 1);
    }
    stateValue = getStateValue(pattern);
    switch ( stateValue )
    {
      case 1:
      {
        _stateInfo[i].nextState_1 = s3;
        break;
      }
      case 2:
      {
        _stateInfo[i].nextState_1 = s2;
        break;
      }
      case 3:
      {
        _stateInfo[i].nextState_1 = s1;
        break;
      }
      case 4:
      default:
      {
        _stateInfo[i].nextState_1 = s0;
        break;
      }
    }
  }
}

int CSequenceDetecor::getStateValue(std::string pattern)
{
  int stateValue = MAX_BITS;

  while( pattern.size() != 0 )
  {
    if( startsWith(pattern) )
    {
      stateValue = MAX_BITS - pattern.size();
      break;
    }
    else
    {
      pattern.erase(0, 1);
    }
  }
  return stateValue;
}

bool CSequenceDetecor::startsWith( std::string const &subSeq )
{
  bool result = false;
  if( _sequence.compare(0, subSeq.size(), subSeq) == 0 )
  {
    result = true;
  }
  return result;
}

int CSequenceDetecor::detectSequence()
{
  EStates presentState, nextState;
  int input, count, output;
  presentState = nextState = s0;
  count = output = 0;
  while( ( input = getchar() ) != '\n' )
  {
    if( input == '0' )
    {
      switch( presentState )
      {
        case s0:
        {
          nextState = _stateInfo[s0].nextState_0;
          output = 0;
          break;
        }
        case s1:
        {
          nextState = _stateInfo[s1].nextState_0;
          output = 0;
          break;
        }
        case s2:
        {
          nextState = _stateInfo[s2].nextState_0;
          output = 0;
          break;
        }
        case s3:
        {
          nextState = _stateInfo[s3].nextState_0;
          output = (_detectedWithZero) ? 1 : 0;
          break;
        }                        
      }
    }
    else if( input == '1' )
    {
      switch( presentState )
      {
        case s0:
        {
          nextState = _stateInfo[s0].nextState_1;
          output = 0;
          break;
        }
        case s1:
        {
          nextState = _stateInfo[s1].nextState_1;
          output = 0;
          break;
        }
        case s2:
        {
          nextState = _stateInfo[s2].nextState_1;
          output = 0;
          break;
        }
        case s3:
        {
          nextState = _stateInfo[s3].nextState_1;
          output = (!_detectedWithZero) ? 1 : 0;
          break;
        }                        
      }
    }
    else
    {
      // Invalid character found in the input sequence
      // only zeros and ones are allowed
      // Exit the state machine
      count = -1;
      break;
    }

    if( output == 1 )
    {
      count++;
      output = 0;
    }
    presentState = nextState;
  }
  return count;
}

void CSequenceDetecor::printTruthTable()
{
  std::cout << "When input = 0" << std::endl;
  for(int i = 0; i < MAX_BITS; i++)
  {
    std::cout << "Next State = s" << _stateInfo[i].nextState_0 << " ";
    if( i == 3 && _detectedWithZero )
    {
      std::cout << "output = 1" << std::endl;
    }
    else
    {
      std::cout << "output = 0" << std::endl;
    }
  }

  std::cout << "When input = 1" << std::endl;
  for(int i = 0; i < MAX_BITS; i++)
  {
    std::cout << "Next State = s" << _stateInfo[i].nextState_1 << " ";
    if( i == 3 && !_detectedWithZero )
    {
      std::cout << "output = 1" << std::endl;
    }
    else
    {
      std::cout << "output = 0" << std::endl;
    }
  }
}

int main()
{
  int count;
  CSequenceDetecor seqDet("1010");
  std::cout << "Enter the sequence: " << std::endl;
  count = seqDet.detectSequence();
  std::cout << "Sequence 1010 detected for " << count << " times" << std::endl;
  return 0;
}
