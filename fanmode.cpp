#include <format>
#include <fstream>
#include <ostream>
#include <print>
#include <string>

std::string path;

int FanSpeed(std::string path) {
  std::ifstream Fan(std::format("{}/fan1_input", path));
  if (Fan.is_open()) {
    int fan_speed = 0;
    if (Fan >> fan_speed) {
      // std::println("Fan Speed: {} rpm", fan_speed);
      return fan_speed;
    } else {
      std::println(stderr, "Failed to read fan speed");
      return 1;
    }
    Fan.close();
  } else {
    std::println(stderr, "WEARECOOKED");
    return 1;
  }
  return -1;
}

bool FanConf() {
  // 0 = full
  // 1 = automatic
  std::ifstream Fan(std::format("{}/pwm1_enable", path));
  if (Fan.is_open()) {
    bool fan_config;
    Fan >> fan_config;
    if (fan_config) {
      return 1;
    } else if (!fan_config) {
      return 0;
    }
  } else {
    std::println(stderr,
                 "Could not read config of state of fan. Check pwm1_enable.");
  }
  return 1;
}

void opt_s() {
  std::println("Fan Speed: {} rpm", FanSpeed(path));
  std::println("Fan Config: {}", (FanConf() ? "Automatic" : "Full"));
}

int check_for_opts(int argc, char **argv) {
  if (argc == 2) {
    if ((std::string)argv[1] == "-s") {
      return 0;
    } else if ((std::string)argv[1] == "-f") {
      return 1;
    } else if ((std::string)argv[1] == "-a") {
      return 2;
    }
  } else {
    std::println("USAGE: ./fanmode -f/-a/-s");
    return -1;
  }
  std::println("USAGE: ./fanmode -f/-a/-s");
  return -1;
}

int main(int argc, char **argv) {
  std::ifstream FanInit("/home/shreyas/.config/fanstuff/fanmode.conf");
  if (FanInit.is_open()) {
    char fan_int;
    FanInit >> fan_int;
    path = std::format("/sys/class/hwmon/hwmon{}", fan_int);
    // path = std::format("/sys/class/hwmon/hwmon4");
    FanInit.close();
  } else {
    std::println(stderr, "Could not open config file for Fan, check "
                         "~/.config/fanstuff/fanmode.conf");
    std::ofstream Fanout("/home/shreyas/.config/fanstuff/fanmode.conf");
    Fanout << 4;
    return 1;
  }
  switch (check_for_opts(argc, argv)) {
  case 0:
    opt_s();
    // case 1:
    //   // change fan speed to full
    // case 2:
    //   // change fan speed to automatic
  }
}
