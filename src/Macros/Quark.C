void print(int value)
{
  cout << "-----------" << endl;
  cout << "value: " << value << endl;
  cout << "t:" <<   int(value/100000& 10) << endl;
  cout << "b:" <<   int(value/10000 % 10) << endl;
  cout << "c:" <<   int(value/1000  % 10) << endl;
  cout << "s:" <<   int(value/100   % 10) << endl;
  cout << "u:" <<   int(value/10    % 10) << endl;
  cout << "d:" <<   int(value  % 10) << endl;
}

int getQuarkNumber(unsigned int flavorIndex, int quarks)
{
  if (quarks/1000000 > 0)
    {
    cout <<" invalid input" << endl;
    return 0;
    }
  int value = 0;
  switch (flavorIndex)
    {
      case 0:  value = int(quarks  % 10); break;
      case 1:  value = int(quarks/10  % 10); break;
      case 2:  value = int(quarks/100  % 10); break;
      case 3:  value = int(quarks/1000  % 10); break;
      case 4:  value = int(quarks/10000  % 10); break;
      case 5:  value = int(quarks/100000  % 10); break;
      //default: throw Exception("Invalid flavor index",__FUNCTION__);
    }
  return value;
}

int Quark()
{
//  print(213425);
//  print(211111);
//  print(10001);
//  print(101);
//  print(2);
  int quarks = 7654321;
  cout << "d:" << getQuarkNumber(0,quarks) << endl;
  cout << "u:" << getQuarkNumber(1,quarks) << endl;
  cout << "s:" << getQuarkNumber(2,quarks) << endl;
  cout << "c:" << getQuarkNumber(3,quarks) << endl;
  cout << "b:" << getQuarkNumber(4,quarks) << endl;
  cout << "t:" << getQuarkNumber(5,quarks) << endl;
  return 0;
}
