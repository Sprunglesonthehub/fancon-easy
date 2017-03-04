#ifndef fancon_MAIN_HPP
#define fancon_MAIN_HPP

#include <algorithm>    // transform, sort
#include <csignal>
#include <cmath>        // floor
#include <iostream>
#include <iomanip>      // setw, left
#include <string>
#include <sstream>
#include <functional>   // ref, reference_wrapped
#include <thread>
#include <sensors/sensors.h>
#include <unistd.h>
#include <sys/stat.h>
#include "Util.hpp"
#include "SensorController.hpp"

using std::cerr;
using std::cout;
using std::endl;
using std::string;
using std::to_string;
using std::stringstream;
using std::setw;
using std::left;
using std::next;
using std::thread;
using std::make_pair;
using std::move;
using std::ref;
using std::reference_wrapper;
using fancon::SensorController;
using fancon::SensorParentInterface;
using fancon::FanTestResult;
using fancon::DaemonState;
using fancon::Util::conf_path;
using fancon::Util::locked;
using fancon::Util::lock;

int main(int argc, char *argv[]);

namespace fancon {
DaemonState daemon_state;

string help();
void firstTimeSetup();

string listFans(SensorController &sensorController);
string listSensors(SensorController &sensorController);

vector<ulong> getThreadTasks(uint nThreads, ulong nTasks);

void test(SensorController &sensorController, uint testRetries, bool singleThread = 0);
void testUID(UID &uid, uint retries = 4);

void handleSignal(int sig);
void start(SensorController &sc, const bool fork_);
void send(DaemonState state);

struct Command {
  Command(const string &name, bool shrtName, const bool requireRoot = true)
      : name(name), called(false), require_root(requireRoot) {
    if (shrtName) {
      shrt_name += name.front();

      // if name contains '-' set first 2 chars of each word as shrt_name
      auto it = name.begin();
      while ((it = find(next(it), name.end(), '-')) != name.end() && next(it) != name.end())
        shrt_name += *(next(it));
    }
  }

  bool operator==(const string &other) { return other == shrt_name || other == name; }

  const string name;
  string shrt_name;
  bool called;
  const bool require_root;
};

struct Option : Command {
public:
  Option(const string &name, bool hasValue = false, bool shrtName = true)
      : Command(name, shrtName, false), has_value(hasValue) {}

  bool has_value;
  uint val = 0;
};
}

#endif //fancon_MAIN_HPP
