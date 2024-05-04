#include <iostream>


#include <vector>

void
test()
{
    static std::vector<int> vec;
    vec.push_back(0);
}

int main()
{
  std::cout << "Hello world" << std::endl;
  test();
  
  for(;;);
  //return 0;
  return 0;
}
