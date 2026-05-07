/* **********************************************************************
 * Copyright (C) 2019-2024, Claude Pruneau, Victor Gonzalez   
 * All rights reserved.
 *
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 *
 * Author: Claude Pruneau,   04/01/2024
 *
 * *********************************************************************/


template <class T >
class Test
{
public:
  Test() {}
  virtual ~Test() {}
  void shoveIt() { x = 0; }
  ClassDef(Test,0)

  int x;
};

class A
{
public:
  A() {}
  virtual ~A() {}
  void what() {}

  ClassDef(A,0)

};

//ClassImp(test<T>);
ClassImp(Test<A>);
ClassImp(A);


int TestCreate()
{
    TClass *myClass = TClass::GetClass("Test<A>");
  if (!myClass)
    {
    cout << "taskClass not instantiated === Failed 1" << endl;
    };
  Test<A> * t =  (Test<A>*)myClass->New();
  if (!t)
    {
    cout << "Test<A> * not created === Failed 2" << endl;
    };
  return 0;
}
