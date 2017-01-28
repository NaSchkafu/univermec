#include "stable.h"
#ifndef __INTERFACE_CONVEX_POLYHEDRON__
#define __INTERFACE_CONVEX_POLYHEDRON__

#include "inonconvexpoly.hpp"
#include <vector>

namespace objects
{
    template<typename T>
    class IConvexPolyhedron : public INonConvexPolyhedron<T>
    {
        public:
            // Bereits Convex. Rückgabe einer Kopie.
            virtual std::vector< IConvexPolyhedron<T>* > clone_to_convex_parts_() const
            {
                std::vector<IConvexPolyhedron<T>*> res;
                res.push_back(this->clone());
                return res;
            }

            inline IConvexPolyhedron<T>* clone() const
            {
                return clone_();
            }

            inline IConvexPolyhedron<T>* simplify(const core::arith::ivector &bounding_box) const
            {
                return simplify_(bounding_box);
            }

        private:
            virtual IConvexPolyhedron<T>* clone_() const = 0;
            virtual IConvexPolyhedron<T>* simplify_(const core::arith::ivector &bounding_box) const = 0;
            // kein copy ctor und assigment op
            IConvexPolyhedron(const IConvexPolyhedron<T> &);
            IConvexPolyhedron& operator=(const IConvexPolyhedron<T> &);

        protected:
            IConvexPolyhedron() { }
    };
}

#endif
