//
// Created by Ivan Pađen on 07/07/2023.
//

#ifndef WRAPWRAP_PROGRESS_BAR_H
#define WRAPWRAP_PROGRESS_BAR_H

#include <string>
#include <iostream>

namespace CGAL {

namespace IO {

class Progress_bar {
public:
  Progress_bar(const int size, const int print_nth)
    : m_size(size), m_print_nth(print_nth), m_current(0)
  {}

  void increment_progress() {++m_current;}

  void print_progress(const bool finished = false) const
  {
    double percent;
    if (finished)
      percent = 100;
    else
    {
      if ((m_current % m_print_nth) != 0) return;
      percent = 100. * double(m_current) / double(m_size);
    }

    std::string bar;

#ifdef CGAL_PROGRESS_BAR_DEBUG
    std::cout << std::endl;
    std::cout << percent << std::endl;
    std::cout << m_current << std::endl;
    std::cout << m_size << std::endl;
#endif

    for (int i = 0; i < 50; i++) {
      if (i < (percent / 2)) {
        bar.replace(i, 1, "=");
      }
      else if (i == (percent / 2)) {
        bar.replace(i, 1, ">");
      }
      else {
        bar.replace(i, 1, " ");
      }
    }
    std::clog << "\r" "    [" << bar << "] ";
    std::clog.width(3);
    std::clog << percent << std::fixed << std::setprecision(0) <<  "%     " << std::flush;
  }

  void increment_and_print()
  {
    increment_progress();
    print_progress();
  }

  void finish()
  {
    print_progress(true);
    std::cout << std::endl;
  }

  int get_current() const
  {
    return m_current;
  }

private:
  const int m_size;
  const int m_print_nth;
  int m_current;
};

} //namespace IO

} //namespace CGAL
#endif //WRAPWRAP_PROGRESS_BAR_H
