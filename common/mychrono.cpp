#include <chrono>

using std::chrono::steady_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;
using std::chrono::time_point;



class Mychrono{
private:
  time_point<steady_clock> cstart;
  time_point<steady_clock> cend;
  double diff;

public:
  Mychrono(){
    /*
    cstart = INIT_VALUE;
    cend = INIT_VALUE;
    */
    diff = 0;
  }

  void start_chrono(){
    cstart = steady_clock::now();
  }

  void end_chrono(){
    /*
    if(cstart == INIT_VALUE){
      throw "cend() before cstart()";
    }
    */
    cend = steady_clock::now();
    auto d = duration_cast<milliseconds>(cend - cstart);
    diff = std::chrono::duration<double>(d).count();
  }

  double get_diff(){
    return diff;
  }
};
