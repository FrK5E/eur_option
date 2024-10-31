#pragma once

#include <ql/time/daycounter.hpp>
namespace ql = QuantLib;
class WEHalf : public ql::DayCounter {

public:
  WEHalf() : ql::DayCounter(implementation()){};
  virtual ~WEHalf() = default;

private:
  class Impl1 final : public ql::DayCounter::Impl {
  public:
    std::string name() const override { return std::string("WEHalf"); }
    ql::Time yearFraction(const ql::Date &d1, const ql::Date &d2,
                          const ql::Date &, const ql::Date &) const override {
      return timeBetween(d1, d2) / timeInYear();
    };
  private:
    ql::Time timeBetween(const ql::Date &d1, const ql::Date &d2) const { return 1.0; }
    ql::Time timeInYear() const { return 365 * 24 * 3600; }
  };
  
  std::shared_ptr<Impl> implementation() { return std::make_shared<Impl1>(); };
};
