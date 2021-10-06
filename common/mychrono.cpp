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
    diff = -1;
  }

  double get_diff(){
    return diff;
  }

  void start_chrono(){
    cstart = steady_clock::now();
  }

  void end_chrono(){
    cend = steady_clock::now();
    auto d = duration_cast<milliseconds>(cend - cstart);
    diff = std::chrono::duration<double>(d).count();
  }



};
