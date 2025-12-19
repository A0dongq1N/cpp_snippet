#include <indicators/progress_bar.hpp>
#include <indicators/cursor_control.hpp>
#include <thread>
#include <chrono>

int main() {
  // Hide cursor
  indicators::show_console_cursor(false);

  indicators::ProgressBar bar{
    indicators::option::BarWidth{50},
    indicators::option::Start{"["},
    indicators::option::Fill{"="},
    indicators::option::Lead{">"},
    indicators::option::Remainder{" "},
    indicators::option::End{" ]"},
    indicators::option::PostfixText{"Extracting Archive"},
    indicators::option::ForegroundColor{indicators::Color::green},
    indicators::option::FontStyles{
        std::vector<indicators::FontStyle>{indicators::FontStyle::bold}}
  };

  // Update bar state
  for (size_t i = 0; i <= 100; ++i) {
    bar.set_option(indicators::option::PostfixText{
        std::to_string(i) + "% completed"
    });
    bar.set_progress(i);
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
  }

  // Show cursor
  indicators::show_console_cursor(true);

  return 0;
}

