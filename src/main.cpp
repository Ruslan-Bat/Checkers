#include <iostream>
#include <sstream>
#include <stdexcept>

#include <Graph_lib/Graph.h>
#include <Graph_lib/Simple_window.h>
#include <CheckersWindow.h>
using namespace Graph_lib;
  


int main()
  try
  {
    CheckersWindow win(Point{100, 100}, 640, 640, "Checkers");
    return gui_main();
  }

catch (std::exception& e)
{
  std::cerr << e.what() << std::endl;
  return 1;
}
catch (...)
{
  std::cerr << "Oops, something went wrong..." << std::endl;
  return 2;
}

