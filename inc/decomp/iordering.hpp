#ifndef __IORDERING_HPP__
#define __IORDERING_HPP__

namespace decomp
{
  namespace details
  {
    /// Describes an ordering relation on boxes
    template<typename T>
    class IOrdering
    {
    public:
      inline bool operator<(const T& f, const T& s) const
        {
          return lt_(f,s);
        }

      inline bool eq(const T& f, const T& s) const
        {
          return !neq(s,f);
        }

      inline bool neq(const T& f, const T& s) const
        {
          return (lt_(f,s) || lt_(s,f));
        }

    private:
      virtual bool lt_(const T& f, const T& s) const = 0;
    };
  }
}

#endif /*__IORDERING_HPP__*/
