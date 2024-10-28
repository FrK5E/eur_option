#pragma once 

#include <ql/time/daycounter.hpp>
namespace ql = QuantLib;
class WEHalf : public ql::DayCounter {

public:
  WEHalf(): ql::DayCounter(implementation()){};
  virtual ~WEHalf() = default;

private:
    class Impl final : public ql::DayCounter::Impl {
          public:
            std::string name() const override { return std::string("Actual/365 (Fixed)"); }
            ql::Time
            yearFraction(const ql::Date& d1, const ql::Date& d2, const ql::Date&, const ql::Date&) const override {
                return daysBetween(d1,d2)/365.0;
            }
        }; 
  std::shared_ptr<Impl> implementation() {};
};